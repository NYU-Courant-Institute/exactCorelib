#include "Feature.h"
#include "Object.h"
#include "Point.h"

#include <math.h>

#define SQ(x) ((x) * (x))

namespace vor2d {

void Feature::set_parent(Object* parent) {
  parent_ = parent;
}

Object* Feature::parent() {
  return parent_;
}

double Feature::lipschitz() const {
  double* m = parent_->m();
  return (1.0 / sqrt(2)) * sqrt(m[0] + m[2] + sqrt(SQ(m[0] - m[2]) + 4 * SQ(m[1])));
}

} // namespace vor2d
