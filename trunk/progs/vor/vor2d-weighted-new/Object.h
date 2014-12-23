#pragma once

#include "Point.h"
#include <vector>
#include <limits>

namespace vor2d {

class Feature;

// A class to represent a collection of features.
class Object {
 public:
  Object(double weight);
  Object(double a, double b, double c);
  ~Object();

  // Whether to use the multiplicative weight or anisotropic
  // weights for the metric.
  bool anisotropic() {
    return weight_ == 0.0;
  }

  // Add a feature to the object.
  void add_feature(Feature* feature);

  // Compute the distance between an object and this point.
  virtual double distance(Point2d point);

  double weight();
  double qm(const Point2d& p);
  double qm2(const Point2d& p, const Point2d& q);
  double* m();

 private:
  // A sequence of features.
  vector<Feature*> features_;
  double weight_;
  double m_[3]; // Weight metric
};

} // namespace vor2d
