#include "Bonus.hpp"

#include "Game.hpp"
#include "rng.hpp"

Bonus::Bonus(UveDX::UveDX* uveDX, int x, int y)
    : UveDX::Sprite(
          uveDX,
          x,
          y,
          global::game->surface_chain_gift_box->getSurf(
              generate_random_number() % 32
          )
      ),
      field_94(generate_random_number() % 5 + 2) {}

void Bonus::update() {
  this->sprite_y += this->field_94;

  if (this->sprite_y > 500)
    this->hasBeenDisposed = true;

  UveDX::Sprite::update();
}
