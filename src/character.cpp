#include "character.hpp"

#include "dice.hpp"

int
initiative::roll() const
{
  return base + temp + roll_d6(dice);
};
