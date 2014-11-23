#pragma once

#include <vector>
#include "./Wall.h"
#include "./Edge.h"
#include "./Corner.h"
#include "./Box3d.h"

class ConfBox3d;

class ConfBox3dPredicate {
 public:
  void classify(ConfBox3d* b);

  // find the nearest feature, and check
  Box::Status checkChildStatus(Box* b, double x, double y, double z) {
    Wall* nearestWall;
    double minDistW = std::numeric_limits<double>::max();
    if (!b->Walls.empty()) {
      list<Wall*>::iterator iterW = b->Walls.begin();
      minDistW = (*iterW)->distance(x, y, z);
      nearestWall = *iterW;
      ++iterW;
      for (; iterW != b->Walls.end(); ++iterW) {
        double dist = (*iterW)->distance(x, y, z);
        if (dist < minDistW) {
          minDistW = dist;
          nearestWall = *iterW;
        }
      }
    }

    double minDistE = std::numeric_limits<double>::max();
    if (!b->Edges.empty()) {
      list<Edge*>::iterator iterE = b->Edges.begin();
      minDistE = (*iterE)->distance(x, y, z);
      ++iterE;
      for (; iterE != b->Edges.end(); ++iterE) {
        double dist = (*iterE)->distance(x, y, z);
        if (dist < minDistE) {
          minDistE = dist;
          // nearestEdge = *iterE;
        }
      }
    }

    double minDistC = std::numeric_limits<double>::max();  //minDistC may not exist, so init to a bigger number
    if (!b->corners.empty()) {
      list<Corner*>::iterator iterC = b->corners.begin();
      minDistC = (*iterC)->distance(x, y, z);
      ++iterC;
      for (; iterC != b->corners.end(); ++iterC) {
        Corner* c = *iterC;
        double dist = c->distance(x, y, z);
        if (dist < minDistC) {
          minDistC = dist;
        }
      }
    }

    bool isFree = false;

    // if the nearest feature is a wall...
    // check the orientation of m(B) with respect to the wall
    if (minDistW < minDistE && minDistW < minDistC) {
      if (nearestWall->isRight(x, y, z)) {
        isFree = true;
      }
    }
    // else, the nearest feature is either an edge or corner, and the box must be free (since all obstacles are convex)
    else isFree = true;

    return isFree ? Box::FREE : Box::STUCK;
  }

};

class ConfBox3d {
 private:
  Box3d* box;
 public:
  int depth;
  int priority;
  bool isLeaf;
  static double r0;
  double rB;
  static int confBoxIdCounter;
  int confBoxId;
  static ConfBox3dPredicate* predicate;
  vector<ConfBox3d*> children;
  vector<ConfBox3d*> neighbor;
  ConfBox3d* parent;
  enum Status { FREE, STUCK, MIXED, UNKNOWN };
  Status status;
  Set* pSet;
  list<Corner*> corners;
  list<Edge*> Edges;
  list<Wall*> Walls;
  list<Sphere*> spheres;

  static int counter;   // time of expansion (used in BFS strategy)
  static vector<ConfBox3d*> boxes;

  double dist2Source;
  int heapId;
  Box* prev;
  bool visited;

 ConfBox3d(double xx, double yy, double zz, double w):
  depth(1), isLeaf(true),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false) {
    box = new Box3d(xx, yy, zz, w);
    confBoxIdCounter++;
    confBoxId = confBoxIdCounter;
    rB = (w * sqrt(3))/2;
    priority = counter;
    boxes.push_back(this);
    predicate = new ConfBox3dPredicate();
 }

  ConfBox3d* getBox(double xx, double yy, double zz) {
    if (isLeaf) {
      return box->containsPoint(xx, yy, zz) ? this : 0;
    } else {
      for (int i = 0; i < children.size(); i++) {
        ConfBox3d* b = children[i]->getBox(xx, yy, zz);
        if (b != 0) {
          return b;
        }
      }
      return 0;
    }
  }

  void addCorner(Corner* c) {
    corners.push_back(c);
  }

  void addEdge(Edge* e) {
    Edges.push_back(e);
  }

  void addWall(Wall* w) {
    Walls.push_back(w);
  }

  void addSphere(Sphere* s) {
    spheres.push_back(s);
  }

  bool isFree() {
    return status == FREE;
  }

  Status getStatus() {
    predicate->classify(this);
    return status;
  }

  vector<ConfBox3d*> getChildren() {
    return children;
  }

  vector<ConfBox3d*> getNeighbor() {
    return neighbor;
  }

  bool isNeighbor(ConfBox3d* other) {
    return box->isAdjacent(other->box);
  }

  bool split(double epsilon) {
    vector<Box3d*>* box3dChildren = box->split(epsilon);
    if (box3dChildren == 0) {
      return false;
    }
    for (int i = 0; i < box3dChildren->size(); i++) {
      Box3d* b = (*box3dChildren)[i];
      children.push_back(new ConfBox3d(b->origin->x, b->origin->y, b->origin->z, b->width));
    }
    for (int i = 0; i < children.size(); i++) {
      for (int j = i + 1; j < children.size(); j++) {
        if (children[i]->isNeighbor(children[j])) {
          children[i]->neighbor.push_back(children[j]);
          children[j]->neighbor.push_back(children[i]);
        }
      }
    }
    for (int i = 0; i < children.size(); i++) {
      for (int j = 0; j < neighbor.size(); j++) {
        if (children[i]->isNeighbor(neighbor[j])) {
          children[i]->neighbor.push_back(neighbor[j]);
          neighbor[i]->neighbor.push_back(children[j]);
        }
      }
    }
    return true;
  }
};
