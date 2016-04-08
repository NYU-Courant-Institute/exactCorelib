#include "vor_box.h"
#include "vor_qt.h"
#include "BiPoly.h"

#include <assert.h>
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

double vor_box::midpoint_clearance() const {
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

/***********************************************************
 *** Box predicates ****************************************
************************************************************/

// New versions.

bool vor_box::cpv() const {
  if (num_features() < 2) {
    return true;
  }

  Interval gx_i(TOP), gy_i(TOP);
  double hw = width_ / 2.0;
  Interval bx(center_[0] - hw, center_[0] + hw);
  Interval by(center_[1] - hw, center_[1] + hw);

  for (int i = 0; i < features_.size(); i++) {
    for (int j = i + 1; j < features_.size(); j++) {
      Feature* f1 = features_[i];
      Feature* f2 = features_[j];

      // Only compute predicate for features that are part of
      // different objects.
      if (f1->parent() == f2->parent()) {
	continue;
      }

      if (f1->dfun_sq(bx, by) != NULL && f2->dfun_sq(bx, by) != NULL) {
	BiPoly grad_x = f1->dfun_sq_grad(bx, by).first  - f2->dfun_sq_grad(bx, by).first;
	BiPoly grad_y = f1->dfun_sq_grad(bx, by).second - f2->dfun_sq_grad(bx, by).second;
	gx_i = grad_x.eval(bx, by);
	gy_i = grad_y.eval(bx, by);
      } else {
	pair<Interval, Interval> f1_grad_i = f1->box_dist_sq_grad(bx, by);
	pair<Interval, Interval> f2_grad_i = f2->box_dist_sq_grad(bx, by);
	gx_i = f1_grad_i.first - f2_grad_i.first;
	gy_i = f1_grad_i.second - f2_grad_i.second;
      }
      
#if DEBUG
      // cout << "PV: " << grad_x.to_string() << ", " << grad_y.to_string() << "\n";
      // cout << "PV: " << gx_i << " " << gy_i << " " << (gx_i * gx_i + gy_i * gy_i) << "\n";
#endif

      if (((gx_i * gx_i) + (gy_i * gy_i)).contains(0)) {
#if DEBUG
	cout << "PV fails.\n";
#endif
	return false;
      }
    }
  }

#if DEBUG
  // cout << "PV succeeds.\n";
#endif

  return true;
}

// Currently this computes the Jacobian condition with respect to
// the system of equations F = (f_{ST}, f_{TU}) where the bisectors
// are induced by the first and second, and second and third active
// sites respectively.
bool vor_box::cjc(double scale) const {
  // TODO: Improve this to work with multi-feature sites and degenerate intersections.
  // assert(num_features() >= 3);
  if (num_features() <= 2) {
    return true;
  }
  
  if (num_features() > 3) {
    cout << "Warning: more than three features. Not ensuring that the Jacobian condition is met.\n";
    return true;
  }

  if (num_objects() < num_features()) {
    cout << "Warning: multi-feature objects. Not ensuring that the Jacobian condition is met.\n";
    return true;
  }

  double sw = scale * width_ / 2;
  Interval bx(center_[0] - sw, center_[0] + sw);
  Interval by(center_[1] - sw, center_[1] + sw);
  Feature* S = features_[0];
  Feature* T = features_[1];
  Feature* U = features_[2];

  if (S->dfun_sq(bx, by) != NULL && T->dfun_sq(bx, by) != NULL & U->dfun_sq(bx, by) != NULL) {
    BiPoly gradST_x = S->dfun_sq_grad(bx, by).first  - T->dfun_sq_grad(bx, by).first;
    BiPoly gradST_y = S->dfun_sq_grad(bx, by).second - T->dfun_sq_grad(bx, by).second;
    BiPoly gradTU_x = T->dfun_sq_grad(bx, by).first  - U->dfun_sq_grad(bx, by).first;
    BiPoly gradTU_y = T->dfun_sq_grad(bx, by).second - U->dfun_sq_grad(bx, by).second;
    BiPoly det_poly = (gradST_x * gradTU_y) - (gradST_y * gradTU_x);
    return !det_poly.eval(bx, by).contains(0);
  }

  pair<Interval, Interval> S_grad_i = S->box_dist_sq_grad(bx, by);
  pair<Interval, Interval> T_grad_i = T->box_dist_sq_grad(bx, by);
  pair<Interval, Interval> U_grad_i = U->box_dist_sq_grad(bx, by);
  Interval ST_x = S_grad_i.first  - T_grad_i.first;
  Interval ST_y = S_grad_i.second - T_grad_i.second;
  Interval TU_x = T_grad_i.first  - U_grad_i.first;
  Interval TU_y = T_grad_i.second - U_grad_i.second;
  return !((ST_x * TU_y) - (ST_y * TU_x)).contains(0);
}

bool vor_box::cmk(double scale) const {
  // Note: These computations should all be vectorized.
  // TODO: Improve this to work with multi-feature sites and degenerate intersections.
  // assert(num_features() >= 3);
  if (num_features() <= 2) {
    return true;
  }
  
  if (num_features() > 3) {
    cout << "Warning: more than three features. Not ensuring that the MK test is met.\n";
    return true;
  }

  if (num_objects() < num_features()) {
    cout << "Warning: multi-feature objects. Not ensuring that the MK test is met.\n";
    return true;
  }

  // Box variables.
  double mx = center()[0];
  double my = center()[1];
  double sw = scale * width_ / 2;
  Interval bx(center_[0] - sw, center_[0] + sw);
  Interval by(center_[1] - sw, center_[1] + sw);
  Interval lft_x(mx - sw);
  Interval rgt_x(mx + sw);
  Interval bot_y(my - sw);
  Interval top_y(my + sw);
  Interval span_x(mx - sw, mx + sw);
  Interval span_y(my - sw, my + sw);

  // Features.
  Feature* S = features_[0];
  Feature* T = features_[1];
  Feature* U = features_[2];

  if (S->dfun_sq(bx, by) != NULL && T->dfun_sq(bx, by) != NULL & U->dfun_sq(bx, by) != NULL) {
#if DEBUG
    cout << "MK branch 1\n";
#endif
    
    // Original system and derivatives (F and F').
    BiPoly f1 = *(S->dfun_sq(bx, by)) - *(T->dfun_sq(bx, by));
    BiPoly f2 = *(T->dfun_sq(bx, by)) - *(U->dfun_sq(bx, by));
    BiPoly f11 = S->dfun_sq_grad(bx, by).first  - T->dfun_sq_grad(bx, by).first;
    BiPoly f12 = S->dfun_sq_grad(bx, by).second - T->dfun_sq_grad(bx, by).second;
    BiPoly f21 = T->dfun_sq_grad(bx, by).first  - U->dfun_sq_grad(bx, by).first;
    BiPoly f22 = T->dfun_sq_grad(bx, by).second - U->dfun_sq_grad(bx, by).second;

    // Determinant computation.
    double z11 = f11.eval(mx, my);
    double z12 = f12.eval(mx, my);
    double z21 = f21.eval(mx, my);
    double z22 = f22.eval(mx, my);
    double zdet = ((z11 * z22) - (z12 * z21));

    if (zdet == 0.0) {
      return false;
    }
    double izdet = 1.0 / zdet;

    // Inverse determinant.
    double y11 = izdet * z22;
    double y12 = izdet * (-z12);
    double y21 = izdet * (-z21);
    double y22 = izdet * z11;

    // Modified system and derivatives (G and G').
    BiPoly g1  = y11 * f1  + y12 * f2;
    BiPoly g2  = y21 * f1  + y22 * f2;
    BiPoly g11 = y11 * f11 + y12 * f21;
    BiPoly g12 = y11 * f12 + y12 * f22;
    BiPoly g21 = y21 * f11 + y22 * f21;
    BiPoly g22 = y21 * f12 + y22 * f22;

#if DEBUG
    cout << "MK applied to box: " << mx << " " << my << " " << sw << "\n";
  
    cout << "MK original system F:\n";
    cout << f1.to_string() << "\n";
    cout << f2.to_string() << "\n";
    cout << f11.to_string() << ", "
	 << f12.to_string() << "\n";
    cout << f21.to_string() << ", "
	 << f22.to_string() << "\n";
  
    cout << "MK modified system G:\n";
    cout << g1.to_string() << "\n";
    cout << g2.to_string() << "\n";
    cout << g11.to_string() << ", "
	 << g12.to_string() << "\n";
    cout << g21.to_string() << ", "
	 << g22.to_string() << "\n";

    cout << "MK modified system evaluated on box midpoint:\n";
    cout << g11.eval(mx, my) << ", "
	 << g12.eval(mx, my) << "\n";
    cout << g21.eval(mx, my) << ", "
	 << g22.eval(mx, my) << "\n";

    /*
      assert(g1.gradient().first == g11 && g1.gradient().second == g12);
      assert(g2.gradient().first == g21 && g2.gradient().second == g22);
    */
#endif

    // Evaluations.
    double g1p = g1.eval(mx + sw, my);
    double g1m = g1.eval(mx - sw, my);
    double g2p = g2.eval(mx, my + sw);
    double g2m = g2.eval(mx, my - sw);

#if DEBUG
    cout << "g1p: " << g1p << " g1m: " << g1m << " g2p: " << g2p << " g2m: " << g2m << "\n";
    cout << "g12+: "  << sw * g12.eval(rgt_x, span_y) << " g21+: " << sw * g21.eval(span_x, top_y)
	 << " g12-: " << sw * g12.eval(lft_x, span_y) << " g21-: " << sw * g21.eval(span_x, bot_y) << "\n";
#endif
  
    // Conditions 2.11.
    if (!(g1p * g1m <= 0)) {
#if DEBUG
      cout << "Condition 1\n";
#endif
      return false;
    }
    if (!(g2p * g2m <= 0)) {
#if DEBUG
      cout << "Condition 2\n";
#endif
      return false;
    }

    // Conditions 2.12.  
    if (!((sw * g12.eval(rgt_x, span_y)).mag() <= fabs(g1p))) {
#if DEBUG
      cout << "Condition 3: " << sw * g12.eval(rgt_x, span_y) << "\n";
#endif
      return false;
    }
    if (!((sw * g21.eval(span_x, top_y)).mag() <= fabs(g2p))) {
#if DEBUG
      cout << "Condition 4: " << sw * g21.eval(span_x, top_y) << "\n";
#endif
      return false;
    }
  
    // Conditions 2.13.
    if (!((sw * g12.eval(lft_x, span_y)).mag() <= fabs(g1m))) {
#if DEBUG
      cout << "Condition 5: " << sw * g12.eval(lft_x, span_y) << "\n";
#endif
      return false;
    }
    if (!((sw * g21.eval(span_x, bot_y)).mag() <= fabs(g2m))) {
#if DEBUG
      cout << "Condition 6: " << sw * g21.eval(span_x, bot_y) << "\n";
#endif
      return false;
    }
#if DEBUG
    cout << "MK succeeds.\n";
    cout << mx << " " << my << "\n";
#endif

    return true;
  }

  /************** ### **************/
  // TODO: Remove redundancy.

#if DEBUG
  cout << "MK branch 2\n";
#endif

  // Determinant computation.
  double z11 = S->dist_sq_grad(mx, my).first  - T->dist_sq_grad(mx, my).first;
  double z12 = S->dist_sq_grad(mx, my).second - T->dist_sq_grad(mx, my).second;
  double z21 = T->dist_sq_grad(mx, my).first  - U->dist_sq_grad(mx, my).first;
  double z22 = T->dist_sq_grad(mx, my).second - U->dist_sq_grad(mx, my).second;
  double zdet = ((z11 * z22) - (z12 * z21));

  if (zdet == 0.0) {
    return false;
  }
  double izdet = 1.0 / zdet;

  // Inverse determinant.
  double y11 = izdet * z22;
  double y12 = izdet * (-z12);
  double y21 = izdet * (-z21);
  double y22 = izdet * z11;

  // Evaluations.
  double g1p = y11 * (S->dist_sq(mx + sw, my) - T->dist_sq(mx + sw, my))
             + y12 * (T->dist_sq(mx + sw, my) - U->dist_sq(mx + sw, my));
  double g1m = y11 * (S->dist_sq(mx - sw, my) - T->dist_sq(mx - sw, my))
             + y12 * (T->dist_sq(mx - sw, my) - U->dist_sq(mx - sw, my));
  double g2p = y21 * (S->dist_sq(mx, my + sw) - T->dist_sq(mx, my + sw))
             + y22 * (T->dist_sq(mx, my + sw) - U->dist_sq(mx, my + sw));
  double g2m = y21 * (S->dist_sq(mx, my - sw) - T->dist_sq(mx, my - sw))
             + y22 * (T->dist_sq(mx, my - sw) - U->dist_sq(mx, my - sw));

  // Conditions 2.11.
  if (!(g1p * g1m <= 0)) {
#if DEBUG
    cout << "Condition 1\n";
#endif
    return false;
  }
  if (!(g2p * g2m <= 0)) {
#if DEBUG
    cout << "Condition 2\n";
#endif
    return false;
  }

  // Conditions 2.12.
  Interval g12_rgt = y11 * (S->box_dist_sq_grad(rgt_x, span_y).second - T->box_dist_sq_grad(rgt_x, span_y).second)
                   + y12 * (T->box_dist_sq_grad(rgt_x, span_y).second - U->box_dist_sq_grad(rgt_x, span_y).second);
  if (!((sw * g12_rgt).mag() <= fabs(g1p))) {
#if DEBUG
    cout << "Condition 3: " << sw * g12_rgt << "\n";
#endif
    return false;
  }

  Interval g21_top = y21 * (S->box_dist_sq_grad(span_x, top_y).first - T->box_dist_sq_grad(span_x, top_y).first)
                   + y22 * (T->box_dist_sq_grad(span_x, top_y).first - U->box_dist_sq_grad(span_x, top_y).first);
  if (!((sw * g21_top).mag() <= fabs(g2p))) {
#if DEBUG
    cout << "Condition 4: " << sw * g21_top << "\n";
#endif
    return false;
  }
  
  // Conditions 2.13.
  Interval g12_lft = y11 * (S->box_dist_sq_grad(lft_x, span_y).second - T->box_dist_sq_grad(lft_x, span_y).second)
                   + y12 * (T->box_dist_sq_grad(lft_x, span_y).second - U->box_dist_sq_grad(lft_x, span_y).second);
  if (!((sw * g12_lft).mag() <= fabs(g1m))) {
#if DEBUG
    cout << "Condition 5: " << sw * g12_lft << "\n";
#endif
    return false;
  }

  Interval g21_bot = y21 * (S->box_dist_sq_grad(span_x, bot_y).first - T->box_dist_sq_grad(span_x, bot_y).first)
		   + y22 * (T->box_dist_sq_grad(span_x, bot_y).first - U->box_dist_sq_grad(span_x, bot_y).first);
  if (!((sw * g21_bot).mag() <= fabs(g2m))) {
#if DEBUG
    cout << "Condition 6: " << sw * g21_bot << "\n";
#endif
    return false;
  }

  return true;
}

/***********************************************************/

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

int vor_box::get_dir(int index) {
  // TODO: Improve direction computation logic.
  switch(index) {
  case 0:
    return 2;
  case 1:
    return 1;
  case 2:
    return -2;
  case 3:
    return -1;
  default:
    cout << "Warning: Failed to compute direction.\n";
    return -1;
  }
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

    int dir = get_dir(j);
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
  // TODO: Fix logic for nodes_.size() > 2. Update: Why is it wrong?
  if (nodes_.size() == 2) {
    segments_.push_back(new vor_seg(*nodes_[0], *nodes_[1]));
  } else {
    Point2d center(center_[0], center_[1]);
    for (Point2d* node : nodes_) {
      segments_.push_back(new vor_seg(*node, center));
    }
  }
}

bool vor_box::contained_in(const vor_box& other, double scale) const {
  double mx = center()[0];
  double my = center()[1];
  double hw = width() / 2.0;

  double ox = other.center()[0];
  double oy = other.center()[1];
  double ow = scale * other.width() / 2.0;

  return (mx - hw >= ox - ow) &&
    (mx + hw <= ox + ow) &&
    (my - hw >= oy - ow) &&
    (my + hw <= oy + ow);
}

} // namespace vor2d
