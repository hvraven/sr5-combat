#ifndef SHADOWRUN5_COMBAT_ERROR_HPP
#define SHADOWRUN5_COMBAT_ERROR_HPP

#include <initializer_list>
#include <system_error>
#include <menu.h>

class curses_error : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

// enum for all possible menu errors
enum class menu_error : int
{
  ok              = E_OK,
  system_error    = E_SYSTEM_ERROR,
  bad_argument    = E_BAD_ARGUMENT,
  posted          = E_POSTED,
  connected       = E_CONNECTED,
  bad_state       = E_BAD_STATE,
  no_room         = E_NO_ROOM,
  not_posted      = E_NOT_POSTED,
  unknown_command = E_UNKNOWN_COMMAND,
  no_match        = E_NO_MATCH,
  not_selectable  = E_NOT_SELECTABLE,
  not_connected   = E_NOT_CONNECTED,
  request_denied  = E_REQUEST_DENIED,
  invalid_field   = E_INVALID_FIELD,
  current         = E_CURRENT
};

class menu_category_impl : public std::error_category
{
  const char* name() const noexcept override;
  std::string message(int ev) const override;
  bool equivalent(const std::error_code& code,
                  int condition) const noexcept override;
};

const menu_category_impl& menu_category();
std::error_condition make_error_condition(menu_error e);
std::error_code make_error_code(menu_error e);

void check_for_menu_error(int ev);
void check_for_menu_error(int ev, std::initializer_list<menu_error> exclude);

// enable conversion constructor
namespace std
{
  template <>
  struct is_error_condition_enum<menu_error>
    : public true_type {};
}

#endif // SHADOWRUN5_COMBAT_ERROR_HPP
