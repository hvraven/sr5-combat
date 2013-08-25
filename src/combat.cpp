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
  auto pass = [&](){
      if (data.empty())
        return 1;
      return begin(data)->pass;
    }();
  for (auto ini = ch.initiative.roll() - (pass-1) * 10;
       ini > 0; ini -= 10, ++pass)
    data.push_back(combat::ini{ini, pass, &ch});
}

} // anonymous namespace

void
combat::add_character(character ch)
{
  auto ret = chars.insert(std::make_pair(ch.get_identifier(), std::move(ch)));
  add_char_to_turn(turn, ret.first->second);
  sort_by_ini(turn.begin(), turn.end());
}

bool
combat::del_character(character::identifier ident)
{
  return chars.erase(ident);
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

void
combat::adjust_ini(character::identifier ident, int mod)
{
  const auto it = chars.find(ident);
  if (it == end(chars))
    throw std::runtime_error(to_string(ident) + " not in combat.");

  const auto p = &it->second;
  for (auto& t : turn)
    if (t.ch == p)
      t.ini += mod;

  turn.erase(
    std::remove_if(begin(turn), end(turn),
                   [](const ini& i){ return i.ini <= 0; }),
    end(turn));

  sort_by_ini(begin(turn), end(turn));
}
