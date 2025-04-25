#pragma once

#include <functional>

template <typename StateType>
class StateBase {
 public:
  StateBase(StateType previousState)
      : callback(nullptr),
        previousState(previousState),
        elapsedTicks(0),
        currentState(this->previousState),
        switchToNextStateTimeout(0) {}

  void switchState(StateType desiredState, int switchToNextStateTimeout) {
    if (desiredState != this->currentState) {
      this->currentState = desiredState;
      this->switchToNextStateTimeout = switchToNextStateTimeout;
    }
  }

  void processTick() {
    ++this->elapsedTicks;

    if (switchToNextStateTimeout > 0 && (--switchToNextStateTimeout) == 0) {
      this->previousState = this->currentState;
      this->elapsedTicks = 0;

      if (this->callback)
        this->callback();
    }
  }

  std::function<void()> callback;
  StateType previousState;
  int elapsedTicks;
  StateType currentState;
  int switchToNextStateTimeout;
};
