#pragma once

#include "Sprite.hpp"

class IntroChicken : public UveDX::Sprite {
 public:
  IntroChicken(UveDX::UveDX* uveDX);

  void update() override;

 private:
  unsigned int velocity;
  unsigned int angle;
};
