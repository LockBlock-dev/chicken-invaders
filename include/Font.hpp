#pragma once

#include <array>
#include <string>

#include "Surface.hpp"
#include "UveBase.hpp"

namespace UveDX {
class Font : public UveBase {
 public:
  enum class TextAlignment {
    Left = 0,
    Right = 2,
    Center = 6,
  };

  Font(UveDX* uveDX, const std::string& filename);
  ~Font();

  void update() override;
  void blitText(
      int x,
      int y,
      const std::string& text,
      TextAlignment alignment = TextAlignment::Left
  );
  int calculateTextWidth(const std::string& text);

  void setSpaceWidth(unsigned int width);

 private:
  static constexpr size_t PRINTABLE_ASCII_COUNT = 95;

  std::array<Surface*, PRINTABLE_ASCII_COUNT> surfaces;
  int charSpacing;
  unsigned int spaceWidth;
  int averageCharWidth;
  int averageCharHeight;
};
}  // namespace UveDX
