#include "Boss.hpp"

#include <array>
#include <cmath>
#include <cstring>

#include "Bonus.hpp"
#include "BoundaryBouncer.hpp"
#include "ChickenLeg.hpp"
#include "Egg.hpp"
#include "Explosion.hpp"
#include "Feather.hpp"
#include "Game.hpp"
#include "Smoke.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

const std::array<unsigned int, 25> bossTargetPositionsX{
    320, 540, 100, 440, 200, 320, 540, 100, 320, 320, 320, 540, 320,
    100, 320, 320, 100, 100, 540, 540, 100, 200, 320, 440, 540,
};

const std::array<unsigned int, 25> bossTargetPositionsY{
    200, 200, 200, 200, 200, 200, 200, 200, 200, 400, 100, 250, 400,
    250, 100, 200, 100, 400, 100, 400, 100, 400, 100, 400, 100,
};

Boss::Boss(UveDX::UveDX* uveDX, unsigned int movementPatternIndex)
    : Enemy(
          uveDX,
          0,
          0,
          global::game->surface_chain_chicken_boss->getSurf(0),
          0,
          static_cast<int>(
              10 * global::game->gameController->getWaveController()
                       ->getCurrentSystem() +
              40
          )
      ),
      x(static_cast<double>(random_range(0, 640))),
      y(-100.0),
      movementPatternIndex(movementPatternIndex),
      angle(128),
      currentTargetStep(0),
      boundaryBouncer(0, 9) {
  this->sprite_x = static_cast<int>(this->x);
  this->sprite_y = static_cast<int>(this->y);
}

void Boss::update() {
  double deltaXToTarget =
      this->x - static_cast<double>(bossTargetPositionsX.at(
                    5 * this->movementPatternIndex + this->currentTargetStep
                ));
  double deltaYToTarget =
      this->y - static_cast<double>(bossTargetPositionsY.at(
                    5 * this->movementPatternIndex + this->currentTargetStep
                ));

  unsigned int targetAngle = calculate_angle(-deltaXToTarget, -deltaYToTarget);
  int adjustedTargetAngle = targetAngle;
  int currentAngle = this->angle;

  if (currentAngle - targetAngle > 128)
    adjustedTargetAngle = targetAngle + 256;

  if (currentAngle - adjustedTargetAngle < -128)
    adjustedTargetAngle -= 256;

  this->angle =
      static_cast<int>(currentAngle * 0.9 + adjustedTargetAngle * 0.1);

  this->angle = (this->angle + 256) % 256;

  this->x =
      global::dcos.at(static_cast<unsigned int>(this->angle)) * 5.0 + this->x;
  this->y =
      global::dsin.at(static_cast<unsigned int>(this->angle)) * 5.0 + this->y;

  if (std::fabs(deltaYToTarget) + std::fabs(deltaXToTarget) < 25.0 &&
      ++this->currentTargetStep > 4)
    this->currentTargetStep = 0;

  this->boundaryBouncer.update();

  this->surface = global::game->surface_chain_chicken_boss->getSurf(
      static_cast<unsigned int>(this->boundaryBouncer.currentSurfaceIndex)
  );
  this->sprite_x = static_cast<int>(this->x);
  this->sprite_y = static_cast<int>(this->y);

  UveDX::Sprite::update();

  if (random_range(0, 10) == 0) {
    unsigned int currentSystem =
        global::game->gameController->getWaveController()->getCurrentSystem();

    unsigned int eggSpeed =
        (currentSystem + 5 == 0 ? 0 : random_range(0u, (currentSystem + 5))) +
        2;

    global::game->gameController->egg_list->add(new Egg{
        this->uveDX, this->sprite_x + random_range(-50, 50),
        this->sprite_y + random_range(0, 50), eggSpeed
    });

    global::game->playSound(global::game->sound_fx110, this->sprite_x);
  }
}

void Boss::handleHit(unsigned int playerId, int damages) {
  if (this->health > 0 && this->invisibilityTimeout <= 0) {
    this->health -= damages;
    this->sprite_y -= 5;

    global::game->playSound(global::game->sound_rdfx31_boss, this->sprite_x);

    auto player = global::game->gameController->players.at(playerId);

    if (player)
      player->increaseScoreBy(250);

    if (global::game->gameController->getGraphicsQuality())
      for (size_t i = 0; i < 2; ++i)
        global::game->gameController->feather_list->add(new Feather{
            this->uveDX, static_cast<double>(this->sprite_x),
            static_cast<double>(this->sprite_y)
        });

    if (this->health <= 0) {
      if (global::game->sound_fx113->uveDX->soundEnabled)
        global::game->sound_fx113->play();

      global::game->gameController->background->setShakeOffset(20);

      for (size_t i = 0; i < 20; ++i)
        global::game->gameController->chicken_leg_list->add(new ChickenLeg{
            this->uveDX,
            static_cast<double>(this->sprite_x + random_range(-32, 68)),
            static_cast<double>(this->sprite_y + random_range(-50, 50))
        });

      for (size_t i = 0; i < 10; ++i) {
        global::game->gameController->explosion_smoke_list->add(new Smoke{
            this->uveDX, this->sprite_x + random_range(-50, 50),
            this->sprite_y + random_range(-50, 50), 50, 1024, 0, 256, true
        });

        global::game->gameController->explosion_smoke_list->add(new Explosion{
            this->uveDX, this->sprite_x, this->sprite_y, 200, 1280, 0, 256, true
        });
      }

      if (player)
        player->increaseScoreBy(static_cast<int>(
            1000 * global::game->gameController->getWaveController()
                       ->getCurrentSystem() +
            10000
        ));

      this->hasBeenDisposed = true;

      auto waveController = global::game->gameController->getWaveController();

      if (!waveController->getHasSpawnedGiftThisWave() &&
          random_range(0, 5) == 0) {
        global::game->gameController->bonus_list->add(
            new Bonus{this->uveDX, this->sprite_x, this->sprite_y}
        );

        waveController->setHasSpawnedGiftThisWave(true);
      }

      if (global::game->gameController->getGraphicsQuality())
        for (size_t i = 0; i < 20; ++i)
          global::game->gameController->feather_list->add(new Feather{
              this->uveDX, static_cast<double>(this->sprite_x),
              static_cast<double>(this->sprite_y)
          });
    }
  }
}

void Boss::handleHitPlayer(unsigned int playerId) {
  global::game->gameController->players.at(playerId)->handleHit();
}
