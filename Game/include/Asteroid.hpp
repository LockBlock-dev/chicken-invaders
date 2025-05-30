#pragma once

#include "Enemy.hpp"
#include "UveDX/UveDX.hpp"

class Asteroid : public Enemy {
 public:
  enum class AsteroidType {
    Rock = 2,
    Fire = 3,
  };

  Asteroid(
      UveDX::UveDX* uveDX,
      double x,
      double y,
      unsigned int face,
      AsteroidType type,
      bool size,
      unsigned int minspeed,
      unsigned int maxspeed
  );

  void update() override;
  void handleHit(unsigned int playerId, int damages) override;
  void handleHitPlayer(unsigned int playerId) override;

 private:
  double positionX;
  double positionY;
  unsigned int face;
  unsigned int speed;
  double velocityX;
  double velocityY;
  AsteroidType type;
  bool isSmall;
};
