#include "GameController.hpp"

#include <format>
#include <fstream>

#include "Game.hpp"
#include "HighScoreEditor.hpp"
#include "State.hpp"
#include "constants.hpp"

GameController::GameController(UveDX::UveDX* uveDX)
    : UveDX::UveListOwner(uveDX),
      StateBase(GameState::Default),
      background(new Background{this->uveDX}),
      players({}),
      playerSpawned({}),
      waveController(nullptr),
      bullet_missile_list(new UveDX::UveListOwner{this->uveDX}),
      egg_list(new UveDX::UveListOwner{this->uveDX}),
      chicken_leg_list(new UveDX::UveListOwner{this->uveDX}),
      feather_list(new UveDX::UveListOwner{this->uveDX}),
      explosion_smoke_list(new UveDX::UveListOwner{this->uveDX}),
      asteroid_explosion_smoke_list(new UveDX::UveListOwner{this->uveDX}),
      bonus_list(new UveDX::UveListOwner{this->uveDX}),
      highscoreEditor_list(new UveDX::UveListOwner{this->uveDX}),
      graphicsQuality(true),
      cheatsEnabled(false),
      highscorePseudos({}),
      highscores({}),
      playerScores({}),
      frameCounter(0) {
  this->callback = [this]() { this->handlePostLevelTransition(); };

  this->add(this->background);

  this->add(this->asteroid_explosion_smoke_list);

  this->add(this->egg_list);

  this->add(this->bullet_missile_list);

  this->add(this->bonus_list);

  this->add(this->chicken_leg_list);

  this->add(this->feather_list);

  this->add(this->explosion_smoke_list);

  this->add(this->highscoreEditor_list);

  this->switchState(GameState::Menu, 1);

  this->highscorePseudos.fill("empty");

  for (size_t i = 0; i < 5; ++i)
    this->highscores.at(i) = 10000 * (10 - i);

  this->readScoresFile();
}

GameController::~GameController() {
  this->saveScoresFile();
}

