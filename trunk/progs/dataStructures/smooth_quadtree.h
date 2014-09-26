/**
 *  File: smooth_quadtree.h
 *  Version 1.0.
 *
 *  Description: 
 *  A templated, d-dimensional implementation of the smooth quadtree data structure.
 *  
 *  Based on:
 *  Bennett, Huck and Chee Yap. "Amortized Analysis of Smooth Quadtrees in All Dimensions."
 *  Scandinavian Symposium and Workshops on Algorithm Theory (SWAT) 2014.
 *
 *  Written by Huck Bennett, 9/21/2014
 *  TODO(Huck):
 *  - Cleanup direction/child indicator bit logic.
 *  - Replace raw pointers with smart pointers.
 *  - Simplify and document the public API.
 */

#ifndef SMOOTH_QUADTREE_H
#define SMOOTH_QUADTREE_H

#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>
#include <math.h>

using std::shared_ptr;
using std::vector;

template <typename T>
class SmoothQuadTree;
  
template <typename T>
class SmoothQuadTreeBox {
 public:
  SmoothQuadTreeBox<T>(int depth, int indicator, double center[], SmoothQuadTree<T>* tree)
  : depth_(depth), indicator_(indicator), center_(center), tree_(tree) {
    neighbors_ = new SmoothQuadTreeBox<T>*[2 * dimension()];
    children_ = nullptr;
    data_ = nullptr;
  }

  ~SmoothQuadTreeBox<T>() {
    // If the box is split then recurse to children.
    if (children_ != nullptr) {
      for (int i = 0; i < (1 << dimension()); i++) {
	delete children_[i];
      }
      delete[] children_;
    }

    delete[] neighbors_;
    delete[] center_;
  }

  void smooth_split() {
    if (is_split()) {
      return;
    }
    smooth_split_aux();
    tree_->inc_smooth_splits();
  }

  void set_data(T* data) {
    data_ = data;
  }

  T* data() const {
    return data_;
  }

  double* center() {
    return center_;
  }

  SmoothQuadTreeBox<T>** children() const {
    return children_;
  }

  bool is_split() const {
    return children_ != nullptr;
  }

  SmoothQuadTreeBox<T>* principal_neighbor_dir(int dir) {
    int axis = fabs(dir);
    int sign = (dir < 0) ? 0 : 1;
    return neighbors_[(axis - 1) + sign * dimension()];
  }

  shared_ptr<vector<SmoothQuadTreeBox<T>*>> leaf_neighbors_dir(int dir) {
    shared_ptr<vector<SmoothQuadTreeBox<T>*>> neighbors (new vector<SmoothQuadTreeBox<T>*>());
    SmoothQuadTreeBox<T>* cur_neighbor = principal_neighbor_dir(dir);

    if (cur_neighbor == nullptr) {
      return neighbors;
    } else if (!cur_neighbor->is_split()) {
      neighbors->push_back(cur_neighbor);
      return neighbors;
    } else { // cur_neighbor is non-null and split.
      return cur_neighbor->enumerate_halfspace_leaf_descendants(-dir);
    }
  }

  int depth() {
    return depth_;
  }

 private:
  SmoothQuadTreeBox<T>** neighbors() const {
    return neighbors_;
  }

  shared_ptr<vector<SmoothQuadTreeBox<T>*>> enumerate_halfspace_leaf_descendants(int dir) {
    assert(is_split());

    shared_ptr<vector<SmoothQuadTreeBox<T>*>> neighbors (new vector<SmoothQuadTreeBox<T>*>());
    int axis = fabs(dir);
    int dir_bit = (dir < 0 ? 0 : 1) << (axis - 1);
    int bottom_mask = (1 << (axis - 1)) - 1;
    int top_mask = (1 << dimension()) - 1 - bottom_mask;

    for (int i = 0; i < (1 << (dimension() - 1)); i++) {
      int ind = ((i & top_mask) << 1) | dir_bit | (i & bottom_mask);
      assert(ind >= 0 && ind < 1 << dimension());
      SmoothQuadTreeBox<T>* cur_child = children_[ind];
      if (!cur_child->is_split()) {
	neighbors->push_back(cur_child);
      } else {
	shared_ptr<vector<SmoothQuadTreeBox<T>*>> descendants = cur_child->enumerate_halfspace_leaf_descendants(dir);
        neighbors->insert(neighbors->begin(), descendants->begin(), descendants->end()); 
      }
    }

    return neighbors;
  }

