#pragma once

#include "Sprite.hpp"

class ChickenLeg : public UveDX::Sprite {
 public:
  ChickenLeg(UveDX::UveDX* uveDX, double x, double y);

  void update() override;

 private:
  double double94;
  double double9C;
  double doubleA4;
  double doubleAC;
  int intB4;
  int intB8;
  int intBC;
};
