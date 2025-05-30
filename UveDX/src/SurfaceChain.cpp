#include "UveDX/SurfaceChain.hpp"

#include <cstddef>
#include <format>

#include "UveDX/UveDX.hpp"

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
  const size_t totalSurfacesToProcess =
      numSurfaces == -1 ? this->totalSurfaces : numSurfaces;

  const size_t lastSurfaceIndex = std::min(
      this->totalSurfaces - 1, totalSurfacesToProcess + startIndex - 1
  );

  for (size_t i = startIndex; i < lastSurfaceIndex; ++i)
    this->getSurf(i)->setNextSurface(this->getSurf(i + 1));

  if (auto lastSurface = this->getSurf(lastSurfaceIndex))
    switch (linkType) {
      case SurfaceLinkType::End:
        lastSurface->setNextSurface(nullptr);
        break;
      case SurfaceLinkType::Self:
        lastSurface->setNextSurface(lastSurface);
        break;
      case SurfaceLinkType::Loop:
        lastSurface->setNextSurface(this->getSurf(startIndex));
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
