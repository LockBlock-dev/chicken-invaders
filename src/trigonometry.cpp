#include "trigonometry.hpp"

#include <cmath>

[[nodiscard]] unsigned int calculate_angle(double x, double y) noexcept {
  if (std::abs(x) < 0.05)
    return y < 0.0 ? 0 : 128;

  double angle = std::atan(-y / x);

  if (x < 0.0)
    angle += PI;

  const int result =
      static_cast<int>(256.0 * (HALF_PI_APPROX - angle) * INV_TWO_PI_FACTOR);

  return ((result % 256) + 256) % 256;
}
