#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <unordered_map>

#include "UveBase.hpp"

namespace UveDX {
class UveInput : public UveBase {
 public:
  enum class AcquireDeviceFlag : int {
    NONE = 0,
    ACQUIRE_KEYBOARD_FLAG = 1 << 0,
    ACQUIRE_MOUSE_FLAG = 1 << 1
  };

  UveInput(UveDX* uveDX, UveInput::AcquireDeviceFlag deviceFlag);

  void update() override;
  static void create(UveDX* uveDX, UveInput::AcquireDeviceFlag deviceFlag);
  bool isKeyPressed(sf::Keyboard::Scancode code, bool allowLongPress = false);
  bool isAnyKeyPressed();
  void waitForAllKeysRelease();
  void waitForKeyPress();

 private:
  bool shouldAcquireKeyboard;
  bool shouldAcquireMouse;
  std::unordered_map<sf::Keyboard::Scancode, bool> currentKeyState;
  std::unordered_map<sf::Keyboard::Scancode, bool> previousKeyState;
};
}  // namespace UveDX
