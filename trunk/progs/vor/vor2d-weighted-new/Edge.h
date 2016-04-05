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
  Edge(const Point2d& p, const Point2d& q, Object* parent);
  double distance(const Point2d& p) override;
  Interval box_dist_sq(const Interval& int_x, const Interval& int_y);
  pair<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y);
  Corner* source() const;
  Corner* dest() const;
  
 private:
  BiPoly make_dfun(Object* parent, const Point2d& p, const Point2d& q);
  double qmv;
  BiPoly qmw;
  BiPoly vmw;
  BiPoly tstar;
  Corner* source_;
  Corner* dest_;
};

} // namespace vor2d
