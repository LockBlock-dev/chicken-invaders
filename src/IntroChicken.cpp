#include "IntroChicken.hpp"
#include "Game.hpp"
#include "UveDX.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

IntroChicken::IntroChicken(UveDX::UveDX *uveDX)
    : UveDX::Sprite(uveDX, 0, 0, nullptr),
      field_94(generate_random_number() % 256),
      field_98(generate_random_number() % 5000 + 2000) {}

void IntroChicken::update() {
  int v1 = 20 - this->field_94 / 50;

  if (v1 < 15)
    v1 = 15;

  if (v1 > 20)
    v1 = 20;

  if (++this->field_98 < 0)
    this->field_98 += 256;

  if (this->field_98 > 255)
    this->field_98 -= 256;

  this->field_94 -= this->field_94 / 50 + 1;

  this->x = static_cast<int>(static_cast<double>(this->field_94) *
                                 global::dcos[this->field_98] +
                             320.0);
  this->y = static_cast<int>(static_cast<double>(this->field_94) *
                                 global::dsin[this->field_98] +
                             240.0);

  this->surface = global::game->surface_chain_smoke->getSurf(v1);

  UveDX::Sprite::update();

  if (this->field_94 < 20)
    this->hasBeenDisposed = 1;
}
