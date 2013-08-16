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
  const char* get_name() const
    { return name.data(); }
  const char* get_desc() const
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
struct function_name_model
{
  const char* get_name() const
    {
      if (name_fun)
        return name_fun(*static_cast<const Derived*>(this));
      else
        return nullptr;
    }
  const char* get_desc() const
    {
      if (desc_fun)
        return desc_fun(*static_cast<const Derived*>(this));
      else
        return nullptr;
    }

  std::function<const char*(const Derived&)> name_fun;
  std::function<const char*(const Derived&)> desc_fun;
};

template <class Derived>
struct str_fun_name_model
{
  const char* get_name() const
    {
      if (name_fun)
        name = name_fun(*static_cast<const Derived*>(this));
      return name.data();
    }
  const char* get_desc() const
    {
      if (desc_fun)
        desc = desc_fun(*static_cast<const Derived*>(this));
      return desc.data();
    }

protected:
  template <class N, class M>
  str_fun_name_model(N&& n, M&& m)
    : name_fun{std::forward<N>(n)}, desc_fun{std::forward<M>(m)},
      name{}, desc{} {}

private:
  const std::function<std::string(const Derived&)> name_fun;
  const std::function<std::string(const Derived&)> desc_fun;
  mutable std::string name;
  mutable std::string desc;
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
  void set_data_ptr(T* t)
    { Derived::ptr->userptr = static_cast<void*>(t); }
  const T& get_data() const
    { return *static_cast<T*>(Derived::ptr->userptr); }
        T& get_data()
    { return *static_cast<T*>(Derived::ptr->userptr); }
  const T* get_data_ptr() const
    { return static_cast<T*>(Derived::ptr->userptr); }
        T* get_data_ptr()
    { return static_cast<T*>(Derived::ptr->userptr); }
};

template <
  template <class Base> class NameModel,
  template <class Base> class DataModel
>
class basic_item
  : public NameModel<basic_item<NameModel, DataModel>>,
    public DataModel<basic_item<NameModel, DataModel>>
{
public:
  explicit operator ITEM*() { return ptr.get(); }
  using name_model = NameModel<basic_item<NameModel,DataModel>>;
  using data_model = DataModel<basic_item<NameModel,DataModel>>;

  template <class N>
  basic_item(N&& n)
    : name_model{},
      data_model{std::forward<N>(n)},
      ptr{new_item(name_model::get_name(),
                   name_model::get_desc()),
          &free_item} {}


  template <class N, class M>
  basic_item(N&& n, M&& m)
    : name_model{std::forward<N>(n), std::forward<M>(m)},
      data_model{},
      ptr{new_item(name_model::get_name(),
                   name_model::get_desc()),
          &free_item} {}

  template <class N, class M, class L>
  basic_item(N&& n, M&& m, L&& l)
    : name_model{std::forward<N>(n), std::forward<M>(m)},
      data_model{std::forward<L>(l)},
      ptr{new_item(name_model::get_name(),
                   name_model::get_desc()),
          &free_item} {}


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
