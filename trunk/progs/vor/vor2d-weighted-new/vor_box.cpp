#include "vor_box.h"
#include "vor_qt.h"

#include "assert.h"
#include <set>

#define DEBUG 1

namespace vor2d {

using std::set;

vor_box::vor_box(int depth, int indicator, double center[], vor_qt* tree)
  : depth_(depth), indicator_(indicator), center_(center), tree_(tree),
    children_(nullptr), num_children_(0), is_active_(false), is_degen_(false) {
  width_ = pow(2, -depth_) * tree_->width();
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
  return width_ / sqrt(2);
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

bool vor_box::scaled_intersect(const vor_box& other, double scale) const {
  assert(scale > 0);

  double sw = scale * width() / 2;
  double osw = scale * other.width() / 2;
  Interval x1 = Interval(center()[0] - sw, center()[0] + sw);
  Interval y1 = Interval(center()[1] - sw, center()[1] + sw);
  Interval x2 = Interval(other.center()[0] - osw, other.center()[0] + osw);
  Interval y2 = Interval(other.center()[1] - osw, other.center()[1] + osw);
  Interval* x_inter = x1.intersect(x2);
  Interval* y_inter = y1.intersect(y2);
  bool intersect = x_inter != NULL && y_inter != NULL;

  if (x_inter != NULL) {
    delete x_inter;
  }

  if (y_inter != NULL) {
    delete y_inter;
  }
  
  return intersect;
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
    
    // TODO: Consolidate this into the "features" loop, and make active objects into a set.
    for (auto it = objects_.begin(); it != objects_.end(); ++it) {
      if ((*it)->distance(mid_point) < child_clearance + 2 * lip * children_[i]->radius()) {
    	children_[i]->add_object(*it);
      }
    }
  }

#if DEBUG
  for (int i = 0; i < num_children(); i++) {
    assert(children_[i]->num_features() > 0);
    assert(children_[i]->num_objects() > 0);
  }
#endif

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

void vor_box::add_feature(Feature* feature) {
  features_.push_back(feature);
}

void vor_box::add_object(Object* object) {
  objects_.push_back(object);
}

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

void vor_box::set_degen(bool is_degen) {
  is_degen_ = is_degen;
}

bool vor_box::is_degen() const {
  return is_degen_;
}

bool vor_box::cpv() const {
  if (objects_.size() < 2) {
    return true;
  }

  // The box decomposed into intervals.
  double rad = radius();
  Interval b_x(center_[0] - rad, center_[0] + rad);
  Interval b_y(center_[1] - rad, center_[1] + rad);

  for (int i = 0; i < features_.size(); i++) {
    for (int j = i + 1; j < features_.size(); j++) {
      Feature* f1 = features_[i];
      Feature* f2 = features_[j];

      // Only compute predicate for features that are part of
      // different objects.
      if (f1->parent() == f2->parent()) {
	continue;
      }
      
#if DEBUG
      cout << "Depth: " << depth_ << ", Box center: " << center_[0] << " " << center_[1] << "\n";
#endif

      // Compute 0 \notin (F_x(B))^2 + (F_y(B))^2
      tuple<Interval, Interval> f1_grad = f1->box_dist_sq_grad(b_x, b_y);
      tuple<Interval, Interval> f2_grad = f2->box_dist_sq_grad(b_x, b_y);
      Interval F_grad_x = std::get<0>(f1_grad) - std::get<0>(f2_grad);
      Interval F_grad_y = std::get<1>(f1_grad) - std::get<1>(f2_grad);
      Interval F_grad_ip = F_grad_x * F_grad_x + F_grad_y * F_grad_y;

#if DEBUG
      // cout << "Gradient: " << F_grad_ip.a_ << " "  << F_grad_ip.b_ << "\n";
      // cout << F_grad_x << " " << F_grad_y << "\n";
      // cout << std::get<0>(f1_grad) << " " << std::get<1>(f1_grad) << "\n";
      // cout << std::get<0>(f2_grad) << " " << std::get<1>(f2_grad) << "\n";
#endif

      if (0 < F_grad_ip.a_ || F_grad_ip.b_ < 0) {
	continue;
      }

      return false;
    }
  }

  return true;
}

/* 
 * A Voronoi vertex arises when two bisectors meet, and so we check for this case.
 */

bool vor_box::cmk(double scale) const {
  // TODO: Improve this to work with multi-feature sites and degenerate intersections.
  assert(num_features() >= 3);
  if (num_features() > 3) {
    cout << "Warning: multi-feature objects. Not ensuring that the MK test is met.\n";
    return true;
  }

  // Features.
  Feature* S = features_[0];
  Feature* T = features_[1];
  Feature* U = features_[2];

  // Intervals for box center.
  Interval m_x(center()[0]);
  Interval m_y(center()[1]);

  // Compute the gradients at midpoints.
  tuple<Interval, Interval> S_grad = S->box_dist_sq_grad(m_x, m_y);
  tuple<Interval, Interval> T_grad = T->box_dist_sq_grad(m_x, m_y);
  tuple<Interval, Interval> U_grad = U->box_dist_sq_grad(m_x, m_y);

  // Compute the gradient of f_{ST} at m_B.
  Interval fst_grad_x = std::get<0>(S_grad) - std::get<0>(T_grad);
  Interval fst_grad_y = std::get<1>(S_grad) - std::get<1>(T_grad);

  // Compute the gradient of f_{TU} at m_B.
  Interval ftu_grad_x = std::get<0>(T_grad) - std::get<0>(U_grad);
  Interval ftu_grad_y = std::get<1>(T_grad) - std::get<1>(U_grad);

  Interval detJmb = (fst_grad_x * ftu_grad_y) - (ftu_grad_x * fst_grad_y);

#if DEBUG
  cout << "CMK: " << center()[0] << ", " << center()[1] << "\n";
  cout << "detJmb: " << detJmb << "\n";
  cout << fst_grad_x << " " << fst_grad_y << " "
       << ftu_grad_x << " " << ftu_grad_y << "\n";
#endif

  // Parallel linear system.
  if (detJmb.contains(0)) {
    return false;
  }

  // Intervals for box edges.
  double sw = scale * width_ / 2;
  Interval bot_y(center()[1] - sw);
  Interval top_y(center()[1] + sw);
  Interval lft_x(center()[0] - sw);
  Interval rgt_x(center()[0] + sw);
  Interval span_x(center()[0] - sw, center()[0] + sw);
  Interval span_y(center()[1] - sw, center()[1] + sw);

  // Check f_{ST} on the left and right edges, and check f_{TU} on the top and bottom edges.
  Interval fst_dist_left  = S->box_dist_sq(lft_x, span_y) - T->box_dist_sq(lft_x, span_y);
  Interval fst_dist_right = S->box_dist_sq(rgt_x, span_y) - T->box_dist_sq(rgt_x, span_y);
  Interval fst_dist_top   = S->box_dist_sq(span_x, top_y) - T->box_dist_sq(span_x, top_y);
  Interval fst_dist_bot   = S->box_dist_sq(span_x, bot_y) - T->box_dist_sq(span_x, bot_y);

  Interval ftu_dist_left  = T->box_dist_sq(lft_x, span_y) - U->box_dist_sq(lft_x, span_y);
  Interval ftu_dist_right = T->box_dist_sq(rgt_x, span_y) - U->box_dist_sq(rgt_x, span_y);
  Interval ftu_dist_top   = T->box_dist_sq(span_x, top_y) - U->box_dist_sq(span_x, top_y);
  Interval ftu_dist_bot   = T->box_dist_sq(span_x, bot_y) - U->box_dist_sq(span_x, bot_y);

  // Check the modified system on appropriate edges.
  Interval g_left   = ( ftu_grad_y * fst_dist_left  - fst_grad_y * ftu_dist_left ) / detJmb;
  Interval g_right  = ( ftu_grad_y * fst_dist_right - fst_grad_y * ftu_dist_right) / detJmb;
  Interval g_top    = (-ftu_grad_x * fst_dist_top   + fst_grad_x * ftu_dist_top  ) / detJmb;
  Interval g_bottom = (-ftu_grad_x * fst_dist_bot   + fst_grad_x * ftu_dist_bot  ) / detJmb;

#if DEBUG
  cout << "g_left:   " << g_left   << "\n";
  cout << "g_right:  " << g_right  << "\n";
  cout << "g_top:    " << g_top    << "\n";
  cout << "g_bottom: " << g_bottom << "\n";
  
  cout << -ftu_grad_x * fst_dist_top << " + " << fst_grad_x * ftu_dist_top << ", "
       << -ftu_grad_x * fst_dist_bot << " + " << fst_grad_x * ftu_dist_bot << "\n";
#endif

  return 0 > g_left && 0 < g_right && 0 > g_bottom && 0 < g_top;
}

// Currently this computes the Jacobian condition with respect to
// the system of equations F = (f_{ST}, f_{TU}) where the bisectors
// are induced by the first and second, and second and third active
// sites respectively.
bool vor_box::cjc(double scale) const {
  // TODO: Improve this to work with multi-feature sites and degenerate intersections.
  assert(num_features() >= 3);
  if (num_features() > 3) {
    cout << "Warning: multi-feature objects. Not ensuring that the Jacobian condition is met.\n";
    return true;
  }
  
  // TODO: Consolidate gradient computation with PV code.
  double sw = scale * width_ / 2;
  Interval b_x(center_[0] - sw, center_[0] + sw);
  Interval b_y(center_[1] - sw, center_[1] + sw);
  Feature* S = features_[0];
  Feature* T = features_[1];
  Feature* U = features_[2];
  tuple<Interval, Interval> S_grad = S->box_dist_sq_grad(b_x, b_y);
  tuple<Interval, Interval> T_grad = T->box_dist_sq_grad(b_x, b_y);
  tuple<Interval, Interval> U_grad = U->box_dist_sq_grad(b_x, b_y);
  
  // Use the first two sites to compute the gradient of f_{ST}.
  Interval fst_grad_x = std::get<0>(S_grad) - std::get<0>(T_grad);
  Interval fst_grad_y = std::get<1>(S_grad) - std::get<1>(T_grad);

  // Use the second and third sites to compute the gradient of f_{TU}.
  Interval ftu_grad_x = std::get<0>(T_grad) - std::get<0>(U_grad);
  Interval ftu_grad_y = std::get<1>(T_grad) - std::get<1>(U_grad);

  // Check whether 0 is contained in the determinant of the Jacobian.
  Interval F_jcdet = (fst_grad_x * ftu_grad_y) - (fst_grad_y * ftu_grad_x);

#if DEBUG
  cout << "CJC Depth: " << depth_ << ", Box center: " << center_[0] << " " << center_[1] << "\n";
  cout << "F_jcdet: " << F_jcdet << "\n";
#endif

  return !F_jcdet.contains(0);
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

int vor_box::num_corner_obj() const {
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

  return objects.size();
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
