#pragma once

#include "Sprite.hpp"

class DelaySprite : public UveDX::Sprite {
 public:
  DelaySprite(
      UveDX::UveDX* uveDX,
      int x,
      int y,
      UveDX::Surface* surface,
      int a5,
      int a6
  );

  virtual void update() override;

 private:
  int field_94;
  int field_98;
};
