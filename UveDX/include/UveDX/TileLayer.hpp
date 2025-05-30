#pragma once

#include <vector>

#include "UveDX/Surface.hpp"
#include "UveDX/UveBase.hpp"

namespace UveDX {
class TileLayer : public UveBase {
 public:
  TileLayer(
      UveDX* uveDX,
      int originX,
      int originY,
      int tileColumns,
      int tileRows,
      int tileWidth,
      int tileHeightOverride
  );

  void update() override;
  void fillTiles(Surface* surface);
  Surface* getTileSurface(int column, int row);

 private:
  int originX;
  int originY;
  int tileColumns;
  int tileRows;
  int tileWidth;
  int tileHeight;
  int maxX;
  int maxY;
  bool wrapColumns;
  bool wrapRows;
  double scaleX;
  double scaleY;
  int updateInterval;
  int updateCounter;
  Surface* defaultSurface;
  std::vector<Surface*> tiles;
};
}  // namespace UveDX
