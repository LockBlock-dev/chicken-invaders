#include "Bullet.hpp"

#include <cstring>

#include "Enemy.hpp"
#include "Explosion.hpp"
#include "Game.hpp"
#include "trigonometry.hpp"

Bullet::Bullet(
    UveDX::UveDX* uveDX,
    unsigned int playerId,
    int x,
    int y,
    unsigned int angle
)
    : UveDX::Sprite(uveDX, 0, 0, nullptr),
      field_94((double)x),
      field_9C((double)y),
      field_A4(playerId),
      field_A8(0),
      angle(angle) {
  int v6 = this->angle + 4;

  if (v6 < 0)
    v6 += 7;

  v6 /= 8;

  if (v6 < 0)
    v6 += 32;

  if (v6 > 31)
    v6 -= 32;

  this->surface = global::game->surface_chain_crystal->getSurf(v6);
}

void Bullet::update() {
  this->field_94 =
      (double)++this->field_A8 * global::dcos[this->angle] + this->field_94;
  this->field_9C =
      (double)this->field_A8 * global::dsin[this->angle] + this->field_9C;

  this->sprite_x = this->field_94;
  this->sprite_y = this->field_9C;

  UveDX::Sprite::update();

  this->handleHitEnemy();

  int v1;

  if (this->surface) {
    int x_coord =
        this->ukwn_2 ? this->sprite_x : this->sprite_x - this->uveDX->xOffset;

    int v3 = x_coord - this->surface->getOffsetX();
    v1 = 1;

    if (v3 < 0 || v3 >= static_cast<int>(this->uveDX->getWidth())) {
      int v4 = v3 + this->surface->getWidth();

      if (v4 < 0 || v4 >= static_cast<int>(this->uveDX->getWidth()))
        v1 = 0;
    }
  } else
    v1 = 0;

  int v5;
  int v7;
  int v8;
  int y_coord;

  if (!v1 ||
      (this->surface
           ? ((!this->ukwn_2 ? (y_coord = this->sprite_y - this->uveDX->yOffset)
                             : (y_coord = this->sprite_y),
               ((v7 = y_coord - this->surface->getOffsetY(), v7 >= 0) &&
                v7 < static_cast<int>(this->uveDX->getHeight())) ||
                   ((v8 = v7 + this->surface->getHeight(), v8 >= 0) &&
                    v8 < static_cast<int>(this->uveDX->getHeight())))
                  ? (v5 = 1)
                  : (v5 = 0))
           : (v5 = 0),
       !v5))
    this->hasBeenDisposed = true;
}

void Bullet::handleHitEnemy() {
  auto currentHead =
      global::game->gameController->getWaveController()->getHead();

  while (true) {
    if (!currentHead)
      break;

    Enemy* originalHead = dynamic_cast<Enemy*>(currentHead);
    currentHead = currentHead->next;

    if (originalHead->getInvisibilityTimeout() == 0 &&
        originalHead->checkCollisionsWith(this)) {
      originalHead->handleHit(this->field_A4, 1);

      void* mem = std::malloc(0x2344);
      std::memset(mem, 0, 0x2344);

      global::game->gameController->explosion_smoke_list->add(new (mem
      ) Explosion{
          this->uveDX, this->sprite_x, this->sprite_y, 10, 512, 0, 256, true
      });

      this->hasBeenDisposed = true;

      return;
    }
  }
}
