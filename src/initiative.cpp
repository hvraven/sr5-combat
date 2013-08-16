#include "initiative.hpp"

#include "dice.hpp"
#include <regex>

int
initiative_c::roll()
{
  current = base + temp + roll_d6(dice);
  return current;
};

/**
 * parses strings like 16+1d6 or 2d6+34
 */
bool
initiative_c::set_initiative(const std::string& s)
{
  static const std::regex rgx{R"(\s*(\d+)\s*\+\s*(\d+)[dwDW]6)\s*)"};
  std::smatch match;
  if (std::regex_match(s, match, rgx))
    {
      set_base(std::stoi(match[1].str()));
      set_dice(std::stoi(match[2].str()));
      return true;
    }
  else
    return false;
}
