#pragma once

#include "Interval.h"
#include "Object.h"
#include "Point.h"
#include "BiPoly.h"

#include <tuple>

namespace vor2d {

class Object;

class Feature {
 public:
  // Compute the distance to a point.
  Feature(BiPoly dfun);
  virtual ~Feature();
  double distance(const Point2d& point);
  // virtual Interval box_dist_sq(const Interval& int_x, const Interval& int_y) = 0; // TODO: Remove.
  BiPoly* dfun_sq();
  pair<BiPoly, BiPoly>* dfun_sq_grad();
  // virtual tuple<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y) = 0; // TODO: Remove.
  double lipschitz() const;
  void set_parent(Object* parent);
  Object* parent();

 protected:
  Object* parent_;
  BiPoly dfun_sq_;
  pair<BiPoly, BiPoly> dfun_sq_grad_;
};

} // namespace vor2d
