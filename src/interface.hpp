#ifndef SHADOWRUN5_COMBAT_INTERFACE_HPP
#define SHADOWRUN5_COMBAT_INTERFACE_HPP

#include "window.hpp"
#include "menu.hpp"
#include "character.hpp"
#include <vector>

// for testing, needs to be done proper later...
class interface
{
public:
  interface();
  ~interface();

  void run();

private:
  using menu_type = basic_menu<pointer_item<character>>;
  title_window win;
  menu_type m;
  std::vector<character> chars;

  void add_char();
};

#endif // SHADOWRUN5_COMBAT_INTERFACE_HPP
