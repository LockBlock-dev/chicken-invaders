#include "IntroChicken.hpp"

#include "Game.hpp"
#include "UveDX/UveDX.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

IntroChicken::IntroChicken(UveDX::UveDX* uveDX)
    : UveDX::Sprite(uveDX, 0, 0, nullptr),
      velocity(random_range(0u, 256u)),
      angle(random_range(2000u, 7000u)) {}

void IntroChicken::update() {
  unsigned int surfaceNumber = 20 - this->velocity / 50;

  surfaceNumber = std::clamp(surfaceNumber, 15u, 20u);

  this->angle = (this->angle + 1 + 256) % 256;

  this->velocity -= this->velocity / 50 + 1;

  this->sprite_x = static_cast<int>(
      static_cast<double>(this->velocity) * global::dcos.at(this->angle) + 320.0
  );
  this->sprite_y = static_cast<int>(
      static_cast<double>(this->velocity) * global::dsin.at(this->angle) + 240.0
  );

  this->surface = global::game->surface_chain_smoke->getSurf(surfaceNumber);

  UveDX::Sprite::update();

  if (this->velocity < 20)
    this->hasBeenDisposed = 1;
}
