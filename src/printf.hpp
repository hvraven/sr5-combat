
#include <typeinfo>
#include <stdexcept>
#include <cstdio>
#include <cassert>
#include <cstddef>

namespace
{

inline
bool
is_uint(char c)
{
  return c == 'o' || c == 'u' || c == 'x' || c == 'X';
}

inline
bool
is_int(char c)
{
  return c == 'd' || c == 'i' || c == 'o' || c == 'x' || c == 'X';
}

inline
bool
is_double(char c)
{
  return c == 'e' || c == 'E' || c == 'f' || c == 'F' ||
    c == 'g' || c == 'G' || c == 'a' || c == 'A';
}

template <class T>
struct check_printf_arg_type
{
  static const char* impl(const char*)
  { throw std::logic_error{std::string{"Format type not supported. Typeid: "} + 
                           typeid(T).name()}; }
};

template <>
struct check_printf_arg_type<long double>
{
  static const char* impl(const char* text)
  {
    if (*text != 'L')
      throw std::logic_error{"long double requires %L_"};

    if (is_double(*++text))
      return ++text;
    else
      throw std::logic_error
        {std::string{"long double with %L"} + *text + " not supported."};
  }
};

template <>
struct check_printf_arg_type<double>
{
  static const char* impl(const char* text)
  {
    if (*text == 'l')
      ++text;

    if (is_double(*text))
      return ++text;
    else
      throw std::logic_error
        {std::string{"double with %"} + *text + " not supported."};
  }
};

template <>
struct check_printf_arg_type<wchar_t*>
{
  static const char* impl(const char* text)
  {
    if (*text == 'l' && *++text == 's')
      return ++text;

    throw std::logic_error{"wchar_t* requires %ls"};
  }
};

template <>
struct check_printf_arg_type<char*>
{
  static const char* impl(const char* text)
  {
    if (*text == 's')
      return ++text;

    throw std::logic_error{std::string{"char* requires %s not %"} + *text};
  }
};

template <std::size_t N>
struct check_printf_arg_type<const char[N]>
{
  static const char* impl(const char* text)
  {
    if (*text == 's')
      return ++text;

    throw std::logic_error{std::string{"char* requires %s not %"} + *text};
  }
};

template <>
struct check_printf_arg_type<char>
{
  static const char* impl(const char* text)
  {
    if (*text == 'c')
      return ++text;

    throw std::logic_error{"char requires %c"};
  }
};


template <>
struct check_printf_arg_type<signed char>
{
  static const char* impl(const char* text)
  {
    if (*text == 'h' && *++text == 'h')
      if (is_int(*++text))
        return ++text;

    throw std::logic_error{"signed char requires %hh_"};
  }
};

template <>
struct check_printf_arg_type<unsigned char>
{
  static const char* impl(const char* text)
  {
    if (*text == 'h' && *++text == 'h')
      if (is_uint(*++text))
        return ++text;

    throw std::logic_error{"unsigned char requires %hh_"};
  }
};

template <>
struct check_printf_arg_type<unsigned int>
{
  static const char* impl(const char* text)
  {
    if (is_uint(*text))
      return ++text;
    else
      throw std::logic_error{std::string{"unsigned int not matching for  %"} + *text};
  }
};

template <>
struct check_printf_arg_type<int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'c' || is_int(*text))
      return ++text;
    else
      throw std::logic_error{std::string{"int not matching for  %"} + *text};
  }
};

template <>
struct check_printf_arg_type<short int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'h' && is_int(*++text))
      return ++text;
    else
      throw std::logic_error{"short int requires %h_"};
  }
};

template <>
struct check_printf_arg_type<unsigned short int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'h' && is_uint(*++text))
      return ++text;
    else
      throw std::logic_error{"unsigned short int requires %h_"};
  }
};

template <>
struct check_printf_arg_type<long int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'l' && is_int(*++text))
      return ++text;
    else
      throw std::logic_error{"long int requires %l_"};
  }
};

template <>
struct check_printf_arg_type<unsigned long int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'l' && is_uint(*++text))
      return ++text;
    else
      throw std::logic_error{"unsigned long int requires %l_"};
  }
};

template <>
struct check_printf_arg_type<long long int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'l' && *++text == 'l' && is_int(*++text))
      return ++text;
    else
      throw std::logic_error{"long long int requires %ll_"};
  }
};

template <>
struct check_printf_arg_type<unsigned long long int>
{
  static const char* impl(const char* text)
  {
    if (*text == 'l' && *++text == 'l' && is_uint(*++text))
      return ++text;
    else
      throw std::logic_error{"unsigned long long int requires %ll_"};
  }
};

void
check_printf_impl(const char* text);
template <class Arg, class... Args>
void
check_printf_impl(const char* text, Arg&& arg, Args&&... args);

void
check_printf_arg_3(const char*)
{
  throw std::logic_error("Too few format specifiers.");
}

