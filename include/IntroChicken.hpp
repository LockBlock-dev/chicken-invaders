#pragma once

#include "Sprite.hpp"

class IntroChicken : public UveDX::Sprite {
 public:
  IntroChicken(UveDX::UveDX* uveDX);

  void update() override;

 private:
  int field_94;
  int field_98;
};
