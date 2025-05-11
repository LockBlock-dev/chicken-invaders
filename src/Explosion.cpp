#include "Explosion.hpp"

#include <cmath>

#include "Game.hpp"
#include "Sprite.hpp"
#include "rng.hpp"
#include "trigonometry.hpp"

Explosion::Explosion(
    UveDX::UveDX* uveDX,
    int x,
    int y,
    int a5,
    unsigned int a6,
    int a7,
    int a8,
    bool a9
)
    : UveDX::UveBase(uveDX), field_14(a9), field_18(a5) {
  if (!a8)
    a8 = 2;

  for (int i = 0; i < this->field_18; ++i) {
    int v10 = 9 * i;
    *(double*)(&this->field_1C + v10) = (double)x;
    *(double*)(&this->field_24 + v10) = (double)y;
    int v16 = random_range(0, a8) - a8 / 2;
    int v11 = v16 + a7 + (v16 + a7 < 0 ? 256 : 0);

    if (v11 > 255)
      v11 -= 256;

    int v12 = a6 == 1 ? 0 : random_range<int>(0, a6 - 1);

    int v13 = v12 + 1;

    if (a8 != 256)
      v13 =
          ((1.0 - std::fabs((long double)v16 / (long double)(a8 / 2))) *
           (long double)v13);

    int v14 = 9 * i;

    *(double*)(&this->field_2C + v14) = (long double)v13 * global::dcos[v11];
    *(double*)(&this->field_34 + v14) = (long double)v13 * global::dsin[v11];
    *(&this->field_3C + v14) = v13;
  }
}

void Explosion::update() {
  auto sprite = UveDX::Sprite{this->uveDX, 0, 0, nullptr};

  int i = 0;
  bool flag = false;
  int* v2 = &this->field_3C;

  while (i < this->field_18) {
    if (*v2 > 0) {
      flag = true;

      *((double*)v2 - 4) = *((double*)v2 - 2) * 0.00390625 + *((double*)v2 - 4);
      *((double*)v2 - 3) = *((double*)v2 - 1) * 0.00390625 + *((double*)v2 - 3);

      if (this->field_14)
        *((double*)v2 - 1) += 16.0;

      int a2 = 1024 - *v2;

      if (a2 < 0)
        a2 += 255;

      a2 /= 256;

      a2 *= 32;

      *v2 -= 32;

      sprite.sprite_x = *((double*)v2 - 4);
      sprite.sprite_y = *((double*)v2 - 3);

      auto angle = calculate_angle(*((double*)v2 - 2), *((double*)v2 - 1));

      // if (angle >= 0)
      //   angle += 7;

      int surfaceNumber = a2 + (angle + 7) / 8;

      // ToDo: not in original code but it is invalid sometimes
      surfaceNumber = std::clamp(surfaceNumber, 0, 127);

      sprite.setSurface(global::game->surface_chain_sparc->getSurf(surfaceNumber
      ));

      sprite.update();
    }

    ++i;
    v2 += 9;
  }

  if (!flag)
    this->hasBeenDisposed = true;
}
