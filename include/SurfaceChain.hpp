#pragma once

#include "Surface.hpp"
#include "UveBase.hpp"
#include <string>
#include <vector>

namespace UveDX {

class SurfaceChain : public UveBase {
public:
  enum class SurfaceLinkType {
    Zero = 0,
    One = 1,
    Two = 2,
  };

  SurfaceChain(UveDX *uveDX, const std::string &filename);
  ~SurfaceChain();

  void update() override;
  void scaleAllSurfaces(double scale);
  void linkSurfaces(SurfaceChain::SurfaceLinkType linkType,
                    unsigned int startIndex, int numSurfaces);
  void applyAnchorPointToAllSurfaces(SurfaceAnchorType anchorType);

  Surface *getSurf(unsigned int surfaceNumber);

private:
  std::vector<Surface *> surfaces;
  unsigned int totalSurfaces;
};
} // namespace UveDX
