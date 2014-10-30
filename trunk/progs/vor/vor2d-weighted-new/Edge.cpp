#include "Corner.h"
#include "Edge.h"

namespace vor2d {

Edge::Edge(Corner* source, Corner* dest) : source_(source), dest_(dest) {
  source->set_next_edge(this);
  dest->set_prev_edge(this);
}

double Edge::distance(const Point2d& p) {
  double x1 = source_->position()[0];
  double x2 = dest_->position()[0];
  double y1 = source_->position()[1];
  double y2 = dest_->position()[1];
  double u = ((p[0] - x1) * (x2 - x1) + (p[1] - y1) * (y2 - y1)) 
    / ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  u = u < 0 ? 0 : u;
  u = u > 1 ? 1 : u;
  double x0 = u * x2 + (1 - u) * x1;
  double y0 = u * y2 + (1 - u) * y1;
  return sqrt(sqr(p[0] - x0) + sqr(p[1] - y0)) / parent_->weight();
}

Corner* Edge::dest() const {
  return dest_;
}

Corner* Edge::source() const {
  return source_;
}

} // namespace vor2d
