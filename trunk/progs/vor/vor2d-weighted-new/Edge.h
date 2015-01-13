#pragma once

#include "Feature.h"
#include "Point.h"

#include <float.h>
#include <math.h>

namespace vor2d {
class Corner;

class Edge : public Feature {
 public:
  Edge(Corner* source, Corner* dest, Object* parent);
  double distance(const Point2d& p) override;
  Corner* source() const;
  Corner* dest() const;
  
 private:
  double aniso_distance(const Point2d& p);
  double iso_distance(const Point2d& p);
  Corner* source_;
  Corner* dest_;
};

} // namespace vor2d
