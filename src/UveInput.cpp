#include "UveInput.hpp"
#include "Game.hpp"
#include "UveDX.hpp"
#include <SFML/Graphics.hpp>
#include <utility>

namespace UveDX {
constexpr UveInput::AcquireDeviceFlag
operator|(UveInput::AcquireDeviceFlag lhs, UveInput::AcquireDeviceFlag rhs) {
  return static_cast<UveInput::AcquireDeviceFlag>(std::to_underlying(lhs) |
                                                  std::to_underlying(rhs));
}

UveInput::UveInput(UveDX *uveDX, UveInput::AcquireDeviceFlag deviceFlag)
    : UveBase(uveDX),
      shouldAcquireKeyboard(
          (deviceFlag | UveInput::AcquireDeviceFlag::ACQUIRE_KEYBOARD_FLAG) !=
          UveInput::AcquireDeviceFlag::NONE),
      shouldAcquireMouse(
          (deviceFlag | UveInput::AcquireDeviceFlag::ACQUIRE_MOUSE_FLAG) !=
          UveInput::AcquireDeviceFlag::NONE),
      currentKeyState({}), previousKeyState({}) {}

void UveInput::update() {
  while (const std::optional event = global::game->window.pollEvent()) {
    if (event->is<sf::Event::Closed>())
      global::game->window.close();
  }

  previousKeyState = currentKeyState;

  if (!global::game->window.hasFocus())
    return;

  for (int key = 0; key < sf::Keyboard::ScancodeCount; ++key)
    currentKeyState[static_cast<sf::Keyboard::Scancode>(key)] =
        sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Scancode>(key));
}

void UveInput::create(UveDX *uveDX, AcquireDeviceFlag deviceFlag) {
  uveDX->uveInput = new UveInput(uveDX, deviceFlag);
}

bool UveInput::isKeyPressed(sf::Keyboard::Scancode code, bool allowLongPress) {
  return allowLongPress ? currentKeyState[code]
                        : !previousKeyState[code] && currentKeyState[code];
}

bool UveInput::isAnyKeyPressed() {
  for (int key = 0; key <= sf::Keyboard::KeyCount; ++key)
    if (currentKeyState.at(static_cast<sf::Keyboard::Scancode>(key)))
      return true;

  return false;
}

void UveInput::waitForAllKeysRelease() {
  bool flag = false;

  while (!flag) {
    flag = true;

    this->update();

    for (int key = 0; key <= sf::Keyboard::KeyCount; ++key)
      if (currentKeyState.at(static_cast<sf::Keyboard::Scancode>(key)))
        flag = false;
  }
}

void UveInput::waitForKeyPress() {
  bool anyKeyPressed = false;

  while (true) {
    anyKeyPressed = this->isAnyKeyPressed();

    if (anyKeyPressed)
      break;

    this->update();
  }
}
} // namespace UveDX
