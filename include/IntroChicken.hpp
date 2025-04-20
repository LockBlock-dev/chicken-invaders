#pragma once
#include "Sprite.hpp"

class IntroChicken : public UveDX::Sprite {
public:
  IntroChicken(UveDX::UveDX *uveDX);

  void update() override;

private:
  int field_98;
  int field_94;
};
