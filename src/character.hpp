#ifndef SHADOWRUN5_COMBAT_CHARACTER_HPP
#define SHADOWRUN5_COMBAT_CHARACTER_HPP

#include "initiative.hpp"
#include <string>

struct character
{
  character() = default;
  character(std::string n)
    : character{}
  { name = std::move(n); }

  std::string name;
  initiative_c initiative;

  const std::string& get_identifier() const { return name; }
};

#endif // SHADOWRUN5_COMBAT_CHARACTER_HPP
