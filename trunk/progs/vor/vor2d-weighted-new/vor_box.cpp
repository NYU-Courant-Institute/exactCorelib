#include "vor_box.h"
#include "vor_qt.h"

#include <memory>
#include <vector>
#include <assert.h>
#include <math.h>

namespace vor2d {

vor_box::vor_box(int depth, int indicator, double center[], vor_qt* tree)
  : depth_(depth), indicator_(indicator), center_(center), tree_(tree) {
  neighbors_ = new vor_box*[2 * dimension()];
  children_ = nullptr;
}

vor_box::~vor_box() {
  // If the box is split then recurse to children.
  if (!is_leaf()) {
    for (int i = 0; i < (1 << dimension()); i++) {
      delete children_[i];
    }
    delete[] children_;
  }

  delete[] neighbors_;
  delete[] center_;
}

void vor_box::smooth_split() {
  if (!is_leaf()) {
    return;
  }
  smooth_split_aux();
  tree_->inc_smooth_splits();
}

double vor_box::width() const {
  pow(2, -depth_) * tree_->width();
}

double* vor_box::center() const {
  return center_;
}

vor_box** vor_box::children() const {
  return children_;
}

bool vor_box::is_leaf() const {
  return children_ == nullptr;
}

vor_box* vor_box::principal_neighbor_dir(int dir) const {
  int axis = fabs(dir);
  int sign = (dir < 0) ? 0 : 1;
  return neighbors_[(axis - 1) + sign * dimension()];
}

shared_ptr<vector<vor_box*>> vor_box::leaf_neighbors_dir(int dir) {
  shared_ptr<vector<vor_box*>> neighbors (new vector<vor_box*>());
  vor_box* cur_neighbor = principal_neighbor_dir(dir);

  if (cur_neighbor == nullptr) {
    return neighbors;
  } else if (cur_neighbor->is_leaf()) {
    neighbors->push_back(cur_neighbor);
    return neighbors;
  } else { // cur_neighbor is non-null and split.
    return cur_neighbor->enumerate_halfspace_leaf_descendants(-dir);
  }
}

int vor_box::depth() const {
  return depth_;
}

vor_box** vor_box::neighbors() const {
  return neighbors_;
}

shared_ptr<vector<vor_box*>> vor_box::enumerate_halfspace_leaf_descendants(int dir) {
  assert(!is_leaf());

  shared_ptr<vector<vor_box*>> neighbors (new vector<vor_box*>());
  int axis = fabs(dir);
  int dir_bit = (dir < 0 ? 0 : 1) << (axis - 1);
  int bottom_mask = (1 << (axis - 1)) - 1;
  int top_mask = (1 << dimension()) - 1 - bottom_mask;

  for (int i = 0; i < (1 << (dimension() - 1)); i++) {
    int ind = ((i & top_mask) << 1) | dir_bit | (i & bottom_mask);
    assert(ind >= 0 && ind < 1 << dimension());
    vor_box* cur_child = children_[ind];
    if (cur_child->is_leaf()) {
      neighbors->push_back(cur_child);
    } else {
      shared_ptr<vector<vor_box*>> descendants = cur_child->enumerate_halfspace_leaf_descendants(dir);
      neighbors->insert(neighbors->begin(), descendants->begin(), descendants->end());
    }
  }

  return neighbors;
}

const int vor_box::dimension() const {
  return tree_->dimension();
}

void vor_box::split() {
  int num_children = 1 << dimension();
  double* center;

  // Initialize the children.
  children_ = new vor_box*[num_children];
  for (int i = 0; i < num_children; i++) {

    // Compute the center of the new child.
    center = new double[dimension()];
    for (int j = 0; j < dimension(); j++) {
      center[j] = center_[j] + ((i & (1 << j)) == 0 ? -1 : 1) * pow(2, -(depth_ + 1)) * tree_->width();
    }
    children_[i] = new vor_box(depth_ + 1, i, center, tree_);
  }

  // Initialize neighbors of new children.
  // Each box has 2 * d principal neighbor pointers, one in each semi-axis direction.
  // Specifically, each box has exactly one sibling neighbor and one non-sibling or null neighbor
  // in the opposite direction along each axis.
  for (int child_ind = 0; child_ind < num_children; child_ind++) {
    vor_box* cur_child = children_[child_ind];
    for (int d = 0; d < dimension(); d++) {
      int neighbor_child_ind = child_ind ^ (1 << d);
      int sibling_dir_ind = (neighbor_child_ind & (1 << d)) == 0 ? 0 : 1;

      // Set sibling neighbor.
      int sib_dim_ind = d + dimension() * sibling_dir_ind;
      cur_child->neighbors_[sib_dim_ind] = children_[neighbor_child_ind];

      // Set non-sibling neighbor and update neighbor's pointers.
      int non_sib_dim_ind = d + dimension() * (1 - sibling_dir_ind);
      vor_box* cur_neighbor = neighbors_[non_sib_dim_ind];

      if (cur_neighbor == nullptr || cur_neighbor->is_leaf()) {
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

void vor_box::smooth_split_aux() {
  assert(is_leaf());
  for (int d = 0; d < dimension(); d++) {
    int non_sib_neighbor_ind = d + (((indicator_ & (1 << d)) == 0) ? 0 : 1) * dimension();
    vor_box* cur_neighbor = neighbors_[non_sib_neighbor_ind];
    if (cur_neighbor != nullptr && cur_neighbor->depth() < depth_) {
      cur_neighbor->smooth_split_aux();
    }
  }
  split();
}

void vor_box::add_corner(Corner* corner) {
  corners_.push_back(corner);
}

void vor_box::add_edge(Edge* edge) {
  edges_.push_back(edge);
}

vector<Corner*>* vor_box::get_corners() {
  return &corners_;
}

vector<Edge*>* vor_box::get_edges() {
  return &edges_;
}

} // namespace vor2d
