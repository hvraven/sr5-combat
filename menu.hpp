#ifndef SHADOWRUN5_COMBAT_MENU_HPP
#define SHADOWRUN5_COMBAT_MENU_HPP

#include <ncurses.h>
#include <menu.h>
#include <vector>
#include <string>

#include "window.hpp"
#include "error.hpp"

struct default_item
{
  using item_ptr = std::unique_ptr<ITEM, decltype(&free_item)>;

  template <class N, class S>
  default_item(N&& n, S&& s)
    : ptr{nullptr, &free_item},
      name{std::forward<N>(n)},
      desc{std::forward<S>(s)}
    { ptr.reset(new_item(name.data(), desc.data())); }

  item_ptr ptr;
  std::string name;
  std::string desc;
};

template <class T>
struct owning_item : public default_item
{
  using data_type = T;

  data_type user_data;

  template <class N, class S>
  owning_item(N&& n, S&& s)
    : default_item{std::forward<N>(n), std::forward<S>(s)},
      user_data{}
    { ptr->userptr = static_cast<void*>(&data); }
  template <class N, class S, class U>
  owning_item(N&& n, S&& s, U&& u)
    : default_item{std::forward<N>(n), std::forward<S>(s)},
      user_data{std::forward<U>(u)}
    { ptr->userptr = static_cast<void*>(&user_data); }

  data_type* data() { return &user_data; }
};

template <class T>
struct pointer_item : public default_item
{
  using data_type = T;

  template <class N, class S>
  pointer_item(N&& n, S&& s)
    : default_item{std::forward<N>(n), std::forward<S>(s)}
    {}
  template <class N, class S>
  pointer_item(N&& n, S&& s, data_type* u)
    : default_item{std::forward<N>(n), std::forward<S>(s)}
    { ptr->userptr = static_cast<void*>(u); }

  data_type* data() const { return static_cast<data_type*>(ptr->userptr); }
  void data(data_type* n) { ptr->userptr = static_cast<void*>(n); }
};

template <class I = default_item>
class basic_menu
{
  using menu_ptr = std::unique_ptr<MENU, decltype(&free_menu)>;
public:
  using item_type = I;

  basic_menu();
  ~basic_menu();

  void move_cursor(int dir);
  void post();
  void unpost();
  template <class W>
  void add_to_window(W& win);
  void add_entry(std::string&& name, std::string&& description);
  void refresh();
  void set_size(int rows, int cols);
  void set_menu_mark(const char* mark) { ::set_menu_mark(men.get(), mark); }
  bool is_posted() const { return posted; }

        std::vector<item_type>& get_entries()       { return entries; }
  const std::vector<item_type>& get_entries() const { return entries; }
  void set_entries(std::vector<item_type>&& e)
    { pointers_valid = false; entries = std::move(e); }

  MENU* get_ptr() { return men.get(); }

private:
  menu_ptr men;
  WINDOW* win;
  std::vector<item_type> entries;
  std::vector<ITEM*> entries_p;
  bool pointers_valid;
  bool posted;

  int rows = 10;
  int cols = 1;

  void update_entries();
};

typedef basic_menu<default_item> menu;

#include <algorithm>

template<class T>
basic_menu<T>::basic_menu()
  : men{new_menu(nullptr), &free_menu},
    win{nullptr},
    entries{},
    entries_p{},
    pointers_valid{false},
    posted{false}
{
};

template<class T>
basic_menu<T>::~basic_menu()
{
  if (is_posted())
    unpost_menu(men.get());
};

template<class T>
inline
void
basic_menu<T>::add_entry(std::string&& name, std::string&& description)
{
  entries.emplace_back(std::move(name), std::move(description));
  pointers_valid = false;
};

template<class T>
inline
void
basic_menu<T>::move_cursor(int dir)
{
  menu_driver(men.get(), dir);
  wrefresh(win);
}

template <class T>
template <class W>
void
basic_menu<T>::add_to_window(W& w)
{
  win = w.ptr();
  const auto borders = w.get_border_width();
  const auto wsize = w.get_window_size();
  const auto width = wsize.cols - borders.right - borders.left;
  const auto height = wsize.lines - borders.top - borders.bottom;
  check_for_menu_error(set_menu_win(men.get(), win));
  check_for_menu_error(set_menu_sub(men.get(), derwin(win, width, height,
                                                      borders.top,
                                                      borders.left)));
  rows = height;
}

template<class T>
void
basic_menu<T>::post()
{
  if (is_posted())
    return;
  if (! pointers_valid)
    update_entries();

  post_menu(men.get());
  posted = true;
  wrefresh(win);
};

template<class T>
inline
void
basic_menu<T>::set_size(int r, int c)
{
  rows = r;
  cols = c;
  if (!entries_p.empty())
    check_for_menu_error(set_menu_format(men.get(), rows, cols));
  refresh();
};

template<class T>
void
basic_menu<T>::refresh()
{
  if (! is_posted())
    return;

  if (! pointers_valid)
    {
      unpost_menu(men.get());
      update_entries();
      post_menu(men.get());
    }

  wrefresh(win);
};

template<class T>
void
basic_menu<T>::update_entries()
{
  decltype(entries_p) nentries_p;
  nentries_p.reserve(entries.size() + 1);
  std::transform(begin(entries), end(entries), std::back_inserter(nentries_p),
    [](item_type& c) { return c.ptr.get(); });
  nentries_p.push_back(nullptr);
  std::swap(nentries_p, entries_p);
  set_menu_items(men.get(), entries_p.data());
  set_menu_format(men.get(), rows, cols);

  pointers_valid = true;
}

#endif // SHADOWRUN5_COMBAT_MENU_HPP
