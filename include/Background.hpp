#pragma once

#include "UveBase.hpp"

class Background : public UveDX::UveBase {
 public:
  Background(UveDX::UveDX* uveDX);

  void update() override;
  void setScrollSpeed(int speed);
  void setField20(int value);

 private:
  int field_14;
  int scrollSpeed;
  int field_1C;
  int field_20;
};
