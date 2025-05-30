#include "UveDX/Font.hpp"

#include "UveDX/SurfaceAnchorType.hpp"
#include "UveDX/UveDX.hpp"

namespace UveDX {
Font::Font(UveDX* uveDX, const std::string& filename)
    : UveBase(uveDX),
      surfaces({}),
      charSpacing(1),
      spaceWidth(10),
      averageCharWidth(0),
      averageCharHeight(0) {
  this->surfaces.fill(nullptr);

  size_t loadedCharsCount = 0;

  for (unsigned int i = 0; i < PRINTABLE_ASCII_COUNT; ++i) {
    auto surfaceFilename = std::format("{}_{}.bmp", filename, i + 33);

    if (this->uveDX->uveFileManager->checkFileExists(surfaceFilename)) {
      auto surface = new Surface{this->uveDX, surfaceFilename};

      surface->setAnchorPoint(SurfaceAnchorType::TopLeftDefault);

      this->averageCharWidth += surface->getWidth();
      this->averageCharHeight += surface->getHeight();

      this->surfaces.at(i) = surface;

      ++loadedCharsCount;
    }
  }

  if (loadedCharsCount > 0) {
    this->averageCharHeight /= loadedCharsCount;
    this->averageCharWidth /= loadedCharsCount;
    this->spaceWidth = static_cast<unsigned int>(this->averageCharWidth * 0.75);
  }
}

Font::~Font() {
  for (auto surface : this->surfaces)
    if (surface)
      delete surface;
}

void Font::update() {}

void Font::blitText(
    int x,
    int y,
    const std::string& text,
    TextAlignment alignment
) {
  switch (alignment) {
    case TextAlignment::Center: {
      x -= this->calculateTextWidth(text) / 2;
      break;
    }
    case TextAlignment::Right: {
      x -= this->calculateTextWidth(text);
      break;
    }
    case TextAlignment::Left:
    default:
      break;
  }

  for (char c : text) {
    if (c == ' ')
      x += this->spaceWidth;
    else if (c >= 33) {
      if (auto surface = this->surfaces.at(static_cast<size_t>(c - 33))) {
        surface->blit(x, y, nullptr, 1.0);

        x += this->charSpacing + surface->getWidth();
      }
    }
  }
}

int Font::calculateTextWidth(const std::string& text) {
  int totalWidth = -static_cast<int>(this->charSpacing);

  for (char c : text) {
    if (c == ' ')
      totalWidth += this->spaceWidth;
    else if (c >= 33)
      if (auto surface = this->surfaces.at(static_cast<size_t>(c - 33)))
        totalWidth += this->charSpacing + surface->getWidth();
  }

  return totalWidth;
}

void Font::setSpaceWidth(unsigned int width) {
  this->spaceWidth = width;
}
}  // namespace UveDX
