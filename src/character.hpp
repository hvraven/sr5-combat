#ifndef SHADOWRUN5_COMBAT_CHARACTER_HPP
#define SHADOWRUN5_COMBAT_CHARACTER_HPP

#include "initiative.hpp"
#include <string>

struct character
{
  character() noexcept;
  explicit character(std::string);

  std::string name;
  initiative_c initiative;

  struct identifier
  {
    unsigned int id;
  };

  identifier get_identifier() const noexcept { return _identifier; }

private:
  const identifier _identifier;
};

bool operator<(character::identifier, character::identifier);

std::string to_string(character::identifier);

// implementation starts here
inline
bool
operator<(character::identifier f, character::identifier s)
{
  return f.id < s.id;
}

inline
std::string
to_string(character::identifier i)
{
  return std::to_string(i.id);
}

#endif // SHADOWRUN5_COMBAT_CHARACTER_HPP
