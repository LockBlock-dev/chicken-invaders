#pragma once

#include "LinkedListNode.hpp"

namespace UveDX {
class UveDX;

class UveBase : public LinkedListNode {
 public:
  UveBase();
  UveBase(UveDX* uveDX);
  UveDX* uveDX;
  bool hasBeenDisposed;
};
}  // namespace UveDX
