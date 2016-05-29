#include "Object.h"
#include "vor_box.h"

namespace vor2d {
Object::Object(double a, double b, double c) : m_{a, b, c} {}
Object::Object(double wt) : Object(wt * wt, 0.0, wt * wt) {}
Object::~Object() {}

void Object::add_feature(Feature* feature) {
  features_.push_back(feature);

  // This dynamic casting is a hack from StackOverflow for handling C++'s lack of "instanceof".
  // TODO: Use typeid or some other functionality?
  Corner* c = dynamic_cast<Corner*>(feature);
  if (c != nullptr) {
    corners_.push_back(c);
  } else {
    edges_.push_back(dynamic_cast<Edge*>(feature));
  }
}

double Object::distance(const Point2d& point) const {
  double min_sep = std::numeric_limits<double>::max();
  for (Feature* feature : features_) {
    double feature_sep = feature->distance(point);
    if (feature_sep < min_sep) {
      min_sep = feature_sep;
    }
  }
  return min_sep;
}

double Object::qm(const Point2d& p) {
  // p^T M p = a p_x^2 + 2 b p_x p_y + c p_y^2
  // return m_[0] * p[0] * p[0] + 2 * m_[1] * p[0] * p[1] + m_[2] * p[1] * p[1];
  return qm2(p, p);
}

double Object::qm2(const Point2d& p, const Point2d& q) {
  // p^T M q = q_x (a p_x + b p_y) + q_y (b p_x + c p_y)
  return q[0] * (m_[0] * p[0] + m_[1] * p[1]) + q[1] * (m_[1] * p[0] + m_[2] * p[1]);
}

Interval Object::qm_b(const Interval& int_x, const Interval& int_y) {
  return m_[0] * int_x.sq() + 2 * m_[1] * int_x * int_y + m_[2] * int_y.sq();
}

Interval Object::qm2_b(const Point2d& p, const Interval& int_x, const Interval& int_y) {
  return (m_[0] * p[0] + m_[1] * p[1]) * int_x + (m_[1] * p[0] + m_[2] * p[1]) * int_y;
}

double* Object::m() {
  return m_;
}

/* Returns the sign of:
 *
 *     | p_x q_x r_x |
 * det | p_y q_y r_y |
 *     |  1   1   1  |
 */
int Object::in_on_out(Point2d p, Point2d q, Point2d r) const {
  double det = p[0] * (q[1] - r[1]) - q[0] * (p[1] - r[1]) + r[0] * (p[1] - q[1]);
  if (det < 0) {
    return -1;
  } else if (det > 0) {
    return 1;
  } else {
    return 0;
  }
}

bool Object::is_polygon() const {
  return features_.size() > 3;
}

bool Object::contains(const Point2d& r) const {
  if (!is_polygon()) {
    return false;
  }

  for (Edge* e : edges_) {
    //cout << in_on_out(e->source()->position(), e->dest()->position(), r) << "\n";
    if (in_on_out(e->source()->position(), e->dest()->position(), r) > 0) {
      return false;
    }
  }

  //cout << "true\n";
  return true;
}

bool Object::contains(const vor_box& b) const {
  double hw = b.width() / 2;
  double cx = b.center()[0];
  double cy = b.center()[1];
  return contains(Point2d{cx + hw, cy + hw}) &&
    contains(Point2d{cx - hw, cy + hw}) &&
    contains(Point2d{cx + hw, cy - hw}) &&
    contains(Point2d{cx - hw, cy - hw});    
}

vector<Feature*>* Object::get_features() {
  return &features_;
}


} // namespace vor2d
