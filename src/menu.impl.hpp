#include <algorithm>

template<class T>
basic_menu<T>::basic_menu()
  : win{nullptr},
    entries{},
    entries_p{},
    men{new_menu(nullptr), &free_menu},
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
  check_for_menu_error(menu_driver(men.get(), dir),
                       {menu_error::request_denied});
  win->refresh();
}

template <class T>
void
basic_menu<T>::add_to_window(basic_window& w)
{
  win = &w;
  const auto borders = w.get_border_width();
  const auto wsize = w.get_window_size();
  const auto width = wsize.cols - borders.right - borders.left;
  const auto height = wsize.lines - borders.top - borders.bottom;
  check_for_menu_error(set_menu_win(men.get(), win->ptr()));
  auto swin = derwin(win->ptr(), height, width,
                     borders.top, borders.left);
  if (swin == nullptr)
    throw curses_error{"derwin for menu failed"};
  check_for_menu_error(set_menu_sub(men.get(), swin));
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

  check_for_menu_error(post_menu(men.get()));
  posted = true;
  win->refresh();
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
      check_for_menu_error(unpost_menu(men.get()));
      update_entries();
      check_for_menu_error(post_menu(men.get()));
    }

  win->refresh();
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
  check_for_menu_error(set_menu_items(men.get(), entries_p.data()));
  check_for_menu_error(set_menu_format(men.get(), rows, cols));

  pointers_valid = true;
}
