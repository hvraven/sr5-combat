#include "interface.hpp"

#include <menu.h>
#include <algorithm>
#include <memory>
#include <string>
#include <clocale>

namespace
{

std::string
ask_user(const char* question)
{
  mvprintw(LINES - 1, 0, question);
  clrtoeol();
  echo();
  nocbreak();
  std::string result;
  for (int c = getch(); c != '\n'; c = getch())
    result += c;
  move(LINES - 1, 0);
  clrtoeol();
  noecho();
  cbreak();
  return result;
}

} // anonymous namespace

interface::interface()
  : win{},
    m{}
{
  setlocale(LC_ALL, "");
  initscr();
  start_color();
  cbreak();
  noecho();
  nonl();
  keypad(stdscr, true);

  init_pair(1, COLOR_RED, COLOR_BLACK);

  win = newwin(10,40,4,4);
  keypad(win.ptr(), true);

  m.set_size(6,1);
  m.set_menu_mark(" * ");

  m.add_entry("test1", "Testeintrag 1");
  m.add_entry("test2", "Testeintrag 2");
  m.add_entry("test3", "Testeintrag 3");
  m.add_entry("test4", "Testeintrag 4");
  m.add_entry("test5", "Langer Testeintrag 5");
  m.add_entry("test6", "Längerer Testeintrag 6");
  m.add_entry("test7", "Testeintrag 7");
  m.add_entry("test8", "Testeintrag 8");

  m.add_to_window(win.ptr(), 6, 38, 3, 1);
  m.post();

  box(win.ptr(), 0, 0);
  wattron(win.ptr(), COLOR_PAIR(1));
  mvwprintw(win.ptr(), 1, 2, "My Menu");
  wattroff(win.ptr(), COLOR_PAIR(1));
  mvwaddch(win.ptr(), 2, 0, ACS_LTEE);
  mvwhline(win.ptr(), 2, 1, ACS_HLINE, 38);
  mvwaddch(win.ptr(), 2, 39, ACS_RTEE);
  refresh();
}

void
interface::run()
{
  int c;
  while ((c = wgetch(win.ptr())) != KEY_F(1))
    {
      switch(c)
        {
        case 'a':
          {
            auto name = ask_user("Add (Name): ");
            auto desc = ask_user("Add (Description): ");
            m.add_entry(name.data(), desc.data());
            auto& entries = m.get_entries();
            std::sort(begin(entries), end(entries),
              [](const menu::item_type& a, const menu::item_type& b)
              { return a.name < b.name; });
            m.refresh();
            pos_menu_cursor(m.get_ptr());
            refresh();
            break;
          }
        case KEY_DOWN:
          m.move_cursor(REQ_DOWN_ITEM);
          break;
        case KEY_UP:
          m.move_cursor(REQ_UP_ITEM);
          break;
        case KEY_NPAGE:
          m.move_cursor(REQ_SCR_DPAGE);
          break;
        case KEY_PPAGE:
          m.move_cursor(REQ_SCR_UPAGE);
          break;
        }
    }
}

interface::~interface()
{
  endwin();
}
