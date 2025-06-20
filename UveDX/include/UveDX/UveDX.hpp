#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "UveDX/Surface.hpp"
#include "UveDX/UveFileManager.hpp"
#include "UveDX/UveInput.hpp"
#include "UveDX/UveListOwner.hpp"
#include "UveDX/UveSound.hpp"
#include "UveDX/UveTimer.hpp"

namespace UveDX {
class UveDX : public UveListOwner {
 public:
  enum class DebugLevel {
    NONE = 0,
    FRAME_TIME_ONLY = 1,
    FRAME_TIME_AND_DEBUG_SHAPE = 2,
  };

  UveDX(
      const std::string& windowTitle,
      bool isFullscreen,
      unsigned int width,
      unsigned int height,
      unsigned int bpp
  );
  ~UveDX() override;

  void update() override;
  void frame();
  void log(const std::string& text);
  void loadAnimations(
      const std::string& filename,
      unsigned int offsetX,
      unsigned int offsetY
  );
  void onError(
      const std::string& title,
      const std::string& message,
      int code = -1
  );
  void showError(
      const std::string& title,
      const std::string& message,
      int code = -1
  );
  void waitForEscapeKey(unsigned int sleepTimeMs);
  unsigned int getWidth() const;
  unsigned int getHeight() const;

  void setBackSurfaceClipRegion(const Rect* bounds = nullptr);

  sf::RenderWindow window;
  Surface* backSurface;
  UveSound* uveSound;
  UveInput* uveInput;
  UveFileManager* uveFileManager;
  bool soundEnabled;
  char byte3E;
  char byte3F;
  bool pauseEngine;
  UveDX::DebugLevel debugMode;
  unsigned int width;
  unsigned int height;
  unsigned int bpp;
  int xOffset;
  int yOffset;
  bool isFullscreen;
  unsigned int timeDiffAfterEngineUpdate;
  unsigned int timeDiffStartEngineUpdate;
  unsigned int timeDiffAfterEngineFrame;
  unsigned int totalFramesRendered;
  unsigned int pixelFormat;
  UveTimer timer;
  Rect bounds;
};

}  // namespace UveDX
