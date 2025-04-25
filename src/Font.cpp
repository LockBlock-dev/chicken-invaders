#include "Font.hpp"

#include "SurfaceAnchorType.hpp"
#include "UveDX.hpp"

namespace UveDX {
Font::Font(UveDX* uveDX, const std::string& filename)
    : UveBase(uveDX),
      surfaces({}),
      field_190(1),
      spaceSize(10),
      field_198(0),
      field_19C(0) {
  int v7 = 0;

  for (unsigned int i = 0; i < 95; ++i) {
    this->surfaces.push_back(nullptr);

    auto surfaceFilename = std::format("{}_{}.bmp", filename, i + 33);

    if (this->uveDX->uveFileManager->checkFileExists(surfaceFilename)) {
      this->surfaces.at(i) = new Surface{this->uveDX, surfaceFilename};

      this->surfaces.at(i)->setAnchorPoint(SurfaceAnchorType::TopLeftDefault);

      this->field_198 += this->surfaces.at(i)->getWidth();
      this->field_19C += this->surfaces.at(i)->getHeight();
      ++v7;
    }
  }

  if (v7 > 0) {
    this->field_19C /= v7;
    this->field_198 /= v7;
    this->spaceSize =
        static_cast<int>(static_cast<double>(this->field_198) * 0.75);
  }
}

Font::~Font() {
  for (auto surface : this->surfaces)
    if (surface)
      delete surface;
}

void Font::update() {}

void Font::blitText(int dstX, int dstY, const std::string& text, int a4) {
  if (a4 == 6)
    dstX -= this->calculateTextWidth(text) / 2;

  if (a4 == 2)
    dstX -= this->calculateTextWidth(text);

  for (unsigned int i = 0; i < text.size(); ++i) {
    char c = text.at(i);

    if (c == ' ')
      dstX += this->spaceSize;
    else if (c >= 33) {
      auto surface = this->surfaces.at(c - 33);

      if (surface) {
        surface->blit(dstX, dstY, nullptr, 1.0);

        dstX += this->field_190 + surface->getWidth();
      }
    }
  }
}

int Font::calculateTextWidth(const std::string& text) {
  int dstX = -this->field_190;

  for (unsigned int i = 0; i < text.size(); ++i) {
    char c = text.at(i);

    if (c == ' ')
      dstX += this->spaceSize;
    else if (c >= 33) {
      auto surface = this->surfaces.at(c - 33);

      if (surface) {
        dstX += this->field_190 + surface->getWidth();
      }
    }
  }

  return dstX;
}

void Font::setField194(int val) {
  this->spaceSize = val;
}
}  // namespace UveDX
