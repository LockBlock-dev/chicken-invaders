#pragma once

#include <string>

#include "UveDX/UveBase.hpp"

class HighScoreEditor : public UveDX::UveBase {
 public:
  HighScoreEditor(UveDX::UveDX* uveDX, unsigned int playerId, int score);

  void update() override;

  std::string pseudo;
  int columnNumber;
  int rowNumber;
  unsigned int playerId;
  int score;
};
