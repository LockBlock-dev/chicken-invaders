#pragma once

#include "UveDX/UveBase.hpp"

namespace UveDX {
class UveSound : public UveBase {
 public:
  UveSound(UveDX* uveDX);

  void update() override;
  static void create(UveDX* uveDX);
  bool isReady() const;

 private:
  bool ready;
};
}  // namespace UveDX
