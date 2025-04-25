#pragma once

#include "Surface.hpp"
#include "UveBase.hpp"

namespace UveDX {
class Sprite : public UveBase {
 public:
  Sprite(UveDX* uveDX, int x, int y, Surface* surface);

  virtual void update() override;
  bool checkCollisionsWith(Sprite* other) const;
  void setSurface(Surface* surface);

  int sprite_x;
  int sprite_y;

 protected:
  Surface* surface;
  int ukwn_1;
  char ukwn_2;
  char ukwn_3;
  double size;
};
}  // namespace UveDX
