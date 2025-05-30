#pragma once

#include "UveDX/Sprite.hpp"

class ChickenLeg : public UveDX::Sprite {
 public:
  ChickenLeg(UveDX::UveDX* uveDX, double x, double y);

  void update() override;

 private:
  double x;
  double y;
  double velocityX;
  double velocityY;
  int animationFrame;
  int rotationSpeed;
  int disposeCountdown;
};
