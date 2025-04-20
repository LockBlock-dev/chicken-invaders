#pragma once

#include "UveBase.hpp"
#include <string>

class HighScoreEditor : public UveDX::UveBase {
public:
  HighScoreEditor(UveDX::UveDX *uveDX, unsigned int playerId, int score);

  void update() override;

  std::string pseudo;
  int columnNumber;
  int rowNumber;
  unsigned int playerId;
  int score;
};
