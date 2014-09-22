/**
 *  File: QuadTree.cc
 *
 *  Description: 
 *  An implementation of the smooth quadtree data structure.
 *
 *  Written by Huck Bennett, 9/21/2014
 */

#include "SmoothQuadTree.h"
#include <math.h>
#include <vector>

template <typename T>
SmoothQuadTreeBox<T>::SmoothQuadTreeBox(double dimension, int depth, double center[]) 
  : dimension_(dimension), depth_(depth), center_(center){
}

template <typename T>
SmoothQuadTreeBox<T>::~SmoothQuadTreeBox() {
  delete center_;
  
  if (data_ != nullptr) {
    delete data_;
  }

  if (children_ != nullptr) {
    for (SmoothQuadTree<T>* child : children_) {
      delete child;
    }
  }
}

template <typename T>
void SmoothQuadTreeBox<T>::split() {
  int num_children = 1 << dimension_;
  double* center;

  // Initialize the children.
  children_ = new SmoothQuadTreeBox<T>*[num_children];
  for (int i = 0; i < num_children; i++) {

    // Compute the center of the new child.
    center = new double[dimension_];
    for (int j = 0; j < dimension_; j++) {
      center[j] = center_[j] + pow(-1, i ^ (1 << j)) * pow(2, -depth_);
    }

    children_[i] = new SmoothQuadTreeBox<T>(dimension_, depth_ + 1, center);  
  }

  // TODO: Initialize neighbors.
}

template <typename T>
void SmoothQuadTreeBox<T>::set_data(T* data) {
  data_ = data;
}

template <typename T>
T* SmoothQuadTreeBox<T>::data() const {
  return data_;
}

template <typename T>
SmoothQuadTreeBox<T>*** SmoothQuadTreeBox<T>::children() const {
  return &children_;
}

template <typename T>
vector<SmoothQuadTreeBox<T>*>* SmoothQuadTreeBox<T>::neighbors() const {
  return &neighbors_;
}
