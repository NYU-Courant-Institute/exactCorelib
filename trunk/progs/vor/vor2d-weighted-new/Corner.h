#pragma once

#include "Feature.h"
#include "Point.h"

namespace vor2d {
class Edge;

class Corner : public Feature {
 public:
  Corner(const Point2d& position, Object* parent);
  ~Corner();
  BiPoly* dfun_sq();
  pair<BiPoly, BiPoly> dfun_sq_grad();

  BiPoly* dfun_sq(const Interval& int_x, const Interval& int_y) override;
  pair<BiPoly, BiPoly> dfun_sq_grad(const Interval& int_x, const Interval& int_y) override;
  double dist_sq(double x, double int_y) override;
  pair<double, double> dist_sq_grad(double x, double int_y) override;
  Interval box_dist_sq(const Interval& int_x, const Interval& int_y) override;
  pair<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y) override;
  
  Point2d position() const;
  bool operator==(const Corner& other);

  friend ostream& operator<<(ostream& os, const Corner& c) {
    return os << "(" << c.position()[0] << ", " << c.position()[1] << ")";
  }

 private:
  BiPoly make_dfun(Object* parent, const Point2d& p);
  
  BiPoly dfun_sq_;
  pair<BiPoly, BiPoly> dfun_sq_grad_;
  const Point2d position_;
};

} // namespace vor2d
