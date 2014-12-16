#include "Object.h"
#include "Point.h"
#include "Feature.h"
#include <vector>
#include <limits>

namespace vor2d {
Object::Object(double weight) : weight_(weight), m_{0.0, 0.0, 0.0} {}
Object::Object(double a, double b, double c) : weight_(0.0), m_{a, b, c} {}
Object::~Object() {}

void Object::add_feature(Feature* feature) {
  features_.push_back(feature);
}

double Object::distance(Point2d point) {
  double min_sep = std::numeric_limits<double>::max();
  for (Feature* feature : features_) {
    double feature_sep = feature->distance(point);
    if (feature_sep < min_sep) {
      min_sep = feature_sep;
    }
  }
  return min_sep;
}

double Object::weight() {
  return weight_;
}

double* Object::m() {
  return m_;
}

} // namespace vor2d
