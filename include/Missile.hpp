#pragma once

#include "Sprite.hpp"

class Missile : public UveDX::Sprite {
 public:
  Missile(UveDX::UveDX* uveDX, unsigned int playerId, double x, double y);

  void update() override;

 private:
  double positionX;
  double positionY;
  double velocity;
  unsigned int playerId;
  unsigned int angleToTarget;
};
