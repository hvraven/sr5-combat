#include "combat.hpp"

#include <algorithm>

namespace
{

template <class It>
void
sort_by_ini(It first, It end)
{
  std::sort(first, end,
     [](const combat::ini& f, const combat::ini& s)
       { return f.pass == s.pass ? f.ini > s.ini : f.pass < s.pass; });
}

void
add_char_to_turn(std::vector<combat::ini>& data, character& ch)
{
  auto pass = 1;
  for (auto ini = ch.initiative.roll(); ini > 0; ini -= 10)
    {
      data.push_back(combat::ini{ini, pass, &ch});
      ++pass;
    }
}

} // anonymous namespace

void
combat::add_character(character ch)
{
  auto ret = chars.insert(std::make_pair(ch.get_identifier(), std::move(ch)));
  add_char_to_turn(turn, ret.first->second);
  sort_by_ini(turn.begin(), turn.end());
}

void
combat::start_new_turn()
{
  turn = get_full_turn();
}

std::vector<combat::ini>
combat::get_full_turn()
{
  std::vector<ini> result;
  for (auto& ch : chars)
    add_char_to_turn(result, ch.second);

  sort_by_ini(result.begin(), result.end());
  return result;
}
