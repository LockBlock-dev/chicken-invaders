#pragma once

enum class GameState {
  Default = 0,
  Menu = 1,
  GameOver = 2,
  InGame = 3,
};

enum class LevelState {
  Default = 0,
  NewWave = 1,
  LevelMessage = 2,
  SystemClear = 3,
  SpawningEnemies = 4,
  GameOver = 5,
  LevelState_6 = 6,
};
