#include "dice.hpp"
#include <random>

namespace {

std::default_random_engine&
get_engine()
{
  static std::random_device rd;
  static std::default_random_engine engine{rd()};
  return engine;
}

template <unsigned int N>
inline
int
roll_dice(unsigned int n)
{
  int out = 0;
  static std::uniform_int_distribution<int> dist{1,N};
  auto& engine = get_engine();

  for (unsigned int i = 0; i < n; ++i)
    out += dist(engine);

  return out;
}

} // anonymous namespace

int
roll_d6(unsigned int n)
{
  return roll_dice<6>(n);
}

