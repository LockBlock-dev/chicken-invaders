#include "DelayText.hpp"

#include "Font.hpp"
#include "Game.hpp"
#include "UveDX.hpp"

DelayText::DelayText(
    UveDX::UveDX* uveDX,
    int y,
    const std::string& text,
    int delayBeforeDisplay,
    int displayDuration
)
    : UveDX::UveBase(uveDX),
      text(text),
      y(y),
      delayBeforeDisplay(delayBeforeDisplay),
      displayDuration(displayDuration) {}

void DelayText::update() {
  if (this->delayBeforeDisplay <= 0) {
    global::game->font_alphabet->blitText(
        320, this->y, this->text, UveDX::Font::TextAlignment::Center
    );

    if (--this->displayDuration <= 0)
      this->hasBeenDisposed = true;
  } else
    --this->delayBeforeDisplay;
}
