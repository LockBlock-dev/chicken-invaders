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
      targetX(x),
      targetY(y),
      prevX(x),
      prevY(y),
      angleX(random_range(0u, 256u)),
      angleY(random_range(0u, 256u)),
      motionRadius(random_range(100, 300)),
      chickenFlyType(chickenFlyType),
      chickenType(chickenType),
      boundaryBouncer(0, this->chickenType == 3 ? 7 : 9) {
  if (this->chickenFlyType == 5) {
    this->sprite_x = this->targetX;
    this->sprite_y = this->targetY;
    this->motionRadius =
        global::game->gameController->getWaveController()->getCurrentSystem() +
        5;
    this->angleX = 112;
  }
  if (this->chickenFlyType == 6) {
    this->sprite_x = this->targetX;
    this->sprite_y = this->targetY;
    this->motionRadius =
        global::game->gameController->getWaveController()->getCurrentSystem() +
        5;
    this->angleX = 144;
  }
}

void Chicken::update() {
  unsigned int spriteAngle = 0;

  if (this->invisibilityTimeout <= 0) {
    this->prevX = this->sprite_x;
    this->prevY = this->sprite_y;

    if (this->chickenFlyType == 1) {
      int minXBoundary =
          global::game->gameController->getWaveController()->waveMinX;

      if (minXBoundary >= this->targetX)
        minXBoundary = this->targetX;

      global::game->gameController->getWaveController()->waveMinX =
          minXBoundary;

      int maxXBoundary =
          global::game->gameController->getWaveController()->waveMaxX;

      if (maxXBoundary <= this->targetX)
        maxXBoundary = this->targetX;

      global::game->gameController->getWaveController()->waveMaxX =
          maxXBoundary;

      this->sprite_x = (global::game->gameController->getWaveController()
                            ->boundaryBouncer.currentSurfaceIndex +
                        this->targetX + this->sprite_x) /
                       2;
      this->sprite_y = (this->targetY + this->sprite_y) / 2;

      spriteAngle = 128;
    }

    if (this->chickenFlyType == 2) {
      this->angleX = (this->angleX + 1 + 256) % 256;
      this->angleY = (this->angleY + 1 + 256) % 256;

      this->targetX = (int)(global::dcos.at(this->angleX) * 300.0 + 320.0);
      this->targetY = (int)(global::dsin.at(this->angleY) * 160.0 + 200.0);

      this->sprite_x = (this->targetX + this->sprite_x) / 2;
      this->sprite_y = (this->targetY + this->sprite_y) / 2;

      spriteAngle = calculate_angle(
          static_cast<double>(this->targetX - this->prevX),
          static_cast<double>(this->targetY - this->prevY)
      );
    }

    if (this->chickenFlyType == 3) {
      this->angleX = (this->angleX + 1 + 256) % 256;

      this->targetX = (int)(global::dcos.at(this->angleX) * 300.0 + 320.0);

      ++this->targetY;

      this->sprite_x = (this->targetX + this->sprite_x) / 2;
      this->sprite_y = (this->targetY + this->sprite_y) / 2;

      spriteAngle = this->angleX + 64;
    }

    if (this->chickenFlyType == 4) {
      this->angleX = (this->angleX + 1 + 256) % 256;

      this->targetX = (int)((long double)this->motionRadius *
                                global::dcos.at(this->angleX) +
                            320.0);
      this->targetY = (int)((long double)this->motionRadius *
                                global::dsin.at(this->angleX) * 0.5 +
                            200.0);

      this->sprite_x = (this->targetX + this->sprite_x) / 2;
      this->sprite_y = (this->targetY + this->sprite_y) / 2;

      spriteAngle = this->angleX + 64;
    }

    if (this->chickenFlyType == 5 || this->chickenFlyType == 6) {
      this->targetX = (int)((long double)this->motionRadius *
                                global::dcos.at(this->angleX) +
                            (long double)this->targetX);
      this->targetY = (int)((long double)this->motionRadius *
                                global::dsin.at(this->angleX) +
                            (long double)(int)this->targetY);

      this->sprite_x = (this->targetX + this->sprite_x) / 2;
      this->sprite_y = (this->targetY + this->sprite_y) / 2;

      spriteAngle = this->angleX;
    }

    if (this->sprite_y > 500)
      this->hasBeenDisposed = true;

    this->boundaryBouncer.update();

    if (this->chickenType == 0)
      this->surface = global::game->surface_chain_chickenb->getSurf(
          this->boundaryBouncer.currentSurfaceIndex
      );
    else if (this->chickenType - 2 == 1) {
      unsigned int animationFrameIndex =
          // (spriteAngle + 4) / 8 + ((spriteAngle + 4) / 8 < 0 ? 32 : 0);
          (spriteAngle + 4) / 8;

      if (animationFrameIndex > 31)
        animationFrameIndex -= 32;

      this->surface = global::game->surface_chain_chicken_attack->getSurf(
          this->boundaryBouncer.currentSurfaceIndex + 8 * animationFrameIndex
      );
    }

    UveDX::Sprite::update();

    if (!(random_range(0, 500))) {
      auto currentSytem =
          global::game->gameController->getWaveController()->getCurrentSystem();

      unsigned int eggSpeed =
          (currentSytem + 5 == 0 ? 0 : random_range(0u, currentSytem + 5)) + 2;

      global::game->gameController->egg_list->add(
          new Egg{this->uveDX, this->sprite_x, this->sprite_y, eggSpeed}
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

      gameController->explosion_smoke_list->add(new Smoke{
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
          random_range(0, 50) == 0) {
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
