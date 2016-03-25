#ifndef poly_tyes_h
#define poly_types_h

#include <map>

struct mon_inside_ {
  int xpow;
  int ypow;
};

struct mon_ {
  double coeff;
  int xpow;
  int ypow;
};

typedef map<int, map<int, double>> bipoly;

#endif // poly_types_h
