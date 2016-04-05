#pragma once

#include "Interval.h"
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

  // Add a feature to the object.
  void add_feature(Feature* feature);

  // Compute the distance between an object and this point.
  double distance(const Point2d& point) const;
  // double Interval box_distance(const Interval& int_x, const Interval& int_y);
  // tuple<Interval, Interval> box_grad(const Interval& int_x, const Interval& int_y);

  double qm(const Point2d& p);
  double qm2(const Point2d& p, const Point2d& q);
  Interval qm_b(const Interval& int_x, const Interval& int_y);
  Interval qm2_b(const Point2d& p, const Interval& int_x, const Interval& int_y);
  double* m();

 private:
  // A sequence of features.
  vector<Feature*> features_;
  double m_[3]; // Weight metric
};

} // namespace vor2d
