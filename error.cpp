#include "error.hpp"

const char*
menu_category_impl::name() const noexcept
{
  return "curses";
};

std::string
menu_category_impl::message(int ev) const
{
  switch (static_cast<menu_error>(ev))
    {
    case menu_error::ok:
      return "The routine succeeded.";
    case menu_error::system_error:
      return "System error occured (see errno).";
    case menu_error::bad_argument:
      return "Routine detected an incorrect or out-of-range argument.";
    case menu_error::posted:
      return "The menu has already been posted.";
    case menu_error::connected:
      return "missing translation";
    case menu_error::bad_state:
      return "missing translation";
    case menu_error::no_room:
      return "missing translation";
    case menu_error::not_posted:
      return "missing translation";
    case menu_error::unknown_command:
      return "missing translation";
    case menu_error::no_match:
      return "missing translation";
    case menu_error::not_selectable:
      return "missing translation";
    case menu_error::not_connected:
      return "No items are connected to the menu.";
    case menu_error::request_denied:
      return "missing translation";
    case menu_error::invalid_field:
      return "missing translation";
    case menu_error::current:
      return "missing translation";
    }
};

bool
menu_category_impl::equivalent(const std::error_code&,
                                 int condition) const noexcept
{
  switch (condition)
    {
    default:
      return false;
    // maybe translate system error correctly
    };
}

const menu_category_impl&
menu_category()
{
  static menu_category_impl instance;
  return instance;
};

std::error_condition
make_error_condition(menu_error e)
{
  return {static_cast<int>(e), menu_category()};
}

std::error_code
make_error_code(menu_error e)
{
  return {static_cast<int>(e), menu_category()};
}

void
check_for_menu_error(int e)
{
  if (e != E_OK)
    throw std::system_error(e, menu_category());
}
