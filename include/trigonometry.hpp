#pragma once

#include <array>

namespace global {
inline std::array<double, 256> dcos{};
inline std::array<double, 256> dsin{};
}  // namespace global

[[nodiscard]] unsigned int calculate_angle(double x, double y) noexcept;
