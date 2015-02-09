#include "Corner.h"
#include "Edge.h"
#include "Point.h"

namespace vor2d {

Corner::Corner(const Point2d& position, Object* parent) :
  position_(position[0], position[1]),
  prev_edge(nullptr), next_edge(nullptr) {
  parent_ = parent;
}

Corner::~Corner() {}

double Corner::distance(const Point2d& point) {
  Point2d p = point - position_;
  return sqrt(parent_->qm(p));
}

Interval Corner::box_distance(const Interval& int_x, const Interval& int_y) {
  Interval p_x(position_[0], position_[0]);
  Interval p_y(position_[1], position_[1]);
  Interval w_x = int_x - p_x;
  Interval w_y = int_y - p_y;
  return parent_->qm_b(w_x, w_y).sqrt_i();
}

tuple<Interval, Interval> Corner::box_grad(const Interval& int_x, const Interval& int_y) {
  Interval p_x(position_[0], position_[0]);
  Interval p_y(position_[1], position_[1]);
  Interval w_x = int_x - p_x;
  Interval w_y = int_y - p_y;
  Interval i = 1.0 / parent_->qm_b(w_x, w_y).sqrt_i();
  double* m = parent_->m();
  Interval r_x = m[0] * w_x + m[1] * w_y;
  Interval r_y = m[1] * w_x + m[2] * w_y;
  return make_tuple(i * r_x, i * r_y);
}

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
