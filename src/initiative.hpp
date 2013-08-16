#ifndef SHADOWRUN5_COMBAT_INITIATIVE_HPP
#define SHADOWRUN5_COMBAT_INITIATIVE_HPP

class initiative_c
{
public:
  constexpr int get_base() const { return base; }
  constexpr int get_dice() const { return dice; }
  constexpr int get_temp() const { return temp; }
  constexpr int get_current() const { return current; }
  int roll();

  void set_base(int b) { base = b; }
  void set_dice(int d) { dice = d; }
  void set_temp(int t) { temp = t; }
  void reset_temp() { temp = 0; }

  initiative_c& operator+=(int n){ temp += n; return *this; }

private:
  int base = 0;
  int dice = 1;
  int temp = 0;
  int current = 0;
};

inline initiative_c& operator-=(initiative_c& ini, int n)
  { return ini += -n; }
inline initiative_c& operator+(initiative_c& ini, int n)
  { ini += n; return ini; }
inline initiative_c& operator-(initiative_c& ini, int n)
  { ini -= n; return ini; }

#endif // SHADOWRUN5_COMBAT_INITIATIVE_HPP