  const int dimension() const {
    return tree_->dimension();
  }

  void split() {
    int num_children = 1 << dimension();
    double* center;

    // Initialize the children.
    children_ = new SmoothQuadTreeBox<T>*[num_children];
    for (int i = 0; i < num_children; i++) {

      // Compute the center of the new child.
      center = new double[dimension()];
      for (int j = 0; j < dimension(); j++) {
	center[j] = center_[j] + ((i & (1 << j)) == 0 ? -1 : 1) * pow(2, -(depth_ + 1) * tree_->width());
      }
      children_[i] = new SmoothQuadTreeBox<T>(depth_ + 1, i, center, tree_);
    }

    // Initialize neighbors of new children.
    // Each box has 2 * d principal neighbor pointers, one in each semi-axis direction.
    // Specifically, each box has exactly one sibling neighbor and one non-sibling or null neighbor
    // in the opposite direction along each axis.
    for (int child_ind = 0; child_ind < num_children; child_ind++) {
      SmoothQuadTreeBox<T>* cur_child = children_[child_ind];
      for (int d = 0; d < dimension(); d++) {
	int neighbor_child_ind = child_ind ^ (1 << d);
	int sibling_dir_ind = (neighbor_child_ind & (1 << d)) == 0 ? 0 : 1;

	// Set sibling neighbor.
	int sib_dim_ind = d + dimension() * sibling_dir_ind;
	cur_child->neighbors_[sib_dim_ind] = children_[neighbor_child_ind];

	// Set non-sibling neighbor and update neighbor's pointers.
	int non_sib_dim_ind = d + dimension() * (1 - sibling_dir_ind);
	SmoothQuadTreeBox<T>* cur_neighbor = neighbors_[non_sib_dim_ind];

	if (cur_neighbor == nullptr || !cur_neighbor->is_split()) {
	  cur_child->neighbors()[non_sib_dim_ind] = cur_neighbor;
	} else {
	  cur_neighbor = cur_neighbor->children()[neighbor_child_ind];
	  cur_child->neighbors()[non_sib_dim_ind] = cur_neighbor;
	  cur_neighbor->neighbors()[sib_dim_ind] = cur_child;
	}
      }
    }
    tree_->inc_splits();
  }

  void smooth_split_aux() {
    assert(!is_split());
    for (int d = 0; d < dimension(); d++) {
      int non_sib_neighbor_ind = d + (((indicator_ & (1 << d)) == 0) ? 0 : 1) * dimension();
      SmoothQuadTreeBox<T>* cur_neighbor = neighbors_[non_sib_neighbor_ind];
      if (cur_neighbor != nullptr && cur_neighbor->depth() < depth_) {
	cur_neighbor->smooth_split_aux();
      }
    }
    split();
  }

  const int depth_;
  const int indicator_;
  double* center_;
  T* data_;
  SmoothQuadTreeBox<T>** children_;
  SmoothQuadTreeBox<T>** neighbors_;
  SmoothQuadTree<T>* tree_;

  friend class SmoothQuadTree<T>;
};

template <typename T>
class SmoothQuadTree {
 public:
  SmoothQuadTree<T>(int dimension, double width) : dimension_(dimension), width_(width) {
    assert(1 <= dimension && dimension <= 31);

    double* center = new double[dimension];
    for (int i = 0; i < dimension; i++) {
      center[i] = 0.0;
    }

    root_ = new SmoothQuadTreeBox<T>(0 /* depth */, 0 /* indicator */, center, this);
    SmoothQuadTreeBox<T>** neighbors = root_->neighbors();
    for (int i = 0; i < 2 * dimension; i++) {
      neighbors[i] = nullptr;
    }
  }

  ~SmoothQuadTree<T>() {
    delete root_;
  }

  SmoothQuadTreeBox<T>* root() {
    return root_;
  }

  const int dimension() const {
    return dimension_;
  }

  void inc_splits() {
    num_splits_++;
  }

  void inc_smooth_splits() {
    num_smooth_splits_++;
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

 private:
  const int dimension_;
  SmoothQuadTreeBox<T>* root_;

  // Box specification.
  double width_;

  // Statistics
  int num_splits_ = 0;
  int num_smooth_splits_ = 0;
};

#endif // SMOOTH_QUADTREE_H
