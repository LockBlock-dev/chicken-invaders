#include "Boss.hpp"
#include "Bonus.hpp"
#include "ChickenLeg.hpp"
#include "Egg.hpp"
#include "Explosion.hpp"
#include "Feather.hpp"
#include "Game.hpp"
#include "Smoke.hpp"
#include "bounding_box.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"
#include <cmath>
#include <cstring>

int dword_44D33C[25] = {320, 540, 100, 440, 200, 320, 540, 100,
                        320, 320, 320, 320, 540, 100, 320, 320,
                        100, 100, 540, 100, 200, 320, 440, 540};

int dword_44D3A0[25] = {200, 200, 200, 400, 100, 250, 400, 250,
                        100, 200, 100, 400, 100, 400, 100, 400,
                        100, 400, 100, 400, 100, 400, 100, 400};

Boss::Boss(UveDX::UveDX *uveDX, unsigned int a3)
    : Enemy(uveDX, 0, 0, global::game->surface_chain_chicken_boss->getSurf(0),
            0),
      field_B4(0), field_AC(a3),
      x_coord((double)(generate_random_number() % 640)), y_coord(-100.0),
      field_B0(128) {
  sub_401688(&this->field_B8, 0, 9, -1, 0);

  this->x = this->x_coord;
  this->y = this->y_coord;
  this->health = 10 * global::game->gameController->getWaveController()
                          ->getCurrentSystem() +
                 40;
}

void Boss::update() {
  int v2 = this->field_B4;

  double v12 = this->x_coord - (double)dword_44D33C[5 * this->field_AC + v2];
  double v14 = this->y_coord - (double)dword_44D3A0[5 * this->field_AC + v2];

  int v3 = calculate_angle(-v12, -v14);
  int v4 = v3;
  int v5 = this->field_B0;

  if (v5 - v3 > 128)
    v4 = v3 + 256;

  if (v5 - v4 < -128)
    v4 -= 256;

  this->field_B0 = ((double)v5 * 0.9 + (double)v4 * 0.1);

  if (this->field_B0 < 0)
    this->field_B0 += 256;

  if (this->field_B0 > 255)
    this->field_B0 -= 256;

  this->x_coord = global::dcos[this->field_B0] * 5.0 + this->x_coord;
  this->y_coord = global::dsin[this->field_B0] * 5.0 + this->y_coord;

  int v13 = std::fabs(v12);

  if (std::fabs(v14) + v13 < 25.0 && ++this->field_B4 > 4)
    this->field_B4 = 0;

  sub_4016F8(&this->field_B8);

  this->surface =
      global::game->surface_chain_chicken_boss->getSurf(this->field_C0);
  this->x = this->x_coord;
  this->y = this->y_coord;

  UveDX::Sprite::update();

  if (generate_random_number() % 10 == 0) {
    unsigned int currentSystem =
        global::game->gameController->getWaveController()->getCurrentSystem();

    int v8 = currentSystem == -5
                 ? 0
                 : generate_random_number() % (currentSystem + 5);

    global::game->gameController->egg_list->add(
        new Egg{this->uveDX, this->x + generate_random_number() % 100 - 50,
                this->y + generate_random_number() % 50, v8 + 2});

    global::game->playSound(global::game->sound_fx110, this->x);
  }
}

void Boss::handleHit(unsigned int playerId, int damages) {
  if (this->health > 0 && this->invisibilityTimeout <= 0) {
    this->health -= damages;
    this->y -= 5;

    global::game->playSound(global::game->sound_rdfx31_boss, this->x);

    auto player = *(&global::game->gameController->player1 + playerId);

    if (player)
      player->increaseScoreBy(250);

    if (global::game->gameController->getGraphicsQuality()) {
      for (size_t i = 0; i < 2; ++i) {
        global::game->gameController->feather_list->add(
            new Feather{this->uveDX, (double)this->x, (double)this->y});
      }
    }

    if (this->health <= 0) {
      if (global::game->sound_fx113->uveDX->soundEnabled)
        global::game->sound_fx113->play();

      global::game->gameController->background->setField20(20);

      for (size_t j = 0; j < 20; ++j) {
        global::game->gameController->chicken_leg_list->add(new ChickenLeg{
            this->uveDX,
            (double)(this->x + generate_random_number() % 100 - 32),
            (double)(this->y + generate_random_number() % 100 - 50)});
      }

      for (size_t k = 0; k < 10; ++k) {
        void *memSmoke = std::malloc(0x2344);
        std::memset(memSmoke, 0, 0x2344);

        global::game->gameController->explosion_smoke_list->add(
            new (memSmoke) Smoke{this->uveDX,
                                 this->x + generate_random_number() % 100 - 50,
                                 this->y + generate_random_number() % 100 - 50,
                                 50, 1024, 0, 256, true});

        void *memExplosion = std::malloc(0x2344);
        std::memset(memExplosion, 0, 0x2344);

        global::game->gameController->explosion_smoke_list->add(
            new (memExplosion) Explosion{this->uveDX, this->x, this->y, 200,
                                         1280, 0, 256, true});
      }

      if (player)
        player->increaseScoreBy(
            1000 * global::game->gameController->getWaveController()
                       ->getCurrentSystem() +
            10000);

      this->hasBeenDisposed = true;

      auto waveController = global::game->gameController->getWaveController();

      if (!waveController->getHasSpawnedGiftThisWave() &&
          generate_random_number() % 5 == 0) {
        global::game->gameController->bonus_list->add(
            new Bonus{this->uveDX, this->x, this->y});

        waveController->setHasSpawnedGiftThisWave(true);
      }

      if (global::game->gameController->getGraphicsQuality()) {
        for (size_t m = 0; m < 20; ++m) {
          global::game->gameController->feather_list->add(
              new Feather{this->uveDX, (double)this->x, (double)this->y});
        }
      }
    }
  }
}

void Boss::handleHitPlayer(unsigned int playerId) {
  (*(&global::game->gameController->player1 + playerId))->handleHit();
}
