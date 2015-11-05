#pragma once

#include <assert.h>
#include <iostream>
#include <limits>
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

  // Singleton constructor.
  Interval(double s) : a_(s), b_(s) { }

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

  friend ostream& operator<<(ostream& os, const Interval& i) {
    return os << "[" << i.a_ << ", " << i.b_ << "]";
  }

  Interval sq() const {
    double asq = a_ * a_;
    double bsq = b_ * b_;
    Interval i2(a_ < 0 && b_ > 0 ? 0.0 : fmin(asq, bsq), fmax(asq, bsq));
    return i2;
  }

  Interval* intersect(const Interval& i) const {
    double left = fmax(a_, i.a_);
    double right = fmin(b_, i.b_);

    if (left > right) {
      return NULL;
    } else {
      return new Interval(left, right);
    }
  }

  Interval sqrt_i() const {
    assert(b_ >= 0.0);

    // TODO: Find a better way to handle this situation.
    if (a_ <= 0.0) {
      cout << "Warning: sqrt_i() input interval is " << *this << "\n";
    }
    Interval i2(a_ >= 0 ? sqrt(a_) : 0.0, sqrt(b_));
    return i2;
  }
 
  // Public variables representing interval [a_, b_].
  double a_; // Left endpoint.
  double b_; // Right endpoint.
};

const Interval TOP(numeric_limits<double>::min(), numeric_limits<double>::max());

inline Interval operator*(double c, const Interval& i) {
  double e1 = c * i.a_;
  double e2 = c * i.b_;
  Interval i2(fmin(e1, e2), fmax(e1, e2));
  return i2;
}

inline Interval operator/(double c, const Interval& i) {
  if (i.a_ <= 0 && i.b_ >= 0) {
    cout << "Warning: denominator interval is " << i << "\n";
    return TOP;
  }

  assert(i.a_ > 0 || i.b_ < 0);
  Interval i2(c / i.b_, c / i.a_);
  return i2;
}
