#include "WaveController.hpp"

#include <format>

#include "Asteroid.hpp"
#include "Boss.hpp"
#include "BoundaryBouncer.hpp"
#include "Chicken.hpp"
#include "Game.hpp"
#include "Planet.hpp"
#include "State.hpp"
#include "rng.hpp"

WaveController::WaveController(UveDX::UveDX* uveDX)
    : UveDX::UveListOwner(uveDX),
      StateBase(LevelState::Default),
      waveMinX(640),
      waveMaxX(0),
      boundaryBouncer(0, 80, 0),
      hasSpawnedGiftThisWave(false),
      currentWave(0),
      currentStage(0),
      currentSystem(0) {
  this->callback = [this]() { this->handleWave(); };

  this->switchState(LevelState::NewWave, 40);
}

void WaveController::update() {
  this->processTick();

  this->boundaryBouncer.firstSurfaceIndex = 40 - this->waveMinX;
  this->boundaryBouncer.lastSurfaceIndex = 600 - this->waveMaxX;

  this->boundaryBouncer.update();

  this->waveMinX = 640;
  this->waveMaxX = 0;

  UveListOwner::update();

  if (this->previousState == LevelState::SpawningEnemies && this->isEmpty() &&
      global::game->gameController->countAlivePlayers() > 0) {
    if (this->currentStage == 9)
      this->switchState(LevelState::SystemClear, 50);
    else
      this->switchState(LevelState::NewWave, 10);
  }

  if (global::game->gameController->areCheatsEnabled()) {
    if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F6)) {
      this->clear();

      this->switchState(LevelState::NewWave, 10);
    }
  }
}

