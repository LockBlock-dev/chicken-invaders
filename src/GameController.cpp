#include "GameController.hpp"
#include "Game.hpp"
#include "HighScoreEditor.hpp"
#include "constants.hpp"
#include <format>
#include <fstream>

GameController::GameController(UveDX::UveDX *uveDX)
    : UveDX::UveListOwner(uveDX), StateBase(GameState::Default),
      background(new Background(this->uveDX)), player1(nullptr),
      player2(nullptr), player1Spawned(false), player2Spawned(false),
      waveController(nullptr),
      bullet_missile_list(new UveDX::UveListOwner(this->uveDX)),
      egg_list(new UveDX::UveListOwner(this->uveDX)),
      chicken_leg_list(new UveDX::UveListOwner(this->uveDX)),
      feather_list(new UveDX::UveListOwner(this->uveDX)),
      explosion_smoke_list(new UveDX::UveListOwner(this->uveDX)),
      asteroid_explosion_smoke_list(new UveDX::UveListOwner(this->uveDX)),
      bonus_list(new UveDX::UveListOwner(this->uveDX)),
      highscoreEditor_list(new UveDX::UveListOwner(this->uveDX)),
      graphicsQuality(true), cheatsEnabled(false), highscorePseudos(5, "empty"),
      highscores(5), field_98(0) {
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

  for (size_t i = 0; i < 5; ++i)
    this->highscores.at(i) = 10000 * (10 - i);

  this->readScoresFile();
}

GameController::~GameController() { this->saveScoresFile(); }

void GameController::update() {
  this->processTick();

  UveListOwner::update();

  ++this->field_98;

  switch (this->previousState) {
  case GameState::Menu: {
    global::game->surface_logo_small->blit(320, 75, nullptr, 1.0);

    global::game->font_3x7->blitText(10, 440, "a-d move left-right", 0);

    global::game->font_3x7->blitText(10, 450, "left shift fires", 0);

    global::game->font_3x7->blitText(10, 460, "left control or s fires missile",
                                     0);

    global::game->font_3x7->blitText(630, 440, "arrows move left-right", 2);

    global::game->font_3x7->blitText(630, 450, "right shift fires", 2);

    global::game->font_3x7->blitText(630, 460,
                                     "right control or enter fires missile", 2);

    global::game->font_3x7->blitText(320, 460, "f1 toggles detail level", 6);

    if (this->field_98 / 100 % 2) {
      if (this->uveDX->totalFramesRendered / 8 % 2 == 0)
        global::game->font_alphabet_small->blitText(320, 250,
                                                    "press fire to start", 6);
    } else {
      for (std::size_t i = 0; i < this->highscorePseudos.size(); ++i) {
        global::game->font_alphabet_small->blitText(
            310, 25 * i + 200, this->highscorePseudos.at(i), 2);

        global::game->font_alphabet_small->blitText(320, 25 * i + 200, "...",
                                                    6);

        global::game->font_alphabet_small->blitText(
            330, 25 * i + 200, std::to_string(this->highscores.at(i)), 0);
      }
    }

    unsigned int currentPlayerId = 0;
    auto currentPlayerJoinKey = playerShootKeys;

    do {
      if (this->uveDX->uveInput->isKeyPressed(*currentPlayerJoinKey)) {
        this->switchState(GameState::InGame, 1);
        this->addPlayer(currentPlayerId);
      }

      ++currentPlayerId;
      ++currentPlayerJoinKey;
    } while (currentPlayerId < 2);

    break;
  }

  case GameState::GameOver: {
    if (!this->highscoreEditor_list->getHead())
      this->switchState(GameState::Menu, 1);

    break;
  }

  case GameState::InGame: {
    bool *currentPlayerSpawned = &this->player1Spawned;
    unsigned int currentPlayerId = 0;
    Player **currentPlayer = &this->player1;
    int *currentPlayerScore = &this->player1score;
    auto currentPlayerJoinKey = playerShootKeys;

    do {
      if (*currentPlayer) {
        auto player = *currentPlayer;

        player->drawInterface();

        if (player->getLives() <= 0) {
          *currentPlayerScore = player->getScore();

          this->remove(player);

          if (player)
            delete player;

          *currentPlayer = nullptr;

          if (this->countAlivePlayers() == 0)
            this->waveController->switchState(LevelState::GameOver, 1);
        }
      } else {
        if (!(this->uveDX->totalFramesRendered / 8 % 2)) {
          if (*currentPlayerSpawned)
            global::game->font_3x7->blitText(590 * currentPlayerId + 8, 5,
                                             "game over!", 0);
          else {
            global::game->font_3x7->blitText(590 * currentPlayerId + 8, 5,
                                             "press fire", 0);
            global::game->font_3x7->blitText(592 * currentPlayerId + 10, 13,
                                             "to join in", 0);
          }
        }

        if (this->uveDX->uveInput->isKeyPressed(*currentPlayerJoinKey))
          this->addPlayer(currentPlayerId);
      }

      ++currentPlayerId;
      ++currentPlayerJoinKey;
      ++currentPlayerSpawned;
      ++currentPlayer;
      ++currentPlayerScore;
    } while (currentPlayerId < 2);

    if (this->cheatsEnabled && this->waveController) {
      global::game->font_3x7->blitText(
          320, 8,
          std::format("wave {}",
                      std::to_string(this->waveController->getCurrentWave())),
          6);

      global::game->font_3x7->blitText(
          320, 16,
          std::format("stage {}",
                      std::to_string(this->waveController->getCurrentStage())),
          6);

      global::game->font_3x7->blitText(
          320, 24,
          std::format("system {}",
                      std::to_string(this->waveController->getCurrentSystem())),
          6);
    }

    break;
  }

  default:
    break;
  }

  if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F9) &&
      this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F10)) {
    this->cheatsEnabled = !this->cheatsEnabled;

    global::game->messenger->showSecondaryMessage(
        this->cheatsEnabled ? "cheat on" : "cheat off", 0, 50, 0);

    if (this->cheatsEnabled)
      this->uveDX->debugMode = UveDX::UveDX::DebugLevel::FRAME_TIME_ONLY;
  }

  if (this->cheatsEnabled &&
      this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F5)) {
    global::game->font_3x7->blitText(10, 50, "f6: skip wave", 0);
    global::game->font_3x7->blitText(10, 60, "f7: more weapons", 0);
    global::game->font_3x7->blitText(10, 70, "f8: extra life", 0);
  }

  if (this->uveDX->uveInput->isKeyPressed(sf::Keyboard::Scancode::F1)) {
    this->graphicsQuality = !this->graphicsQuality;

    global::game->messenger->showPrimaryMessage(
        this->graphicsQuality ? "high detail" : "low detail", 0, 50, 0);
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
    s.read(reinterpret_cast<char *>(&this->highscores.at(i)), sizeof(int));
  }
}

