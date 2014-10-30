#pragma once

#include "Corner.h"
#include "Edge.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace vor2d {

class vor_qt;

class vor_box {
 public:
  vor_box(int depth, int indicator, double center[], vor_qt* tree);
  ~vor_box();
  void smooth_split();
  double width() const;
  double* center() const;
  vor_box** children() const;
  bool is_leaf() const;
  vor_box* principal_neighbor_dir(int dir) const;
  const int dimension() const;
  int depth() const;
  shared_ptr<vector<vor_box*>> leaf_neighbors_dir(int dir);
  shared_ptr<vector<vor_box*>> enumerate_halfspace_leaf_descendants(int dir);
  
  // Voronoi specific methods.
  void add_corner(Corner* corner);
  void add_edge(Edge* edge);

  vector<Corner*>* get_corners();
  vector<Edge*>* get_edges();

 protected:
  vor_box** neighbors() const;
  void split();
  void smooth_split_aux();

  const int depth_;
  const int indicator_;
  double* center_;
  vor_box** children_;
  vor_box** neighbors_;
  vor_qt* tree_;

  vector<Corner*> corners_;
  vector<Edge*> edges_;

  friend class vor_qt;
};

} // namespace vor2d
