#include "rng.hpp"

#include <random>

int generate_random_number() {
  static std::default_random_engine engine{std::random_device{}()};

  static std::uniform_int_distribution<unsigned int> uniform_dist{
      0,
      0x7FFFFFFF,
  };

  return uniform_dist(engine);
}
