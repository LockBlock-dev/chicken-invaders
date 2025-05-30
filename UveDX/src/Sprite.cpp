#include "UveDX/Sprite.hpp"

#include "UveDX/UveDX.hpp"

namespace UveDX {
Sprite::Sprite(UveDX* uveDX, int x, int y, Surface* surface)
    : UveBase(uveDX),
      sprite_x(x),
      sprite_y(y),
      surface(surface),
      animationCounter(0),
      isAbsolutePosition(false),
      isHidden(false),
      size(1.0) {}

void Sprite::update() {
  if (this->surface && !this->isHidden) {
    int x = 0;
    int y = 0;
    bool isSurfaceVisible = false;

    if (this->isAbsolutePosition)
      x = this->sprite_x;
    else
      x = this->sprite_x - this->uveDX->xOffset;

    if (this->isAbsolutePosition)
      y = this->sprite_y;
    else
      y = this->sprite_y - this->uveDX->yOffset;

    if (this->surface) {
      int adjustedSpriteX = 0;

      if (this->isAbsolutePosition)
        adjustedSpriteX = this->sprite_x;
      else
        adjustedSpriteX = this->sprite_x - this->uveDX->xOffset;

      int relativePositionX = adjustedSpriteX - this->surface->getOffsetX();
      isSurfaceVisible = true;

      if (relativePositionX < 0 ||
          relativePositionX >= static_cast<int>(this->uveDX->width)) {
        int adjustedSurfaceRightX =
            relativePositionX + this->surface->getWidth();

        if (adjustedSurfaceRightX < 0 ||
            adjustedSurfaceRightX >= static_cast<int>(this->uveDX->width))
          isSurfaceVisible = false;
      }
    }

    if (isSurfaceVisible) {
      bool shouldRender = false;

      if (this->surface) {
        int y_coord = this->isAbsolutePosition
                          ? this->sprite_y
                          : this->sprite_y - this->uveDX->yOffset;

        int surfaceHeight = static_cast<int>(this->surface->getHeight());
        int screenHeight = static_cast<int>(this->uveDX->getHeight());

        int surfaceTop =
            y_coord - static_cast<int>(this->surface->getOffsetY());
        int surfaceBottom = surfaceTop + surfaceHeight;

        bool overlapsTop = surfaceTop >= 0 && surfaceTop < screenHeight;
        bool overlapsBottom =
            surfaceBottom >= 0 && surfaceBottom < screenHeight;

        shouldRender = overlapsTop || overlapsBottom;
      }

      shouldRender = true;

      if (shouldRender) {
        this->surface->blit(x, y, nullptr, this->size);

        if (this->uveDX->debugMode ==
            UveDX::DebugLevel::FRAME_TIME_AND_DEBUG_SHAPE)
          this->surface->drawDebugShape(x, y);
      }
    }

    if (this->surface->getAnimationDelay() <= ++this->animationCounter) {
      this->animationCounter = 0;

      auto nextSurface = this->surface->getNextSurface();

      if (nextSurface)
        this->surface = nextSurface;
      else
        this->hasBeenDisposed = true;
    }
  }
}

bool Sprite::checkCollisionsWith(Sprite* other) const {
  int halfWidth = this->surface->getWidth() / 2;
  int halfHeight = this->surface->getHeight() / 2;
  int centerX = halfWidth + this->sprite_x - this->surface->getOffsetX();
  int centerY = halfHeight + sprite_y - this->surface->getOffsetY();
  int scaledHalfWidth =
      (int)((long double)halfWidth * this->surface->getScaleFactor());
  int scaledHalfHeight =
      (int)((long double)halfHeight * this->surface->getScaleFactor());
  int otherCenterX = other->surface->getWidth() / 2 + other->sprite_x -
                     other->surface->getOffsetX();
  int otherCenterY = other->surface->getHeight() / 2 + other->sprite_y -
                     other->surface->getOffsetY();
  int otherScaledHalfWidth =
      (int)((long double)(other->surface->getWidth() / 2) *
            other->surface->getScaleFactor());
  int otherScaledHalfHeight =
      (int)((long double)(other->surface->getHeight() / 2) *
            other->surface->getScaleFactor());

  if ((int)(scaledHalfWidth + centerX) < otherCenterX - otherScaledHalfWidth ||
      scaledHalfHeight + centerY < otherCenterY - otherScaledHalfHeight ||
      (int)(centerX - scaledHalfWidth) > otherScaledHalfWidth + otherCenterX ||
      centerY - scaledHalfHeight > otherScaledHalfHeight + otherCenterY)
    return false;

  int thisScaledMaxRadius =
      scaledHalfWidth <= scaledHalfHeight ? scaledHalfHeight : scaledHalfWidth;

  int otherScaledMaxRadius = otherScaledHalfWidth <= otherScaledHalfHeight
                                 ? otherScaledHalfHeight
                                 : otherScaledHalfWidth;

  return ((centerY - otherCenterY) * (centerY - otherCenterY) +
          (centerX - otherCenterX) * (centerX - otherCenterX)) <
         (otherScaledMaxRadius + thisScaledMaxRadius) *
             (otherScaledMaxRadius + thisScaledMaxRadius);
}

void Sprite::setSurface(Surface* surface) {
  this->surface = surface;
}
}  // namespace UveDX
