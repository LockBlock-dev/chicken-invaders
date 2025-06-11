#include <cstdint>

#include "UveDX/Color.hpp"

namespace UveDX {
Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    : r(red), g(green), b(blue), a(alpha) {}

Color::Color(uint32_t rgba)
    : r((rgba >> 24) & 0xFF),
      g((rgba >> 16) & 0xFF),
      b((rgba >> 8) & 0xFF),
      a(rgba & 0xFF) {}

uint32_t Color::toRGBA() const noexcept {
  return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) |
         (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
}

const Color Color::Black{0, 0, 0};
const Color Color::Red{255, 0, 0};
const Color Color::Green{0, 255, 0};
const Color Color::Blue{0, 0, 255};
const Color Color::White{255, 255, 255};
}  // namespace UveDX
