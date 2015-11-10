#pragma once

#include "Feature.h"
#include "Point.h"

namespace vor2d {
class Edge;

class Corner : public Feature {
 public:
  Corner(const Point2d& position, Object* parent);
  ~Corner();
  double distance(const Point2d& point) override;
  Interval box_dist_sq(const Interval& int_x, const Interval& int_y) override;
  tuple<Interval, Interval> box_dist_sq_grad(const Interval& int_x, const Interval& int_y) override;

  // TODO: Generalize.
  // TEST CODE ONLY.
  static Interval pair_box_dist_sq(
    const Corner& u, const Corner& v, const Interval& int_x, const Interval& int_y);
  static tuple<Interval, Interval> pair_dist_sq_grad(
    const Corner& u, const Corner& v, const Interval& int_x, const Interval& int_y);
  
  bool is_isolated();
  bool is_dangling();
  void set_prev_edge(Edge* edge);
  void set_next_edge(Edge* edge);
  const Point2d position() const;
  bool operator==(const Corner& other);

  friend ostream& operator<<(ostream& os, const Corner& c) {
    return os << "(" << c.position()[0] << ", " << c.position()[1] << ")";
  }

 private:
  const Point2d position_;
  Edge* prev_edge;
  Edge* next_edge;
};

} // namespace vor2d
