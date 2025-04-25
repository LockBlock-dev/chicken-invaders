#include "Messenger.hpp"

#include "Game.hpp"

Messenger::Messenger(UveDX::UveDX* uveDX)
    : UveDX::UveBase(uveDX),
      primaryMessage(""),
      secondaryMessage(""),
      primaryMessageDisplayDuration(0),
      secondaryMessageDisplayDuration(0),
      primaryMessageDelayBeforeDisplay(0),
      secondaryMessageDelayBeforeDisplay(0),
      primaryMessageBlinkFrequency(0),
      secondaryMessageBlinkFrequency(0) {}

void Messenger::update() {
  if (this->primaryMessageDelayBeforeDisplay <= 0) {
    if (this->primaryMessageDisplayDuration > 0) {
      auto blinkFrequency = this->primaryMessageBlinkFrequency;

      if (blinkFrequency == 0 ||
          this->primaryMessageDisplayDuration / blinkFrequency % 2 == 0)
        global::game->font_alphabet->blitText(
            320, 150, this->primaryMessage, 6
        );

      --this->primaryMessageDisplayDuration;
    }
  } else
    --this->primaryMessageDelayBeforeDisplay;

  if (this->secondaryMessageDelayBeforeDisplay <= 0) {
    if (this->secondaryMessageDisplayDuration > 0) {
      auto blinkFrequency = this->secondaryMessageBlinkFrequency;

      if (blinkFrequency == 0 ||
          this->secondaryMessageDisplayDuration / blinkFrequency % 2 == 0)
        global::game->font_alphabet_small->blitText(
            320, 190, this->secondaryMessage, 6
        );

      --this->secondaryMessageDisplayDuration;
    }
  } else
    --this->secondaryMessageDelayBeforeDisplay;
}

void Messenger::showPrimaryMessage(
    const std::string& message,
    int delayBeforeDisplay,
    unsigned int displayDuration,
    unsigned int blinkFrequency
) {
  this->primaryMessage = message;
  this->primaryMessageDelayBeforeDisplay = delayBeforeDisplay;
  this->primaryMessageDisplayDuration = displayDuration;
  this->primaryMessageBlinkFrequency = blinkFrequency;
}

void Messenger::showSecondaryMessage(
    const std::string& message,
    int delayBeforeDisplay,
    unsigned int displayDuration,
    unsigned int blinkFrequency
) {
  this->secondaryMessage = message;
  this->secondaryMessageDelayBeforeDisplay = delayBeforeDisplay;
  this->secondaryMessageDisplayDuration = displayDuration;
  this->secondaryMessageBlinkFrequency = blinkFrequency;
}
