#pragma once

#include <SFML/System/Clock.hpp>

namespace UveDX {
class UveTimer {
public:
  UveTimer(int interval = 0);

  void waitForTimeInterval();
  int getInterval();
  void setInterval(int interval);
  int getElapsedTimeInMilliseconds();

private:
  sf::Clock clock;
  int interval;
};
} // namespace UveDX
