/**
 *  File: QuadTree.h
 *
 *  Description: 
 *  An implementation of the smooth quadtree data structure.
 *
 *  Written by Huck Bennett, 9/21/2014
 */

#include <vector>

#ifndef SMOOTH_QUADTREE_H
#define SMOOTH_QUADTREE_H

using std::vector;
  
template <typename T>
class SmoothQuadTreeBox {
 public:
  SmoothQuadTreeBox<T>(double dimension, int depth, double center[]);
  ~SmoothQuadTreeBox<T>();
  void split();
  void set_data(T* data);
  T* data() const;
  SmoothQuadTreeBox*** children() const;
  vector<SmoothQuadTreeBox<T>*>* neighbors() const;

 private:
  T* data_;
  SmoothQuadTreeBox<T>* children_[];
  vector<SmoothQuadTreeBox<T>*> neighbors_;
  const int dimension_;
  const int depth_;
  const double center_[];
};

template <typename T>
class SmoothQuadTree {
 public:
  SmoothQuadTree<T>(double dimension);
  SmoothQuadTree<T>* root();

 private:
  SmoothQuadTreeBox<T>* root_;
};

#endif // SMOOTH_QUADTREE_H
