#pragma once

#include "BoundaryBouncer.hpp"
#include "State.hpp"
#include "StateBase.hpp"
#include "UveDX.hpp"
#include "UveListOwner.hpp"

class WaveController : public UveDX::UveListOwner,
                       public StateBase<LevelState> {
 public:
  WaveController(UveDX::UveDX* uveDX);

  void update() override;
  void handleWave();
  unsigned int getCurrentWave() const;
  unsigned int getCurrentStage() const;
  unsigned int getCurrentSystem() const;
  bool getHasSpawnedGiftThisWave() const;
  void setHasSpawnedGiftThisWave(bool value);
  int field_30;
  int field_34;
  BoundaryBouncer boundaryBouncer;

 private:
  bool hasSpawnedGiftThisWave;
  unsigned int currentWave;
  unsigned int currentStage;
  unsigned int currentSystem;
};
