#include "character.hpp"

#include "dice.hpp"

int
initiative_c::roll() const
{
  return base + temp + roll_d6(dice);
};
