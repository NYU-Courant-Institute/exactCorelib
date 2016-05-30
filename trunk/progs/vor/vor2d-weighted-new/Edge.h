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
  BiPoly dfun_sq() override;
  pair<BiPoly, BiPoly> dfun_sq_grad() override;
  BiPoly get_tstar();

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
