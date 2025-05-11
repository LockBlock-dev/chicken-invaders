#pragma once

#include "Sprite.hpp"

class Bullet : public UveDX::Sprite {
 public:
  Bullet(
      UveDX::UveDX* uveDX,
      unsigned int playerId,
      int x,
      int y,
      unsigned int angle
  );

  void update() override;
  void handleHitEnemy();

 private:
  double x;
  double y;
  unsigned int playerId;
  int travelDistance;
  unsigned int angle;
};
