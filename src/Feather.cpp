#include "Feather.hpp"

#include "BoundaryBouncer.hpp"
#include "Game.hpp"
#include "rng.hpp"

Feather::Feather(UveDX::UveDX* uveDX, double x, double y)
    : UveDX::Sprite(
          uveDX,
          0,
          0,
          global::game->surface_chain_feather->getSurf(0)
      ),
      x_coord(x),
      y_coord(y),
      field_A4((double)(generate_random_number() % 20 - 10)),
      field_AC((double)(generate_random_number() % 20 - 10)),
      field_B4(generate_random_number() % 50 + 50),
      boundaryBouncer(0, 15) {}

void Feather::update() {
  this->boundaryBouncer.update();

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

  this->sprite_x = static_cast<int>(this->x_coord);
  this->sprite_y = static_cast<int>(this->y_coord);

  auto surfaceNumber = this->boundaryBouncer.currentSurfaceIndex / 2;

  if (surfaceNumber < 0)
    surfaceNumber += 1;

  this->surface = global::game->surface_chain_feather->getSurf(surfaceNumber);

  UveDX::Sprite::update();

  if (--this->field_B4 <= 0)
    this->hasBeenDisposed = true;
}
