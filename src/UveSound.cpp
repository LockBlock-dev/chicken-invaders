#include "UveSound.hpp"

#include "UveDX.hpp"

namespace UveDX {
UveSound::UveSound(UveDX* uveDX) : UveBase(uveDX), ready(true) {}

void UveSound::update() {}

void UveSound::create(UveDX* uveDX) {
  uveDX->uveSound = new UveSound{uveDX};

  uveDX->log("Creating UveSound object...");
}

bool UveSound::isReady() const {
  return this->ready;
}
}  // namespace UveDX
