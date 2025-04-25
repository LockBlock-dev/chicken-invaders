#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>

namespace constants {
inline constexpr sf::Keyboard::Scancode playerMoveLeftKeys[] = {
    sf::Keyboard::Scancode::A, sf::Keyboard::Scancode::Left
};
inline constexpr sf::Keyboard::Scancode playerMoveTopKeys[] = {
    sf::Keyboard::Scancode::W, sf::Keyboard::Scancode::Up
};
inline constexpr sf::Keyboard::Scancode playerMoveRightKeys[] = {
    sf::Keyboard::Scancode::D, sf::Keyboard::Scancode::Right
};
inline constexpr sf::Keyboard::Scancode playerMoveDownKeys[] = {
    sf::Keyboard::Scancode::S, sf::Keyboard::Scancode::Down
};
inline constexpr sf::Keyboard::Scancode playerShootKeys[] = {
    sf::Keyboard::Scancode::LShift, sf::Keyboard::Scancode::RShift
};
inline constexpr sf::Keyboard::Scancode playerMissileKeys[] = {
    sf::Keyboard::Scancode::LControl, sf::Keyboard::Scancode::RControl
};

constexpr unsigned int screenWidth = 640;
constexpr unsigned int screenHeight = 480;
constexpr unsigned int targetFPS = 26;
constexpr std::int64_t DISCORD_CLIENT_ID = 1360702972744433684;
}  // namespace constants
