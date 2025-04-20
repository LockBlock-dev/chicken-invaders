#include "Player.hpp"
#include "Bullet.hpp"
#include "Enemy.hpp"
#include "Explosion.hpp"
#include "Game.hpp"
#include "Missile.hpp"
#include "Smoke.hpp"
#include "bounding_box.hpp"
#include "constants.hpp"
#include "rng.hpp"
#include <cstring>
#include <string>

Player::Player(UveDX::UveDX *uveDX, unsigned int playerId)
    : UveDX::Sprite(uveDX, 320, 500,
                    global::game->surface_chain_fighter->getSurf(4)),
      x_coord(320), y_coord(400), horizontalVelocity(0), playerId(playerId),
      spaceshipAngle(4), invisibilityTimeout(0), shieldTimeout(50), lives(3),
      lasersLevel(0), missilesCount(0), consumedChickenLegs(0), score(0),
      scoreDisplayed(0), shotAtPreviousFrame(false)

{
  sub_401688(&this->unknown_1, 0, 10, 0, 0);
}

void Player::update() {
  if (this->invisibilityTimeout <= 0) {
    if (this->uveDX->uveInput->isKeyPressed(playerMoveLeftKeys[playerId],
                                            true)) {
      this->horizontalVelocity -= 3;
      this->spaceshipAngle -= 2;
    }

    if (this->uveDX->uveInput->isKeyPressed(playerMoveRightKeys[playerId],
                                            true)) {
      this->horizontalVelocity += 3;
      this->spaceshipAngle += 2;
    }

    bool isShootKeyPressed =
        this->uveDX->uveInput->isKeyPressed(playerShootKeys[playerId]);

    if (isShootKeyPressed && !this->shotAtPreviousFrame) {
      global::game->playSound(global::game->sound_tr3_239, this->x);

      this->y += 10;

      if (this->lasersLevel != 1 && this->lasersLevel != 4 &&
          this->lasersLevel != 6)
        global::game->gameController->bullet_missile_list->add(
            new Bullet{this->uveDX, this->playerId, this->x, this->y - 20, 0});

      if (this->lasersLevel == 1 || this->lasersLevel == 2) {
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x - 10, this->y - 10, 0});
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x + 10, this->y - 10, 0});
      }

      if (this->lasersLevel >= 3) {
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x - 10, this->y - 15, 248});
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x + 10, this->y - 15, 8});
      }

      if (this->lasersLevel >= 4) {
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x - 20, this->y - 10, 240});
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x + 20, this->y - 10, 16});
      }

      if (this->lasersLevel >= 6) {
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x - 5, this->y - 18, 252});
        global::game->gameController->bullet_missile_list->add(new Bullet{
            this->uveDX, this->playerId, this->x + 5, this->y - 18, 4});
      }
    }

    this->shotAtPreviousFrame = isShootKeyPressed;

    if (this->missilesCount > 0 &&
        this->uveDX->uveInput->isKeyPressed(playerMissileKeys[playerId])) {
      global::game->gameController->bullet_missile_list->add(new Missile{
          this->uveDX, this->playerId, (double)(this->x), (double)(this->y)});

      global::game->playSound(global::game->sound_gpolice_12, this->x);

      --this->missilesCount;
    }

    if (global::game->gameController->areCheatsEnabled()) {
      if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F7)) {
        ++this->lasersLevel;
        ++this->missilesCount;
      }

      if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F8))
        ++this->lives;
    }

    if (this->horizontalVelocity > 0)
      --this->horizontalVelocity;

    if (this->horizontalVelocity < 0)
      ++this->horizontalVelocity;

    if (this->spaceshipAngle > 4)
      --this->spaceshipAngle;

    if (this->spaceshipAngle < 4)
      ++this->spaceshipAngle;

    if (this->horizontalVelocity < -10)
      this->horizontalVelocity = -10;

    if (this->horizontalVelocity > 10)
      this->horizontalVelocity = 10;

    if (this->spaceshipAngle < 0)
      this->spaceshipAngle = 0;

    if (this->spaceshipAngle > 8)
      this->spaceshipAngle = 8;

    this->x_coord += this->horizontalVelocity;

    if (this->x_coord < 20)
      this->x_coord = 20;

    if (this->x_coord > 620)
      this->x_coord = 620;

    this->surface =
        global::game->surface_chain_fighter->getSurf(this->spaceshipAngle);
    this->x = (this->x_coord + this->x) / 2;
    this->y = (this->y_coord + this->y) / 2;

    int v40 = 4 - this->spaceshipAngle;
    int v41 = (v40 < 0) ? (v40 - 1) / 2 : v40 / 2;

    v41 /= 2;

    if (v41 < 0)
      v41 += 1;

    int v57 = this->y + 12;

    int v54 = this->x - (10 - v41);

    auto exhaust_surface = global::game->surface_chain_exhaust->getSurf(
        this->uveDX->totalFramesRendered % 2 + 2 * this->playerId);

    exhaust_surface->blit(v54, v57, nullptr, 1.0);

    int v55 = this->x + 10 - v41;

    exhaust_surface->blit(v55, v57, nullptr, 1.0);

    if (this->shieldTimeout > 0) {
      --this->shieldTimeout;

      sub_4016F8(&this->unknown_1);

      global::game->surface_chain_shield->getSurf(this->shieldSurfaceNumber)
          ->blit(this->x, this->y, nullptr, 1.0);
    }

    UveDX::Sprite::update();

    if (global::game->gameController->getWaveController()->previousState ==
        LevelState::SystemClear) {
      void *memExplosion = std::malloc(0x2344);
      std::memset(memExplosion, 0, 0x2344);

      global::game->gameController->asteroid_explosion_smoke_list->add(
          new (memExplosion) Explosion{this->uveDX, this->x - 10, this->y + 18,
                                       10, 1280, 128, 32, true});

      void *memExplosionTwo = std::malloc(0x2344);
      std::memset(memExplosionTwo, 0, 0x2344);

      global::game->gameController->asteroid_explosion_smoke_list->add(new (
          memExplosionTwo) Explosion{this->uveDX, this->x + 10, this->y + 18,
                                     10, 1280, 128, 32, true});
    }

    this->handleBonus();

    if (!this->shieldTimeout) {
      this->handleHitByEnemy();
      this->handleHitByEgg();
    }

    this->handleChickenLeg();

    int newScoreDisplayed = (this->score + this->scoreDisplayed) / 2;
    this->scoreDisplayed = newScoreDisplayed;

    if (newScoreDisplayed == this->score - 1)
      this->scoreDisplayed = this->score;
  } else
    --this->invisibilityTimeout;
}

