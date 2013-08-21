#ifndef SHADOWRUN5_COMBAT_ITEM_HPP
#define SHADOWRUN5_COMBAT_ITEM_HPP

#include "error.hpp"
#include <ncurses.h>
#include <menu.h>
#include <functional>
#include <memory>
#include <string>

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

class basic_item
{
public:
  void set_selectable(bool);
  bool get_selectable() const noexcept;

        ITEM* get()       noexcept { return ptr.get(); }
  const ITEM* get() const noexcept { return ptr.get(); }

protected:
  basic_item(ITEM* p)
    : ptr{p, &free_item}
  { if (!ptr) throw curses_error{"Item construction failed"};
    ptr->userptr = static_cast<void*>(this); }

  using item_ptr = std::unique_ptr<ITEM, decltype(&free_item)>;
  item_ptr ptr;
};

template <
  template <class Base> class NameModel,
  template <class Base> class DataModel
>
class item
  : public DataModel<item<NameModel, DataModel>>,
    public NameModel<item<NameModel, DataModel>>,
    public basic_item
{
public:
  explicit operator ITEM*() { return ptr.get(); }
  using name_model = NameModel<item<NameModel,DataModel>>;
  using data_model = DataModel<item<NameModel,DataModel>>;

  template <class N>
  item(N&& n)
    : data_model{std::forward<N>(n)},
      name_model{},
      basic_item{new_item(name_model::get_name(),
                          name_model::get_desc())}
  {}

  template <class N, class M>
  item(N&& n, M&& m)
    : data_model{},
      name_model{std::forward<N>(n), std::forward<M>(m)},
      basic_item{new_item(name_model::get_name(),
                          name_model::get_desc())}
  {}

  template <class N, class M, class L>
  item(N&& n, M&& m, L&& l)
    : data_model{std::forward<L>(l)},
      name_model{std::forward<N>(n), std::forward<M>(m)},
      basic_item{new_item(name_model::get_name(),
                          name_model::get_desc())}
  {}
};

using default_item = item<owning_name_model, no_data_model>;

#endif // SHADOWRUN5_COMBAT_ITEM_HPP
