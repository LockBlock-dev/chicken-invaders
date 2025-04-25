#pragma once

#include <string>
#include <vector>

#include "Surface.hpp"
#include "UveBase.hpp"

namespace UveDX {
class Font : public UveBase {
 public:
  Font(UveDX* uveDX, const std::string& filename);
  ~Font();

  void update() override;
  void blitText(int dstX, int dstY, const std::string& text, int a4);
  int calculateTextWidth(const std::string& text);

  void setField194(int val);

 private:
  std::vector<Surface*> surfaces;
  int field_190;
  int spaceSize;
  int field_198;
  int field_19C;
};
}  // namespace UveDX
