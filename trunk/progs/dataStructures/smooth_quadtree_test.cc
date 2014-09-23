/**
 *  File: smooth_quadtree_test.cc
 *
 *  Description: 
 *  An implementation of the smooth quadtree data structure.
 *
 *  Written by Huck Bennett, 9/21/2014
 */

#include "smooth_quadtree.h"
#include <iostream>

using namespace std;

struct Pair {
  int x;
  int y;
};

int main(int argc, char** argv) {
  SmoothQuadTree<Pair>* tree = new SmoothQuadTree<Pair>(2);
  SmoothQuadTreeBox<Pair>* root = tree->root();
  cout << root->center()[0] << " " << root->center()[1] << "\n";

  root->smooth_split();

  for (int i = 0; i < 4; i++) {
    SmoothQuadTreeBox<Pair>* child = root->children()[i];
    cout << "  " << child->center()[0] << " " << child->center()[1] << "\n";
    child->smooth_split();
    
    for (int j = 0; j < 4; j++) {
      SmoothQuadTreeBox<Pair>* grand_child = child->children()[j];
      cout << "    " << grand_child->center()[0] << " " << grand_child->center()[1] << "\n";
    }
  }

  delete tree;
  exit(0);
}
