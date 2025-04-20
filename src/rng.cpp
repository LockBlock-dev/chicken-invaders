#include "rng.hpp"
#include <random>

int generate_random_number() {
  std::random_device r;

  std::default_random_engine engine{r()};

  std::uniform_int_distribution<unsigned int> uniform_dist{0, 0x7FFFFFFF};

  return uniform_dist(engine);
}
