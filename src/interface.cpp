#include "interface.hpp"

#include "ncurses.hpp"
#include <ncurses.h>
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
  ncurses::echo(true);
  ncurses::cbreak(false);
  std::string result;
  for (int c = getch(); c != '\n'; c = getch())
    result += c;
  move(LINES - 1, 0);
  clrtoeol();
  ncurses::echo(false);
  ncurses::cbreak(true);
  return result;
}

} // anonymous namespace

interface::interface()
  : win{},
    m{}
{
  setlocale(LC_ALL, "");
  ncurses::init();
  ncurses::cbreak(true);
  ncurses::echo(false);
  ncurses::keypad(true);

  init_pair(1, COLOR_RED, COLOR_BLACK);

  win = title_window{LINES - 1,0,0,0,"Testmenü"};
  win.keypad(true);

  m.add_entry("test1", "Testeintrag 1");
  m.add_entry("test2", "Testeintrag 2");
  m.add_entry("test3", "Testeintrag 3");
  m.add_entry("test4", "Testeintrag 4");
  m.add_entry("test5", "Langer Testeintrag 5");
  m.add_entry("test6", "Längerer Testeintrag 6");
  m.add_entry("test7", "Testeintrag 7");
  m.add_entry("test8", "Testeintrag 8");

  m.set_menu_mark(" * ");
  m.add_to_window(win);
  m.post();

  win.draw_decoration();
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
            entries.sort(
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
