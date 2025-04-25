#include "UveTimer.hpp"

#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

namespace UveDX {
UveTimer::UveTimer(int interval) : clock(), interval(interval) {}

void UveTimer::waitForTimeInterval() {
  auto elapsedTime = this->clock.getElapsedTime().asMilliseconds();
  int timeInterval = this->interval - elapsedTime;

  if (timeInterval > 0)
    sf::sleep(sf::milliseconds(timeInterval));

  this->clock.restart();
}

int UveTimer::getInterval() {
  return this->interval;
}

void UveTimer::setInterval(int interval) {
  this->interval = interval;
}

int UveTimer::getElapsedTimeInMilliseconds() {
  return this->clock.getElapsedTime().asMilliseconds();
}
}  // namespace UveDX
