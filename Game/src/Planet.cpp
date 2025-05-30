#include "Planet.hpp"
#include "Game.hpp"
#include "UveDX/UveDX.hpp"

Planet::Planet(UveDX::UveDX* uveDX, unsigned int x, unsigned int y)
    : Enemy(uveDX, x, -100, global::game->surface_earth, 0),
      x_coord(x),
      y_coord(y) {}

void Planet::update() {
  this->sprite_y =
      static_cast<unsigned int>(this->sprite_y * 0.9 + this->y_coord * 0.1);

  UveDX::Sprite::update();

  if (global::game->gameController->getWaveController()->size() == 1)
    this->y_coord = 600;

  if (this->sprite_y > 580)
    this->hasBeenDisposed = true;
}
