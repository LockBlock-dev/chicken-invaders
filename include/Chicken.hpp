#pragma once

#include "Enemy.hpp"
#include "UveDX.hpp"

class Chicken : public Enemy {
public:
  Chicken(UveDX::UveDX *uveDX, int x, int y, int chickenType,
          int chickenFlyType, int invisibilityTimeout);

  void update() override;
  void handleHit(unsigned int playerId, int damages) override;
  void handleHitPlayer(unsigned int playerId) override;

private:
  int xCoordBase;
  int yCoordBase;
  int x_coord;
  int y_coord;
  int field_AC;
  int field_B0;
  int field_B4;
  int chickenFlyType;
  int chickenType;
  int dwordC0;
  int animationFramesMaxIndex;
  int currentAnimationFrame;
  int dwordCC;
};
