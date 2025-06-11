#include "Background.hpp"

#include <SFML/Graphics.hpp>

#include "Game.hpp"
#include "UveDX/UveDX.hpp"

Background::Background(UveDX::UveDX* uveDX)
    : UveDX::UveBase(uveDX),
      currentScrollSpeed(-1),
      targetScrollSpeed(-1),
      scrollOffset(0),
      shakeOffset(0) {}

void Background::update() {
  if (this->currentScrollSpeed < this->targetScrollSpeed)
    ++this->currentScrollSpeed;

  if (this->currentScrollSpeed > this->targetScrollSpeed)
    --this->currentScrollSpeed;

  this->scrollOffset += this->shakeOffset + this->currentScrollSpeed;

  int tempScrollValue = 0;

  if (this->shakeOffset <= 0) {
    tempScrollValue = this->shakeOffset;

    if (tempScrollValue < 0)
      this->shakeOffset = -tempScrollValue - 1;
  } else
    this->shakeOffset = 1 - this->shakeOffset;

  if (this->shakeOffset != 0)
    this->uveDX->yOffset = -this->shakeOffset;

  this->scrollOffset = (this->scrollOffset + 480) % 480;

  Rect rect{
      0,
      this->scrollOffset,
      static_cast<int>(global::game->surface_salmonsky->getWidth()),
      static_cast<int>(global::game->surface_salmonsky->getHeight()) -
          this->scrollOffset,

  };

  global::game->surface_salmonsky->blit(0, 0, &rect);

  if (this->scrollOffset > 0) {
    rect.y = 0;
    rect.h = this->scrollOffset;

    global::game->surface_salmonsky->blit(
        0,
        static_cast<int>(global::game->surface_salmonsky->getHeight()) -
            this->scrollOffset,
        &rect
    );
  }
}

void Background::setTargetScrollSpeed(int speed) {
  this->targetScrollSpeed = speed;
}

void Background::setShakeOffset(int value) {
  this->shakeOffset = value;
}
