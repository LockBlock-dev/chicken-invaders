#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "SurfaceAnchorType.hpp"
#include "UveBase.hpp"

namespace UveDX {

class Surface : public UveBase {
 public:
  Surface(UveDX* uveDX);
  Surface(UveDX* uveDX, const std::string& filename);
  ~Surface();

  void update() override;
  void blit(int dstX, int dstY, const sf::IntRect* rect, double scale);
  void blitWithColor(
      sf::IntRect* rect = nullptr,
      sf::Color color = sf::Color::Black
  );
  void createAsBackSurface();
  void loadBMP(const std::string& filename, unsigned int x, unsigned int y);
  void drawDebugShape(int x, int y);

  int getWidth() const;
  int getHeight() const;
  int getOffsetX() const;
  int getOffsetY() const;
  double getScaleFactor() const;
  int getField28() const;
  Surface* getNextSurface();

  void setScaleFactor(double scale);
  void setField28(int val);
  void setAnchorPoint(SurfaceAnchorType anchorType);
  void setNextSurface(Surface* nextSurface);

 private:
  Surface* nextSurface;
  int width;
  int height;
  int offsetX;
  int offsetY;
  int field28;
  double scaleFactor;
  sf::Texture* texture;
  sf::Sprite* sprite;
};
}  // namespace UveDX
