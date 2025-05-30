#pragma once

#include <array>

#include "Particle.hpp"
#include "UveDX/UveBase.hpp"

class Explosion : public UveDX::UveBase {
 public:
  Explosion(
      UveDX::UveDX* uveDX,
      int originX,
      int originY,
      unsigned int particleCount,
      unsigned int speedRange,
      int baseAngle,
      int angleSpread,
      bool gravityEnabled
  );

  void update() override;

 private:
  bool gravityEnabled;
  unsigned int particleCount;
  std::array<Particle, 200> particles;
};
