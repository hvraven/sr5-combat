#include "item.hpp"

void
basic_item::set_selectable(bool b)
{
  check_for_menu_error(
    set_item_opts(ptr.get(), b ? O_SELECTABLE : 0));
}

bool
basic_item::get_selectable() const noexcept
{
  return item_opts(ptr.get()) & O_SELECTABLE;
}

