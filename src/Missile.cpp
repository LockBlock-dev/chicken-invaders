#include "Missile.hpp"

#include <cstring>

#include "Enemy.hpp"
#include "Explosion.hpp"
#include "Game.hpp"
#include "Smoke.hpp"
#include "trigonometry.hpp"

Missile::Missile(UveDX::UveDX* uveDX, unsigned int playerId, double x, double y)
    : UveDX::Sprite(
          uveDX,
          0,
          0,
          global::game->surface_chain_missile->getSurf(0)
      ),
      field_94(x),
      field_9C(y),
      field_A4(0),
      playerId(playerId),
      field_B0(calculate_angle(320.0 - this->field_94, 200.0 - this->field_9C)
      ) {}

void Missile::update() {
  int v1 = this->field_B0 + 128;

  if (v1 < 0)
    v1 += 256;

  if (v1 > 255)
    v1 -= 256;

  this->field_A4 += 0.25;

  this->field_94 =
      global::dcos[this->field_B0] * this->field_A4 + this->field_94;
  this->field_9C =
      global::dsin[this->field_B0] * this->field_A4 + this->field_9C;

  int v2 = this->field_B0 + 4;

  if (v2 < 0)
    v2 += 7;

  int surfaceNumber = v2 / 8;

  if (surfaceNumber < 0)
    surfaceNumber += 32;

  if (surfaceNumber > 31)
    surfaceNumber -= 32;

  this->surface = global::game->surface_chain_missile->getSurf(surfaceNumber);

  void* memSmoke = std::malloc(0x2344);
  std::memset(memSmoke, 0, 0x2344);

  global::game->gameController->asteroid_explosion_smoke_list->add(new (memSmoke
  ) Smoke{
      this->uveDX, (int)(global::dcos[v1] * 14.0 + this->field_94),
      (int)(global::dsin[v1] * 14.0 + this->field_9C), 15, 512, v1, 128, true
  });

  this->sprite_x = (int)this->field_94;
  this->sprite_y = (int)this->field_9C;

  UveDX::Sprite::update();

  if (this->field_9C < 200.0) {
    auto currentHead =
        global::game->gameController->getWaveController()->getHead();

    while (true) {
      if (!currentHead)
        break;

      Enemy* originalHead = dynamic_cast<Enemy*>(currentHead);
      currentHead = currentHead->next;

      originalHead->handleHit(this->playerId, 50);
    }

    global::game->gameController->background->setField20(20);

    if (global::game->sound_fx113->uveDX->soundEnabled)
      global::game->sound_fx113->play();

    void* memExplosion = std::malloc(0x2344);
    std::memset(memExplosion, 0, 0x2344);

    global::game->gameController->explosion_smoke_list->add(new (memExplosion
    ) Explosion{
        this->uveDX, this->sprite_x, this->sprite_y, 200, 1280, 0, 256, true
    });

    this->hasBeenDisposed = true;
  }
}
