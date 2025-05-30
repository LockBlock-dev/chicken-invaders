#include "Enemy.hpp"

Enemy::Enemy(
    UveDX::UveDX* uveDX,
    int x,
    int y,
    UveDX::Surface* surface,
    int invisibilityTimeout,
    int health
)
    : UveDX::Sprite(uveDX, x, y, surface),
      health(health),
      invisibilityTimeout(invisibilityTimeout) {}

int Enemy::getInvisibilityTimeout() const {
  return this->invisibilityTimeout;
}
