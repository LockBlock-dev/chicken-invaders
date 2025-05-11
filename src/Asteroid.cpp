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
    unsigned int face,
    AsteroidType type,
    bool isSmall,
    unsigned int minspeed,
    unsigned int maxspeed
)
    : Enemy(uveDX, 0, 0, nullptr, 0, 10),
      positionX(x),
      positionY(y),
      face(face),
      speed(minspeed + (maxspeed != 0 ? random_range(0u, maxspeed) : 0)),
      velocityX((double)this->speed * global::dcos[this->face]),
      velocityY((double)this->speed * global::dsin[this->face]),
      type(type),
      isSmall(isSmall) {
  if (this->type == AsteroidType::Rock) {
    unsigned int surfaceNumber = random_range(0u, 32u);

    this->surface =
        isSmall ? global::game->surface_chain_asteroid_small_rock->getSurf(
                      surfaceNumber
                  )
                : global::game->surface_chain_asteroid_big_rock->getSurf(
                      surfaceNumber
                  );
  } else if (this->isSmall)
    this->surface = global::game->surface_chain_asteroid_small_fire->getSurf(
        random_range(0u, 32u)
    );
  else
    this->surface = global::game->surface_chain_asteroid_big_fire->getSurf(
        random_range(0u, 32u)
    );

  if (!this->isSmall &&
      global::game->gameController->getWaveController()->getCurrentSystem() ==
          0)
    this->health = 5;
}

void Asteroid::update() {
  int adjustedDirectionAngle = static_cast<int>(this->face) + 128;

  this->face = (this->face + 256) % 256;

  this->positionX = this->velocityX + this->positionX;
  this->positionY = this->velocityY + this->positionY;

  if (this->positionY > 500.0 ||
      (this->positionY < -40.0 && this->velocityY < 0.0) ||
      (this->positionX < -40.0 && this->velocityX < 0.0) ||
      (this->positionX > 680.0 && this->velocityX > 0.0))
    this->hasBeenDisposed = true;

  if (global::game->gameController->getGraphicsQuality()) {
    bool isSurfaceVisible = false;

    if (this->surface) {
      int spriteXOffset = this->isAbsolutePosition
                              ? this->sprite_x
                              : this->sprite_x - this->uveDX->xOffset;
      isSurfaceVisible = true;
      int surfaceXOffset =
          spriteXOffset - static_cast<int>(this->surface->getOffsetX());

      if (surfaceXOffset < 0 ||
          surfaceXOffset >= static_cast<int>(this->uveDX->getWidth())) {
        int surfaceXOffsetWrapped =
            surfaceXOffset + static_cast<int>(this->surface->getWidth());

        if (surfaceXOffsetWrapped < 0 ||
            surfaceXOffsetWrapped >= static_cast<int>(this->uveDX->getWidth()))
          isSurfaceVisible = false;
      }
    }

    if (isSurfaceVisible) {
      bool shouldRender = false;

      if (this->surface) {
        int y_coord = this->isAbsolutePosition
                          ? this->sprite_y
                          : this->sprite_y - this->uveDX->yOffset;

        int surfaceHeight = static_cast<int>(this->surface->getHeight());
        int screenHeight = static_cast<int>(this->uveDX->getHeight());

        int surfaceTop =
            y_coord - static_cast<int>(this->surface->getOffsetY());
        int surfaceBottom = surfaceTop + surfaceHeight;

        shouldRender = (surfaceTop >= 0 && surfaceTop < screenHeight) ||
                       (surfaceBottom >= 0 && surfaceBottom < screenHeight);
      }

      if (shouldRender) {
        unsigned int v9 = this->speed * 64;

        v9 = std::clamp(v9, 0u, 1279u);

        void* mem = std::malloc(0x2344);
        std::memset(mem, 0, 0x2344);

        global::game->gameController->asteroid_explosion_smoke_list->add(
            this->type == AsteroidType::Fire
                ? dynamic_cast<UveBase*>(new (mem) Explosion{
                      this->uveDX, this->sprite_x, this->sprite_y, 10, v9,
                      adjustedDirectionAngle, 128, true
                  })
                : dynamic_cast<UveBase*>(new (mem) Smoke{
                      this->uveDX, this->sprite_x, this->sprite_y, 10, v9,
                      adjustedDirectionAngle, 128, true
                  })
        );
      }
    }
  }

  this->sprite_x = static_cast<int>(this->positionX);
  this->sprite_y = static_cast<int>(this->positionY);

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
          (this->isSmall ? 3u : 5u) * 256, 0, 256, true
      });

      auto waveController = gameController->getWaveController();

      if (!waveController->getHasSpawnedGiftThisWave() &&
          random_range(0, 30) == 0) {
        gameController->bonus_list->add(
            new Bonus{this->uveDX, this->sprite_x, this->sprite_y}
        );

        waveController->setHasSpawnedGiftThisWave(true);
      }

      if (auto player = gameController->players.at(playerId))
        player->increaseScoreBy(
            100 * static_cast<int>(this->velocityX + this->velocityY) + 300
        );

      this->hasBeenDisposed = true;
    }
  }
}

void Asteroid::handleHitPlayer(unsigned int playerId) {
  global::game->gameController->players.at(playerId)->handleHit();
}
