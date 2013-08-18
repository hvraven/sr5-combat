#include "ncurses.hpp"

#include <ncurses.h>

ncurses&
ncurses::init()
{
  static ncurses n;
  return n;
};

void
ncurses::cbreak(bool b)
{
  if (b)
    ::cbreak();
  else
    ::nocbreak();
}

void
ncurses::echo(bool b)
{
  if (b)
    ::echo();
  else
    ::noecho();
}

void
ncurses::keypad(bool b)
{
  ::keypad(stdscr, b);
}

ncurses::ncurses()
{
  initscr();
  start_color();
  nonl();
}

ncurses::~ncurses()
{
  endwin();
}
