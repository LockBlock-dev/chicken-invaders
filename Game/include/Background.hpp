#pragma once

#include "UveDX/UveBase.hpp"

class Background : public UveDX::UveBase {
 public:
  Background(UveDX::UveDX* uveDX);

  void update() override;
  void setTargetScrollSpeed(int speed);
  void setShakeOffset(int value);

 private:
  int currentScrollSpeed;
  int targetScrollSpeed;
  int scrollOffset;
  int shakeOffset;
};
