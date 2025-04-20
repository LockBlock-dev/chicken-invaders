#include "Enemy.hpp"

Enemy::Enemy(UveDX::UveDX *uveDX, unsigned int x, unsigned int y,
             UveDX::Surface *surface, int invisibilityTimeout)
    : UveDX::Sprite(uveDX, x, y, surface),
      invisibilityTimeout(invisibilityTimeout), health(0) {}

int Enemy::getInvisibilityTimeout() const { return this->invisibilityTimeout; }
