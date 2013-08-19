#ifndef SHADOWRUN5_COMBAT_MENU_HPP
#define SHADOWRUN5_COMBAT_MENU_HPP

#include <ncurses.h>
#include <menu.h>
#include <functional>
#include <initializer_list>
#include <list>
#include <string>
#include <vector>

#include "window.hpp"
#include "error.hpp"

template <class Derived>
struct owning_name_model
{
  const char* get_name() const noexcept
    { return name.data(); }
  const char* get_desc() const noexcept
    { return desc.data(); }

  std::string name;
  std::string desc;

protected:
  ~owning_name_model() {}
};

template <class Derived>
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

template <class Derived>
struct data_name_model
{
  const char* get_name() const
    { return static_cast<const Derived*>(this)->get_data().get_name(); }
  const char* get_desc() const
    { return static_cast<const Derived*>(this)->get_data().get_desc(); }
};

template <class Derived>
struct function_name_model : public pointer_name_model<Derived>
{
protected:
  function_name_model(std::function<const char*(const Derived&)> n,
                      std::function<const char*(const Derived&)> m)
    : pointer_name_model<Derived>
        {n(*static_cast<const Derived*>(this)),
         m ? m(*static_cast<const Derived*>(this)) : nullptr}
  {}
};

template <class Derived>
struct str_fun_name_model : public owning_name_model<Derived>
{
protected:
  str_fun_name_model(std::function<std::string(const Derived&)> n,
                     std::function<std::string(const Derived&)> m)
    : owning_name_model<Derived>
        {n(*static_cast<const Derived*>(this)),
         m ? m(*static_cast<const Derived*>(this)) : ""}
  {}
};

template <class Derived>
struct no_data_model
{
};

template <class T, class Derived>
struct owning_data_model
{
  void set_data(T t) { data = std::move(t); }
  const T& get_data() const { return data; }
        T& get_data()       { return data; }

  T data;
};

template <class T, class Derived>
struct pointer_data_model
{
  void set_data_ptr(T* t) noexcept { ptr = t; }
  const T& get_data() const noexcept { return *ptr; }
        T& get_data()       noexcept { return *ptr; }
  const T* get_data_ptr() const noexcept { return ptr; }
        T* get_data_ptr()       noexcept { return ptr; }

private:
  T* ptr;
};

template <
  template <class Base> class NameModel,
  template <class Base> class DataModel
>
class basic_item
  : public DataModel<basic_item<NameModel, DataModel>>,
    public NameModel<basic_item<NameModel, DataModel>>
{
public:
  explicit operator ITEM*() { return ptr.get(); }
  using name_model = NameModel<basic_item<NameModel,DataModel>>;
  using data_model = DataModel<basic_item<NameModel,DataModel>>;

  template <class N>
  basic_item(N&& n)
    : data_model{std::forward<N>(n)},
      name_model{},
      ptr{new_item(name_model::get_name(),
                   name_model::get_desc()),
          &free_item}
  { if (!ptr) throw curses_error{"Item construction failed"};
    ptr->userptr = static_cast<void*>(this); }

  template <class N, class M>
  basic_item(N&& n, M&& m)
    : data_model{},
      name_model{std::forward<N>(n), std::forward<M>(m)},
      ptr{new_item(name_model::get_name(),
                   name_model::get_desc()),
          &free_item}
  { if (!ptr) throw curses_error{"Item construction failed"};
    ptr->userptr = static_cast<void*>(this); }

  template <class N, class M, class L>
  basic_item(N&& n, M&& m, L&& l)
    : data_model{std::forward<L>(l)},
      name_model{std::forward<N>(n), std::forward<M>(m)},
      ptr{new_item(name_model::get_name(),
                   name_model::get_desc()),
          &free_item}
  { if (!ptr) throw curses_error{"Item construction failed"};
    ptr->userptr = static_cast<void*>(this); }

  void set_selectable(bool);
  bool get_selectable() const noexcept;

  using item_ptr = std::unique_ptr<ITEM, decltype(&free_item)>;
  item_ptr ptr;
};

using default_item = basic_item<owning_name_model, no_data_model>;

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

extern template class basic_menu<default_item>;

#endif // SHADOWRUN5_COMBAT_MENU_HPP
