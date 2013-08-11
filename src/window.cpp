#include "window.hpp"

#include "error.hpp"

template <class D>
crtp_window<D>::crtp_window(int nlines, int ncols, int y, int x)
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

template <class D>
void
crtp_window<D>::refresh()
{
  draw_decoration();
  wrefresh(win.get());
}

template <class D>
void
crtp_window<D>::resize(int l, int c)
{
  lines = l;
  cols = c;
  if (wresize(win.get(), lines, cols) != OK)
    throw curses_error{"wresize failed"};
}

template <class D>
void
crtp_window<D>::keypad(bool b)
{
  ::keypad(win.get(), b);
}

void
bordered_window::draw_implementation()
{
  box(win.get(), 0, 0);
};

void
title_window::draw_implementation()
{
  box(win.get(), 0, 0);
  mvwprintw(win.get(), 1, 2, title.data());
  mvwaddch(win.get(), 2, 0, ACS_LTEE);
  mvwhline(win.get(), 2, 1, ACS_HLINE, cols - 2);
  mvwaddch(win.get(), 2, cols - 1, ACS_RTEE);
};

template class crtp_window<window>;
template class crtp_window<bordered_window>;
template class crtp_window<title_window>;
