#include <iostream>
#include "./Box.h"
#include <assert.h>
#include "./Predicate.h"

using namespace std;

BoxIter::BoxIter(const Box* bb, int direc):b(bb), direction(direc) {
  neighborVec.clear();
  index = 1;

  Box* n = b->pChildren[direction];
  if (!n) {
    neighborVec.push_back(0);
  } else if (n->isLeaf) {
    neighborVec.push_back(n);
  } else {
    // n is an internal node, and we need to recursively add the relevant leaves to neighborVec
    storeNeighbors(n);
  }
  neighborVec.push_back(0);
}

int BoxIter::size() {
  return neighborVec.size();
}

int BoxIter::getIndex() {
  return index;
}

Box* BoxIter::First() {
  return neighborVec[0];
}

Box* BoxIter::End() {
  return neighborVec[neighborVec.size() - 1];
}

Box* BoxIter::Next() {
  Box* next = neighborVec[index];
  index++;
  return next;
}

void BoxIter::storeAllNeighbors(Box* n, int a, int b, int c, int d) {
  storeNeighbors(n->pChildren[a]);
  storeNeighbors(n->pChildren[b]);
  storeNeighbors(n->pChildren[c]);
  storeNeighbors(n->pChildren[d]);
}

// recursively store neighbors in neighborVec
void BoxIter::storeNeighbors (Box* n) {
  if (n->isLeaf) {
    neighborVec.push_back(n);
    return;
  }
  switch (direction) {
  case 0:
    storeAllNeighbors(n, 4, 5, 6, 7);
    break;
  case 1:
    storeAllNeighbors(n, 0, 1, 2, 3);
    break;
  case 2:
    storeAllNeighbors(n, 1, 2, 5, 6);
    break;
  case 3:
    storeAllNeighbors(n, 0, 3, 4, 7);
    break;
  case 4:
    storeAllNeighbors(n, 0, 1, 4, 5);
    break;
  case 5:
    storeAllNeighbors(n, 2, 3, 6, 7);
    break;
  default:
    std::cerr << "Direction should be 0 - 5" << std::endl;
    exit(1);
  }
}

double Box::r0 = 0;
int Box::boxIdCounter = 0;
int Box::counter = 0;
vector<Box*> Box::boxes;
Predicate* Box::predicate;

bool Box::inIntervalAtXByY(double p, double x, double y) {
  return (p > x - y) && (p < x + y);
}

bool Box::isOverLimit(const Box* base, const Box* nextBox) {
  double halfWidth = base->width / 2;
  if ( (inIntervalAtXByY(nextBox->x, base->x, halfWidth) &&
        inIntervalAtXByY(nextBox->y, base->y, halfWidth)) ||
       (inIntervalAtXByY(nextBox->x, base->x, halfWidth) &&
        inIntervalAtXByY(nextBox->z, base->z, halfWidth)) ||
       (inIntervalAtXByY(nextBox->y, base->y, halfWidth) &&
        inIntervalAtXByY(nextBox->z, base->z, halfWidth)) ) {
    return false;
  }
  return true;
}

Box::Box(double xx, double yy, double zz, double w):
  depth(1), x(xx), y(yy), z(zz), width(w), isLeaf(true),
  pParent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false) {
  Box::boxIdCounter++;
  boxId = boxIdCounter;
  for (int i = 0; i < 8; ++i) {
    pChildren[i] = 0;
  }
  rB = (width * sqrt(3))/2;
  priority = Box::counter;
  boxes.push_back(this);
  predicate = new BoxPredicate();
}

Box::Status Box::getStatus() {
  predicate->classify(this);
  return status;
}