void Player::drawInterface() {
  int v1 = 10;

  if (this->playerId > 0)
    v1 = -10;

  for (size_t i = 0; i < this->lives; ++i)
    global::game->surface_heart->blit(12 * i + 590 * this->playerId + 10, 15,
                                      nullptr, 1.0);

  for (size_t j = 0; j < this->missilesCount; ++j)
    global::game->surface_chain_missile->getSurf(0)->blit(
        j * v1 + 620 * this->playerId + 10, 460, nullptr, 1.0);

  if (this->playerId > 0)
    global::game->font_alphabet_small->blitText(
        540 * this->playerId + 50 -
            global::game->font_alphabet_small->calculateTextWidth(
                std::to_string(this->scoreDisplayed)),
        8, std::to_string(this->scoreDisplayed), 0);
  else
    global::game->font_alphabet_small->blitText(
        50, 8, std::to_string(this->scoreDisplayed), 0);
}

void Player::handleBonus() {
  auto currentHead = global::game->gameController->bonus_list->getHead();

  while (true) {
    if (!currentHead)
      break;

    Sprite *originalHead = dynamic_cast<Sprite *>(currentHead);
    currentHead = currentHead->next;

    if (originalHead->checkCollisionsWith(this)) {
      if (global::game->sound_space->uveDX->soundEnabled)
        global::game->sound_space->play();

      originalHead->hasBeenDisposed = true;

      if (++this->lasersLevel < 0)
        this->lasersLevel = 0;

      return;
    }
  }
}

void Player::handleChickenLeg() {
  auto currentHead = global::game->gameController->chicken_leg_list->getHead();

  while (true) {
    if (!currentHead)
      break;

    Sprite *originalHead = dynamic_cast<Sprite *>(currentHead);
    currentHead = currentHead->next;

    if (originalHead->checkCollisionsWith(this)) {
      global::game->playSound(global::game->sound_chomp, this->x);

      originalHead->hasBeenDisposed = true;

      this->increaseScoreBy(200);

      if (++this->consumedChickenLegs >= 25) {
        this->consumedChickenLegs = 0;

        if (global::game->sound_space->uveDX->soundEnabled)
          global::game->sound_space->play();

        ++this->missilesCount;
      }

      return;
    }
  }
}

void Player::handleHit() {
  global::game->playSound(global::game->sound_fx113, this->x);

  void *memSmoke = std::malloc(0x2344);
  std::memset(memSmoke, 0, 0x2344);

  global::game->gameController->explosion_smoke_list->add(new (memSmoke) Smoke{
      this->uveDX, this->x, this->y, 100, 768, 0, 256, true});

  void *memExplosion = std::malloc(0x2344);
  std::memset(memExplosion, 0, 0x2344);

  global::game->gameController->explosion_smoke_list->add(
      new (memExplosion)
          Explosion{this->uveDX, this->x, this->y, 100, 768, 0, 256, true});

  int random_number = generate_random_number();

  if (random_number % 3 != 0) {
    if (random_number % 3 == 1)
      global::game->messenger->showPrimaryMessage("doh!", 0, 50, 0);
    else if (random_number % 3 == 2)
      global::game->messenger->showPrimaryMessage("ouch!", 0, 50, 0);
  } else
    global::game->messenger->showPrimaryMessage("oops!", 0, 50, 0);

  --this->lives;

  this->lasersLevel /= 2;

  this->y = 500;
  this->invisibilityTimeout = 50;
  this->shieldTimeout = 50;

  global::game->gameController->background->setField20(10);
}

void Player::handleHitByEnemy() {
  auto currentHead =
      global::game->gameController->getWaveController()->getHead();

  while (true) {
    if (!currentHead)
      break;

    Enemy *originalHead = dynamic_cast<Enemy *>(currentHead);
    currentHead = currentHead->next;

    if (originalHead->checkCollisionsWith(this)) {
      originalHead->handleHitPlayer(this->playerId);

      return;
    }
  }
}

void Player::handleHitByEgg() {
  auto currentHead = global::game->gameController->egg_list->getHead();

  while (true) {
    if (!currentHead)
      break;

    Sprite *originalHead = dynamic_cast<Sprite *>(currentHead);
    currentHead = currentHead->next;

    if (originalHead->checkCollisionsWith(this)) {
      originalHead->hasBeenDisposed = true;

      this->handleHit();

      return;
    }
  }
}

unsigned int Player::getLives() { return this->lives; }

int Player::getScore() { return this->score; }

void Player::increaseScoreBy(int amount) { this->score += amount; }
