#include "Chicken.hpp"

#include <cstring>

#include "Bonus.hpp"
#include "BoundaryBouncer.hpp"
#include "ChickenLeg.hpp"
#include "Egg.hpp"
#include "Game.hpp"
#include "Smoke.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

Chicken::Chicken(
    UveDX::UveDX* uveDX,
    int x,
    int y,
    int chickenType,
    int chickenFlyType,
    int invisibilityTimeout
)
    : Enemy(
          uveDX,
          10 * x - 2880,
          -4 * y,
          global::game->surface_chain_chickenb->getSurf(0),
          invisibilityTimeout,
          global::game->gameController->getWaveController()->getCurrentSystem(
          ) + 1
      ),
      xCoordBase(x),
      yCoordBase(y),
      x_coord(x),
      y_coord(y),
      field_AC(generate_random_number() % 256),
      field_B0(generate_random_number() % 256),
      field_B4(generate_random_number() % 200 + 100),
      chickenFlyType(chickenFlyType),
      chickenType(chickenType),
      boundaryBouncer(0, this->chickenType == 3 ? 7 : 9) {
  if (this->chickenFlyType == 5) {
    this->sprite_x = this->xCoordBase;
    this->sprite_y = this->yCoordBase;
    this->field_B4 =
        global::game->gameController->getWaveController()->getCurrentSystem() +
        5;
    this->field_AC = 112;
  }
  if (this->chickenFlyType == 6) {
    this->sprite_x = this->xCoordBase;
    this->sprite_y = this->yCoordBase;
    this->field_B4 =
        global::game->gameController->getWaveController()->getCurrentSystem() +
        5;
    this->field_AC = 144;
  }
}

