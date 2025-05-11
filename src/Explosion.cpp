#include "Explosion.hpp"

#include <cmath>
#include <cstddef>

#include "Game.hpp"
#include "Sprite.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

Explosion::Explosion(
    UveDX::UveDX* uveDX,
    int originX,
    int originY,
    unsigned int particleCount,
    unsigned int speedRange,
    int baseAngle,
    int angleSpread,
    bool gravityEnabled
)
    : UveDX::UveBase(uveDX),
      gravityEnabled(gravityEnabled),
      particleCount(particleCount),
      particles({}) {
  if (!angleSpread)
    angleSpread = 2;

  for (size_t i = 0; i < this->particleCount; ++i) {
    auto& particle = this->particles.at(i);

    particle.originX = originX;
    particle.originY = originY;

    int v16 = random_range(0, angleSpread) - angleSpread / 2;
    int angle = v16 + baseAngle + (v16 + baseAngle < 0 ? 256 : 0);

    if (angle > 255)
      angle -= 256;

    int speed =
        (speedRange == 1 ? 0 : random_range<int>(0, speedRange - 1)) + 1;

    if (angleSpread != 256)
      speed =
          ((1.0 - std::fabs((long double)v16 / (long double)(angleSpread / 2))
           ) *
           (long double)speed);

    particle.velocityX = speed * global::dcos.at(angle);
    particle.velocityY = speed * global::dsin.at(angle);
    particle.speed = speed;
  }
}

void Explosion::update() {
  auto sprite = UveDX::Sprite{this->uveDX, 0, 0, nullptr};

  bool anyAlive = false;

  for (size_t i = 0; i < this->particleCount; ++i) {
    auto& particle = this->particles.at(i);

    if (particle.speed > 0) {
      anyAlive = true;

      particle.originX += particle.velocityX * INVERSE_OF_256;
      particle.originY += particle.velocityY * INVERSE_OF_256;

      if (this->gravityEnabled)
        particle.velocityY += 16.0;

      int lifeFactor = 32 * ((1024 - particle.speed) / 256);

      particle.speed -= 32;

      sprite.sprite_x = static_cast<int>(particle.originX);
      sprite.sprite_y = static_cast<int>(particle.originY);

      auto angle = calculate_angle(particle.velocityX, particle.velocityY);

      sprite.setSurface(
          global::game->surface_chain_sparc->getSurf(angle / 8 + lifeFactor)
      );

      sprite.update();
    }
  }

  if (!anyAlive)
    this->hasBeenDisposed = true;
}
