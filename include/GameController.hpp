#pragma once

#include "Background.hpp"
#include "Player.hpp"
#include "State.hpp"
#include "StateBase.hpp"
#include "UveDX.hpp"
#include "UveListOwner.hpp"
#include "WaveController.hpp"
#include <string>
#include <vector>

class GameController : public UveDX::UveListOwner, public StateBase<GameState> {
public:
  GameController(UveDX::UveDX *uveDX);
  ~GameController();

  void update() override;
  void readScoresFile();
  void saveScoresFile();
  unsigned int countAlivePlayers();
  void addPlayer(unsigned int playerId);
  void handlePostLevelTransition();
  void addHighScoreEntry(const std::string &pseudo, int score);
  bool areCheatsEnabled() const;
  bool getGraphicsQuality() const;
  WaveController *getWaveController();
  Background *background;
  Player *player1;
  Player *player2;
  bool player1Spawned;
  bool player2Spawned;
  WaveController *waveController;
  UveDX::UveListOwner *bullet_missile_list;
  UveDX::UveListOwner *egg_list;
  UveDX::UveListOwner *chicken_leg_list;
  UveDX::UveListOwner *feather_list;
  UveDX::UveListOwner *explosion_smoke_list;
  UveDX::UveListOwner *asteroid_explosion_smoke_list;
  UveDX::UveListOwner *bonus_list;
  UveDX::UveListOwner *highscoreEditor_list;

private:
  bool graphicsQuality;
  bool cheatsEnabled;
  std::vector<std::string> highscorePseudos;
  std::vector<int> highscores;
  int player1score;
  int player2score;
  int field_98;
};
