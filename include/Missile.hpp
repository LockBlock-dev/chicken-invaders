#pragma once

#include "Sprite.hpp"

class Missile : public UveDX::Sprite {
public:
  Missile(UveDX::UveDX *uveDX, unsigned int playerId, double x, double y);

  void update() override;

private:
  double field_94;
  double field_9C;
  double field_A4;
  int playerId;
  int field_B0;
};
