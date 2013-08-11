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

class basic_window
{
  using win_ptr = std::unique_ptr<WINDOW, decltype(&delwin)>;
public:
  basic_window()
    : win{nullptr, &delwin}, lines{0}, cols{0}, starty{0}, startx{0} {}
  basic_window(int lines, int cols, int begin_y, int begin_x);
  explicit basic_window(WINDOW* w);
  basic_window(basic_window&&) = default;
  basic_window& operator=(basic_window&&) = default;

  WINDOW* ptr() { return win.get(); }
  explicit operator WINDOW*() { return win.get(); }

  void resize(int l, int c);
  void keypad(bool);

  void refresh();

  virtual void draw_decoration() = 0;
  virtual border_width get_border_width() const = 0;
  window_size get_window_size() const
    { return {lines, cols, starty, startx}; }

protected:
  win_ptr win;
  int lines;
  int cols;
  int starty;
  int startx;
};

class window : public basic_window
{
public:
  using basic_window::basic_window;
  void draw_decoration() override {}
  border_width get_border_width() const override
    { return {0,0,0,0}; }
};

class bordered_window : public basic_window
{
public:
  using basic_window::basic_window;
  void draw_decoration() override;
  border_width get_border_width() const override
    { return {1,1,1,1}; }
};

class title_window : public basic_window
{
public:
  title_window() = default;
  title_window(int lines, int cols, int begin_y, int begin_x,
               std::string t)
    : basic_window{lines, cols, begin_y, begin_x},
      title{std::move(t)} {}
  using basic_window::basic_window;
  void draw_decoration() override;
  border_width get_border_width() const override
    { return {3,1,1,1}; }

  void set_title(std::string t) { title = std::move(t); }
  const std::string& get_title() const { return title; }

private:
  std::string title;
};

#endif // SHADOWRUN5_COMBAT_WINDOW_HPP
