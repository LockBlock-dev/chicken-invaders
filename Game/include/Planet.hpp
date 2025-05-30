#pragma once

#include "Enemy.hpp"

class Planet : public Enemy {
 public:
  Planet(UveDX::UveDX* uveDX, unsigned int x, unsigned int y);

  void update() override;
  void handleHit(
      [[maybe_unused]] unsigned int playerId,
      [[maybe_unused]] int damages
  ) override {};
  void handleHitPlayer([[maybe_unused]] unsigned int playerId) override {};

 private:
  int x_coord;
  int y_coord;
};
