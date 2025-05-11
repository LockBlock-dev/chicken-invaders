#pragma once

#include <array>

constexpr double HALF_PI_APPROX = 1.5705;
constexpr double PI = 3.141;
constexpr double TWO_PI = 6.282;
constexpr double INV_TWO_PI_FACTOR = 0.15918497293855459973;
constexpr double INVERSE_OF_256 = 0.00390625;

namespace global {
inline std::array<double, 256> dcos{};
inline std::array<double, 256> dsin{};
}  // namespace global

[[nodiscard]] unsigned int calculate_angle(double x, double y) noexcept;
