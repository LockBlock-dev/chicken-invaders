#pragma once

#include "BoundaryBouncer.hpp"
#include "Sprite.hpp"
#include "UveDX.hpp"

class Player : public UveDX::Sprite {
 public:
  Player(UveDX::UveDX* uveDX, unsigned int playerId);

  void update() override;
  void drawInterface();

  void handleBonus();
  void handleChickenLeg();
  void handleHit();
  void handleHitByEnemy();
  void handleHitByEgg();

  unsigned int getLives();
  int getScore();
  void increaseScoreBy(int amount);

 private:
  int x_coord;
  int y_coord;
  int horizontalVelocity;
  unsigned int playerId;
  int spaceshipAngle;
  BoundaryBouncer boundaryBouncer;
  int invisibilityTimeout;
  int shieldTimeout;
  unsigned int lives;
  unsigned int lasersLevel;
  unsigned int missilesCount;
  unsigned int consumedChickenLegs;
  int scoreDisplayed;
  int score;
  bool shotAtPreviousFrame;
};
