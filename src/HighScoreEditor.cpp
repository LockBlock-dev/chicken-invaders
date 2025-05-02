#include "HighScoreEditor.hpp"

#include "Game.hpp"
#include "RoundedRectangleShape.hpp"
#include "UveDX.hpp"
#include "constants.hpp"

HighScoreEditor::HighScoreEditor(
    UveDX::UveDX* uveDX,
    unsigned int playerId,
    int score
)
    : UveDX::UveBase(uveDX),
      pseudo(""),
      columnNumber(0),
      rowNumber(0),
      playerId(playerId),
      score(score) {}

void HighScoreEditor::update() {
  if (this->uveDX->uveInput->isKeyPressed(
          constants::playerMoveLeftKeys[this->playerId]
      ))
    --this->columnNumber;

  if (this->uveDX->uveInput->isKeyPressed(
          constants::playerMoveRightKeys[this->playerId]
      ))
    ++this->columnNumber;

  if (this->uveDX->uveInput->isKeyPressed(
          constants::playerMoveTopKeys[this->playerId]
      ))
    --this->rowNumber;

  if (this->uveDX->uveInput->isKeyPressed(
          constants::playerMoveDownKeys[this->playerId]
      ))
    ++this->rowNumber;

  this->columnNumber = (this->columnNumber + 6) % 6;
  this->rowNumber = (this->rowNumber + 5) % 5;

  if (this->rowNumber == 4)
    this->columnNumber = std::clamp(this->columnNumber, 0, 4);

  if (this->uveDX->uveInput->isKeyPressed(
          constants::playerShootKeys[this->playerId]
      )) {
    if (this->rowNumber >= 4 && this->columnNumber >= 3) {
      if (this->columnNumber == 3) {
        if (this->pseudo.size() > 0)
          this->pseudo = this->pseudo.substr(0, this->pseudo.size() - 1);
      } else if (this->columnNumber == 4) {
        if (global::game->sound_space->uveDX->soundEnabled)
          global::game->sound_space->play();

        global::game->gameController->addHighScoreEntry(
            this->pseudo, this->score
        );

        this->hasBeenDisposed = true;
      }
    } else {
      char letter = this->columnNumber + 6 * this->rowNumber + 97;

      if (this->rowNumber == 4 && this->columnNumber == 2)
        letter = ' ';

      if (this->pseudo.size() < 10)
        this->pseudo.append(1, letter);

      if (global::game->sound_tr3_239->uveDX->soundEnabled)
        global::game->sound_tr3_239->play();
    }
  }

  const float rectWidth = 35.f;
  const float rectHeight = 35.f;

  sf::RoundedRectangleShape roundedRect{
      sf::Vector2f{rectWidth, rectHeight}, 5, 8
  };

  roundedRect.setOutlineColor(sf::Color::White);
  roundedRect.setOutlineThickness(1.f);

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 6; ++j) {
      if (i < 4 || j < 5) {
        const float posX = 40 * j + 270 * this->playerId + 70;
        const float posY = 40 * i + 200;

        roundedRect.setPosition({posX, posY});

        if (j == this->columnNumber && i == this->rowNumber &&
            ((this->uveDX->totalFramesRendered % 2) == 0))
          roundedRect.setFillColor(sf::Color::White);
        else
          roundedRect.setFillColor(sf::Color::Transparent);

        global::game->window.draw(roundedRect);
      }
    }
  }

  global::game->font_alphabet_small->blitText(
      270 * this->playerId + 70, 140,
      std::format("well done! {} points", this->score)
  );
  global::game->font_alphabet->blitText(
      270 * this->playerId + 70, 160, this->pseudo
  );

  for (size_t k = 0; k < 5; ++k) {
    for (size_t m = 0; m < 6; ++m) {
      if (k >= 4 && m >= 3) {
        if (m == 3)
          global::game->font_alphabet_small->blitText(
              270 * this->playerId + 200, 40 * k + 211, "del"
          );
        else if (m == 4)
          global::game->font_alphabet_small->blitText(
              270 * this->playerId + 240, 40 * k + 211, "end"
          );
      } else {
        char v42 = m + 6 * k + 97;

        if (k == 4 && m == 2)
          v42 = 32;

        global::game->font_alphabet_small->blitText(
            40 * m + 270 * this->playerId + 70 + 15, 40 * k + 211,
            std::string{v42}
        );
      }
    }
  }
}
