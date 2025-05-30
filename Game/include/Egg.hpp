#pragma once

#include "UveDX/Sprite.hpp"

class Egg : public UveDX::Sprite {
 public:
  Egg(UveDX::UveDX* uveDX, int x, int y, unsigned int speed);

  void update() override;

 private:
  unsigned int brokenStateFrameCount;
  unsigned int speed;
};
