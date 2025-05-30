#include "Bonus.hpp"

#include "Game.hpp"
#include "rng.hpp"

Bonus::Bonus(UveDX::UveDX* uveDX, int x, int y)
    : UveDX::Sprite(
          uveDX,
          x,
          y,
          global::game->surface_chain_gift_box->getSurf(random_range(0u, 32u))
      ),
      speed(random_range(2u, 7u)) {}

void Bonus::update() {
  this->sprite_y += this->speed;

  if (this->sprite_y > 500)
    this->hasBeenDisposed = true;

  UveDX::Sprite::update();
}
