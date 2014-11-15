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
  double lipschitz() const override;
  bool is_isolated();
  bool is_dangling();
  void set_prev_edge(Edge* edge);
  void set_next_edge(Edge* edge);
  const Point2d position() const;
  bool operator==(const Corner& other);

 private:
  const Point2d position_;
  Edge* prev_edge;
  Edge* next_edge;
};

} // namespace vor2d
