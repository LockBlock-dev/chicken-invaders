#include "Asteroid.hpp"

#include <cstring>

#include "Bonus.hpp"
#include "Explosion.hpp"
#include "Game.hpp"
#include "Smoke.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

Asteroid::Asteroid(
    UveDX::UveDX* uveDX,
    double x,
    double y,
    int face,
    int type,
    int size,
    int minspeed,
    int maxspeed
)
    : Enemy(uveDX, 0, 0, nullptr, 0, 10),
      x_coord(x),
      y_coord(y),
      face(face),
      speed(
          minspeed + (maxspeed != 0 ? generate_random_number() % maxspeed : 0)
      ),
      dx((double)this->speed * global::dcos[this->face]),
      dy((double)this->speed * global::dsin[this->face]),
      type(type),
      size(size) {
  if (this->type == 2) {
    int surfaceNumber = generate_random_number() % 32;

    this->surface =
        size ? global::game->surface_chain_asteroid_small_rock->getSurf(
                   surfaceNumber
               )
             : global::game->surface_chain_asteroid_big_rock->getSurf(
                   surfaceNumber
               );
  } else if (this->size)
    this->surface = global::game->surface_chain_asteroid_small_fire->getSurf(
        generate_random_number() % 32
    );
  else
    this->surface = global::game->surface_chain_asteroid_big_fire->getSurf(
        generate_random_number() % 32
    );

  if (this->size == 0 &&
      global::game->gameController->getWaveController()->getCurrentSystem() ==
          0)
    this->health = 5;
}

void Asteroid::update() {
  int a8 = this->face + 128;

  if (this->face < 0)
    this->face += 256;

  if (this->face > 255)
    this->face -= 256;

  this->x_coord = this->dx + this->x_coord;
  this->y_coord = this->dy + this->y_coord;

  if (this->y_coord > 500.0 || (this->y_coord < -40.0 && this->dy < 0.0) ||
      (this->x_coord < -40.0 && this->dx < 0.0) ||
      (this->x_coord > 680.0 && this->dx > 0.0))
    this->hasBeenDisposed = true;

  if (global::game->gameController->getGraphicsQuality()) {
    bool flag = false;

    if (this->surface) {
      int v2 =
          this->ukwn_2 ? this->sprite_x : this->sprite_x - this->uveDX->xOffset;
      flag = true;
      int v3 = v2 - this->surface->getOffsetX();

      if (v3 < 0 || v3 >= static_cast<int>(this->uveDX->getWidth())) {
        int v4 = v3 + this->surface->getWidth();

        if (v4 < 0 || v4 >= static_cast<int>(this->uveDX->getWidth()))
          flag = false;
      }
    }

    if (flag) {
      bool flag2 = false;

      if (this->surface) {
        int v7;
        int v6 = this->ukwn_2 ? this->sprite_y
                              : this->sprite_y - this->uveDX->yOffset;
        flag2 = ((v7 = v6 - this->surface->getOffsetY(), v7 >= 0) &&
                 v7 < static_cast<int>(this->uveDX->getHeight())) ||
                (v7 + this->surface->getHeight() >= 0 &&
                 this->surface->getHeight() + v7 <
                     static_cast<int>(this->uveDX->getHeight()));
      }

      if (flag2) {
        int v9 = this->speed;

        if (v9 < 0)
          v9 += 3;

        v9 = (v9 / 4) * 256;

        if (v9 < 0)
          v9 = 0;

        if (v9 > 1279)
          v9 = 1279;

        void* mem = std::malloc(0x2344);
        std::memset(mem, 0, 0x2344);

        global::game->gameController->asteroid_explosion_smoke_list->add(
            this->type == 3 ? dynamic_cast<UveBase*>(new (mem) Explosion{
                                  this->uveDX, this->sprite_x, this->sprite_y,
                                  10, v9, a8, 128, true
                              })
                            : dynamic_cast<UveBase*>(new (mem) Smoke{
                                  this->uveDX, this->sprite_x, this->sprite_y,
                                  10, v9, a8, 128, true
                              })
        );
      }
    }
  }

  this->sprite_x = static_cast<int>(this->x_coord);
  this->sprite_y = static_cast<int>(this->y_coord);

  UveDX::Sprite::update();
}

void Asteroid::handleHit(unsigned int playerId, int damages) {
  if (this->health > 0) {
    this->health -= damages;

    if (this->health <= 0) {
      global::game->playSound(global::game->sound_cannonfire, this->sprite_x);

      auto gameController = global::game->gameController;

      void* mem = std::malloc(0x2344);
      std::memset(mem, 0, 0x2344);

      gameController->explosion_smoke_list->add(new (mem) Explosion{
          this->uveDX, this->sprite_x, this->sprite_y, 50,
          (this->size ? 3 : 5) * 256, 0, 256, true
      });

      auto waveController = gameController->getWaveController();

      if (!waveController->getHasSpawnedGiftThisWave() &&
          generate_random_number() % 30 == 0) {
        gameController->bonus_list->add(
            new Bonus{this->uveDX, this->sprite_x, this->sprite_y}
        );

        waveController->setHasSpawnedGiftThisWave(true);
      }

      auto player = gameController->players.at(playerId);

      if (player)
        player->increaseScoreBy(100 * (int)(this->dx + this->dy) + 300);

      this->hasBeenDisposed = true;
    }
  }
}

void Asteroid::handleHitPlayer(unsigned int playerId) {
  global::game->gameController->players.at(playerId)->handleHit();
}