void GameController::update() {
  this->processTick();

  UveListOwner::update();

  ++this->frameCounter;

  switch (this->previousState) {
    case GameState::Menu: {
      global::game->surface_logo_small->blit(320, 75);

      global::game->font_3x7->blitText(10, 440, "a-d move left-right");

      global::game->font_3x7->blitText(10, 450, "left shift fires");

      global::game->font_3x7->blitText(
          10, 460, "left control or s fires missile"
      );

      global::game->font_3x7->blitText(
          630, 440, "arrows move left-right", UveDX::Font::TextAlignment::Right
      );

      global::game->font_3x7->blitText(
          630, 450, "right shift fires", UveDX::Font::TextAlignment::Right
      );

      global::game->font_3x7->blitText(
          630, 460, "right control or enter fires missile",
          UveDX::Font::TextAlignment::Right
      );

      global::game->font_3x7->blitText(
          320, 460, "f1 toggles detail level",
          UveDX::Font::TextAlignment::Center
      );

      if (this->frameCounter / 100 % 2) {
        if (this->uveDX->totalFramesRendered / 8 % 2 == 0)
          global::game->font_alphabet_small->blitText(
              320, 250, "press fire to start",
              UveDX::Font::TextAlignment::Center
          );
      } else {
        for (std::size_t i = 0; i < this->highscorePseudos.size(); ++i) {
          global::game->font_alphabet_small->blitText(
              310, 25 * i + 200, this->highscorePseudos.at(i),
              UveDX::Font::TextAlignment::Right
          );

          global::game->font_alphabet_small->blitText(
              320, 25 * i + 200, "...", UveDX::Font::TextAlignment::Center
          );

          global::game->font_alphabet_small->blitText(
              330, 25 * i + 200, std::to_string(this->highscores.at(i))
          );
        }
      }

      for (size_t playerId = 0; playerId < this->players.size(); ++playerId) {
        if (this->uveDX->uveInput->isKeyPressed(
                constants::playerShootKeys[playerId]
            )) {
          this->switchState(GameState::InGame, 1);
          this->addPlayer(playerId);
        }
      }

      break;
    }

    case GameState::GameOver: {
      if (!this->highscoreEditor_list->getHead())
        this->switchState(GameState::Menu, 1);

      break;
    }

    case GameState::InGame: {
      for (size_t playerId = 0; playerId < this->players.size(); ++playerId) {
        auto& player = this->players.at(playerId);

        if (player) {
          player->drawInterface();

          if (player->getLives() <= 0) {
            this->playerScores.at(playerId) = player->getScore();

            this->remove(player);

            if (player)
              delete player;

            player = nullptr;

            if (this->countAlivePlayers() == 0)
              this->waveController->switchState(LevelState::GameOver, 1);
          }
        } else {
          if (!(this->uveDX->totalFramesRendered / 8 % 2)) {
            if (this->playerSpawned.at(playerId))
              global::game->font_3x7->blitText(
                  590 * playerId + 8, 5, "game over!"
              );
            else {
              global::game->font_3x7->blitText(
                  590 * playerId + 8, 5, "press fire"
              );
              global::game->font_3x7->blitText(
                  592 * playerId + 10, 13, "to join in"
              );
            }
          }

          if (this->uveDX->uveInput->isKeyPressed(
                  constants::playerShootKeys[playerId]
              ))
            this->addPlayer(playerId);
        }
      }

      if (this->cheatsEnabled && this->waveController) {
        global::game->font_3x7->blitText(
            320, 8,
            std::format(
                "wave {}",
                std::to_string(this->waveController->getCurrentWave())
            ),
            UveDX::Font::TextAlignment::Center
        );

        global::game->font_3x7->blitText(
            320, 16,
            std::format(
                "stage {}",
                std::to_string(this->waveController->getCurrentStage())
            ),
            UveDX::Font::TextAlignment::Center
        );

        global::game->font_3x7->blitText(
            320, 24,
            std::format(
                "system {}",
                std::to_string(this->waveController->getCurrentSystem())
            ),
            UveDX::Font::TextAlignment::Center
        );
      }

      break;
    }

    case GameState::Default:
    default:
      break;
  }

  if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F9) &&
      this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F10)) {
    this->cheatsEnabled = !this->cheatsEnabled;

    global::game->messenger->showSecondaryMessage(
        this->cheatsEnabled ? "cheat on" : "cheat off", 0, 50, 0
    );

    this->uveDX->debugMode = this->cheatsEnabled
                                 ? UveDX::UveDX::DebugLevel::FRAME_TIME_ONLY
                                 : UveDX::UveDX::DebugLevel::NONE;
  }

  if (this->cheatsEnabled &&
      this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F5)) {
    global::game->font_3x7->blitText(10, 50, "f6: skip wave");
    global::game->font_3x7->blitText(10, 60, "f7: more weapons");
    global::game->font_3x7->blitText(10, 70, "f8: extra life");
  }

  if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F1)) {
    this->graphicsQuality = !this->graphicsQuality;

    global::game->messenger->showPrimaryMessage(
        this->graphicsQuality ? "high detail" : "low detail", 0, 50, 0
    );
  }

  if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::Escape)) {
    if (global::game->handlePauseScreen()) {
      switch (this->previousState) {
        case GameState::Menu:
          global::game->shouldQuit = true;
          break;

        case GameState::GameOver:
          this->highscoreEditor_list->clear();
          break;

        case GameState::InGame:
          this->switchState(GameState::Menu, 1);
          break;

        case GameState::Default:
        default:
          break;
      }
    }
  }
}

void GameController::readScoresFile() {
  if (!this->uveDX->uveFileManager->verifyFileExists("ChickenInvaders.hst"))
    return;

  std::ifstream s{"ChickenInvaders.hst", std::ios::binary};

  for (size_t i = 0; i < 5; ++i) {
    this->highscorePseudos.at(i).resize(10);

    s.read(this->highscorePseudos.at(i).data(), 10);
    s.read(reinterpret_cast<char*>(&this->highscores.at(i)), sizeof(int));
  }
}

