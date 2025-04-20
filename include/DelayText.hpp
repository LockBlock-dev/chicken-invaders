#pragma once
#include "UveBase.hpp"
#include <string>

class DelayText : public UveDX::UveBase {
public:
  DelayText(UveDX::UveDX *uveDX, int a2, const std::string &text, int a4,
            int a5);

  void update() override;

private:
  const std::string text;
  int dstY;
  int delayBeforeDisplay;
  int displayDuration;
};
