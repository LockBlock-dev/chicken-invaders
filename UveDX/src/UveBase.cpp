#include "UveDX/UveBase.hpp"

namespace UveDX {
UveBase::UveBase() : uveDX(nullptr), hasBeenDisposed(false) {}

UveBase::UveBase(UveDX* uveDX) : uveDX(uveDX), hasBeenDisposed(false) {}
}  // namespace UveDX
