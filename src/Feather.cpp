#include "Feather.hpp"

#include "BoundaryBouncer.hpp"
#include "Game.hpp"
#include "rng.hpp"

Feather::Feather(UveDX::UveDX* uveDX, double x, double y)
    : UveDX::Sprite(
          uveDX,
          0,
          0,
          global::game->surface_chain_feather->getSurf(0)
      ),
      x_coord(x),
      y_coord(y),
      velocityX(static_cast<double>(random_range(-10, 10))),
      velocityY(static_cast<double>(random_range(-10, 10))),
      disposeCountdown(random_range(50, 100)),
      boundaryBouncer(0, 15) {}

void Feather::update() {
  this->boundaryBouncer.update();

  if (this->velocityX > 0.0)
    this->velocityX = this->velocityX - 0.5;

  if (this->velocityX < 0.0)
    this->velocityX = this->velocityX + 0.5;

  if (this->velocityY > 0.0)
    this->velocityY = this->velocityY - 0.5;

  if (this->velocityY < 0.0)
    this->velocityY = this->velocityY + 0.5;

  this->x_coord += this->velocityX;
  this->y_coord += this->velocityY + 2.0;

  this->sprite_x = static_cast<int>(this->x_coord);
  this->sprite_y = static_cast<int>(this->y_coord);

  auto surfaceNumber = this->boundaryBouncer.currentSurfaceIndex / 2;

  if (surfaceNumber < 0)
    ++surfaceNumber;

  this->surface = global::game->surface_chain_feather->getSurf(surfaceNumber);

  UveDX::Sprite::update();

  if (--this->disposeCountdown <= 0)
    this->hasBeenDisposed = true;
}
