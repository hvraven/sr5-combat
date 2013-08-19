#include "initiative.hpp"

#include "dice.hpp"
#include <regex>

int
initiative_c::roll()
{
  return base + temp + roll_d6(dice);
};

/**
 * parses strings like 16+1d6
 */
bool
initiative_c::set_initiative(const std::string& s)
{
  static const std::regex full{R"(\s*(\d+)\s*\+\s*(\d+)[dwDW]6)\s*)"};
  static const std::regex value{R"(\s*(\d+)\s*)"};
  std::smatch match;
  if (std::regex_match(s, match, full))
    {
      set_base(std::stoi(match[1].str()));
      set_dice(std::stoi(match[2].str()));
      return true;
    }
  else if (std::regex_match(s, match, value))
    {
      set_base(std::stoi(match[1].str()));
      set_dice(0);
      return true;
    }
  else
    return false;
}
