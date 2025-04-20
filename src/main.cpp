#include "Game.hpp"

Game *global::game = nullptr;

int main() {
  global::game = new Game();

  global::game->init();
  global::game->paint();

  delete global::game;

  return 0;
}
