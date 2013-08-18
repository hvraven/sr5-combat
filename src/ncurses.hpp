#ifndef SHADOWRUN5_COMBAT_NCURSES_HPP
#define SHADOWRUN5_COMBAT_NCURSES_HPP

class ncurses
{
public:
  static ncurses& init();
  static void cbreak(bool);
  static void echo(bool);
  static void keypad(bool);

  ncurses(const ncurses&) = delete;
  ncurses(ncurses&&) = delete;
  ncurses& operator=(const ncurses&) = delete;
  ncurses& operator=(ncurses&&) = delete;

private:
  ncurses();
  ~ncurses();
};

#endif // SHADOWRUN5_COMBAT_NCURSES_HPP
