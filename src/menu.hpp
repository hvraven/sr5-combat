#ifndef SHADOWRUN5_COMBAT_MENU_HPP
#define SHADOWRUN5_COMBAT_MENU_HPP

#include <ncurses.h>
#include <menu.h>
#include <vector>
#include <list>
#include <string>
#include <initializer_list>

#include "window.hpp"
#include "error.hpp"

struct owning_name_model
{
  const char* get_name() const
    { return name.data(); }
  const char* get_desc() const
    { return desc.data(); }

  std::string name;
  std::string desc;

protected:
  ~owning_name_model() {}
};

struct pointer_name_model
{
  const char* get_name() const
    { return name; }
  const char* get_desc() const
    { return desc; }

  // TODO remove those two pointers
  const char* name;
  const char* desc;
};

struct no_data_model
{};

template <class T>
struct owning_data_model
{
  void set_data(T t) { data = std::move(t); }
  const T& get_data() const { return data; }
        T& get_data()       { return data; }

  T data;
};

/*
template <class T>
struct pointer_data_model
{
  void set_data(T t) { data = std::move(t); }
  const T& get_data() const { return data; }
        T& get_data()       { return data; }
};
*/

template <class NameModel = owning_name_model>
class basic_item : public NameModel
{
public:
  explicit operator ITEM*() { return ptr.get(); }

  template <class N, class M>
  basic_item(N&& n, M&& m)
    : NameModel{std::forward<N>(n), std::forward<M>(m)},
      ptr{new_item(NameModel::get_name(),
                   NameModel::get_desc()),
          &free_item} {}

  using item_ptr = std::unique_ptr<ITEM, decltype(&free_item)>;
  item_ptr ptr;
};

using default_item = basic_item<owning_name_model>;

template <class T, class NameModel = owning_name_model>
struct owning_item : public basic_item<NameModel>
{
  using data_type = T;
  using base = basic_item<NameModel>;

  using base::base;

  const data_type& get_data() const { return user_data; }
        data_type& get_data()       { return user_data; }

  void set_data(data_type d)
    { base::ptr->userptr = static_cast<void*>(&user_data);
      user_data = std::move(d); }

protected:
  data_type user_data;
};

template <class T, class NameModel = owning_name_model>
struct pointer_item : public basic_item<NameModel>
{
  using data_type = T;
  using base = basic_item<NameModel>;

  using base::base;

  data_type* get_data() const
    { return static_cast<data_type*>(base::ptr->userptr); }
  void set_data(data_type* n)
    { base::ptr->userptr = static_cast<void*>(n); }
};

template <class I = default_item>
class basic_menu
{
  using menu_ptr = std::unique_ptr<MENU, decltype(&free_menu)>;
public:
  using item_type = I;

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

  std::list<item_type>& get_entries()
    { pointers_valid = false; return entries;}
  const std::list<item_type>& get_entries() const { return entries; }
  void set_entries(std::list<item_type>&& e)
    { pointers_valid = false; entries = std::move(e); }

        MENU* get_ptr()       noexcept { return men.get(); }
  const MENU* get_ptr() const noexcept { return men.get(); }

private:
  basic_window* win;
  std::list<item_type> entries;
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

extern template class basic_menu<default_item>;

#endif // SHADOWRUN5_COMBAT_MENU_HPP
