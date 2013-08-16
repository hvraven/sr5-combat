#ifndef SHADOWRUN5_COMBAT_CHARACTER_HPP
#define SHADOWRUN5_COMBAT_CHARACTER_HPP

#include <string>

class initiative_c
{
public:
  constexpr int get_base() const { return base; }
  constexpr int get_dice() const { return dice; }
  constexpr int get_temp() const { return temp; }
  int roll() const;

  void set_base(int b) { base = b; }
  void set_dice(int d) { dice = d; }
  void set_temp(int t) { temp = t; }
  void reset_temp() { temp = 0; }

  initiative_c& operator+=(int n){ temp += n; return *this; }

private:
  int base = 0;
  int dice = 1;
  int temp = 0;
};

inline initiative_c& operator-=(initiative_c& ini, int n)
  { return ini += -n; }
inline initiative_c& operator+(initiative_c& ini, int n)
  { ini += n; return ini; }
inline initiative_c& operator-(initiative_c& ini, int n)
  { ini -= n; return ini; }

struct character
{
  character() = default;
  character(std::string n)
    : character{}
  { name = std::move(n); }

  std::string name;
  initiative_c initiative;

  const char* get_name() const { return name.data(); }
  const char* get_desc() const { return nullptr; }

  const std::string& get_identifier() const { return name; }
};

#endif // SHADOWRUN5_COMBAT_CHARACTER_HPP