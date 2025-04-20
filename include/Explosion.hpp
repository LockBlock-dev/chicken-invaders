#pragma once

#include "UveBase.hpp"

class Explosion : public UveDX::UveBase {
public:
  Explosion(UveDX::UveDX *uveDX, int x, int y, int a5, int a6, int a7, int a8,
            bool a9);

  void update() override;

private:
  bool field_14;
  int field_18;
  int field_1C;
  int field_20;
  int field_24;
  int field_28;
  int field_2C;
  int field_30;
  int field_34;
  int field_38;
  int field_3C;
};
