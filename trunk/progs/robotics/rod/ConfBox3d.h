#pragma once

#include <vector>
#include <list>
#include <assert.h>
#include "./Wall.h"
#include "./Edge.h"
#include "./Corner.h"
#include "./Box3d.h"
#include "./Rot3dSide.h"
#include <iostream>

class ConfBox3d;
class Set;

enum Status { FREE, STUCK, MIXED, UNKNOWN };

class ConfBox3dPredicate {
 public:
  void classify(ConfBox3d* b);
  Status checkChildStatus(ConfBox3d* b, double x, double y, double z, int boxId);

};

class ConfBox3d {
 private:
  Box3d* box = 0;
  Rot3dSide* rot = 0;
 public:
  double x;
  double y;
  double z;
  double rot_x;
  double rot_y;
  double rot_z;
  double width;
  double rot_width;
  int depth;
  int priority;
  static double r0;
  double rB;
  static int boxIdCounter;
  int boxId;
  static ConfBox3dPredicate* predicate;
  vector<ConfBox3d*> children;
  vector<ConfBox3d*> neighbors;
  ConfBox3d* parent;
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
  ConfBox3d* prev;
  bool visited;

 ConfBox3d(double xx, double yy, double zz, double w):
  depth(1), x(xx), y(yy), z(zz), width(w),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false) {
    box = new Box3d(xx, yy, zz, w);
    boxIdCounter++;
    boxId = boxIdCounter;
    rB = (w * sqrt(3))/2;
    priority = counter;
    boxes.push_back(this);
    predicate = new ConfBox3dPredicate();
    cout << boxId << "\t" << xx << "\t" << yy << "\t" << zz << "\t" << width << endl;
 }

 ConfBox3d(double xx, double yy, double zz, double w, double rot_xx, double rot_yy, double rot_zz, double rot_ww):
  depth(1), x(xx), y(yy), z(zz), width(w),
  rot_x(rot_xx), rot_y(rot_yy), rot_z(rot_zz), rot_width(rot_ww),
  parent(0), status(UNKNOWN),
  pSet(0), dist2Source(-1), heapId(-1), prev(0), visited(false) {
    box = new Box3d(xx, yy, zz, w);
    rot = new Rot3dSide(rot_xx, rot_yy, rot_zz, rot_ww);
    boxIdCounter++;
    boxId = boxIdCounter;
    rB = (w * sqrt(3))/2;
    priority = counter;
    boxes.push_back(this);
    predicate = new ConfBox3dPredicate();
    cout << boxId << "\t" << xx << "\t" << yy << "\t" << zz << "\t" << width << endl;
 }

  bool isLeaf() {
    return children.empty();
  }

  ConfBox3d* getBox(double xx, double yy, double zz) {
    if (isLeaf()) {
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

  vector<ConfBox3d*> getNeighbors() {
    return neighbors;
  }

  // When any of the boxes does not have restrictions on the rotational degrees of freedom, just compare their translation boxes;
  // Otherwise, one of the components should be the same and the other adjacent
  bool isNeighbor(ConfBox3d* other) {
    if (!rot || !other->rot) {
      return box->isAdjacent(other->box);
    } else {
      return false;
        (rot->isIdentical(other->rot) && box->isAdjacent(other->box)) ||
        (rot->isAdjacent(other->rot) && box->isIdentical(other->box));
    }
  }

  bool split(double epsilon) {
    if (!children.empty() || !box->split(epsilon)) {
      return false;
    }
    vector<Box3d*> box3dChildren = box->children;
    for (int i = 0; i < box3dChildren.size(); i++) {
      Box3d* b = box3dChildren[i];
      children.push_back(new ConfBox3d(b->origin->x, b->origin->y, b->origin->z, b->width));
      children[i]->parent = this;
      children[i]->depth = this->depth + 1;
      children[i]->Walls.insert(children[i]->Walls.begin(),
                                Walls.begin(),
                                Walls.end());
      children[i]->Edges.insert(children[i]->Edges.begin(),
                                Edges.begin(),
                                Edges.end());
      children[i]->corners.insert(children[i]->corners.begin(),
                                  corners.begin(),
                                  corners.end());
      children[i]->spheres.insert(children[i]->spheres.begin(),
                                  spheres.begin(),
                                  spheres.end());
    }
    for (int i = 0; i < children.size(); i++) {
      for (int j = i + 1; j < children.size(); j++) {
        if (children[i]->isNeighbor(children[j])) {
          children[i]->neighbors.push_back(children[j]);
          children[j]->neighbors.push_back(children[i]);
        }
      }
    }
    for (int i = 0; i < children.size(); i++) {
      for (int j = 0; j < neighbors.size(); j++) {
        bool isNeighbor = children[i]->isNeighbor(neighbors[j]);
        if (isNeighbor) {
          children[i]->neighbors.push_back(neighbors[j]);
          neighbors[j]->neighbors.push_back(children[i]);
        }
      }
    }
    return true;
  }
};
