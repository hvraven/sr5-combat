#ifndef SHADOWRUN5_COMBAT_CHARACTER_HPP
#define SHADOWRUN5_COMBAT_CHARACTER_HPP

#define _XOPEN_SOURCE_EXTENDED
#include <ncurses.h>
#include <menu.h>
#include <vector>
#include <memory>
#include <string>
#include "menu.hpp"

// for testing, needs to be done proper later...
class interface
{
public:
  interface();
  ~interface();

  void run();

private:
  window win;
  menu m;
};

#endif // SHADOWRUN5_COMBAT_CHARACTER_HPP
