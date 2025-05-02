#include "DelayText.hpp"

#include "Font.hpp"
#include "Game.hpp"
#include "UveDX.hpp"

DelayText::DelayText(
    UveDX::UveDX* uveDX,
    int dstY,
    const std::string& text,
    int delayBeforeDisplay,
    int displayDuration
)
    : UveDX::UveBase(uveDX),
      text(text),
      dstY(dstY),
      delayBeforeDisplay(delayBeforeDisplay),
      displayDuration(displayDuration) {}

void DelayText::update() {
  if (this->delayBeforeDisplay <= 0) {
    global::game->font_alphabet->blitText(
        320, this->dstY, this->text, UveDX::Font::TextAlignment::Center
    );

    if (--this->displayDuration <= 0)
      this->hasBeenDisposed = true;
  } else
    --this->delayBeforeDisplay;
}