void Chicken::update() {
  int v1 = 0;

  if (this->invisibilityTimeout <= 0) {
    this->x_coord = this->sprite_x;
    this->y_coord = this->sprite_y;

    if (this->chickenFlyType == 1) {
      int v2 = global::game->gameController->getWaveController()->field_30;

      if (v2 >= this->xCoordBase)
        v2 = this->xCoordBase;

      int v3 = v2;

      int v4 = global::game->gameController->getWaveController()->field_34;

      global::game->gameController->getWaveController()->field_30 = v3;

      if (v4 <= this->xCoordBase)
        v4 = this->xCoordBase;

      global::game->gameController->getWaveController()->field_34 = v4;

      this->sprite_x = (global::game->gameController->getWaveController()
                            ->boundaryBouncer.currentSurfaceIndex +
                        this->xCoordBase + this->sprite_x) /
                       2;
      this->sprite_y = (this->yCoordBase + this->sprite_y) / 2;

      v1 = 128;
    }

    if (this->chickenFlyType == 2) {
      this->field_AC = (this->field_AC + 1 + 256) % 256;
      this->field_B0 = (this->field_B0 + 1 + 256) % 256;

      this->xCoordBase = (int)(global::dcos[this->field_AC] * 300.0 + 320.0);
      this->yCoordBase = (int)(global::dsin[this->field_B0] * 160.0 + 200.0);

      this->sprite_x = (this->xCoordBase + this->sprite_x) / 2;
      this->sprite_y = (this->yCoordBase + this->sprite_y) / 2;

      v1 = static_cast<int>(calculate_angle(
          (double)(this->xCoordBase - this->x_coord),
          (double)(this->yCoordBase - this->y_coord)
      ));
    }

    if (this->chickenFlyType == 3) {
      this->field_AC = (this->field_AC + 1 + 256) % 256;

      this->xCoordBase = (int)(global::dcos[this->field_AC] * 300.0 + 320.0);

      ++this->yCoordBase;

      this->sprite_x = (this->xCoordBase + this->sprite_x) / 2;
      this->sprite_y = (this->yCoordBase + this->sprite_y) / 2;

      v1 = this->field_AC + 64;
    }

    if (this->chickenFlyType == 4) {
      this->field_AC = (this->field_AC + 1 + 256) % 256;

      this->xCoordBase =
          (int)((long double)this->field_B4 * global::dcos[this->field_AC] +
                320.0);
      this->yCoordBase = (int)((long double)this->field_B4 *
                                   global::dsin[this->field_AC] * 0.5 +
                               200.0);

      this->sprite_x = (this->xCoordBase + this->sprite_x) / 2;
      this->sprite_y = (this->yCoordBase + this->sprite_y) / 2;

      v1 = this->field_AC + 64;
    }

    if (this->chickenFlyType == 5 || this->chickenFlyType == 6) {
      this->xCoordBase =
          (int)((long double)this->field_B4 * global::dcos[this->field_AC] +
                (long double)this->xCoordBase);
      this->yCoordBase =
          (int)((long double)this->field_B4 * global::dsin[this->field_AC] +
                (long double)(int)this->yCoordBase);

      this->sprite_x = (this->xCoordBase + this->sprite_x) / 2;
      this->sprite_y = (this->yCoordBase + this->sprite_y) / 2;

      v1 = this->field_AC;
    }

    if (this->sprite_y > 500)
      this->hasBeenDisposed = true;

    this->boundaryBouncer.update();

    if (this->chickenType == 0)
      this->surface = global::game->surface_chain_chickenb->getSurf(
          this->boundaryBouncer.currentSurfaceIndex
      );
    else if (this->chickenType - 2 == 1) {
      int v10 = (v1 + 4) / 8 + ((v1 + 4) / 8 < 0 ? 32 : 0);

      if (v10 > 31)
        v10 -= 32;

      this->surface = global::game->surface_chain_chicken_attack->getSurf(
          this->boundaryBouncer.currentSurfaceIndex + 8 * v10
      );
    }

    UveDX::Sprite::update();

    if (!(generate_random_number() % 500)) {
      auto currentSytem =
          global::game->gameController->getWaveController()->getCurrentSystem();

      int v13 = currentSytem + 5 == 0
                    ? 0
                    : generate_random_number() % (currentSytem + 5);

      global::game->gameController->egg_list->add(
          new Egg{this->uveDX, this->sprite_x, this->sprite_y, v13 + 2}
      );

      global::game->playSound(global::game->sound_fx110, this->sprite_x);
    }
  } else
    --this->invisibilityTimeout;
}

void Chicken::handleHit(unsigned int playerId, int damages) {
  if (this->health > 0 && this->invisibilityTimeout <= 0) {
    this->health -= damages;
    this->sprite_y -= 5;

    if (this->health <= 0) {
      global::game->playSound(global::game->sound_rdfx31, this->sprite_x);

      auto gameController = global::game->gameController;

      gameController->chicken_leg_list->add(new ChickenLeg{
          this->uveDX, (double)this->sprite_x, (double)this->sprite_y
      });

      void* mem = std::malloc(0x2344);
      std::memset(mem, 0, 0x2344);

      gameController->explosion_smoke_list->add(new (mem) Smoke{
          this->uveDX, this->sprite_x, this->sprite_y, 50, 512, 0, 256, true
      });

      auto player = global::game->gameController->players.at(playerId);
      auto waveController = gameController->getWaveController();

      if (player)
        player->increaseScoreBy(
            100 * waveController->getCurrentSystem() + 1000
        );

      this->hasBeenDisposed = true;

      if (!waveController->getHasSpawnedGiftThisWave() &&
          generate_random_number() % 50 == 0) {
        gameController->bonus_list->add(
            new Bonus{this->uveDX, this->sprite_x, this->sprite_y}
        );

        waveController->setHasSpawnedGiftThisWave(true);
      }
    }
  }
}

void Chicken::handleHitPlayer(unsigned int playerId) {
  this->handleHit(playerId, 100);

  global::game->gameController->players.at(playerId)->handleHit();
}
