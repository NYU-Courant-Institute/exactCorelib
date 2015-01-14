#include "Feature.h"
#include "Object.h"
#include "Point.h"

#include <math.h>

namespace vor2d {

void Feature::set_parent(Object* parent) {
  parent_ = parent;
}

Object* Feature::parent() {
  return parent_;
}

double Feature::lipschitz() const {
  double* m = parent_->m();
  double tmp = (m[0] - m[2]);
  double discr =  (tmp * tmp) + (4.0 * m[1] * m[1]);
  return sqrt(m[0] + m[2] + sqrt(discr));
}

} // namespace vor2d
