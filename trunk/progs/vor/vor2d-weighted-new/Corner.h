#pragma once

#include "Feature.h"
#include "Point.h"

namespace vor2d {
class Edge;

class Corner : public Feature {
 public:
  Corner(const Point2d& position, Object* parent);
  ~Corner();
  BiPoly dfun_sq() override;
  pair<BiPoly, BiPoly> dfun_sq_grad() override;
  
  Point2d position() const;

  friend ostream& operator<<(ostream& os, const Corner& c) {
    return os << "(" << c.position()[0] << ", " << c.position()[1] << ")";
  }

 private:
  BiPoly make_dfun(Object* parent, const Point2d& p);
  
  BiPoly dfun_sq_;
  pair<BiPoly, BiPoly> dfun_sq_grad_;
  const Point2d position_;
};

} // namespace vor2d
