#pragma once

#include <array>
#include <string>

#include "Background.hpp"
#include "Player.hpp"
#include "State.hpp"
#include "StateBase.hpp"
#include "UveDX/UveDX.hpp"
#include "UveDX/UveListOwner.hpp"
#include "WaveController.hpp"

class GameController : public UveDX::UveListOwner, public StateBase<GameState> {
 public:
  GameController(UveDX::UveDX* uveDX);
  ~GameController() override;

  void update() override;
  void readScoresFile();
  void saveScoresFile();
  unsigned int countAlivePlayers();
  void addPlayer(unsigned int playerId);
  void handlePostLevelTransition();
  void addHighScoreEntry(const std::string& pseudo, int score);
  bool areCheatsEnabled() const;
  bool getGraphicsQuality() const;
  WaveController* getWaveController();
  Background* background;
  std::array<Player*, 2> players;
  std::array<bool, 2> playerSpawned;
  WaveController* waveController;
  UveDX::UveListOwner* bullet_missile_list;
  UveDX::UveListOwner* egg_list;
  UveDX::UveListOwner* chicken_leg_list;
  UveDX::UveListOwner* feather_list;
  UveDX::UveListOwner* explosion_smoke_list;
  UveDX::UveListOwner* asteroid_explosion_smoke_list;
  UveDX::UveListOwner* bonus_list;
  UveDX::UveListOwner* highscoreEditor_list;

 private:
  bool graphicsQuality;
  bool cheatsEnabled;
  std::array<std::string, 5> highscorePseudos;
  std::array<int, 5> highscores;
  std::array<int, 2> playerScores;
  unsigned int frameCounter;
};
