#include "trigonometry.hpp"
#include <cmath>

double global::dcos[256] = {0};
double global::dsin[256] = {0};

double calculate_angle(double x, double y) {
  if (std::abs(x) < 0.05)
    return y;

  double angle = std::atan(-y / x);

  if (x < 0.0)
    angle = angle + 3.141;

  return 256.0 * (1.5705 - angle) * 0.15918497293855459973;
}
