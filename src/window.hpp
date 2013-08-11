#ifndef SHADOWRUN5_COMBAT_WINDOW_HPP
#define SHADOWRUN5_COMBAT_WINDOW_HPP

#include <ncurses.h>
#include <memory>
#include <string>

struct window_size
{
  int lines;
  int cols;
  int starty;
  int startx;
};

struct border_width
{
  int top;
  int bottom;
  int left;
  int right;
};

template <class Derived>
class crtp_window
{
  using win_ptr = std::unique_ptr<WINDOW, decltype(&delwin)>;
public:
  crtp_window()
    : win{nullptr, &delwin}, lines{0}, cols{0}, starty{0}, startx{0} {}
  crtp_window(int lines, int cols, int begin_y, int begin_x);
  explicit crtp_window(WINDOW* w);
  crtp_window(crtp_window&&) = default;
  crtp_window& operator=(crtp_window&&) = default;

  WINDOW* ptr() { return win.get(); }
  explicit operator WINDOW*() { return win.get(); }

  void resize(int l, int c);
  void keypad(bool);

  void refresh();

  void draw_decoration()
    { static_cast<Derived*>(this)->draw_implementation(); }
  border_width get_border_width() const
    { return static_cast<const Derived*>(this)->get_border_width_impl(); }
  window_size get_window_size() const
    { return {lines, cols, starty, startx}; }

protected:
  win_ptr win;
  int lines;
  int cols;
  int starty;
  int startx;
};

class window : public crtp_window<window>
{
public:
  using crtp_window::crtp_window;
  void draw_implementation() {}
  border_width get_border_width_impl() const
    { return {0,0,0,0}; }
};

class bordered_window : public crtp_window<bordered_window>
{
public:
  using crtp_window::crtp_window;
  void draw_implementation();
  border_width get_border_width_impl() const
    { return {1,1,1,1}; }
};

class title_window : public crtp_window<title_window>
{
public:
  using crtp_window::crtp_window;
  void draw_implementation();
  border_width get_border_width_impl() const
    { return {3,1,1,1}; }

  void set_title(std::string t) { title = std::move(t); }
  const std::string& get_title() const { return title; }

private:
  std::string title;
};

#endif // SHADOWRUN5_COMBAT_WINDOW_HPP
