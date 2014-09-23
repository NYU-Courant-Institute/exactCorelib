/**
 *  File: smooth_quadtree.h
 *
 *  Description: 
 *  A templated implementation of the smooth quadtree data structure
 *  presented in Bennett + Yap SWAT '14.
 *
 *  Written by Huck Bennett, 9/21/2014
 */

#ifndef SMOOTH_QUADTREE_H
#define SMOOTH_QUADTREE_H

#include <vector>
#include <math.h>
  
template <typename T>
class SmoothQuadTreeBox {
 public:
  SmoothQuadTreeBox<T>(int dimension, int depth, int indicator, double center[])
    : dimension_(dimension), depth_(depth), indicator_(indicator), center_(center) {
    neighbors_ = new SmoothQuadTreeBox<T>*[2 * dimension_];
    children_ = nullptr;
    data_ = nullptr;
  }

  ~SmoothQuadTreeBox<T>() {
    // If the box is split then recurse to children.
    if (children_ != nullptr) {
      for (int i = 0; i < (1 << dimension_); i++) {
	delete children_[i];
      }
      delete[] children_;
    }
  
    if (data_ != nullptr) {
      delete data_;
    }

    delete[] neighbors_;
    delete[] center_;
  }

  void smooth_split() {
    split();
    for (int d = 0; d < dimension_; d++) {
      int non_sib_neighbor_ind = d + (((indicator_ & (1 << d)) == 0) ? 0 : 1) * dimension_;
      SmoothQuadTreeBox<T>* cur_neighbor = neighbors_[non_sib_neighbor_ind];
      if (cur_neighbor != nullptr && cur_neighbor->depth() < depth_) {
	cur_neighbor->smooth_split();
      }
    }
  }

  int depth() {
    return depth_;
  }

  void set_data(T* data) {
    data_ = data;
  }

  double* center() const {
    return center_;
  }

  T* data() const {
    return data_;
  }

  bool is_split() const {
    return children_ != nullptr;
  }

  SmoothQuadTreeBox<T>** children() const {
    return children_;
  }

  SmoothQuadTreeBox<T>** neighbors() const {
    return neighbors_;
  }

 private:
  void split() {
    int num_children = 1 << dimension_;
    double* center;

    // Initialize the children.
    children_ = new SmoothQuadTreeBox<T>*[num_children];
    for (int i = 0; i < num_children; i++) {

      // Compute the center of the new child.
      center = new double[dimension_];
      for (int j = 0; j < dimension_; j++) {
	center[j] = center_[j] + pow(-1, (i & (1 << j)) == 0 ? 0 : 1) * pow(2, -(depth_ + 1));
      }
      children_[i] = new SmoothQuadTreeBox<T>(dimension_, depth_ + 1, i, center);
    }

    // Initialize neighbors of new children.
    // Each box has 2 * d principal neighbor pointers, one in each semi-axis direction.
    // Specifically, each box has exactly one sibling neighbor and one non-sibling or null neighbor
    // in the opposite direction along each axis.
    for (int child_ind = 0; child_ind < num_children; child_ind++) {
      SmoothQuadTreeBox<T>* cur_child = children_[child_ind];
      for (int d = 0; d < dimension_; d++) {
	int neighbor_child_ind = child_ind ^ (1 << d);
	int sibling_dir_ind = (neighbor_child_ind & (1 << d)) == 0 ? 0 : 1;

	// Set sibling neighbor.
	int sib_dim_ind = d + dimension_ * sibling_dir_ind;
	cur_child->neighbors_[sib_dim_ind] = children_[neighbor_child_ind];

	// Set non-sibling neighbor.
	int non_sib_dim_ind = d + dimension_ * (1 - sibling_dir_ind);
	SmoothQuadTreeBox<T>* cur_neighbor = neighbors_[non_sib_dim_ind];
	cur_child->neighbors_[non_sib_dim_ind] =  cur_neighbor == nullptr || !cur_neighbor->is_split()
	  ? cur_neighbor : cur_neighbor->children()[neighbor_child_ind];
      }
    }
  }

  const int dimension_;
  const int depth_;
  const int indicator_;
  double* center_;
  T* data_;
  SmoothQuadTreeBox<T>** children_;
  SmoothQuadTreeBox<T>** neighbors_;
};

template <typename T>
class SmoothQuadTree {
 public:
  SmoothQuadTree<T>(int dimension) {
    double* root_center = new double[dimension];
    for (int i = 0; i < dimension; i++) {
      root_center[i] = 0.0;
    }
    root_ = new SmoothQuadTreeBox<T>(dimension, 0 /* depth */, 0 /* indicator */, root_center);

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

 private:
  SmoothQuadTreeBox<T>* root_;
};

#endif // SMOOTH_QUADTREE_H
