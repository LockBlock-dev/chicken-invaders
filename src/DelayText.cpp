#include "DelayText.hpp"
#include "Game.hpp"
#include "UveDX.hpp"

DelayText::DelayText(UveDX::UveDX *uveDX, int dstY, const std::string &text,
                     int delayBeforeDisplay, int displayDuration)
    : UveDX::UveBase(uveDX), dstY(dstY), text(text),
      delayBeforeDisplay(delayBeforeDisplay), displayDuration(displayDuration) {
}

void DelayText::update() {
  if (this->delayBeforeDisplay <= 0) {
    global::game->font_alphabet->blitText(320, this->dstY, this->text, 6);

    if (--this->displayDuration <= 0)
      this->hasBeenDisposed = true;
  } else
    --this->delayBeforeDisplay;
}