template <class Arg, class... Args>
void
check_printf_arg_3(const char* text, Arg&&, Args&&... args)
{
  // TODO missing support for %lc (wint_t)
  // intmax_t, uintmax_t, size_t and ptrdiff_t can be any type,
  // resulting in double specialization for that cases
  if (*text == 'j')
    {
      if (is_int(*++text))
        {
          if (!std::is_same<intmax_t,
                            typename std::remove_reference<
                              typename std::remove_cv<Arg>::type
                           >::type>())
            throw std::logic_error{"Expecting intmax_t for %j_."};
          return check_printf_impl(++text, args...);
        }
      if (is_uint(*text))
        {
          if (!std::is_same<uintmax_t,
                            typename std::remove_reference<
                              typename std::remove_cv<Arg>::type
                           >::type>())
            throw std::logic_error{"Expecting uintmax_t for %j_."};
          return check_printf_impl(++text, args...);
        }
      if (*text == 'n')
        {
          if (!std::is_same<intmax_t*,
                            typename std::remove_reference<
                              typename std::remove_cv<Arg>::type
                           >::type>())
            throw std::logic_error{"Expecting intmax_t* for %jn."};
          return check_printf_impl(++text, args...);
        }
      throw std::logic_error{"Expecting (u)intmax_t for %j_."};
    }

  if (*text == 'z')
    {
      if (*++text == 'n')
        {
          if (!std::is_same<size_t*,
                            typename std::remove_reference<
                              typename std::remove_cv<Arg>::type
                           >::type>())
            throw std::logic_error{"Expecting size_t* for %zn."};
          return check_printf_impl(++text, args...);
        }
      if (is_int(*text) || is_uint(*text))
        {
          if (!std::is_same<size_t,
                            typename std::remove_reference<
                              typename std::remove_cv<Arg>::type
                           >::type>())
            throw std::logic_error{"Expecting size_t for %z_."};
          return check_printf_impl(++text, args...);
        }
    }

  if (*text == 't')
    {
      if (!std::is_same<ptrdiff_t,
                        typename std::remove_reference<
                          typename std::remove_cv<Arg>::type
                       >::type>())
        throw std::logic_error{"Expecting ptrdiff_t for %z_."};
      switch (*++text)
        {
        default:
          throw std::logic_error
            {std::string{"ptrdiff_t with %z"} + *text + " not supported."};
        case 'd': case 'i': case 'o': case'x': case 'X': case 'u':
          return check_printf_impl(++text, args...);
        }
    }

  text = check_printf_arg_type<typename std::remove_reference<
                                typename std::remove_cv<Arg>::type
                              >::type>::impl(text);
  return check_printf_impl(text, args...);
}

void
check_printf_arg_2(const char*)
{
  throw std::logic_error("Too few format specifiers.");
}

template <class Arg, class... Args>
void
check_printf_arg_2(const char* text, Arg&& arg, Args&&... args)
{
  // precision
  if (*text == '.')
    {
      if (*++text == '*')
        {
          if (!std::is_same<typename std::remove_reference<
                              typename std::remove_cv<Arg>::type
                           >::type, int>())
            throw std::logic_error{"Expecting int for variable precision."};
          return check_printf_arg_3(++text, args...);
        }
      else
        {
          for (; *text; ++text)
            if (!isdigit(*text))
              return check_printf_arg_3(text, arg, args...);
          throw std::logic_error("Format string ended to early.");
        }
    }
  return check_printf_arg_3(text, arg, args...);
}

template <class Arg, class... Args>
void
check_printf_arg_1(const char* text, Arg&& arg, Args&&... args)
{
  // skip flags
  for (;*text; ++text)
    if (*text != '-' && *text != '+' && *text != ' ' &&
        *text != '#' && *text != '0')
      break;

  // field width
  if (*text == '*')
    {
      if (!std::is_same<typename std::remove_reference<
                         typename std::remove_cv<Arg>::type
                       >::type, int>())
        throw std::logic_error{"Expecting int for variable field width."};
      return check_printf_arg_2(++text, args...);
    }
  else
    for (;*text; ++text)
      if (!isdigit(*text))
        return check_printf_arg_2(text, arg, args...);

  throw std::logic_error("Format string ended to early.");
}

void
check_printf_impl(const char* text)
{
  for (; *text; ++text)
    {
      if (*text != '%' || *++text == '%')
        continue;
      throw std::logic_error("Too few format specifiers.");
    }
}

template <class Arg, class... Args>
void
check_printf_impl(const char* text, Arg&& arg, Args&&... args)
{
  for (; *text; ++text)
    {
      if (*text != '%' || *++text == '%')
        continue;
      return check_printf_arg_1(text, arg, args...);
    }

  throw std::logic_error("Too few format specifiers.");
}

} // anonymous namespace

template <class... Args>
void
check_printf(const char* text, Args&&... args)
{
  check_printf_impl(text, args...);
}

template <class... Args>
int
safe_printf(const char* text, Args&&... args)
{
  try
    {
      check_printf(text, args...);
    }
  catch (const std::logic_error& e)
    {
      throw std::logic_error{std::string{"Type error while printing \""} + 
                             text + "\"\n" + e.what()};
    }
  return printf(text, args...);
}
