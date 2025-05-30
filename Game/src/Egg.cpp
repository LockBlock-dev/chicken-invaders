#include "Egg.hpp"

#include "Game.hpp"

Egg::Egg(UveDX::UveDX* uveDX, int x, int y, unsigned int speed)
    : UveDX::Sprite(uveDX, x, y, global::game->surface_chain_egg->getSurf(0)),
      brokenStateFrameCount(0),
      speed(speed) {}

void Egg::update() {
  UveDX::Sprite::update();

  if (this->brokenStateFrameCount != 0) {
    if (++this->brokenStateFrameCount == 50)
      this->hasBeenDisposed = true;
  } else {
    this->sprite_y += this->speed;

    if (this->sprite_y > 450) {  // Switch to broken egg state
      global::game->playSound(global::game->sound_fx11, this->sprite_x);

      this->surface = global::game->surface_chain_egg->getSurf(1);
      this->brokenStateFrameCount = 1;
    }
  }
}
