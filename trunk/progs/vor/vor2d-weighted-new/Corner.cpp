#include "Corner.h"
#include "Edge.h"
#include "Point.h"


namespace vor2d {

BiPoly Corner::make_dfun(Object* parent, const Point2d& p) {
  double a = parent->m()[0];
  double b = parent->m()[1];
  double c = parent->m()[2];
  
  BiPoly w_x;
  w_x.add_monomial(1.0, 1, 0);
  w_x.add_monomial(-p[0], 0, 0);
  
  BiPoly w_y;
  w_y.add_monomial(1.0, 0, 1);
  w_y.add_monomial(-p[1], 0, 0);
  
  return (a * w_x * w_x) + (2 * b * w_x * w_y) + (c * w_y * w_y);
}

Corner::Corner(const Point2d& position, Object* parent) :
  Feature(make_dfun(parent, position)),
  position_(position[0], position[1]),
  prev_edge(nullptr), next_edge(nullptr) {
  parent_ = parent;
}

Corner::~Corner() {}

// double Corner::distance(const Point2d& point) {
//   Point2d p = point - position_;
//   return sqrt(parent_->qm(p));
// }

// Interval Corner::box_dist_sq(const Interval& int_x, const Interval& int_y) {
//   Interval p_x(position_[0]);
//   Interval p_y(position_[1]);
//   Interval w_x = int_x - p_x;
//   Interval w_y = int_y - p_y;
//   return parent_->qm_b(w_x, w_y);
// }

// tuple<Interval, Interval> Corner::box_dist_sq_grad(const Interval& int_x, const Interval& int_y) {
//   double* m = parent_->m();
//   Interval p_x(position_[0]);
//   Interval p_y(position_[1]);
//   Interval w_x = int_x - p_x;
//   Interval w_y = int_y - p_y;
//   Interval r_x = 2 * (m[0] * w_x + m[1] * w_y);
//   Interval r_y = 2 * (m[1] * w_x + m[2] * w_y);
//   return make_tuple(r_x, r_y);
// }

bool Corner::is_isolated() {
  return prev_edge == nullptr && next_edge == nullptr;
}

bool Corner::is_dangling() {
  return prev_edge == nullptr || next_edge == nullptr;    
}

void Corner::set_prev_edge(Edge* edge) {
  prev_edge = edge;
}

void Corner::set_next_edge(Edge* edge) {
  next_edge = edge;
}

const Point2d Corner::position() const {
  return position_;
}

bool Corner::operator==(const Corner& other) {
  return position_ == other.position();
}

} // namespace vor2d
