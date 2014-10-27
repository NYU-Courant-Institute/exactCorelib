#include "vor_box.h"
#include "vor_qt.h"

#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <math.h>

namespace vor2d {

using std::shared_ptr;
using std::vector;

vor_qt::vor_qt(int dimension, double width) : dimension_(dimension), width_(width) {
  assert(1 <= dimension && dimension <= 31);

  double* center = new double[dimension];
  for (int i = 0; i < dimension; i++) {
    center[i] = 0.0;
  }

  root_ = new vor_box(0 /* depth */, 0 /* indicator */, center, this);
  vor_box** neighbors = root_->neighbors();
  for (int i = 0; i < 2 * dimension; i++) {
    neighbors[i] = nullptr;
  }
}

vor_qt::~vor_qt() {
  delete root_;
}

vor_box* vor_qt::root() {
  return root_;
}

vor_box* vor_qt::get_box(const vector<double>& point) {
  if (point.size() != dimension_) {
    return nullptr;
  }
  
  // Verify that the query point is within the initial bounding box.
  for (int i = 0; i < dimension_; i++) {
    if (point[i] < -width_ || point[i] > width_) {
      return nullptr;
    }
  }

  // Recurse through the tree.
  vor_box* cur_box = root_;
  while (!cur_box->is_leaf()) {
    int ind = 0;
    for (int i = 0; i < dimension_; i++) {
      if (point[i] > cur_box->center()[i]) {
	ind |= (1 << i);
      }
    }
    cur_box = cur_box->children()[ind];
  }

  return cur_box;
}

const int vor_qt::dimension() const {
  return dimension_;
}

int vor_qt::splits() {
  return num_splits_;
}

int vor_qt::smooth_splits() {
  return num_smooth_splits_;
}

double vor_qt::width() {
  return width_;
}

void vor_qt::inc_splits() {
  num_splits_++;
}

void vor_qt::inc_smooth_splits() {
  num_smooth_splits_++;
}

} // namespace vor2d
