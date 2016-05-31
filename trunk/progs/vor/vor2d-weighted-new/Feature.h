#pragma once

#include "Interval.h"
#include "Point.h"
#include "BiPoly.h"

namespace vor2d {

class Object;

class Feature {
 public:
  Feature();
  virtual BiPoly dfun_sq() = 0;
  virtual pair<BiPoly, BiPoly> dfun_sq_grad() = 0;

  virtual double distance(const Point2d& point);
  double lipschitz() const;
  void set_parent(Object* parent);
  Object* parent();
  bool is_corner();
  bool is_edge();

 protected:
  Object* parent_;
};

} // namespace vor2d
