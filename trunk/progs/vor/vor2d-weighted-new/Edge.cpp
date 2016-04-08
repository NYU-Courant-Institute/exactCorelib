#include "Corner.h"
#include "Edge.h"
#include <math.h>

namespace vor2d {

BiPoly Edge::make_dsegfun(Object* parent, const Point2d& p, const Point2d& q) {
  double a = parent->m()[0];
  double b = parent->m()[1];
  double c = parent->m()[2];

  Point2d v = p - q;
  double v_x = v[0];
  double v_y = v[1];

  BiPoly w_x;
  w_x.add_monomial(1.0, 1, 0);
  w_x.add_monomial(-p[0], 0, 0);
  
  BiPoly w_y;
  w_y.add_monomial(1.0, 0, 1);
  w_y.add_monomial(-p[1], 0, 0);

  // State changing assignments.
  qmw = (a * w_x * w_x) + (2 * b * w_x * w_y) + (c * w_y * w_y);
  vmw = v_x * (a * w_x + b * w_y) + v_y * (b * w_x + c * w_y);
  qmv = parent->qm(v);
  tstar = (1.0 / qmv) * vmw;

  return qmw - (tstar * vmw);
}

Edge::Edge(Corner* source, Corner* dest, Object* parent) :
  source_(source), dest_(dest) {
  parent_ = parent;
  dfun_seg_sq_ = make_dsegfun(parent, source->position(), dest->position());
  dfun_seg_sq_grad_ = dfun_seg_sq_.gradient();
  // source->set_next_edge(this);
  // dest->set_prev_edge(this);
}

Edge::Edge(const Point2d& p, const Point2d& q, Object* parent) :
  Edge(new Corner(p, parent), new Corner(q, parent), parent) {
}

double Edge::distance(const Point2d& p) {
  double tp = tstar.eval(p[0], p[1]);
  if (tp <= 0) {
    return source_->distance(p);
  } else if (tp >= 1) {
    return dest_->distance(p);
  } else { // p in (0, 1)
    return sqrt(dfun_seg_sq_.eval(p[0], p[1]));
  }
}

#define EQ_OR_UN(u, i) (u == TOP ? i : u.convex_union(i))
Interval Edge::box_dist_sq(const Interval& int_x, const Interval& int_y) {
  Interval tp = tstar.eval(int_x, int_y);
  Interval u(TOP);

  if (0 >= tp) {
    u = EQ_OR_UN(u, source_->dfun_sq()->eval(int_x, int_y));
  }
  if (1 <= tp) {
    u = EQ_OR_UN(u, dest_->dfun_sq()->eval(int_x, int_y));
  }
  if (!(0 > tp || 1 < tp)) {
    u = EQ_OR_UN(u, dfun_seg_sq_.eval(int_x, int_y));
  }

  return u;
}

pair<Interval, Interval> Edge::box_dist_sq_grad(const Interval& int_x, const Interval& int_y) {
  Interval tp = tstar.eval(int_x, int_y);
  Interval ux(TOP);
  Interval uy(TOP);
  
  if (0 >= tp) {
    ux = EQ_OR_UN(ux, source_->dfun_sq_grad().first.eval(int_x, int_y));
    uy = EQ_OR_UN(uy, source_->dfun_sq_grad().second.eval(int_x, int_y));
  }
  if (1 <= tp) {
    ux = EQ_OR_UN(ux, dest_->dfun_sq_grad().first.eval(int_x, int_y));
    uy = EQ_OR_UN(uy, dest_->dfun_sq_grad().second.eval(int_x, int_y));
  }
  if (!(0 > tp || 1 < tp)) {
    ux = EQ_OR_UN(ux, dfun_seg_sq_grad_.first.eval(int_x, int_y));
    uy = EQ_OR_UN(uy, dfun_seg_sq_grad_.second.eval(int_x, int_y));
  }

  return pair<Interval, Interval>{ux, uy};
}

// double Edge::distance(const Point2d& r) {
//   // Anisotropic distance computation.
//   const Point2d p = source_->position();
//   const Point2d q = dest_->position();
//   Point2d v = q - p;
//   Point2d w = r - p;
//   double ts = parent_->qm2(v, w) / parent_->qm(v);
//   ts = (ts < 0) ? 0 : ts;
//   ts = (ts > 1) ? 1 : ts;
//   Point2d v2(-ts * v[0], -ts * v[1]);
//   Point2d y = w + v2;
//   return sqrt(parent_->qm(y));
// }

// // TODO: Variables p, q, v, and qmv don't depend on the input. Cache them.
// double Edge::distance(const Point2d& r) {
//   // Anisotropic distance computation.
//   const Point2d p = source_->position();
//   const Point2d q = dest_->position();
//   Point2d v = q - p;
//   Point2d w = r - p;
//   double qmv = parent_->qm(v);
//   double qmw = parent_->qm(w);
//   double vmw = parent_->qm2(v, w);
//   double ts = vmw / qmv;
  
//   if (ts <= 0) {
//     return sqrt(parent_->qm(w));
//   }
//   if (ts >= 1) {
//     Point2d y = r - q;
//     return sqrt(parent_->qm(y));
//   }
//   return sqrt(qmw - vmw * ts);
// }

// Interval Edge::box_dist_sq(const Interval& int_x, const Interval& int_y) {
//   const Point2d p = source_->position();
//   const Point2d q = dest_->position();
//   Point2d v = p - q;
//   Interval p_x(p[0], p[0]);
//   Interval p_y(p[1], p[1]);
//   Interval w_x = int_x - p_x;
//   Interval w_y = int_y - p_y;
//   Interval qmw = parent_->qm_b(w_x, w_y);
//   Interval vmw = parent_->qm2_b(v, w_x, w_y);
//   double qmv = parent_->qm(v);
//   return qmw - (vmw.sq() / qmv);
// }

// tuple<Interval, Interval> Edge::box_dist_sq_grad(const Interval& int_x, const Interval& int_y) {
//   double* m = parent_->m();
//   const Point2d p = source_->position();
//   const Point2d q = dest_->position();
//   Point2d v = p - q;
//   Interval p_x(p[0], p[0]);
//   Interval p_y(p[1], p[1]);
//   Interval w_x = int_x - p_x;
//   Interval w_y = int_y - p_y;
//   Interval v_x(v[0], v[0]);
//   Interval v_y(v[1], v[1]);
//   Interval vmw = parent_->qm2_b(v, w_x, w_y);
//   double qmv = parent_->qm(v);
//   Interval ts = vmw / qmv;
//   Interval u_x = w_x - ts * v_x;
//   Interval u_y = w_y - ts * v_y;
//   Interval r_x = m[0] * u_x + m[1] * u_y;
//   Interval r_y = m[1] * u_x + m[2] * u_y;
//   return make_tuple(2 * r_x, 2 * r_y);
// }

Corner* Edge::dest() const {
  return dest_;
}

Corner* Edge::source() const {
  return source_;
}

} // namespace vor2d
