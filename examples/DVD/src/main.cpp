#include <UveDX/Color.hpp>
#include <UveDX/Surface.hpp>
#include <UveDX/UveBase.hpp>
#include <UveDX/UveDX.hpp>
#include <cstdint>
#include <cstdlib>
#include <random>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int SPEED = 5;
constexpr double DVD_LOGO_SCALE = 0.5;

UveDX::Color randomRGBColor() {
  static std::mt19937 rng{std::random_device{}()};
  static std::uniform_int_distribution<int> dist(0, 255);
  return UveDX::Color{
      static_cast<uint8_t>(dist(rng)), static_cast<uint8_t>(dist(rng)),
      static_cast<uint8_t>(dist(rng))
  };
}

class ColoredSprite : public UveDX::UveBase {
 public:
  ColoredSprite(
      UveDX::UveDX* uveDX,
      int x,
      int y,
      UveDX::Surface* surface,
      UveDX::Color color
  )
      : UveDX::UveBase(uveDX),
        x(x),
        y(y),
        dx(SPEED),
        dy(SPEED),
        scale(DVD_LOGO_SCALE),
        surface(surface),
        color(color) {}

  void update() override {
    this->x += this->dx;
    this->y += this->dy;

    const double halfSpriteWidth = surface->getWidth() * scale / 2.0;
    const double halfSpriteHeight = surface->getHeight() * scale / 2.0;

    if (x - halfSpriteWidth < 0) {
      x = halfSpriteWidth;
      dx = SPEED;

      this->color = randomRGBColor();
    }

    if (x + halfSpriteWidth > SCREEN_WIDTH) {
      x = SCREEN_WIDTH - halfSpriteWidth;
      dx = -SPEED;

      this->color = randomRGBColor();
    }

    if (y - halfSpriteHeight < 0) {
      y = halfSpriteHeight;
      dy = SPEED;

      this->color = randomRGBColor();
    }

    if (y + halfSpriteHeight > SCREEN_HEIGHT) {
      y = SCREEN_HEIGHT - halfSpriteHeight;
      dy = -SPEED;

      this->color = randomRGBColor();
    }

    this->surface->blitEx(
        x + (surface->getOffsetX() * scale),
        y + (surface->getOffsetY() * scale), nullptr, this->scale, this->color
    );

    if (this->uveDX->debugMode ==
        UveDX::UveDX::DebugLevel::FRAME_TIME_AND_DEBUG_SHAPE)
      this->surface->drawDebugShape(x, y);
  }

 private:
  int x;
  int y;
  int dx;
  int dy;
  double scale;
  UveDX::Surface* surface;
  UveDX::Color color;
};

int main() {
  UveDX::UveDX uveDX{
      "Bouncing DVD logo", false, SCREEN_WIDTH, SCREEN_HEIGHT, 16
  };

  uveDX.timer.setInterval(30);

  UveDX::UveInput::create(&uveDX, UveDX::UveInput::AcquireDeviceFlag::NONE);

  UveDX::Surface dvdSurface{&uveDX, "assets/DVD.png"};

  ColoredSprite dvdSprite{
      &uveDX, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, &dvdSurface,
      UveDX::Color{255, 0, 0}
  };

  dvdSurface.setAnchorPoint(UveDX::SurfaceAnchorType::Center);
  dvdSurface.setScaleFactor(DVD_LOGO_SCALE);

  uveDX.add(&dvdSprite);

  while (true) {
    uveDX.update();
  }

  return EXIT_SUCCESS;
}
