#pragma once

#include "Object.h"
#include "Point.h"

namespace vor2d {

class Object;

class Feature {
 public:
  // Compute the distance to a point.
  virtual double distance(const Point2d& point) = 0;
  void set_parent(Object* parent);
  Object* parent();

 protected:
  Object* parent_;
};

} // namespace vor2d
