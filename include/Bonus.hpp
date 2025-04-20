#pragma once

#include "Sprite.hpp"

class Bonus : public UveDX::Sprite {
public:
  Bonus(UveDX::UveDX *uveDX, int x, int y);

  void update() override;

private:
  int field_94;
};
