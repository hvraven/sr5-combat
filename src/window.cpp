#include "window.hpp"

#include "error.hpp"

basic_window::basic_window(int nlines, int ncols, int y, int x)
  : win{newwin(nlines, ncols, y, x), &delwin},
    lines{nlines},
    cols{ncols},
    starty{y},
    startx{x}
{
  if (nlines == 0)
    lines = LINES - starty;
  if (ncols == 0)
    cols = COLS - startx;
}

void
basic_window::refresh()
{
  draw_decoration();
  wrefresh(win.get());
}

void
basic_window::resize(int l, int c)
{
  lines = l;
  cols = c;
  if (wresize(win.get(), lines, cols) != OK)
    throw curses_error{"wresize failed"};
}

void
basic_window::keypad(bool b)
{
  ::keypad(win.get(), b);
}

void
bordered_window::draw_decoration()
{
  box(win.get(), 0, 0);
};

void
title_window::draw_decoration()
{
  box(win.get(), 0, 0);
  mvwprintw(win.get(), 1, 2, title.data());
  mvwaddch(win.get(), 2, 0, ACS_LTEE);
  mvwhline(win.get(), 2, 1, ACS_HLINE, cols - 2);
  mvwaddch(win.get(), 2, cols - 1, ACS_RTEE);
};
