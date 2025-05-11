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
      x(static_cast<double>(x)),
      y(static_cast<double>(y)),
      playerId(playerId),
      travelDistance(0),
      angle(angle) {
  auto surfaceNumber = (this->angle + 4) / 8;

  this->surface =
      global::game->surface_chain_crystal->getSurf(surfaceNumber % 32);
}

void Bullet::update() {
  ++this->travelDistance;

  this->x =
      static_cast<double>(this->travelDistance) * global::dcos[this->angle] +
      this->x;
  this->y =
      static_cast<double>(this->travelDistance) * global::dsin[this->angle] +
      this->y;

  this->sprite_x = static_cast<int>(this->x);
  this->sprite_y = static_cast<int>(this->y);

  UveDX::Sprite::update();

  this->handleHitEnemy();

  bool isSurfaceVisible = false;

  if (this->surface) {
    int x_coord = this->isAbsolutePosition
                      ? this->sprite_x
                      : this->sprite_x - this->uveDX->xOffset;

    int adjustedSurfaceX =
        x_coord - static_cast<int>(this->surface->getOffsetX());
    isSurfaceVisible = true;

    if (adjustedSurfaceX < 0 ||
        adjustedSurfaceX >= static_cast<int>(this->uveDX->getWidth())) {
      int surfaceRightEdgeX =
          adjustedSurfaceX + static_cast<int>(this->surface->getWidth());

      if (surfaceRightEdgeX < 0 ||
          surfaceRightEdgeX >= static_cast<int>(this->uveDX->getWidth()))
        isSurfaceVisible = false;
    }
  }

  int y_coord = this->isAbsolutePosition
                    ? this->sprite_y
                    : this->sprite_y - this->uveDX->yOffset;

  int surfaceHeight = static_cast<int>(this->surface->getHeight());
  int screenHeight = static_cast<int>(this->uveDX->getHeight());

  int surfaceTop = y_coord - static_cast<int>(this->surface->getOffsetY());
  int surfaceBottom = surfaceTop + surfaceHeight;

  bool shouldRender = false;

  if (!isSurfaceVisible || !this->surface)
    shouldRender = false;
  else
    shouldRender = (surfaceTop >= 0 && surfaceTop < screenHeight) ||
                   (surfaceBottom >= 0 && surfaceBottom < screenHeight);

  if (!shouldRender)
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
      originalHead->handleHit(this->playerId, 1);

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
