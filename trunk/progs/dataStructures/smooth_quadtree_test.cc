/**
 *  File: smooth_quadtree_test.cc
 *
 *  Description: 
 *  An implementation of the smooth quadtree data structure.
 *
 *  Written by Huck Bennett, 9/21/2014
 */

#include "smooth_quadtree.h"
#include <assert.h>
#include <iostream>

using namespace std;

struct Pair {
  Pair (int x, int y) : x_(x), y_(y) {}
  int x_;
  int y_;
};

int main(int argc, char** argv) {
  SmoothQuadTree<Pair>* tree = new SmoothQuadTree<Pair>(2);
  SmoothQuadTreeBox<Pair>* root = tree->root();
  cout << root->center()[0] << " " << root->center()[1] << "\n";

  root->smooth_split();
  root->children()[0]->smooth_split();
  SmoothQuadTreeBox<Pair>* cur_child = root->children()[1];

  Pair p1 (3, 4);
  Pair p2 (4, 5);

  root->set_data(&p1);
  cur_child->set_data(&p2);
  assert(!cur_child->is_split());
  root->children()[0]->children()[3]->smooth_split();
  root->children()[0]->children()[3]->children()[3]->smooth_split();
  assert(cur_child->is_split());
  assert(root->children()[3]->is_split());
  cout << "Data: " << cur_child->data()->x_ << " " << cur_child->data()->y_ << "\n";

  cout << "Dir 1 neighbors:\n";
  shared_ptr<vector<SmoothQuadTreeBox<Pair>*>> neighbors = cur_child->leaf_neighbors_dir(1);
  for (SmoothQuadTreeBox<Pair>* neighbor : *neighbors) {
    cout << neighbor->center()[0] << " " << neighbor->center()[1] << "\n";
  }

  cout << "Dir -1 neighbors:\n";
  neighbors = cur_child->leaf_neighbors_dir(-1); 
  for (SmoothQuadTreeBox<Pair>* neighbor : *neighbors) {
    cout << neighbor->center()[0] << " " << neighbor->center()[1] << "\n";
  }

  cout << "Dir 2 neighbors:\n";
  neighbors = cur_child->leaf_neighbors_dir(2);
  for (SmoothQuadTreeBox<Pair>* neighbor : *neighbors) {
    cout << neighbor->center()[0] << " " << neighbor->center()[1] << "\n";
  }

  assert(tree->splits() == 9);
  cout << "# splits: " << tree->splits() << "\n";
  cout << "# smooth splits: " << tree->smooth_splits() << "\n";

  delete tree;
  exit(0);
}
