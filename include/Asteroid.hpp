#pragma once

#include "Enemy.hpp"
#include "UveDX.hpp"

class Asteroid : public Enemy {
 public:
  Asteroid(
      UveDX::UveDX* uveDX,
      double x,
      double y,
      int face,
      int type,
      int size,
      int minspeed,
      int maxspeed
  );

  void update() override;
  void handleHit(unsigned int playerId, int damages) override;
  void handleHitPlayer(unsigned int playerId) override;

 private:
  double x_coord;
  double y_coord;
  int face;
  int speed;
  double dx;
  double dy;
  int type;
  int size;
};
