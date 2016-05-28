#pragma once

#include "Corner.h"
#include "Edge.h"
#include "Point.h"
#include "Feature.h"
#include "Interval.h"
#include "Point.h"
#include <vector>
#include <limits>

namespace vor2d {

class Corner;
class Edge;
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
  bool is_polygon() const;
  bool contained_in(const Point2d& p) const;
  bool contained_in(const Interval& int_x, const Interval& int_y) const;

 private:
  // Determines whether r is to the left, right or on the line going through p, q.
  int in_on_out(Point2d p, Point2d q, Point2d r) const;
  
  vector<Feature*> features_;
  vector<Corner*> corners_;
  vector<Edge*> edges_;
  double m_[3]; // Weight metric
};

} // namespace vor2d
