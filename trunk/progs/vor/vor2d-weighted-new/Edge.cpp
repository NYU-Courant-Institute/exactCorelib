#include "Corner.h"
#include "Edge.h"

namespace vor2d {

Edge::Edge(Corner* source, Corner* dest, Object* parent) : source_(source), dest_(dest) {
  parent_ = parent;
  source->set_next_edge(this);
  dest->set_prev_edge(this);
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

// TODO: Variables p, q, v, and qmv don't depend on the input. Cache them.
double Edge::distance(const Point2d& r) {
  // Anisotropic distance computation.
  const Point2d p = source_->position();
  const Point2d q = dest_->position();
  Point2d v = q - p;
  Point2d w = r - p;
  double qmv = parent_->qm(v);
  double qmw = parent_->qm(w);
  double vmw = parent_->qm2(v, w);
  double ts = vmw / qmv;
  
  if (ts <= 0) {
    return sqrt(parent_->qm(w));
  }
  if (ts >= 1) {
    Point2d y = r - q;
    return sqrt(parent_->qm(y));
  }
  return sqrt(qmw - vmw * ts);
}

Interval Edge::box_distance(const Interval& int_x, const Interval& int_y) {
  const Point2d p = source_->position();
  const Point2d q = dest_->position();
  Point2d v = p - q;
  Interval p_x(p[0], p[0]);
  Interval p_y(p[1], p[1]);
  Interval w_x = int_x - p_x;
  Interval w_y = int_y - p_y;
  Interval qmw = parent_->qm_b(w_x, w_y);
  Interval vmw = parent_->qm2_b(v, w_x, w_y);
  double qmv = parent_->qm(v);
  return Interval::sqrt_i(qmw - (vmw * vmw / qmv));
}

tuple<Interval, Interval> Edge::box_grad(const Interval& int_x, const Interval& int_y) {
  const Point2d p = source_->position();
  const Point2d q = dest_->position();
  Point2d v = p - q;
  Interval p_x(p[0], p[0]);
  Interval p_y(p[1], p[1]);
  Interval w_x = int_x - p_x;
  Interval w_y = int_y - p_y;
  Interval v_x(v[0], v[0]);
  Interval v_y(v[1], v[1]);
  Interval qmw = parent_->qm_b(w_x, w_y);
  Interval vmw = parent_->qm2_b(v, w_x, w_y);
  double qmv = parent_->qm(v);
  Interval r = Interval::sqrt_i(qmw - (vmw * vmw / qmv));
  Interval ts = vmw / qmv;
  Interval u_x = w_x - ts * v_x;
  Interval u_y = w_y - ts * v_y;
  double* m = parent_->m();
  return make_tuple(m[0] * u_x + m[1] * u_y, m[1] * u_x + m[2] * u_y);
}

Corner* Edge::dest() const {
  return dest_;
}

Corner* Edge::source() const {
  return source_;
}

} // namespace vor2d
