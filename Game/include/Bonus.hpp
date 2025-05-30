#pragma once

#include "UveDX/Sprite.hpp"

class Bonus : public UveDX::Sprite {
 public:
  Bonus(UveDX::UveDX* uveDX, int x, int y);

  void update() override;

 private:
  unsigned int speed;
};
