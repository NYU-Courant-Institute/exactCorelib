#include "Corner.h"
#include "Edge.h"

namespace vor2d {

Edge::Edge(Corner* source, Corner* dest, Object* parent) : source_(source), dest_(dest) {
  parent_ = parent;
  source->set_next_edge(this);
  dest->set_prev_edge(this);
}

double Edge::distance(const Point2d& r) {
  // Anisotropic distance computation.
  const Point2d p = source_->position();
  const Point2d q = dest_->position();
  Point2d v = q - p;
  Point2d w = r - p;
  double ts = parent_->qm2(v, w) / parent_->qm(v);
  ts = (ts < 0) ? 0 : ts;
  ts = (ts > 1) ? 1 : ts;
  Point2d v2(-ts * v[0], -ts * v[1]);
  Point2d y = w + v2;
  return sqrt(parent_->qm(y));
}

Corner* Edge::dest() const {
  return dest_;
}

Corner* Edge::source() const {
  return source_;
}

} // namespace vor2d
