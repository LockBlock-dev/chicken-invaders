#pragma once

#include <string>

#include "UveBase.hpp"

class DelayText : public UveDX::UveBase {
 public:
  DelayText(
      UveDX::UveDX* uveDX,
      int y,
      const std::string& text,
      int delayBeforeDisplay,
      int displayDuration
  );

  void update() override;

 private:
  const std::string text;
  int y;
  int delayBeforeDisplay;
  int displayDuration;
};
