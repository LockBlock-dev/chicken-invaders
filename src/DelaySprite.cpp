#include "DelaySprite.hpp"

#include "UveDX.hpp"

DelaySprite::DelaySprite(
    UveDX::UveDX* uveDX,
    int x,
    int y,
    UveDX::Surface* surface,
    int a5,
    int a6
)
    : UveDX::Sprite(uveDX, x, y, surface), field_94(a5), field_98(a6) {}

void DelaySprite::update() {
  if (this->field_94 <= 0) {
    UveDX::Sprite::update();

    --this->field_98;

    if (this->field_98 <= 0)
      this->hasBeenDisposed = true;
  } else
    --this->field_94;
}
