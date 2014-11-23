#pragma once
#include <vector>
#include "./Vector.h"

using namespace std;

class Box3d {
 private:
  Vector* origin;
  double width;
  vector<Box3d*>* children;
  vector<Box3d*>* neighbor;

 public:
  Box3d(double x, double y, double z, double width) {
    origin = new Vector(x, y, z);
    this->width = width;
  }

  bool approxEqual(double x, double y) {
    return abs(x - y) < 0.001;
  }

  bool isAdjacent(Box3d* other) {
    double dx = abs(other->origin->x - origin->x);
    double dy = abs(other->origin->y - origin->y);
    double dz = abs(other->origin->z - origin->z);
    double sw = width + other->width;
    double dw = abs(width - other->width);
    return
      (approxEqual(dx, sw) && approxEqual(dy, dw) && approxEqual(dz, dw)) ||
      (approxEqual(dx, dw) && approxEqual(dy, sw) && approxEqual(dz, dw)) ||
      (approxEqual(dx, dw) && approxEqual(dy, dw) && approxEqual(dz, sw));
  }

  bool isIdentical(Box3d* other) {
    return origin == other->origin && width == other->width;
  }

  vector<Box3d*>* split(double epsilon) {
    if (width < epsilon) {
      return 0;
    }
    for (int i = 0; i < 2; i++) {
      double nx = origin->x - width / 4 + width / 2 * i;
      for (int j = 0; j < 2; j++) {
        double ny = origin->y - width / 4 + width / 2 * i;
        for (int k = 0; k < 2; k++) {
          double nz = origin->z - width / 4 + width / 2 * i;
          children->push_back(new Box3d(nx, ny, nz, width / 2));
        }
      }
    }
    for (int i = 0; i < 8; i++) {
      for (int j = i + 1; j < 8; j++) {
        if ((*children)[i]->isAdjacent((*children)[j])) {
          (*children)[i]->neighbor->push_back((*children)[j]);
          (*children)[j]->neighbor->push_back((*children)[i]);
        }
      }
    }
    return children;
  }
};
