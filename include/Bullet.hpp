#pragma once

#include "Sprite.hpp"

class Bullet : public UveDX::Sprite {
public:
  Bullet(UveDX::UveDX *uveDX, unsigned int playerId, int x, int y,
         unsigned int angle);

  void update() override;
  void handleHitEnemy();

private:
  double field_94;
  double field_9C;
  int field_A4;
  int field_A8;
  unsigned int angle;
};
