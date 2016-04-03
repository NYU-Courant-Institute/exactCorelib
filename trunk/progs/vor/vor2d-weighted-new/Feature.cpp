#include "Feature.h"
#include "Object.h"
#include "Point.h"

#include <math.h>

#define SQ(x) ((x) * (x))

namespace vor2d {

Feature::Feature(BiPoly dfun_sq) {
  dfun_sq_ = dfun_sq;
  dfun_sq_grad_ = dfun_sq.gradient();
}

Feature::~Feature() {}

void Feature::set_parent(Object* parent) {
  parent_ = parent;
}

Object* Feature::parent() {
  return parent_;
}

// See derivation of Lipschitz constant in paper.
double Feature::lipschitz() const {
  double* m = parent_->m();
  return (1.0 / sqrt(2)) * sqrt(m[0] + m[2] + sqrt(SQ(m[0] - m[2]) + 4 * SQ(m[1])));
}

BiPoly* Feature::dfun_sq() {
  return &dfun_sq_;
}

pair<BiPoly, BiPoly>* Feature::dfun_sq_grad() {
  return &dfun_sq_grad_;
}

// double Feature::distance(const Point2d& point) {
//   return sqrt(dfun_sq_.eval(point[0], point[1]));
// }

} // namespace vor2d
