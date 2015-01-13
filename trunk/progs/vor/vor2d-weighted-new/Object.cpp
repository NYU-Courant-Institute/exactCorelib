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

double Object::qm(const Point2d& p) {
  // p^T M p = a p_x^2 + 2 b p_x p_y + c p_y^2
  return m_[0] * p[0] * p[0] + 2 * m_[1] * p[0] * p[1] + m_[2] * p[1] * p[1];
}

double Object::qm2(const Point2d& p, const Point2d& q) {
  // p^T M q = q_x (a p_x + b p_y) + q_y (b p_x + c p_y)
  return q[0] * (m_[0] * p[0] + m_[1] * p[1]) + q[1] * (m_[1] * p[0] + m_[2] * p[1]);
}

double* Object::m() {
  return m_;
}

} // namespace vor2d
