/**
 *  An adaptation of the smooth quadtree data structure for 2 dimensional Voronoi diagrams.
 *   
 *  Based on:
 *  Bennett, Huck and Chee Yap. "Amortized Analysis of Smooth Quadtrees in All Dimensions."
 *  Scandinavian Symposium and Workshops on Algorithm Theory (SWAT) 2014.
 *
 *  The ith bit of the child index indicates whether the box is above or below
 *  the center of the box in the ith dimension.
 *  
 *  For example, in 2 dimensions the children are indexed as follows:
 *   ______ ______
 *  |      |      |
 *  |  10  |  11  |
 *  |______|______|
 *  |      |      |
 *  |  00  |  01  |
 *  |______|______|
 * 
 *  Written by Huck Bennett, 9/21/2014
 *  TODO(Huck):
 *  - Cleanup direction/child indicator bit logic.
 *  - Replace raw pointers with smart pointers.
 *  - Simplify and document the public API.
 *  - Use only one of #pragma/header guard?
 */

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <math.h>

namespace vor2d {

using std::shared_ptr;
using std::vector;

template <typename T>
class vor_qt {
 public:
  vor_qt(int dimension, double width) : dimension_(dimension), width_(width) {
    assert(1 <= dimension && dimension <= 31);

    double* center = new double[dimension];
    for (int i = 0; i < dimension; i++) {
      center[i] = 0.0;
    }

    root_ = new vor_qtBox(0 /* depth */, 0 /* indicator */, center, this);
    vor_qtBox** neighbors = root_->neighbors();
    for (int i = 0; i < 2 * dimension; i++) {
      neighbors[i] = nullptr;
    }
  }

  ~vor_qt() {
    delete root_;
  }

  vor_qtBox* root() const {
    return root_;
  }

  vor_qtBox* get_box(const vector<double>& point) {
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
    vor_qtBox* cur_box = root_;
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

  const int dimension() const {
    return dimension_;
  }

  int splits() {
    return num_splits_;
  }

  int smooth_splits() {
    return num_smooth_splits_;
  }

  double width() {
    return width_;
  }

 protected:
  void inc_splits() {
    num_splits_++;
  }

  void inc_smooth_splits() {
    num_smooth_splits_++;
  }

  const int dimension_;
  vor_box* root_;

  // Box specification.
  double width_;

  // Statistics
  int num_splits_ = 0;
  int num_smooth_splits_ = 0;

  friend class vor_box;
};

} // namespace vor2d
