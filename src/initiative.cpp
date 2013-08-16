#include "initiative.hpp"

#include "dice.hpp"

int
initiative_c::roll()
{
  current = base + temp + roll_d6(dice);
  return current;
};
