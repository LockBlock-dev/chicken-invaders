#include "SurfaceChain.hpp"

#include <format>

#include "UveDX.hpp"

namespace UveDX {
SurfaceChain::SurfaceChain(UveDX* uveDX, const std::string& filename)
    : UveBase(uveDX), surfaces({}) {
  unsigned int surfacesCount = 0;
  auto surfaceFilename = std::format("{}_{}.bmp", filename, surfacesCount);

  while (this->uveDX->uveFileManager->checkFileExists(surfaceFilename)) {
    this->surfaces.push_back(new Surface{this->uveDX, surfaceFilename});

    ++surfacesCount;

    surfaceFilename = std::format("{}_{}.bmp", filename, surfacesCount);
  }

  this->uveDX->log(
      std::format("[{}] Loaded total of {} surfaces.", filename, surfacesCount)
  );

  if (surfacesCount == 0)
    this->uveDX->onError(
        "SurfaceChain::SurfaceChain()",
        std::format("Could not find any images (tried {})", surfaceFilename)
    );

  this->totalSurfaces = surfacesCount;
}

SurfaceChain::~SurfaceChain() {
  for (auto surface : this->surfaces)
    if (surface)
      delete surface;
}

void SurfaceChain::update() {}

void SurfaceChain::scaleAllSurfaces(double scale) {
  for (auto surface : this->surfaces)
    surface->setScaleFactor(scale);
}

void SurfaceChain::linkSurfaces(
    SurfaceLinkType linkType,
    unsigned int startIndex,
    int numSurfaces
) {
  int totalSurfaces = numSurfaces;

  if (numSurfaces == -1)
    totalSurfaces = this->totalSurfaces;

  auto v5 = totalSurfaces + startIndex - 1;
  unsigned int i = 0;

  if (v5 > this->totalSurfaces - 1)
    v5 = this->totalSurfaces - 1;

  for (i = startIndex; v5 > i; ++i)
    this->getSurf(i)->setNextSurface(this->getSurf(i + 1));

  switch (linkType) {
    case SurfaceLinkType::Zero:
      this->getSurf(i)->setNextSurface(nullptr);
      break;
    case SurfaceLinkType::One:
      this->getSurf(i)->setNextSurface(this->getSurf(i));
      break;
    case SurfaceLinkType::Two:
      this->getSurf(i)->setNextSurface(this->getSurf(startIndex));
      break;
    default:
      break;
  }
}

void SurfaceChain::applyAnchorPointToAllSurfaces(SurfaceAnchorType anchorType) {
  for (auto surface : this->surfaces)
    surface->setAnchorPoint(anchorType);
}

Surface* SurfaceChain::getSurf(unsigned int surfaceNumber) {
  if (surfaceNumber >= this->surfaces.size())
    this->uveDX->onError(
        "SurfaceChain::getSurf()", std::format(
                                       "Requested surface {}, only {} present",
                                       surfaceNumber, this->surfaces.size()
                                   )
    );

  return this->surfaces.at(surfaceNumber);
}
}  // namespace UveDX
