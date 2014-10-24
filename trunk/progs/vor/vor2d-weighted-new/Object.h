#pragma once

#include "Point.h"
#include <vector>
#include <limits>

namespace vor2d {

class Feature;

// A class to represent a semantically related
// collection of features.
class Object {
 public:
  Object(double weight);
  ~Object();

  // Add a feature to the object.
  void add_feature(Feature* feature);

  // Compute the distance between an object and this point.
  virtual double distance(Point2d point);

  double weight();

 private:
  // A sequence of features.
  vector<Feature*> features_;
  double weight_;
};

} // namespace vor2d
