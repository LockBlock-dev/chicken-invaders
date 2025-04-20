#include "Chicken.hpp"
#include "Bonus.hpp"
#include "ChickenLeg.hpp"
#include "Egg.hpp"
#include "Game.hpp"
#include "Smoke.hpp"
#include "bounding_box.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"
#include <cstring>

Chicken::Chicken(UveDX::UveDX *uveDX, int x, int y, int chickenType,
                 int chickenFlyType, int invisibilityTimeout)
    : Enemy(uveDX, 10 * x - 2880, -4 * y,
            global::game->surface_chain_chickenb->getSurf(0),
            invisibilityTimeout),
      xCoordBase(x), x_coord(x), yCoordBase(y), y_coord(y),
      chickenType(chickenType), chickenFlyType(chickenFlyType),
      field_AC(generate_random_number() % 256),
      field_B0(generate_random_number() % 256),
      field_B4(generate_random_number() % 200 + 100) {
  sub_401688(&this->dwordC0, 0, 9, -1, 0);

  if (this->chickenType == 3)
    sub_401688(&this->dwordC0, 0, 7, -1, 0);

  this->health =
      global::game->gameController->getWaveController()->getCurrentSystem() + 1;

  if (this->chickenFlyType == 5) {
    this->x = this->xCoordBase;
    this->y = this->yCoordBase;
    this->field_B4 =
        global::game->gameController->getWaveController()->getCurrentSystem() +
        5;
    this->field_AC = 112;
  }
  if (this->chickenFlyType == 6) {
    this->x = this->xCoordBase;
    this->y = this->yCoordBase;
    this->field_B4 =
        global::game->gameController->getWaveController()->getCurrentSystem() +
        5;
    this->field_AC = 144;
  }
}

void Chicken::update() {
  int v1;

  if (this->invisibilityTimeout <= 0) {
    this->x_coord = this->x;
    this->y_coord = this->y;

    if (this->chickenFlyType == 1) {
      int *v2 = global::game->gameController->getWaveController()->bounding_box;

      if (*v2 >= this->xCoordBase)
        v2 = &this->xCoordBase;

      int v3 = *v2;

      int *v4 =
          &global::game->gameController->getWaveController()->bounding_box[1];

      global::game->gameController->getWaveController()->bounding_box[0] = v3;

      if (*v4 <= this->xCoordBase)
        v4 = &this->xCoordBase;

      global::game->gameController->getWaveController()->bounding_box[1] = *v4;

      this->x = (global::game->gameController->getWaveController()->field_40 +
                 this->xCoordBase + this->x) /
                2;
      this->y = (this->yCoordBase + this->y) / 2;

      v1 = 128;
    }

    if (this->chickenFlyType == 2) {
      if (++this->field_AC < 0)
        this->field_AC += 256;

      if (this->field_AC > 255)
        this->field_AC -= 256;

      if (++this->field_B0 < 0)
        this->field_B0 += 256;

      if (this->field_B0 > 255)
        this->field_B0 -= 256;

      this->xCoordBase = (int)(global::dcos[this->field_AC] * 300.0 + 320.0);
      this->yCoordBase = (int)(global::dsin[this->field_B0] * 160.0 + 200.0);

      this->x = (this->xCoordBase + this->x) / 2;
      this->y = (this->yCoordBase + this->y) / 2;

      v1 = calculate_angle((double)(this->xCoordBase - this->x_coord),
                           (double)(this->yCoordBase - this->y_coord));
    }

    if (this->chickenFlyType == 3) {
      if (++this->field_AC < 0)
        this->field_AC += 256;

      if (this->field_AC > 255)
        this->field_AC -= 256;

      this->xCoordBase = (int)(global::dcos[this->field_AC] * 300.0 + 320.0);

      ++this->yCoordBase;

      this->x = (this->xCoordBase + this->x) / 2;
      this->y = (this->yCoordBase + this->y) / 2;

      v1 = this->field_AC + 0x40;
    }

    if (this->chickenFlyType == 4) {
      if (++this->field_AC < 0)
        this->field_AC += 256;

      if (this->field_AC > 255)
        this->field_AC -= 256;

      this->xCoordBase =
          (int)((long double)this->field_B4 * global::dcos[this->field_AC] +
                320.0);
      this->yCoordBase = (int)((long double)this->field_B4 *
                                   global::dsin[this->field_AC] * 0.5 +
                               200.0);

      this->x = (this->xCoordBase + this->x) / 2;
      this->y = (this->yCoordBase + this->y) / 2;

      v1 = this->field_AC + 0x40;
    }

    if (this->chickenFlyType == 5 || this->chickenFlyType == 6) {
      this->xCoordBase =
          (int)((long double)this->field_B4 * global::dcos[this->field_AC] +
                (long double)this->xCoordBase);
      this->yCoordBase =
          (int)((long double)this->field_B4 * global::dsin[this->field_AC] +
                (long double)(int)this->yCoordBase);

      this->x = (this->xCoordBase + this->x) / 2;
      this->y = (this->yCoordBase + this->y) / 2;

      v1 = this->field_AC;
    }

    if (this->y > 500)
      this->hasBeenDisposed = true;

    sub_4016F8(&this->dwordC0);

    if (this->chickenType == 0)
      this->surface = global::game->surface_chain_chickenb->getSurf(
          this->currentAnimationFrame);
    else if (this->chickenType - 2 == 1) {
      int v10 = (v1 + 4) / 8 + ((v1 + 4) / 8 < 0 ? 32 : 0);

      if (v10 > 31)
        v10 -= 32;

      this->surface = global::game->surface_chain_chicken_attack->getSurf(
          this->currentAnimationFrame + 8 * v10);
    }

    UveDX::Sprite::update();

    if (!(generate_random_number() % 500)) {
      auto currentSytem =
          global::game->gameController->getWaveController()->getCurrentSystem();

      int v13 = currentSytem == -5
                    ? 0
                    : generate_random_number() % (currentSytem + 5);

      global::game->gameController->egg_list->add(
          new Egg{this->uveDX, this->x, this->y, v13 + 2});

      global::game->playSound(global::game->sound_fx110, this->x);
    }
  } else
    --this->invisibilityTimeout;
}

void Chicken::handleHit(unsigned int playerId, int damages) {
  if (this->health > 0 && this->invisibilityTimeout <= 0) {
    this->health -= damages;
    this->y -= 5;

    if (this->health <= 0) {
      global::game->playSound(global::game->sound_rdfx31, this->x);

      auto gameController = global::game->gameController;

      gameController->chicken_leg_list->add(
          new ChickenLeg{this->uveDX, (double)this->x, (double)this->y});

      void *mem = std::malloc(0x2344);
      std::memset(mem, 0, 0x2344);

      gameController->explosion_smoke_list->add(new (mem) Smoke{
          this->uveDX, this->x, this->y, 50, 512, 0, 256, true});

      auto player = *(&gameController->player1 + playerId);
      auto waveController = gameController->getWaveController();

      if (player)
        player->increaseScoreBy(100 * waveController->getCurrentSystem() +
                                1000);

      this->hasBeenDisposed = true;

      if (!waveController->getHasSpawnedGiftThisWave() &&
          generate_random_number() % 50 == 0) {
        gameController->bonus_list->add(
            new Bonus{this->uveDX, this->x, this->y});

        waveController->setHasSpawnedGiftThisWave(true);
      }
    }
  }
}

void Chicken::handleHitPlayer(unsigned int playerId) {
  this->handleHit(playerId, 100);

  (*(&global::game->gameController->player1 + playerId))->handleHit();
}
