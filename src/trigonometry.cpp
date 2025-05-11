#include "trigonometry.hpp"

#include <cmath>

constexpr double PI = 3.141;
constexpr double HALF_PI_APPROX = 1.5705;
constexpr double INV_TWO_PI_FACTOR = 0.15918497293855459973;

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
