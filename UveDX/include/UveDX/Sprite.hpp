#pragma once

#include "UveDX/Surface.hpp"
#include "UveDX/UveBase.hpp"

namespace UveDX {
class Sprite : public UveBase {
 public:
  Sprite(UveDX* uveDX, int x, int y, Surface* surface);

  void update() override;
  bool checkCollisionsWith(Sprite* other) const;
  void setSurface(Surface* surface);

  int sprite_x;
  int sprite_y;

 protected:
  Surface* surface;
  unsigned int animationCounter;
  bool isAbsolutePosition;
  bool isHidden;
  double size;
};
}  // namespace UveDX
