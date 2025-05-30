#pragma once

#include "BoundaryBouncer.hpp"
#include "UveDX/Sprite.hpp"

class Feather : public UveDX::Sprite {
 public:
  Feather(UveDX::UveDX* uveDX, double x, double y);

  void update() override;

 private:
  double x_coord;
  double y_coord;
  double velocityX;
  double velocityY;
  int disposeCountdown;
  BoundaryBouncer boundaryBouncer;
};
