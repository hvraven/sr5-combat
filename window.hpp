#ifndef SHADOWRUN5_COMBAT_WINDOW_HPP
#define SHADOWRUN5_COMBAT_WINDOW_HPP

#include <ncurses.h>
#include <memory>

class window
{
  using win_ptr = std::unique_ptr<WINDOW, decltype(&delwin)>;
public:
  window() : win{nullptr, &delwin} {}
  window(WINDOW* w) : win{w, &delwin} {}
  window(window&&) = default;
  window& operator=(window&&) = default;

  WINDOW* ptr() { return win.get(); }
  explicit operator WINDOW*() { return win.get(); }

  void set_size(int w, int h)
    { width = w; height = h; }

private:
  win_ptr win;
  int width;
  int height;
};

class bordered_window : public window
{
  using window::window;
};

#endif // SHADOWRUN5_COMBAT_WINDOW_HPP
