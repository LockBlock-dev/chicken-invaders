#pragma once

#include "BoundaryBouncer.hpp"
#include "Enemy.hpp"
#include "UveDX/UveDX.hpp"

class Chicken : public Enemy {
 public:
  Chicken(
      UveDX::UveDX* uveDX,
      int x,
      int y,
      int chickenType,
      int chickenFlyType,
      int invisibilityTimeout
  );

  void update() override;
  void handleHit(unsigned int playerId, int damages) override;
  void handleHitPlayer(unsigned int playerId) override;

 private:
  int targetX;
  int targetY;
  int prevX;
  int prevY;
  unsigned int angleX;
  unsigned int angleY;
  int motionRadius;
  int chickenFlyType;
  int chickenType;
  BoundaryBouncer boundaryBouncer;
};
