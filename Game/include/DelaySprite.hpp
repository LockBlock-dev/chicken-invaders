#pragma once

#include "UveDX/Sprite.hpp"

class DelaySprite : public UveDX::Sprite {
 public:
  DelaySprite(
      UveDX::UveDX* uveDX,
      int x,
      int y,
      UveDX::Surface* surface,
      int delayBeforeDisplay,
      int displayDuration
  );

  virtual void update() override;

 private:
  int delayBeforeDisplay;
  int displayDuration;
};
