#include "UveDX/TileLayer.hpp"

#include "UveDX/UveBase.hpp"
#include "UveDX/UveDX.hpp"

namespace UveDX {
TileLayer::TileLayer(
    UveDX* uveDX,
    int originX,
    int originY,
    int tileColumns,
    int tileRows,
    int tileWidth,
    int tileHeightOverride
)
    : UveBase(uveDX),
      originX(originX),
      originY(originY),
      tileColumns(tileColumns),
      tileRows(tileRows),
      tileWidth(tileWidth),
      tileHeight(tileHeightOverride ? tileHeightOverride : this->tileWidth),
      maxX(this->originX + this->tileWidth * this->tileColumns - 1),
      maxY(this->originY + this->tileHeight * this->tileRows - 1),
      wrapColumns(false),
      wrapRows(false),
      scaleX(1.0),
      scaleY(1.0),
      updateInterval(1),
      updateCounter(0),
      defaultSurface(nullptr),
      tiles(static_cast<size_t>(this->tileRows * this->tileColumns), nullptr) {
  this->uveDX->log(std::format(
      "New {}x{} tile layer, ({},{})-({},{})), tile size {}x{}.",
      this->tileColumns, this->tileRows, this->originX, this->originY,
      this->maxX, this->maxY, this->tileWidth, this->tileHeight
  ));
}

void TileLayer::update() {
  int screenOffsetX = static_cast<int>(
      static_cast<double>(this->uveDX->xOffset) * this->scaleX +
      static_cast<double>(this->uveDX->bounds.position.x)
  );
  int screenOffsetY = static_cast<int>(
      static_cast<double>(this->uveDX->yOffset) * this->scaleY +
      static_cast<double>(this->uveDX->bounds.position.y)
  );
  int startColumn = (screenOffsetX - this->originX) / this->tileWidth;
  int startRow = (screenOffsetY - this->originY) / this->tileHeight;
  int drawStartX = this->uveDX->bounds.position.x -
                   (screenOffsetX - this->originX) % this->tileWidth;

  for (int drawY = this->uveDX->bounds.position.y -
                   (screenOffsetY - this->originY) % this->tileHeight;
       (this->uveDX->bounds.position.y + this->uveDX->bounds.size.y) > drawY;
       drawY += this->tileHeight) {
    int currentColumn = startColumn;

    for (int drawX = drawStartX;
         drawX < (this->uveDX->bounds.position.x + this->uveDX->bounds.size.x);
         drawX += this->tileWidth) {
      if (auto surf = this->getTileSurface(currentColumn, startRow))
        surf->blit(drawX, drawY, nullptr, 1.0);

      ++currentColumn;
    }

    ++startRow;
  }

  if (++this->updateCounter >= this->updateInterval) {
    this->updateCounter = 0;

    for (auto& surface : this->tiles) {
      if (surface)
        surface = surface->getNextSurface();
    }

    if (this->defaultSurface)
      this->defaultSurface = this->defaultSurface->getNextSurface();
  }
}

void TileLayer::fillTiles(Surface* surface) {
  for (auto& tile : this->tiles)
    tile = surface;
}

Surface* TileLayer::getTileSurface(int column, int row) {
  int wrappedColumn = this->wrapColumns ? column % this->tileColumns : column;
  int wrappedRow = this->wrapRows ? row % this->tileRows : row;

  return (wrappedColumn >= 0 && wrappedColumn < this->tileColumns &&
          wrappedRow >= 0 && wrappedRow < this->tileRows)
             ? this->tiles.at(this->tileColumns * wrappedRow + wrappedColumn)
             : this->defaultSurface;
}
}  // namespace UveDX
