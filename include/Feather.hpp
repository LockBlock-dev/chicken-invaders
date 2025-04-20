#pragma once

#include "Sprite.hpp"

class Feather : public UveDX::Sprite {
public:
  Feather(UveDX::UveDX *uveDX, double x, double y);

  void update() override;

private:
  double x_coord;
  double y_coord;
  double field_A4;
  double field_AC;
  int field_B4;
  int field_B8;
  int field_BC;
  int field_C0;
  int field_C4;
};
