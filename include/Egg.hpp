#pragma once

#include "Sprite.hpp"

class Egg : public UveDX::Sprite {
 public:
  Egg(UveDX::UveDX* uveDX, int x, int y, int speed);

  void update() override;

 private:
  int dword94;
  int speed;
};
