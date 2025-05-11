#include "ChickenLeg.hpp"

#include <cmath>

#include "Game.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

ChickenLeg::ChickenLeg(UveDX::UveDX* uveDX, double x, double y)
    : UveDX::Sprite(
          uveDX,
          static_cast<int>(x),
          static_cast<int>(y),
          global::game->surface_chain_chicken_leg->getSurf(0)
      ),
      x(x),
      y(y),
      animationFrame(random_range(0, 256)),
      rotationSpeed(random_range(-32, 32)),
      disposeCountdown(0) {
  unsigned int angle = random_range(0u, 256u);
  unsigned int initialSpeed = random_range(0u, 10u);

  this->velocityX = initialSpeed * global::dcos.at(angle);
  this->velocityY = initialSpeed * global::dsin.at(angle);
}

void ChickenLeg::update() {
  if (this->disposeCountdown > 0 && --this->disposeCountdown == 0)
    this->hasBeenDisposed = true;

  this->x += this->velocityX;
  this->y += this->velocityY;

  if (this->y <= 460.0)
    this->velocityY += 0.5;
  else if (std::fabs(this->velocityY) >= 1.0)
    this->velocityY = -std::fabs(this->velocityY * 0.5);
  else {
    this->velocityY = 0.0;
    this->velocityX = 0.0;
    this->rotationSpeed = 0;

    if (this->disposeCountdown == 0)
      this->disposeCountdown = static_cast<int>(random_range(10, 60));
  }

  this->animationFrame += this->rotationSpeed;

  this->animationFrame = (this->animationFrame + 256) % 256;

  this->sprite_x = static_cast<int>(this->x);
  this->sprite_y = static_cast<int>(this->y);
  this->surface = global::game->surface_chain_chicken_leg->getSurf(
      this->animationFrame / 8
  );

  UveDX::Sprite::update();
}
