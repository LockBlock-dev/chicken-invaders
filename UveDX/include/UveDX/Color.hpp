#pragma once

#include <cstdint>

namespace UveDX {
class Color {
 public:
  Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
  explicit Color(uint32_t rgba);

  uint32_t toRGBA() const noexcept;

  uint8_t r, g, b, a;

  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color White;
};
}  // namespace UveDX
