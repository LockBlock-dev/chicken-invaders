#include "DelaySprite.hpp"

#include "UveDX/UveDX.hpp"

DelaySprite::DelaySprite(
    UveDX::UveDX* uveDX,
    int x,
    int y,
    UveDX::Surface* surface,
    int delayBeforeDisplay,
    int displayDuration
)
    : UveDX::Sprite(uveDX, x, y, surface),
      delayBeforeDisplay(delayBeforeDisplay),
      displayDuration(displayDuration) {}

void DelaySprite::update() {
  if (this->delayBeforeDisplay <= 0) {
    UveDX::Sprite::update();

    if (--this->displayDuration <= 0)
      this->hasBeenDisposed = true;
  } else
    --this->delayBeforeDisplay;
}
