#ifndef SHADOWRUN5_COMBAT_COMBAT_HPP
#define SHADOWRUN5_COMBAT_COMBAT_HPP

#include "character.hpp"
#include <list>
#include <utility>
#include <vector>

class combat
{
  using chars_type = std::list<character>;
public:
  struct ini
  {
    int ini;
    int pass;
    character* ch;
  };

  void add_character(character);
        chars_type& get_characters()       noexcept { return chars; }
  const chars_type& get_characters() const noexcept { return chars; }

  void start_new_turn();
  std::vector<ini> get_full_turn();
        std::vector<ini>& get_remaining_turn()       noexcept
    { return turn; }
  const std::vector<ini>& get_remaining_turn() const noexcept
    { return turn; }

private:
  chars_type chars;
  std::vector<ini> turn;

};

#endif // SHADOWRUN5_COMBAT_COMBAT_HPP
