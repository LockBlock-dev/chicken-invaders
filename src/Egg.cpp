#include "Egg.hpp"
#include "Game.hpp"

Egg::Egg(UveDX::UveDX *uveDX, int x, int y, int speed)
    : UveDX::Sprite(uveDX, x, y, global::game->surface_chain_egg->getSurf(0)),
      dword94(0), speed(speed) {}

void Egg::update() {
  UveDX::Sprite::update();

  if (this->dword94 != 0) {
    if (++this->dword94 == 50)
      this->hasBeenDisposed = true;
  } else {
    this->y += this->speed;

    if (this->y > 450) {
      global::game->playSound(global::game->sound_fx11, this->x);

      this->surface = global::game->surface_chain_egg->getSurf(1);
      this->dword94 = 1;
    }
  }
}
