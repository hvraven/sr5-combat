#include "character.hpp"

#include <limits>

namespace
{

character::identifier
next_identifier() noexcept
{
  static auto next = 0u;
  if (next == std::numeric_limits<decltype(next)>::max())
    throw std::out_of_range{"Identifier limit reached."};
  return {++next};
}

} // anonymous namespace

character::character() noexcept
  : name{},
    initiative{},
    _identifier(next_identifier())
{
}

character::character(std::string n)
  : name{std::move(n)},
    initiative{},
    _identifier(next_identifier())
{
}

