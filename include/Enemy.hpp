#pragma once

#include "Sprite.hpp"
#include "Surface.hpp"
#include "UveDX.hpp"

class Enemy : public UveDX::Sprite {
public:
  Enemy(UveDX::UveDX *uveDX, unsigned int x, unsigned int y,
        UveDX::Surface *surface, int invisibilityTimeout);

  int getInvisibilityTimeout() const;
  virtual void handleHit(unsigned int playerId, int damages) = 0;
  virtual void handleHitPlayer(unsigned int playerId) = 0;

protected:
  int health;
  int invisibilityTimeout;
};