void GameController::saveScoresFile() {
  std::ofstream s{"ChickenInvaders.hst", std::ios::binary};

  for (size_t i = 0; i < 5; ++i) {
    s.write(this->highscorePseudos.at(i).c_str(), 10);
    s.write(reinterpret_cast<const char *>(&this->highscores.at(i)),
            sizeof(int));
  }
}

unsigned int GameController::countAlivePlayers() {
  unsigned int count = 0;
  int playerIndex = 0;
  Player **currentPlayer = &this->player1;

  do {
    if (*currentPlayer)
      ++count;

    ++playerIndex;
    ++currentPlayer;
  } while (playerIndex < 2);

  return count;
}

void GameController::addPlayer(unsigned int playerId) {
  if (this->waveController &&
      this->waveController->previousState == LevelState::LevelMessage)
    return;

  if (*(&this->player1Spawned + playerId) == true)
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

  *(&this->player1 + playerId) = newPlayer;

  this->emplaceAfter(newPlayer, this->chicken_leg_list);

  *(&this->player1Spawned + playerId) = true;

  if (global::game->sound_fanfar1->uveDX->soundEnabled)
    global::game->sound_fanfar1->play();
}

void GameController::handlePostLevelTransition() {
  switch (this->previousState) {
  case GameState::Menu: {
    auto sound = global::game->sound_2001;

    if (!sound->isPlaying() && sound->uveDX->soundEnabled)
      sound->play();

    bool *currentPlayerSpawned = &this->player1Spawned;
    unsigned int currentPlayerId = 0;

    do {
      *currentPlayerSpawned = false;

      ++currentPlayerId;
      ++currentPlayerSpawned;
    } while (currentPlayerId < 2);

    currentPlayerId = 0;
    Player **currentPlayer = &this->player1;
    do {
      if (*currentPlayer) {
        this->remove(*currentPlayer);

        if (*currentPlayer)
          delete *currentPlayer;

        *currentPlayer = nullptr;
      }

      ++currentPlayerId;
      ++currentPlayer;
    } while (currentPlayerId < 2);

    currentPlayerId = 0;
    auto currentPlayerScore = &this->player1score;
    do {
      *currentPlayerScore = 0;

      ++currentPlayerId;
      ++currentPlayerScore;
    } while (currentPlayerId < 2);

    this->field_98 = 0;

    break;
  }
  case GameState::GameOver: {
    bool *currentPlayerSpawned = &this->player1Spawned;
    unsigned int currentPlayerId = 0;
    auto currentPlayerScore = &this->player1score;

    do {
      if (*currentPlayerSpawned &&
          *currentPlayerScore > this->highscores.back())
        this->highscoreEditor_list->add(new HighScoreEditor(
            this->uveDX, currentPlayerId, *currentPlayerScore));

      ++currentPlayerId;
      ++currentPlayerScore;
      ++currentPlayerSpawned;
    } while (currentPlayerId < 2);

    break;
  }
  case GameState::InGame: {
    auto sound = global::game->sound_2001;

    if (sound->uveDX->soundEnabled)
      sound->stop();

    break;
  }
  default:
    break;
  }
}

void GameController::addHighScoreEntry(const std::string &pseudo, int score) {
  if (this->highscores.back() <= score) {
    int currentIndex = this->highscores.size() - 1;

    while (currentIndex >= 0 && score > this->highscores.at(currentIndex)) {
      if (currentIndex < this->highscores.size() - 1) {
        this->highscores.at(currentIndex + 1) =
            this->highscores.at(currentIndex);
        this->highscorePseudos.at(currentIndex + 1) =
            this->highscorePseudos.at(currentIndex);
      }

      --currentIndex;
    }

    size_t insertIndex = currentIndex + 1;

    insertIndex = std::max(static_cast<size_t>(0),
                           std::min(insertIndex, this->highscores.size() - 1));

    this->highscorePseudos.at(insertIndex) = pseudo;
    this->highscores.at(insertIndex) = score;
  }
}

bool GameController::areCheatsEnabled() const { return this->cheatsEnabled; }

bool GameController::getGraphicsQuality() const {
  return this->graphicsQuality;
}

WaveController *GameController::getWaveController() {
  return this->waveController;
}
