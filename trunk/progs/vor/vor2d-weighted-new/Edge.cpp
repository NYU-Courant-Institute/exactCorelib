#include "Corner.h"
#include "Edge.h"

namespace vor2d {

Edge::Edge(Corner* source, Corner* dest, Object* parent) : source_(source), dest_(dest) {
  parent_ = parent;
  source->set_next_edge(this);
  dest->set_prev_edge(this);
}

double Edge::distance(const Point2d& p) {
  return parent_->anisotropic() ? aniso_distance(p) : iso_distance(p);
}

double Edge::iso_distance(const Point2d& p) {
  double x1 = source_->position()[0];
  double y1 = source_->position()[1];
  double x2 = dest_->position()[0];
  double y2 = dest_->position()[1];
  double u = ((p[0] - x1) * (x2 - x1) + (p[1] - y1) * (y2 - y1)) 
    / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  u = u < 0 ? 0 : u;
  u = u > 1 ? 1 : u;
  double x0 = u * x2 + (1 - u) * x1;
  double y0 = u * y2 + (1 - u) * y1;
  return parent_->weight() * sqrt(sqr(p[0] - x0) + sqr(p[1] - y0));
}

double Edge::aniso_distance(const Point2d& r) {
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
