#include "Feather.hpp"
#include "Game.hpp"
#include "bounding_box.hpp"
#include "rng.hpp"

Feather::Feather(UveDX::UveDX *uveDX, double x, double y)
    : UveDX::Sprite(uveDX, 0, 0,
                    global::game->surface_chain_feather->getSurf(0)),
      x_coord(x), y_coord(y), field_B4(generate_random_number() % 50 + 50),
      field_A4((double)(generate_random_number() % 20 - 10)),
      field_AC((double)(generate_random_number() % 20 - 10)), field_BC(0),
      field_C0(0), field_C4(0) {
  sub_401688(&this->field_B4 + 1, 0, 15, -1, 0);
}

void Feather::update() {
  sub_4016F8(&this->field_B8);

  if (this->field_A4 > 0.0)
    this->field_A4 = this->field_A4 - 0.5;

  if (this->field_A4 < 0.0)
    this->field_A4 = this->field_A4 + 0.5;

  if (this->field_AC > 0.0)
    this->field_AC = this->field_AC - 0.5;

  if (this->field_AC < 0.0)
    this->field_AC = this->field_AC + 0.5;

  this->x_coord = this->field_A4 + this->x_coord;
  this->y_coord = this->field_AC + 2.0 + this->y_coord;

  this->x = this->x_coord;
  this->y = this->y_coord;

  auto surfaceNumber = this->field_C0 / 2;

  if (surfaceNumber < 0)
    surfaceNumber += 1;

  this->surface = global::game->surface_chain_feather->getSurf(surfaceNumber);

  UveDX::Sprite::update();

  if (--this->field_B4 <= 0)
    this->hasBeenDisposed = true;
}
