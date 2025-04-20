#include "ChickenLeg.hpp"
#include "Game.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"
#include <cmath>

ChickenLeg::ChickenLeg(UveDX::UveDX *uveDX, double x, double y)
    : UveDX::Sprite(uveDX, x, y,
                    global::game->surface_chain_chicken_leg->getSurf(0)),
      double94(x), double9C(y), intB4(generate_random_number() % 256),
      intB8(generate_random_number() % 64 - 32), intBC(0) {
  int v4 = generate_random_number() % 256;
  int v5 = generate_random_number() % 10;

  this->doubleA4 = (double)v5 * global::dcos[v4];
  this->doubleAC = (double)v5 * global::dsin[v4];
}

void ChickenLeg::update() {
  if (this->intBC > 0 && !--this->intBC)
    this->hasBeenDisposed = true;

  this->double94 = this->doubleA4 + this->double94;
  this->double9C = this->doubleAC + this->double9C;

  if (this->double9C <= 460.0)
    this->doubleAC = this->doubleAC + 0.5;
  else if (std::fabs(this->doubleAC) >= 1.0)
    this->doubleAC = -std::fabs(this->doubleAC * 0.5);
  else {
    this->doubleAC = 0.0;
    this->doubleA4 = 0.0;
    this->intB8 = 0;

    if (!this->intBC)
      this->intBC = generate_random_number() % 50 + 10;
  }

  this->intB4 += this->intB8;

  if (this->intB4 < 0)
    this->intB4 += 256;

  if (this->intB4 > 255)
    this->intB4 -= 256;

  this->x = this->double94;
  this->y = this->double9C;
  this->surface =
      global::game->surface_chain_chicken_leg->getSurf(this->intB4 / 8);

  UveDX::Sprite::update();
}
