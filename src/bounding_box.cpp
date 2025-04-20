#include "bounding_box.hpp"
#include "rng.hpp"

void sub_401688(int *a1, int a2, int a3, int a4, int a5) {
  *a1 = a2;
  a1[1] = a3;

  if (a4 == -1) {
    int v6;

    if (a1[1] == *a1)
      v6 = 0;
    else {
      int v5 = a1[1] - *a1;
      v6 = generate_random_number() % v5;
    }

    a1[2] = *a1 + v6;
  } else if (a4 != 0)
    a1[2] = a4;
  else
    a1[2] = *a1;

  if (a5 != 0)
    a1[3] = a5;
  else
    a1[3] = 2 * (generate_random_number() % 2) - 1;
}

void sub_4016F8(int *a1) {
  if (a1[2] <= *a1)
    a1[3] = 1;

  if (a1[2] >= a1[1])
    a1[3] = -1;

  a1[2] += a1[3];

  if (*a1 > a1[2])

    a1[2] = *a1;

  int v2 = a1[1];

  if (v2 < a1[2])
    a1[2] = v2;
}
