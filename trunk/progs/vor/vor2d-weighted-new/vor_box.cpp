#include "vor_box.h"
#include "vor_qt.h"

#include "assert.h"
#include <set>

namespace vor2d {

using std::set;

vor_box::vor_box(int depth, int indicator, double center[], vor_qt* tree)
  : depth_(depth), indicator_(indicator), center_(center), tree_(tree),
    children_(nullptr), num_children_(0), is_active_(false) {
  width_ = pow(2, -depth_) * tree_->width();
  radius_ = width_ / sqrt(2);
  neighbors_ = new vor_box*[2 * dimension()];
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
  return width_;
}

double vor_box::radius() const {
  return radius_;
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

vor_box* vor_box::principal_neighbor(int dir) const {
  int axis = fabs(dir);
  int sign = (dir < 0) ? 0 : 1;
  return neighbors_[(axis - 1) + sign * dimension()];
}

shared_ptr<vector<vor_box*>> vor_box::leaf_neighbors_dir(int dir) {
  shared_ptr<vector<vor_box*>> neighbors (new vector<vor_box*>());
  vor_box* cur_neighbor = principal_neighbor(dir);

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
  num_children_ = 1 << dimension();
  double* center;

  // Initialize the children.
  children_ = new vor_box*[num_children_];
  for (int i = 0; i < num_children_; i++) {

    // Compute the center of the new child.
    center = new double[dimension()];
    for (int j = 0; j < dimension(); j++) {
      center[j] = center_[j] + ((i & (1 << j)) == 0 ? -1 : 1) * width_ / 4.0;
    }
    children_[i] = new vor_box(depth_ + 1, i, center, tree_);

    // Compute active features for the new child.
    double lip = max_lipschitz(); // Maximum Lipschitz constant of relevant features.
    Point2d mid_point(center[0], center[1]);
    double child_clearance = clearance(mid_point);

    for (auto it = features_.begin(); it != features_.end(); ++it) {
      if ((*it)->distance(mid_point) < child_clearance + 2 * lip * children_[i]->radius()) {
	children_[i]->add_feature(*it);
      }
    }

    // for (auto it = corners_.begin(); it != corners_.end(); ++it) {
    //   if ((*it)->distance(mid_point) < child_clearance + 2 * lip * children_[i]->radius()) {
    // 	children_[i]->add_corner(*it);
    //   }
    // }

    // for (auto it = edges_.begin(); it != edges_.end(); ++it) {
    //   if ((*it)->distance(mid_point) < child_clearance + 2 * lip * children_[i]->radius()) {
    // 	children_[i]->add_edge(*it);
    //   }
    // }

    // TODO: Consolidate this into the "features" loop, and make active objects into a set.
    for (auto it = objects_.begin(); it != objects_.end(); ++it) {
      if ((*it)->distance(mid_point) < child_clearance + 2 * lip * children_[i]->radius()) {
    	children_[i]->add_object(*it);
      }
    }
  }

  // Initialize neighbors of new children.
  // Each box has 2 * d principal neighbor pointers, one in each semi-axis direction.
  // Specifically, each box has exactly one sibling neighbor and one non-sibling or null neighbor
  // in the opposite direction along each axis.
  for (int child_ind = 0; child_ind < num_children_; child_ind++) {
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

// void vor_box::add_corner(Corner* corner) {
//   corners_.push_back(corner);
// }

// void vor_box::add_edge(Edge* edge) {
//   edges_.push_back(edge);
// }

void vor_box::add_feature(Feature* feature) {
  features_.push_back(feature);
}

void vor_box::add_object(Object* object) {
  objects_.push_back(object);
}

// vector<Corner*>* vor_box::get_corners() {
//   return &corners_;
// }

// vector<Edge*>* vor_box::get_edges() {
//   return &edges_;
// }

vector<Feature*>* vor_box::get_features() {
  return &features_;
}

vector<Object*>* vor_box::get_objects() {
  return &objects_;
}

const vector<vor_seg*>* vor_box::get_segments() const {
  return &segments_;
}

int vor_box::num_children() const {
  return num_children_;
}

double vor_box::clearance(const Point2d& point) const {
  double min_sep = std::numeric_limits<double>::max();
  
  // Compute clearance in terms of objects.
  for (auto it = objects_.begin(); it != objects_.end(); ++it) {
    double dist = (*it)->distance(point);
    if (dist < min_sep) {
      min_sep = dist;
    }
  }

  return min_sep;
}

double vor_box::clearance() const {
  Point2d mid_point(center_[0], center_[1]);
  return clearance(mid_point);
}

double vor_box::max_lipschitz() const {
  double lip = 0.0;

  for (auto it = features_.begin(); it != features_.end(); ++it) {
    double l = (*it)->lipschitz();
    if (l > lip) {
      lip = l;
    }
  }  

  // // TODO(Huck): Consolidate these loops into a single loop iterating over all types of features.
  // for (auto it = corners_.begin(); it != corners_.end(); ++it) {
  //   double l = (*it)->lipschitz();
  //   if (l > lip) {
  //     lip = l;
  //   }
  // }

  // for (auto it = edges_.begin(); it != edges_.end(); ++it) {
  //   double l = (*it)->lipschitz();
  //   if (l > lip) {
  //     lip = l;
  //   }
  // }

  return lip;
}

int vor_box::num_features() const {
  return features_.size();
}

int vor_box::num_objects() const {
  return objects_.size();
}

void vor_box::set_active(bool is_active) {
  is_active_ = is_active;
}

bool vor_box::is_active() const {
  return is_active_;
}

bool vor_box::cpv() const {
  if (objects_.size() < 2) {
    return true;
  }

  // The box decomposed into intervals.
  Interval b_x(center_[0] - radius_, center_[0] + radius_);
  Interval b_y(center_[1] - radius_, center_[1] + radius_);

  for (int i = 0; i < features_.size(); i++) {
    for (int j = i + 1; j < features_.size(); j++) {
      Feature* f1 = features_[i];
      Feature* f2 = features_[j];

      // Only compute predicate for features that are part of
      // different objects.
      if (f1->parent() == f2->parent()) {
	continue;
      }

      // Compute 0 \notin F(B)
      Interval f1_sep = f1->box_distance(b_x, b_y);
      Interval f2_sep = f2->box_distance(b_x, b_y);
      Interval F_sep = f1_sep - f2_sep;
      if (0 <= F_sep.a_ || F_sep.b_ <= 0) {
	continue;
      }

      // Compute 0 \notin (F_x(B))^2 + (F_y(B))^2
      tuple<Interval, Interval> f1_grad = f1->box_grad(b_x, b_y);
      tuple<Interval, Interval> f2_grad = f2->box_grad(b_x, b_y);
      Interval F_grad_x = std::get<0>(f1_grad) - std::get<0>(f2_grad);
      Interval F_grad_y = std::get<1>(f1_grad) - std::get<1>(f2_grad);
      Interval F_grad_ip = F_grad_x * F_grad_x + F_grad_y * F_grad_y;
      if (0 <= F_grad_ip.a_ || F_grad_ip.b_ <= 0) {
	continue;
      }

      return false;
    }
  }

  return true;
}

Object* vor_box::nearest_obj(const Point2d& point) const {
  double min_dist = std::numeric_limits<double>::max();
  Object* nearest = nullptr;

  for (Object* obj : objects_) {
    double cur_dist = obj->distance(point);
    if (cur_dist < min_dist) {
      min_dist = cur_dist;
      nearest = obj;
    } else if (cur_dist == min_dist) {
      cout << "Warning: possible degeneracy: " << center_[0] << " " << center_[1] << "\n";
    }
  }

  return nearest;
}

Point2d* get_midpoint(const Point2d& p, const Point2d& q) {
  return new Point2d((p[0] + q[0]) / 2.0, (p[1] + q[1]) / 2.0);
}

// TODO: Factor construction logic out into a separate file?
void vor_box::gen_vertices() {
  double x = center_[0];
  double y = center_[1];
  double hw = width_ / 2.0;
  set<Object*> objects;

  Point2d corners[4];
  corners[0] = Point2d(x + hw, y + hw);
  corners[1] = Point2d(x + hw, y - hw);
  corners[2] = Point2d(x - hw, y - hw);
  corners[3] = Point2d(x - hw, y + hw);

  assert(is_leaf());

  // Compute nearest objects for each corner.
  Object* objs[4];
  for (int i = 0; i < 4; i++) {
    objs[i] = nearest_obj(corners[i]);
    objects.insert(objs[i]);
  }

  if (objects.size() == 1) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    int j = (i + 1) % 4;

    // Add a node iff adjacent corners have different labels.
    // TODO: Add VP predicate so that we still ensure topological correctness.
    if (objs[i] == objs[j]) {
      continue;
    }

    // TODO: Improve direction computation logic.
    int dir;
    if (j == 0) {
      dir = 2;
    } else if (j == 1) {
      dir = 1;
    } else if (j == 2) {
      dir = -2;
    } else if (j == 3) {
      dir = -1;
    } else {
      cout << "Warning: Failed to compute direction.\n";
    }

    Point2d* midpoint = get_midpoint(corners[i], corners[j]);
    Object* mid_closest = nearest_obj(*midpoint);
    vor_box* neighbor = principal_neighbor(dir);
    if (neighbor != nullptr && !neighbor->is_leaf()) {
      // TODO: Use Voronoi nodes from neighbor instead of recomputing them?
      // TODO: Clean up.
      Point2d* sub_midpoint;
      if (objs[i] != mid_closest) {
	sub_midpoint = get_midpoint(corners[i], *midpoint);
	nodes_.push_back(sub_midpoint);
	nodes_map_[dir] = sub_midpoint;
      } else if (objs[j] != mid_closest) {
	sub_midpoint = get_midpoint(corners[j], *midpoint);
	nodes_.push_back(sub_midpoint);
	nodes_map_[dir] = sub_midpoint;
      } else {
	cout << "Warning: No node added.\n";
      }
    } else {
      nodes_.push_back(midpoint);
      nodes_map_[dir] = midpoint;
    }
  }
  
  // Generate Voronoi segments.
  // TODO: Fix logic for nodes_.size() > 2.
  if (nodes_.size() == 2) {
    segments_.push_back(new vor_seg(*nodes_[0], *nodes_[1]));
  } else {
    Point2d center(center_[0], center_[1]);
    for (Point2d* node : nodes_) {
      segments_.push_back(new vor_seg(*node, center));
    }
  }
}

} // namespace vor2d
