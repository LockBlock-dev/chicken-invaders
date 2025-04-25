#include "Sprite.hpp"

#include "UveDX.hpp"

namespace UveDX {
Sprite::Sprite(UveDX* uveDX, int x, int y, Surface* surface)
    : UveBase(uveDX),
      sprite_x(x),
      sprite_y(y),
      surface(surface),
      ukwn_1(0),
      ukwn_2(0),
      ukwn_3(0),
      size(1.0) {}

void Sprite::update() {
  if (this->surface && !this->ukwn_3) {
    int xCoord = 0;
    int yCoord = 0;
    bool v3 = false;

    if (this->ukwn_2)
      xCoord = this->sprite_x;
    else
      xCoord = this->sprite_x - this->uveDX->xOffset;

    if (this->ukwn_2)
      yCoord = this->sprite_y;
    else
      yCoord = this->sprite_y - this->uveDX->yOffset;

    if (this->surface) {
      int v4 = 0;

      if (this->ukwn_2)
        v4 = this->sprite_x;
      else
        v4 = this->sprite_x - this->uveDX->xOffset;

      int v5 = v4 - this->surface->getOffsetX();
      v3 = true;

      if (v5 < 0 || v5 >= static_cast<int>(this->uveDX->width)) {
        int v6 = v5 + this->surface->getWidth();

        if (v6 < 0 || v6 >= static_cast<int>(this->uveDX->width))
          v3 = false;
      }
    }

    if (v3) {
      bool v7 = false;

      if (this->surface) {
        int v9 = 0;

        int v8 = this->ukwn_2 ? this->sprite_y
                              : this->sprite_y - this->uveDX->yOffset;
        v7 =
            ((v9 = v8 - this->surface->getOffsetY(), v9 >= 0) &&
             v9 < static_cast<int>(this->uveDX->height)) ||
            (v9 + surface->getHeight() >= 0 &&
             surface->getHeight() + v9 < static_cast<int>(this->uveDX->height));
      }

      if (v7) {
        this->surface->blit(xCoord, yCoord, nullptr, this->size);

        if (this->uveDX->debugMode ==
            UveDX::DebugLevel::FRAME_TIME_AND_DEBUG_SHAPE)
          this->surface->drawDebugShape(xCoord, yCoord);
      }
    }

    if (this->surface->getField28() <= ++this->ukwn_1) {
      this->ukwn_1 = 0;

      auto nextSurface = this->surface->getNextSurface();

      if (nextSurface)
        this->surface = nextSurface;
      else
        this->hasBeenDisposed = true;
    }
  }
}

bool Sprite::checkCollisionsWith(Sprite* other) const {
  int v4 = this->surface->getWidth() / 2;
  int v17 = v4 + this->sprite_x - this->surface->getOffsetX();
  int v18 =
      this->surface->getHeight() / 2 + sprite_y - this->surface->getOffsetY();
  int v15 = (int)((long double)v4 * this->surface->getScaleFactor());
  int v16 = (int)((long double)(this->surface->getHeight() / 2) *
                  this->surface->getScaleFactor());
  int v13 = other->surface->getWidth() / 2 + other->sprite_x -
            other->surface->getOffsetX();
  int v14 = other->surface->getHeight() / 2 + other->sprite_y -
            other->surface->getOffsetY();
  int v11 = (int)((long double)(other->surface->getWidth() / 2) *
                  other->surface->getScaleFactor());
  int v12 = (int)((long double)(other->surface->getHeight() / 2) *
                  other->surface->getScaleFactor());

  if ((int)(v15 + v17) < v13 - v11 || v16 + v18 < v14 - v12 ||
      (int)(v17 - v15) > v11 + v13 || v18 - v16 > v12 + v14)
    return false;

  int* v8;

  if (v15 <= v16)
    v8 = &v16;
  else
    v8 = &v15;

  int v9 = *v8;

  int* v10;

  if (v11 <= v12)
    v10 = &v12;
  else
    v10 = &v11;

  return ((v18 - v14) * (v18 - v14) + (v17 - v13) * (v17 - v13)) <
         (*v10 + v9) * (*v10 + v9);
}

void Sprite::setSurface(Surface* surface) {
  this->surface = surface;
}
}  // namespace UveDX
