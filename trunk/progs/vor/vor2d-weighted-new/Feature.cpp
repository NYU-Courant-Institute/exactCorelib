#include "Feature.h"
#include "Object.h"
#include "Point.h"

namespace vor2d {

void Feature::set_parent(Object* parent) {
  parent_ = parent;
}

Object* Feature::parent() {
  return parent_;
}

double Feature::lipschitz() const {
  return parent_->weight();
}

} // namespace vor2d
