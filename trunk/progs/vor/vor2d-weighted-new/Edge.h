#pragma once

#include "Object.h"
#include "Corner.h"
#include "Feature.h"
#include "Point.h"
#include "BiPoly.h"

#include <float.h>
#include <math.h>

namespace vor2d {

class Edge : public Feature {
 public:
  Edge(Corner* source, Corner* dest, Object* parent);
  Edge(const Point2d& p, const Point2d& q, Object* parent);

  BiPoly* dfun_sq(const Interval& int_x, const Interval& int_y) override;
  pair<BiPoly, BiPoly> dfun_sq_grad(const Interval& int_x, const Interval& int_y) override;
  double dist_sq(double x, double int_y) override;
  pair<double, double> dist_sq_grad(double x, double int_y) override;
  Interval box_dist_sq(const Interval& int_x, const Interval& int_y) override;
  pair<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y) override;

  Corner* source() const;
  Corner* dest() const;
  
 private:
  BiPoly make_dsegfun(Object* parent, const Point2d& p, const Point2d& q);

  double qmv;
  BiPoly qmw;
  BiPoly vmw;
  BiPoly tstar;
  Corner* source_;
  Corner* dest_;

  // These polynomials represent the distance to the line through p and q.
  BiPoly dfun_seg_sq_;
  pair<BiPoly, BiPoly> dfun_seg_sq_grad_;
};

} // namespace vor2d
