#pragma once

#include "Feature.h"
#include "Point.h"

#include <float.h>
#include <math.h>

namespace vor2d {
class Corner;

class Edge : public Feature {
 public:
  Edge(Corner* source, Corner* dest, Object* parent);
  double distance(const Point2d& p) override;
  Interval box_dist_sq(const Interval& int_x, const Interval& int_y) override;
  tuple<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y) override;
  
  Corner* source() const;
  Corner* dest() const;
  
 private:
  static BiPoly make_dfun(Object* parent, const Point2d& p, const Point2d& q);
  
  Corner* source_;
  Corner* dest_;
};

} // namespace vor2d
