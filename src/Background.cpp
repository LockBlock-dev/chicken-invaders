#include "Background.hpp"
#include "Game.hpp"
#include "UveDX.hpp"
#include <SFML/Graphics.hpp>

Background::Background(UveDX::UveDX *uveDX)
    : UveDX::UveBase(uveDX), field_14(-1), scrollSpeed(-1), field_1C(0),
      field_20(0) {}

void Background::update() {
  if (this->field_14 < this->scrollSpeed)
    ++this->field_14;

  if (this->field_14 > this->scrollSpeed)
    this->field_14 -= 1;

  this->field_1C += this->field_20 + this->field_14;

  int v2 = 0;

  if (this->field_20 <= 0) {
    v2 = this->field_20;

    if (v2 < 0)
      this->field_20 = -v2 - 1;
  } else
    this->field_20 = 1 - this->field_20;

  if (this->field_20)
    this->uveDX->yOffset = -this->field_20;

  if (this->field_1C < 0)
    this->field_1C += 480;

  if (this->field_1C > 479)
    this->field_1C -= 480;

  sf::IntRect rect{sf::Vector2i{0, this->field_1C},
                   sf::Vector2i{global::game->surface_salmonsky->getWidth(),
                                global::game->surface_salmonsky->getHeight() -
                                    this->field_1C}};

  global::game->surface_salmonsky->blit(0, 0, &rect, 1.0);

  if (this->field_1C > 0) {
    rect.position.y = 0;
    rect.size.y = this->field_1C;

    global::game->surface_salmonsky->blit(
        0, global::game->surface_salmonsky->getHeight() - this->field_1C, &rect,
        1.0);
  }
}

void Background::setScrollSpeed(int speed) { this->scrollSpeed = speed; }

void Background::setField20(int value) { this->field_20 = value; }
