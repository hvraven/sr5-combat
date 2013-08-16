#include "interface.hpp"

#include "ncurses.hpp"
#include <ncurses.h>
#include <algorithm>
#include <memory>
#include <string>
#include <clocale>
#include <locale>

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
    m{},
    chars{}
{
  setlocale(LC_ALL, "");
  ncurses::init();
  ncurses::cbreak(true);
  ncurses::echo(false);
  ncurses::keypad(true);

  win = title_window{LINES - 2,0,0,0,"Testmenü"};
  mvprintw(LINES - 1, 0, "[A]dd");
  win.keypad(true);

  m.set_menu_mark(" ");
  m.set_opts({menu_type::opt::one_value,
              menu_type::opt::show_match});

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
          add_char();
          break;
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

namespace
{

std::string
gen_display_name(const interface::item_type& item)
{
  return item.data.name + ' ' + std::to_string(item.data.initiative.roll());
}

} // anonymous namespace

void
interface::add_char()
{
  auto name = ask_user("Add (Name): ");
  if (!name.empty())
    {
      auto& entries = m.get_entries();
      character c;
      c.name = std::move(name);
      entries.emplace_back(&gen_display_name, nullptr, c);
      entries.sort(
        [](const menu_type::item_type& a, const menu_type::item_type& b)
        { return std::locale("")(std::string{a.get_name()},
                                 std::string{b.get_name()}); });
      m.refresh();
    }
  pos_menu_cursor(m.get_ptr());
  refresh();
}

interface::~interface()
{
  endwin();
}
