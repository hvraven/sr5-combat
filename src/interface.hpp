#ifndef SHADOWRUN5_COMBAT_INTERFACE_HPP
#define SHADOWRUN5_COMBAT_INTERFACE_HPP

#include "menu.hpp"
#include "character.hpp"
#include "window.hpp"
#include <vector>

template <class Derived>
struct character_name_model
{
  const char* get_name() const
    { return Derived::c.name.data(); }
  const char* get_desc() const
    { return nullptr; }
};

template <class T>
using character_data_model = owning_data_model<character, T>;

using character_item = basic_item<str_fun_name_model, character_data_model>;

// for testing, needs to be done proper later...
class interface
{
public:
  interface();
  ~interface();

  void run();

  using item_type = character_item;
  using menu_type = basic_menu<item_type>;
private:
  title_window win;
  menu_type m;
  std::vector<character> chars;

  void add_char();
};

#endif // SHADOWRUN5_COMBAT_INTERFACE_HPP
