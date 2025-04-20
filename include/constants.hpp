#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <cstdint>

extern const sf::Keyboard::Scancode playerMoveLeftKeys[];
extern const sf::Keyboard::Scancode playerMoveTopKeys[];
extern const sf::Keyboard::Scancode playerMoveRightKeys[];
extern const sf::Keyboard::Scancode playerMoveDownKeys[];
extern const sf::Keyboard::Scancode playerShootKeys[];
extern const sf::Keyboard::Scancode playerMissileKeys[];

constexpr unsigned int screenWidth = 640;
constexpr unsigned int screenHeight = 480;
constexpr unsigned int targetFPS = 26;
constexpr std::int64_t DISCORD_CLIENT_ID = 1360702972744433684;
