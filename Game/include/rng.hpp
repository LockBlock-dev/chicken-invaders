#pragma once

#include <concepts>
#include <random>

[[nodiscard]] unsigned int generate_random_number();

template <std::integral T = unsigned int>
[[nodiscard]] T random_range(T min, T max) {
  static std::default_random_engine engine{std::random_device{}()};

  std::uniform_int_distribution<T> dist(min, max - 1);

  return dist(engine);
}
