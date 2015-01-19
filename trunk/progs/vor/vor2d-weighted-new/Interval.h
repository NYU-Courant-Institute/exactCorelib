#pragma once

#include <assert.h>
#include <math.h>

using namespace std;

/**
 * A class for basic interval arithmetic.
 * Reference: http://interval.louisiana.edu/preprints/survey.pdf
 */
class Interval {
 public:
  Interval(double a, double b) : a_(a), b_(b) {
    assert(a_ <= b_);
  }

  Interval operator+(const Interval& i2) const {
    Interval i(a_ + i2.a_, b_ + i2.b_);
    return i;
  }

  Interval operator-(const Interval& i2) const {
    Interval i(a_ - i2.b_, b_ - i2.a_);
    return i;
  }

  Interval operator*(const Interval& i2) const {
    double ac = a_ * i2.a_;
    double ad = a_ * i2.b_;
    double bc = b_ * i2.a_;
    double bd = b_ * i2.b_;
    double l = fmin(fmin(ac, ad), fmin(bc, bd));
    double r = fmax(fmax(ac, ad), fmax(bc, bd));
    Interval i(l, r);
    return i;
  }

  Interval operator/(const Interval& i2) const {
    // TODO: Consolidate with "operator*"? 
    double ac = a_ / i2.a_;
    double ad = a_ / i2.b_;
    double bc = b_ / i2.a_;
    double bd = b_ / i2.b_;
    double l = fmin(fmin(ac, ad), fmin(bc, bd));
    double r = fmax(fmax(ac, ad), fmax(bc, bd));
    Interval i(l, r);
    return i;
  }

  Interval operator/(const double c) const {
    double ac = a_ / c;
    double bc = b_ / c;
    Interval i(fmin(ac, bc), fmax(ac, bc));
    return i;
  }

  static Interval sqrt_i(const Interval& i) {
    assert(i.a_ >= 0.0);
    Interval i2(sqrt(i.a_), sqrt(i.b_));
    return i2;
  }
 
  double a_;
  double b_;
};

inline Interval operator*(double c, const Interval& i) {
  double e1 = c * i.a_;
  double e2 = c * i.b_;
  Interval i2(fmin(e1, e2), fmax(e1, e2));
  return i2;
}

inline Interval operator/(double c, const Interval& i) {
  assert(i.a_ >= 0 || i.b_ <= 0);
  Interval i2(c / i.b_, c / i.a_);
  return i2;
}
