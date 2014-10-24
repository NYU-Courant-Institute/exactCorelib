#include "Corner.h"
#include "Edge.h"
#include "Point.h"

namespace vor2d {

Corner::Corner(const Point2d& position) : position_(position) {
  prev_edge = next_edge = nullptr;
}
Corner::~Corner() {}

double Corner::distance(const Point2d& point) {
  return (point - position_).norm() / parent_->weight();
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
