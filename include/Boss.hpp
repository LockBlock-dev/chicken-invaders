#pragma once

#include "BoundaryBouncer.hpp"
#include "Enemy.hpp"

class Boss : public Enemy {
 public:
  Boss(UveDX::UveDX* uveDX, unsigned int a3);

  void update() override;
  void handleHit(unsigned int playerId, int damages) override;
  void handleHitPlayer(unsigned int playerId) override;

 private:
  double x;
  double y;
  unsigned int movementPatternIndex;
  int angle;
  unsigned int currentTargetStep;
  BoundaryBouncer boundaryBouncer;
};