void GameController::saveScoresFile() {
  std::ofstream s{"ChickenInvaders.hst", std::ios::binary};

  for (size_t i = 0; i < 5; ++i) {
    auto pseudo = this->highscorePseudos.at(i);

    pseudo.resize(10, '\0');

    s.write(pseudo.data(), 10);
    s.write(
        reinterpret_cast<const char*>(&this->highscores.at(i)), sizeof(int)
    );
  }
}

unsigned int GameController::countAlivePlayers() {
  unsigned int count = 0;

  for (auto player : players)
    if (player)
      ++count;

  return count;
}

void GameController::addPlayer(unsigned int playerId) {
  if (this->waveController &&
      this->waveController->previousState == LevelState::LevelMessage)
    return;

  if (this->playerSpawned.at(playerId))
    return;

  if (this->countAlivePlayers() == 0) {
    if (this->waveController) {
      this->remove(this->waveController);

      if (this->waveController)
        delete this->waveController;

      this->waveController = nullptr;
    }

    this->waveController = new WaveController{this->uveDX};

    this->emplaceAfter(this->waveController, this->egg_list);

    this->egg_list->clear();
  }

  auto newPlayer = new Player{this->uveDX, playerId};

  this->players.at(playerId) = newPlayer;

  this->emplaceAfter(newPlayer, this->chicken_leg_list);

  this->playerSpawned.at(playerId) = true;

  if (global::game->sound_fanfar1->uveDX->soundEnabled)
    global::game->sound_fanfar1->play();
}

void GameController::handlePostLevelTransition() {
  switch (this->previousState) {
    case GameState::Menu: {
      auto sound = global::game->sound_2001;

      if (!sound->isPlaying() && sound->uveDX->soundEnabled)
        sound->play();

      for (auto& spawned : this->playerSpawned)
        spawned = 0;

      for (auto& player : this->players) {
        if (player) {
          this->remove(player);

          if (player)
            delete player;

          player = nullptr;
        }
      }

      for (auto& score : this->playerScores)
        score = 0;

      this->frameCounter = 0;

      break;
    }
    case GameState::GameOver: {
      for (size_t playerId = 0; playerId < this->players.size(); ++playerId) {
        auto spawned = this->playerSpawned.at(playerId);
        auto score = this->playerScores.at(playerId);

        if (spawned && score > this->highscores.back())
          this->highscoreEditor_list->add(new HighScoreEditor{
              this->uveDX, static_cast<unsigned int>(playerId), score
          });
      }

      break;
    }
    case GameState::InGame: {
      auto sound = global::game->sound_2001;

      if (sound->uveDX->soundEnabled)
        sound->stop();

      break;
    }
    case GameState::Default:
    default:
      break;
  }
}

void GameController::addHighScoreEntry(const std::string& pseudo, int score) {
  if (this->highscores.back() <= score) {
    int maximumIndex = this->highscores.size() - 1;
    int index = maximumIndex;

    while (index >= 0 && score > this->highscores.at(index)) {
      if (index < maximumIndex) {
        this->highscores.at(index + 1) = this->highscores.at(index);
        this->highscorePseudos.at(index + 1) = this->highscorePseudos.at(index);
      }

      --index;
    }

    size_t insertIndex = index + 1;

    insertIndex = std::max(
        static_cast<size_t>(0),
        std::min(insertIndex, this->highscores.size() - 1)
    );

    this->highscorePseudos.at(insertIndex) = pseudo;
    this->highscores.at(insertIndex) = score;
  }
}

bool GameController::areCheatsEnabled() const {
  return this->cheatsEnabled;
}

bool GameController::getGraphicsQuality() const {
  return this->graphicsQuality;
}

WaveController* GameController::getWaveController() {
  return this->waveController;
}
