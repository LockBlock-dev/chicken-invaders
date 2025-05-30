#include <cstdlib>
#include <memory>

#include "Game.hpp"

int main() {
  global::game = std::make_unique<Game>();

  global::game->init();
  global::game->paint();

  return EXIT_SUCCESS;
}
