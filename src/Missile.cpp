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
      positionX(x),
      positionY(y),
      velocity(0),
      playerId(playerId),
      angleToTarget(
          calculate_angle(320.0 - this->positionX, 200.0 - this->positionY)
      ) {}

void Missile::update() {
  unsigned int smokeAngle = this->angleToTarget + 128;

  smokeAngle = (smokeAngle + 256) % 256;

  this->velocity += 0.25;

  this->positionX += global::dcos.at(this->angleToTarget) * this->velocity;
  this->positionY += global::dsin.at(this->angleToTarget) * this->velocity;

  int frameAngleOffset = this->angleToTarget + 4;

  if (frameAngleOffset < 0)
    frameAngleOffset += 7;

  int surfaceNumber = frameAngleOffset / 8;

  surfaceNumber = (surfaceNumber + 32) % 32;

  this->surface = global::game->surface_chain_missile->getSurf(surfaceNumber);

  void* memSmoke = std::malloc(0x2344);
  std::memset(memSmoke, 0, 0x2344);

  global::game->gameController->asteroid_explosion_smoke_list->add(new (memSmoke
  ) Smoke{
      this->uveDX, (int)(global::dcos[smokeAngle] * 14.0 + this->positionX),
      (int)(global::dsin[smokeAngle] * 14.0 + this->positionY), 15, 512,
      static_cast<int>(smokeAngle), 128, true
  });

  this->sprite_x = (int)this->positionX;
  this->sprite_y = (int)this->positionY;

  UveDX::Sprite::update();

  if (this->positionY < 200.0) {
    auto currentHead =
        global::game->gameController->getWaveController()->getHead();

    while (true) {
      if (!currentHead)
        break;

      Enemy* originalHead = dynamic_cast<Enemy*>(currentHead);
      currentHead = currentHead->next;

      originalHead->handleHit(this->playerId, 50);
    }

    global::game->gameController->background->setShakeOffset(20);

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
