#pragma once

#include "Corner.h"
#include "Edge.h"
#include "Object.h"
#include "Point.h"
#include "vor2d.h"

#include <limits>
#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace vor2d {

class vor_qt;

struct vor_seg {
  Point2d p;
  Point2d q;
};

class vor_box {
 public:
  vor_box(int depth, int indicator, double center[], vor_qt* tree);
  ~vor_box();
  void smooth_split();
  double width() const;
  double radius() const;
  double* center() const;
  vor_box** children() const;
  bool is_leaf() const;
  vor_box* principal_neighbor_dir(int dir) const;
  const int dimension() const;
  int depth() const;
  int num_children() const;
  shared_ptr<vector<vor_box*>> leaf_neighbors_dir(int dir);
  shared_ptr<vector<vor_box*>> enumerate_halfspace_leaf_descendants(int dir);
  
  // Voronoi specific methods.
  void add_corner(Corner* corner);
  void add_edge(Edge* edge);
  void add_object(Object* object);
  vector<Corner*>* get_corners();
  vector<Edge*>* get_edges();
  vector<Object*>* get_objects();
  int num_features() const;
  double max_lipschitz() const;
  double clearance() const;
  double clearance(const Point2d&) const;
  int num_objects() const;
  void set_active(bool is_active);
  bool is_active() const;
  void gen_vertices();

 protected:
  vor_box** neighbors() const;
  void split();
  void smooth_split_aux();
  Object* nearest_obj(const Point2d&) const;

  // Fields.
  bool is_active_;
  const int depth_;
  const int indicator_;
  double width_;
  double radius_;
  double* center_;
  int num_children_;
  vor_qt* tree_;

  // Collections.
  vor_box** children_;
  vor_box** neighbors_;
  vector<Corner*> corners_;
  vector<Edge*> edges_;
  vector<Object*> objects_;
  vector<Point2d*> nodes_;
  vector<vor_seg*> segments_;

  friend class vor_qt;
};

} // namespace vor2d
