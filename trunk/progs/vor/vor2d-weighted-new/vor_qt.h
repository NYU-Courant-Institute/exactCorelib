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

#include "vor_box.h"

#include <memory>
#include <vector>

namespace vor2d {

using std::shared_ptr;
using std::vector;

class vor_qt {
 public:
  vor_qt(int dimension, double width);
  ~vor_qt();

  vor_box* root();
  vor_box* get_box(const vector<double>& point);
  const int dimension() const;
  int splits();
  int smooth_splits();
  double width();

 protected:
  void inc_splits();
  void inc_smooth_splits();

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
