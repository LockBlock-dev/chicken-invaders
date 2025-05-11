#include "rng.hpp"

[[nodiscard]] unsigned int generate_random_number() {
  static std::default_random_engine engine{std::random_device{}()};

  static std::uniform_int_distribution<unsigned int> uniform_dist{
      0,
      INT32_MAX,
  };

  return uniform_dist(engine);
}
