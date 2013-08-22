#ifndef SHADOWRUN5_COMBAT_INTERFACE_HPP
#define SHADOWRUN5_COMBAT_INTERFACE_HPP

#include "combat.hpp"
#include "menu.hpp"
#include "window.hpp"

// for testing, needs to be done proper later...
class interface
{
public:
  interface();

  void run();

  template <class T>
  using character_data_model = owning_data_model<combat::ini, T>;
  using character_item = item<str_fun_name_model, character_data_model>;
  using item_type = character_item;
  using menu_type = basic_menu<item_type>;

private:
  title_window win;
  menu_type m;
  combat data;

  void add_char();
  void update_entries();
  void handle_interrupt();
  void handle_next();
};

#endif // SHADOWRUN5_COMBAT_INTERFACE_HPP
