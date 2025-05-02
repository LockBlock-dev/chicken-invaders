#pragma once

#include <string>
#include <vector>
#include "Surface.hpp"
#include "UveBase.hpp"

namespace UveDX {

class SurfaceChain : public UveBase {
 public:
  enum class SurfaceLinkType {
    End = 0,
    Self = 1,
    Loop = 2,
  };

  SurfaceChain(UveDX* uveDX, const std::string& filename);
  ~SurfaceChain();

  void update() override;
  void scaleAllSurfaces(double scale);
  void linkSurfaces(
      SurfaceChain::SurfaceLinkType linkType,
      unsigned int startIndex,
      int numSurfaces
  );
  void applyAnchorPointToAllSurfaces(SurfaceAnchorType anchorType);

  Surface* getSurf(unsigned int surfaceNumber);

 private:
  std::vector<Surface*> surfaces;
  size_t totalSurfaces;
};
}  // namespace UveDX
