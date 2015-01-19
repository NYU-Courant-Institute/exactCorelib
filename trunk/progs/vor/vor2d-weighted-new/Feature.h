#pragma once

#include "Interval.h"
#include "Object.h"
#include "Point.h"

#include <tuple>

namespace vor2d {

class Object;

class Feature {
 public:
  // Compute the distance to a point.
  virtual double distance(const Point2d& point) = 0;
  virtual Interval box_distance(const Interval& int_x, const Interval& int_y) = 0;
  virtual tuple<Interval, Interval> box_grad(const Interval& int_x, const Interval& int_y) = 0;
  double lipschitz() const;
  void set_parent(Object* parent);
  Object* parent();

 protected:
  Object* parent_;
};

} // namespace vor2d
