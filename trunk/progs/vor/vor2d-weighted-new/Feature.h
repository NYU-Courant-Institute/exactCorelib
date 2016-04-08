#pragma once

#include "Interval.h"
#include "Object.h"
#include "Point.h"
#include "BiPoly.h"

namespace vor2d {

class Object;

class Feature {
 public:
  Feature();
  virtual double distance(const Point2d& point) = 0;
  virtual Interval box_dist_sq(const Interval& int_x, const Interval& int_y) = 0;
  virtual pair<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y) = 0;
  double lipschitz() const;
  void set_parent(Object* parent);
  Object* parent();

 protected:
  Object* parent_;
};

} // namespace vor2d
