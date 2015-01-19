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
  // The following is an upper bound on the Lipschitz constant.
  // TODO: Compute expression for exact Lipschitz constant.
  return sqrt(m[0] + 2 * fabs(m[1]) + m[2]);
}

} // namespace vor2d
