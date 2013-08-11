#ifndef SHADOWRUN5_COMBAT_CHARACTER_HPP
#define SHADOWRUN5_COMBAT_CHARACTER_HPP

#include "window.hpp"
#include "menu.hpp"

// for testing, needs to be done proper later...
class interface
{
public:
  interface();
  ~interface();

  void run();

private:
  title_window win;
  menu m;
};

#endif // SHADOWRUN5_COMBAT_CHARACTER_HPP
