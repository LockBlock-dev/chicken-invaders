#pragma once

#include <string>

#include "UveBase.hpp"

class Messenger : public UveDX::UveBase {
 public:
  Messenger(UveDX::UveDX* uveDX);

  void update() override;
  void showPrimaryMessage(
      const std::string& message,
      int delayBeforeDisplay,
      unsigned int displayDuration,
      unsigned int blinkFrequency
  );
  void showSecondaryMessage(
      const std::string& message,
      int delayBeforeDisplay,
      unsigned int displayDuration,
      unsigned int blinkFrequency
  );

 private:
  std::string primaryMessage;
  std::string secondaryMessage;
  unsigned int primaryMessageDisplayDuration;
  unsigned int secondaryMessageDisplayDuration;
  int primaryMessageDelayBeforeDisplay;
  int secondaryMessageDelayBeforeDisplay;
  unsigned int primaryMessageBlinkFrequency;
  unsigned int secondaryMessageBlinkFrequency;
};
