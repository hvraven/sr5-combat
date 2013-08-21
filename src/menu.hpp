#ifndef SHADOWRUN5_COMBAT_MENU_HPP
#define SHADOWRUN5_COMBAT_MENU_HPP

#include "item.hpp"

#include <ncurses.h>
#include <menu.h>
#include <initializer_list>
#include <list>
#include <memory>
#include <string>
#include <vector>

class basic_window;

template <class I = default_item>
class basic_menu
{
  using menu_ptr = std::unique_ptr<MENU, decltype(&free_menu)>;
public:
  using item_type = I;
  using entries_type = std::list<item_type>;

  enum class opt
  {
    one_value        = O_ONEVALUE,
    show_description = O_SHOWDESC,
    row_major        = O_ROWMAJOR,
    ignore_case      = O_IGNORECASE,
    show_match       = O_SHOWMATCH,
    non_cyclic       = O_NONCYCLIC
  };

  basic_menu();
  ~basic_menu();

  void move_cursor(int dir);
  void post();
  void unpost();
  void add_to_window(basic_window& win);
  void add_entry(std::string&& name, std::string&& description);
  void refresh();
  void set_size(int rows, int cols);
  void set_menu_mark(const char* mark) { ::set_menu_mark(men.get(), mark); }
  bool is_posted() const { return posted; }
  bool is_empty() const { return entries.empty(); }

  void opt_on(opt);
  void opt_off(opt);
  void set_opts(std::initializer_list<opt> opts);
  opt get_opts() const noexcept;

  entries_type& get_entries() noexcept
    { pointers_valid = false; return entries;}
  const entries_type& get_entries() const noexcept { return entries; }
  void set_entries(std::list<item_type> e) noexcept
    { pointers_valid = false; entries = std::move(e); }

        item_type* get_current();
  const item_type* get_current() const;

        MENU* get_ptr()       noexcept { return men.get(); }
  const MENU* get_ptr() const noexcept { return men.get(); }

private:
  basic_window* win;
  entries_type entries;
  std::vector<ITEM*> entries_p;
  menu_ptr men;
  bool pointers_valid;
  bool posted;

  int rows = 10;
  int cols = 1;

  void update_entries();
};

typedef basic_menu<default_item> menu;

#include "menu.impl.hpp"

#endif // SHADOWRUN5_COMBAT_MENU_HPP
