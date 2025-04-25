#include "Enemy.hpp"

Enemy::Enemy(
    UveDX::UveDX* uveDX,
    unsigned int x,
    unsigned int y,
    UveDX::Surface* surface,
    int invisibilityTimeout
)
    : UveDX::Sprite(uveDX, x, y, surface),
      health(0),
      invisibilityTimeout(invisibilityTimeout) {}

int Enemy::getInvisibilityTimeout() const {
  return this->invisibilityTimeout;
}