void WaveController::handleWave() {
  auto asteroidType = Asteroid::AsteroidType::Rock;

  if (this->currentSystem % 2 != 0)
    asteroidType = Asteroid::AsteroidType::Fire;

  switch (this->previousState) {
    case LevelState::NewWave: {
      ++this->currentWave;
      this->hasSpawnedGiftThisWave = false;
      this->currentStage = (this->currentWave - 1) % 10;
      this->currentSystem = (this->currentWave - 1) / 10;

      global::game->gameController->background->setTargetScrollSpeed(-1);

      global::game->messenger->showPrimaryMessage(
          random_range(0u, 8u) == 0 ? "wake up!" : "get ready!", 0, 50, 0
      );

      global::game->messenger->showSecondaryMessage(
          std::format("wave {}", this->currentWave), 15, 20, 0
      );

      this->switchState(LevelState::LevelMessage, 30);

      break;
    }

    case LevelState::LevelMessage: {
      if (this->currentStage == 2 || this->currentStage == 8) {
        global::game->messenger->showPrimaryMessage("warning!", 0, 50, 0);

        global::game->messenger->showSecondaryMessage(
            "asteroids incoming!", 0, 30, 1
        );

        this->switchState(LevelState::SpawningEnemies, 25);
      }

      if (this->currentStage == 5) {
        global::game->messenger->showPrimaryMessage("brace!", 0, 50, 0);

        global::game->messenger->showSecondaryMessage(
            "high-speed asteroids incoming!", 0, 30, 1
        );

        this->switchState(LevelState::SpawningEnemies, 25);
      }

      if (this->currentStage == 6) {
        global::game->messenger->showSecondaryMessage(
            "be quick or be dead", 0, 30, 1
        );

        this->switchState(LevelState::SpawningEnemies, 25);
      }

      if (this->currentStage == 9) {
        global::game->messenger->showSecondaryMessage(
            "show'em who's boss!", 0, 30, 1
        );

        this->switchState(LevelState::SpawningEnemies, 25);
      }

      this->switchState(LevelState::SpawningEnemies, 1);

      break;
    }

    case LevelState::SystemClear: {
      if (global::game->sound_harley1->uveDX->soundEnabled)
        global::game->sound_harley1->play();

      global::game->gameController->background->setTargetScrollSpeed(-64);

      global::game->messenger->showPrimaryMessage("system clear!", 0, 50, 1);

      global::game->messenger->showSecondaryMessage(
          "prepare for warp", 0, 30, 0
      );

      this->switchState(LevelState::NewWave, 150);

      break;
    }

    case LevelState::SpawningEnemies: {
      if (this->currentStage <= 1)
        for (int i = 0; i < 40; ++i)
          this->add(new Chicken{
              this->uveDX, 60 * (i % 8) + 50, 40 * (i / 8) + 60, 0, 1, i
          });

      if (this->currentStage == 2) {
        for (int j = 0; j < 15; ++j) {
          auto y = static_cast<double>(random_range(-640, 480 - 640));
          auto x = static_cast<double>(-random_range(0, 640));

          this->add(new Asteroid{
              this->uveDX, x, y, 96, asteroidType, true, 2,
              this->currentSystem + 5
          });
        }

        for (int k = 0; k < 5; ++k) {
          auto y = static_cast<double>(random_range(-640, 480 - 640));
          auto x = static_cast<double>(-random_range(0, 640));

          this->add(new Asteroid{
              this->uveDX, x, y, 96, asteroidType, false, 4,
              this->currentSystem + 5
          });
        }
      }

      if (this->currentStage == 3 || this->currentStage == 4) {
        if (this->currentSystem % 2)
          for (unsigned int m = 0; m < this->currentSystem + 25; ++m)
            this->add(new Chicken{this->uveDX, 0, 0, 3, 2, 0});

        else
          for (unsigned int n = 0; n < this->currentSystem + 25; ++n)
            this->add(new Chicken{this->uveDX, 0, 0, 3, 4, 0});

        this->add(new Planet{this->uveDX, 320, 200});
      }

      if (this->currentStage == 5) {
        for (int ii = 0; ii < 15; ++ii) {
          auto y = static_cast<double>(-random_range(0, 300));
          auto x = static_cast<double>(random_range(0, 640));

          this->add(new Asteroid{
              this->uveDX, x, y, 128, asteroidType, true, 8,
              this->currentSystem + 10
          });
        }

        for (int jj = 0; jj < 15; ++jj) {
          auto y = static_cast<double>(-1000 - random_range(0, 300));
          auto x = static_cast<double>(random_range(0, 640));

          this->add(new Asteroid{
              this->uveDX, x, y, 128, asteroidType, true, 8,
              this->currentSystem + 10
          });
        }
      }

      if (this->currentStage == 6)
        for (unsigned int kk = 0; kk < this->currentSystem + 5; ++kk)
          this->add(new Chicken{
              this->uveDX, 0, static_cast<int>(10 * kk + 60), 3, 3,
              static_cast<int>(kk)
          });

      if (this->currentStage == 7) {
        for (unsigned int mm = 0; this->currentSystem + 3 > mm; ++mm) {
          for (int nn = 0; nn < 8; ++nn)
            this->add(new Chicken{
                this->uveDX, 80 * nn, -50, 3, 5, static_cast<int>(80 * mm)
            });

          for (int i1 = 0; i1 < 8; ++i1)
            this->add(new Chicken{
                this->uveDX, 80 * i1, -50, 3, 6, static_cast<int>(80 * mm + 40)
            });
        }
      }

      if (this->currentStage == 8) {
        for (int i2 = 0; i2 < 8; ++i2) {
          auto face = 128 - random_range(0u, 32u);
          auto y = static_cast<double>(-random_range(0, 500));
          auto x = static_cast<double>(random_range(0, 320));

          this->add(new Asteroid{
              this->uveDX, x, y, face, asteroidType, true, 5,
              this->currentSystem + 5
          });

          face = random_range(128u, 128u + 32u);
          y = static_cast<double>(-random_range(0, 500));
          x = static_cast<double>(random_range(320, 640));

          this->add(new Asteroid{
              this->uveDX, x, y, face, asteroidType, true, 5,
              this->currentSystem + 5
          });

          face = random_range(96u, 64u + 96u);
          y = static_cast<double>(-500 - random_range(0, 500));
          x = static_cast<double>(random_range(0, 640));

          this->add(new Asteroid{
              this->uveDX, x, y, face, asteroidType, false, 8,
              this->currentSystem + 5
          });
        }
      }

      if (this->currentStage == 9)
        this->add(new Boss{this->uveDX, this->currentSystem % 5});

      break;
    }

    case LevelState::GameOver: {
      if (global::game->sound_bonewah->uveDX->soundEnabled)
        global::game->sound_bonewah->play();

      global::game->messenger->showPrimaryMessage("game over!", 0, 100, 1);

      this->switchState(LevelState::LevelState_6, 100);

      break;
    }

    case LevelState::LevelState_6: {
      global::game->gameController->switchState(GameState::GameOver, 1);

      break;
    }

    case LevelState::Default:
    default:
      break;
  }
}

bool WaveController::getHasSpawnedGiftThisWave() const {
  return this->hasSpawnedGiftThisWave;
}

void WaveController::setHasSpawnedGiftThisWave(bool value) {
  this->hasSpawnedGiftThisWave = value;
}

unsigned int WaveController::getCurrentWave() const {
  return this->currentWave;
}

unsigned int WaveController::getCurrentStage() const {
  return this->currentStage;
}

unsigned int WaveController::getCurrentSystem() const {
  return this->currentSystem;
}
