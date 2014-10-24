#pragma once

#include "Feature.h"
#include "Point.h"

#include <float.h>
#include <math.h>

namespace vor2d {
class Corner;

class Edge : public Feature {
 public:
  Edge(Corner* source, Corner* dest);

  double distance(const Point2d& p) override;
  
 private:
  Corner* source_;
  Corner* dest_;
};

} // namespace vor2d
