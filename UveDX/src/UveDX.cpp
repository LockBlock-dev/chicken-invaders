#include "UveDX/UveDX.hpp"

#include <tinyfiledialogs.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <print>

#include "UveDX/Animator.hpp"
#include "UveDX/UveTimer.hpp"

namespace UveDX {

UveDX::UveDX(
    bool isFullscreen,
    unsigned int width,
    unsigned int height,
    unsigned int bpp
)
    : UveListOwner(this),
      window(
          sf::RenderWindow(sf::VideoMode({width, height}), "Chicken Invaders")
      ),
      backSurface(new Surface{this}),
      uveSound(nullptr),
      uveInput(nullptr),
      uveFileManager(new UveFileManager{this}),
      soundEnabled(true),
      pauseEngine(false),
      debugMode(DebugLevel::NONE),
      width(width),
      height(height),
      bpp(bpp),
      xOffset(0),
      yOffset(0),
      isFullscreen(isFullscreen),
      timeDiffAfterEngineUpdate(0),
      timeDiffStartEngineUpdate(0),
      timeDiffAfterEngineFrame(0),
      totalFramesRendered(0),
      pixelFormat(0),
      timer(0),
      bounds() {
  this->backSurface->createAsBackSurface();

  this->setBackSurfaceClipRegion();

  /*
  if (type == GL_UNSIGNED_SHORT_5_6_5)
      this->pixelFormat = 565; // RGB565
  else if (type == GL_UNSIGNED_SHORT_5_5_5_1)
      this->pixelFormat = 555; // RGB555
  else
      this->pixelFormat = 0;
  */
}

UveDX::~UveDX() {
  if (this->uveFileManager)
    delete this->uveFileManager;

  if (this->uveSound)
    delete this->uveSound;

  if (this->uveInput)
    delete this->uveInput;

  if (this->backSurface)
    delete this->backSurface;
}

void UveDX::update() {
  if (this->pauseEngine)
    return;

  this->timeDiffStartEngineUpdate = this->timer.getElapsedTimeInMilliseconds();

  this->window.clear(sf::Color::Black);

  UveListOwner::update();

  this->timeDiffAfterEngineUpdate = this->timer.getElapsedTimeInMilliseconds();

  if (this->debugMode > UveDX::DebugLevel::NONE) {
    Rect rect{
        0,
        0,
        this->timer.getInterval(),
        4,
    };

    this->backSurface->blitWithColor(&rect, Color::White);

    rect.y = 1;
    rect.w = static_cast<int>(this->timeDiffAfterEngineFrame);
    rect.h = 2;

    this->backSurface->blitWithColor(&rect, Color::Blue);

    rect.w = static_cast<int>(this->timeDiffAfterEngineUpdate);

    this->backSurface->blitWithColor(&rect, Color::Green);

    rect.w = static_cast<int>(this->timeDiffStartEngineUpdate);

    this->backSurface->blitWithColor(&rect, Color::Red);
  }

  this->frame();

  this->timeDiffAfterEngineFrame = this->timer.getElapsedTimeInMilliseconds();

  this->timer.waitForTimeInterval();

  ++this->totalFramesRendered;

  if (this->uveInput)
    this->uveInput->update();
}

void UveDX::frame() {
  this->window.display();
}

void UveDX::log(const std::string& text) {
  std::println("{}", text);
}

void UveDX::loadAnimations(
    const std::string& filename,
    unsigned int offsetX,
    unsigned int offsetY
) {
  UveTimer timer{};
  Animator animator{this, filename, 0};

  animator.offsetX = offsetX;
  animator.offsetY = offsetY;

  animator.loadFromFile(filename);

  if (animator.framerate)
    timer.setInterval(animator.framerate);
  else
    timer.setInterval(this->timer.getInterval());

  do {
    if (this->uveInput) {
      this->uveInput->update();

      if (this->uveInput->isKeyPressed(sf::Keyboard::Scan::Escape))
        break;
    }

    animator.process();
    this->frame();

    timer.waitForTimeInterval();
  } while (!animator.isFinished);
}

void UveDX::onError(
    const std::string& functionName,
    const std::string& message,
    int code
) {
  this->showError(functionName, message, code);

  this->pauseEngine = true;

  std::exit(-1);
}

void UveDX::showError(
    const std::string& functionName,
    const std::string& message,
    int code
) {
  tinyfd_messageBox(
      "UveDX Error",
      std::format(
          "Function {} failed with error: {}, error code: {} ({})",
          functionName, message, code, "No description"
      )
          .c_str(),
      "ok", "error", 1
  );
}

void UveDX::waitForEscapeKey(unsigned int sleepTimeMs) {
  for (unsigned int i = 0; sleepTimeMs / 100 > i; ++i) {
    if (this->uveInput) {
      this->uveInput->update();

      if (this->uveInput->isKeyPressed(sf::Keyboard::Scan::Escape))
        break;
    }

    sf::sleep(sf::milliseconds(100));
  }
}

unsigned int UveDX::getWidth() const {
  return this->width;
}

unsigned int UveDX::getHeight() const {
  return this->height;
}

void UveDX::setBackSurfaceClipRegion(const Rect* inRect) {
  this->bounds = !inRect ? Rect{
    0, 0,
    static_cast<int>(this->backSurface->getWidth()),
     static_cast<int>(this->backSurface->getHeight()),
  } : *inRect;

  // this->backSurface->setClipRegion(this->bounds);
}
}  // namespace UveDX
