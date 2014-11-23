#pragma once

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <utility>
#include "./Wall.h"
#include "./Edge.h"
#include "./Corner.h"
#include "./Box3d.h"

class Set;
class Box;
class Predicate;
class BoxPredicate;

using namespace std;

class BoxIter {
 private:
  const Box* b;
  vector<Box*> neighborVec;
  int direction;
  unsigned long index;
  void storeNeighbors(Box* n);
  void storeAllNeighbors(Box* n, int a, int b, int c, int d);

 public:
  BoxIter(const Box* bb, int direc);

  int size();
  int getIndex();

  Box* First();

  Box* Next();

  Box* End();
};  // class BoxIter

class Box {
 private:
  static bool inIntervalAtXByY(double p, double x, double y);

  static bool isOverLimit(const Box* base, const Box* nextBox);

 public:
  friend class BoxIter;
  friend class QuadTree;
  int depth;
  double x, y, z;
  double width;
  int priority;
  bool isLeaf;
  static double r0;
  double rB;
  static int boxIdCounter;
  int boxId;

  static int counter;   // time of expansion (used in BFS strategy)
  static vector<Box*> boxes;
  static Predicate* predicate;

  // Pointers to children, but when no children (i.e., leaf),
  //    the pointers are used as neighbor pointers
  // where
  //    0 = TNW, 1 = TEN, 2 = TSE, 3 = TWS  [T = Top]
  //      4 = BNW, 5 = BEN, 6 = BSE, 7 = BWS  [B = Bottom]
  Box* pChildren[8];

  Box* pParent;  // parent in octree
  enum Status { FREE, STUCK, MIXED, UNKNOWN };
  Status status;
  Set* pSet;
  list<Corner*> corners;
  list<Edge*> Edges;
  list<Wall*> Walls;
  list<pair<Corner*, int> > spheres;

  // for shortest path
  double dist2Source;
  int heapId;
  Box* prev;
  bool visited;

  Box(double xx, double yy, double zz, double w);

  Box* getBox(double xx, double yy, double zz) {
    if (xx > x + width / 2 || xx < x - width / 2 ||
        yy > y + width / 2 || yy < y - width / 2 ||
        zz > z + width / 2 || zz < z - width / 2) {
      return 0;
    }

    Box* b = this;
    while (!b->isLeaf) {
      double dx = xx - b->x;
      double dy = yy - b->y;
      double dz = zz - b->z;

      if (dx <= 0 && dy >= 0 && dz >= 0) {
        b = b->pChildren[3];
      } else if (dx >= 0 && dy >= 0 && dz >= 0) {
        b = b->pChildren[2];
      } else if (dx >= 0 && dy >= 0 && dz <= 0) {
        b = b->pChildren[1];
      } else if (dx <= 0 && dy >= 0 && dz <= 0) {
        b = b->pChildren[0];
      } else if (dx <= 0 && dy <= 0 && dz >= 0) {
        b = b->pChildren[7];
      } else if (dx >= 0 && dy <= 0 && dz >= 0) {
        b = b->pChildren[6];
      } else if (dx >= 0 && dy <= 0 && dz <= 0) {
        b = b->pChildren[5];
      } else if (dx <= 0 && dy <= 0 && dz <= 0) {
        b = b->pChildren[4];
      }
    }
    return b;
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

  void addSphere(Corner* s, double radius) {
    spheres.push_back(make_pair(s, radius));
  }

  bool isFree() {
    return status == FREE;
  }

  Status getStatus();

  // split(eps)
  //         returns false if we fail to split for some reason
  //
  bool split(double epsilon) {
    if (this->width < epsilon) {
      return false;
    }

    if (!this->isLeaf || this->status != MIXED) {
      return false;
    }

    //record the time of this split event, will be used to set priority of children
    ++Box::counter;

    Box* children[8];

    children[0] = new Box(x - width / 4, y + width / 4, z - width / 4, width / 2);
    children[1] = new Box(x + width / 4, y + width / 4, z - width / 4, width / 2);
    children[2] = new Box(x + width / 4, y + width / 4, z + width / 4, width / 2);
    children[3] = new Box(x - width / 4, y + width / 4, z + width / 4, width / 2);

    children[4] = new Box(x - width / 4, y - width / 4, z - width / 4, width / 2);
    children[5] = new Box(x + width / 4, y - width / 4, z - width / 4, width / 2);
    children[6] = new Box(x + width / 4, y - width / 4, z + width / 4, width / 2);
    children[7] = new Box(x - width / 4, y - width / 4, z + width / 4, width / 2);

    for (int i = 0; i < 8; ++i) {
      children[i]->depth = this->depth + 1;

      // add pointers to siblings
      switch (i) {
      case 0:
        children[i]->pChildren[1] = children[4];
        children[i]->pChildren[3] = children[1];
        children[i]->pChildren[4] = children[3];
        break;
      case 1:
        children[i]->pChildren[1] = children[5];
        children[i]->pChildren[2] = children[0];
        children[i]->pChildren[4] = children[3];
        break;
      case 2:
        children[i]->pChildren[1] = children[6];
        children[i]->pChildren[2] = children[3];
        children[i]->pChildren[5] = children[1];
        break;
      case 3:
        children[i]->pChildren[1] = children[7];
        children[i]->pChildren[3] = children[2];
        children[i]->pChildren[5] = children[0];
        break;
      case 4:
        children[i]->pChildren[0] = children[0];
        children[i]->pChildren[3] = children[5];
        children[i]->pChildren[4] = children[7];
        break;
      case 5:
        children[i]->pChildren[0] = children[1];
        children[i]->pChildren[2] = children[4];
        children[i]->pChildren[4] = children[6];
        break;
      case 6:
        children[i]->pChildren[0] = children[2];
        children[i]->pChildren[2] = children[7];
        children[i]->pChildren[5] = children[5];
        break;
      case 7:
        children[i]->pChildren[0] = children[5];
        children[i]->pChildren[3] = children[6];
        children[i]->pChildren[5] = children[4];
        break;
      default:
        std::cerr << "Only eight children" << std::endl;
        exit(1);
      }
    }

    for (int i = 0; i < 6; ++i) {
      //init box neighbor iterator for direction i
      BoxIter* iter = new BoxIter(this, i);
      Box* neighbor = iter->First();

      if (!neighbor) {
        continue;
      }
      switch (i) {
      case 0:
        while (neighbor != iter->End()) {
          if (!isOverLimit(children[0], neighbor) || !isOverLimit(neighbor, children[0])) {
            if (children[0]->depth >= neighbor->depth)  // if neighbor is larger...
              {
                children[0]->pChildren[i] = neighbor;
                if (children[0]->depth == neighbor->depth) neighbor->pChildren[1] = children[0];
              }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (temp->depth != children[0]->depth) temp = temp->pParent;
                children[0]->pChildren[i] = temp;
                neighbor->pChildren[1] = children[0];
              }
          }
          if (!isOverLimit(children[1], neighbor) || !isOverLimit(neighbor, children[1])) {
            if (children[1]->depth >= neighbor->depth) {
              children[1]->pChildren[i] = neighbor;
              if (children[1]->depth == neighbor->depth) neighbor->pChildren[1] = children[1];
            } else
              {
                // neighbor is smaller, so we need to point to its ancestor of
                // equal depth
                Box* temp = neighbor;
                while (!(temp->depth == children[1]->depth)) temp = temp->pParent;
                children[1]->pChildren[i] = temp;
                neighbor->pChildren[1] = children[1];
              }
          }
          if (!isOverLimit(children[2], neighbor) || !isOverLimit(neighbor, children[2])) {
            if (children[2]->depth >= neighbor->depth) {
              children[2]->pChildren[i] = neighbor;
              if (children[2]->depth == neighbor->depth) neighbor->pChildren[1] = children[2];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[2]->depth)) temp = temp->pParent;
                children[2]->pChildren[i] = temp;
                neighbor->pChildren[1] = children[2];
              }
          }
          if (!isOverLimit(children[3], neighbor) || !isOverLimit(neighbor, children[3])) {
            if (children[3]->depth >= neighbor->depth) {
              children[3]->pChildren[i] = neighbor;
              if (children[3]->depth == neighbor->depth) neighbor->pChildren[1] = children[3];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[3]->depth)) temp = temp->pParent;
                children[3]->pChildren[i] = temp;
                neighbor->pChildren[1] = children[3];
              }
          }

          neighbor = iter->Next();
        }
        break;
      case 1:
        while (neighbor != iter->End()) {
          if (!isOverLimit(children[4], neighbor) || !isOverLimit(neighbor, children[4])) {
            if (children[4]->depth >= neighbor->depth) {
              children[4]->pChildren[i] = neighbor;
              if (children[4]->depth == neighbor->depth) neighbor->pChildren[0] = children[4];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[4]->depth)) temp = temp->pParent;
                children[4]->pChildren[i] = temp;
                neighbor->pChildren[0] = children[4];
              }
          }
          if (!isOverLimit(children[5], neighbor) || !isOverLimit(neighbor, children[5])) {
            if (children[5]->depth >= neighbor->depth) {
              children[5]->pChildren[i] = neighbor;
              if (children[5]->depth == neighbor->depth) neighbor->pChildren[0] = children[5];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[5]->depth)) temp = temp->pParent;
                children[5]->pChildren[i] = temp;
                neighbor->pChildren[0] = children[5];
              }
          }
          if (!isOverLimit(children[6], neighbor) || !isOverLimit(neighbor, children[6])) {
            if (children[6]->depth >= neighbor->depth) {
              children[6]->pChildren[i] = neighbor;
              if (children[6]->depth == neighbor->depth) neighbor->pChildren[0] = children[6];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[6]->depth)) temp = temp->pParent;
                children[6]->pChildren[i] = temp;
                neighbor->pChildren[0] = children[6];
              }
          }
          if (!isOverLimit(children[7], neighbor) || !isOverLimit(neighbor, children[7])) {
            if (children[7]->depth >= neighbor->depth) {
              children[7]->pChildren[i] = neighbor;
              if (children[7]->depth == neighbor->depth) neighbor->pChildren[0] = children[7];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[7]->depth)) temp = temp->pParent;
                children[7]->pChildren[i] = temp;
                neighbor->pChildren[0] = children[7];
              }
          }

          neighbor = iter->Next();
        }
        break;
      case 2:
        while (neighbor != iter->End()) {
          if (!isOverLimit(children[0], neighbor) || !isOverLimit(neighbor, children[0])) {
            if (children[0]->depth >= neighbor->depth) {
              children[0]->pChildren[i] = neighbor;
              if (children[0]->depth == neighbor->depth) neighbor->pChildren[3] = children[0];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[0]->depth)) temp = temp->pParent;
              children[0]->pChildren[i] = temp;
              neighbor->pChildren[3] = children[0];
            }
          }
          if (!isOverLimit(children[3], neighbor) || !isOverLimit(neighbor, children[3])) {
            if (children[3]->depth >= neighbor->depth) {
              children[3]->pChildren[i] = neighbor;
              if (children[3]->depth == neighbor->depth) neighbor->pChildren[3] = children[3];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[3]->depth)) temp = temp->pParent;
                children[3]->pChildren[i] = temp;
                neighbor->pChildren[3] = children[3];
              }
          }
          if (!isOverLimit(children[4], neighbor) || !isOverLimit(neighbor, children[4])) {
            if (children[4]->depth >= neighbor->depth) {
              children[4]->pChildren[i] = neighbor;
              if (children[4]->depth == neighbor->depth) neighbor->pChildren[3] = children[4];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[4]->depth)) temp = temp->pParent;
              children[4]->pChildren[i] = temp;
              neighbor->pChildren[3] = children[4];
            }
          }
          if (!isOverLimit(children[7], neighbor) || !isOverLimit(neighbor, children[7])) {
            if (children[7]->depth >= neighbor->depth) {
              children[7]->pChildren[i] = neighbor;
              if (children[7]->depth == neighbor->depth) neighbor->pChildren[3] = children[7];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[7]->depth)) temp = temp->pParent;
              children[7]->pChildren[i] = temp;
              neighbor->pChildren[3] = children[7];
            }
          }
          neighbor = iter->Next();
        }
        break;
      case 3:
        while (neighbor != iter->End()) {
          if (!isOverLimit(children[1], neighbor) || !isOverLimit(neighbor, children[1])) {
            if (children[1]->depth >= neighbor->depth) {
              children[1]->pChildren[i] = neighbor;
              if (children[1]->depth == neighbor->depth) neighbor->pChildren[2] = children[1];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[1]->depth)) temp = temp->pParent;
              children[1]->pChildren[i] = temp;
              neighbor->pChildren[2] = children[1];
            }
          }
          if (!isOverLimit(children[2], neighbor) || !isOverLimit(neighbor, children[2])) {
            if (children[2]->depth >= neighbor->depth) {
              children[2]->pChildren[i] = neighbor;
              if (children[2]->depth == neighbor->depth) neighbor->pChildren[2] = children[2];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[2]->depth)) temp = temp->pParent;
              children[2]->pChildren[i] = temp;
              neighbor->pChildren[2] = children[2];
            }
          }
          if (!isOverLimit(children[5], neighbor) || !isOverLimit(neighbor, children[5])) {
            if (children[5]->depth >= neighbor->depth) {
              children[5]->pChildren[i] = neighbor;
              if (children[5]->depth == neighbor->depth) neighbor->pChildren[2] = children[5];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[5]->depth)) temp = temp->pParent;
                children[5]->pChildren[i] = temp;
                neighbor->pChildren[2] = children[5];
              }
          }
          if (!isOverLimit(children[6], neighbor) || !isOverLimit(neighbor, children[6])) {
            if (children[6]->depth >= neighbor->depth) {
              children[6]->pChildren[i] = neighbor;
              if (children[6]->depth == neighbor->depth) neighbor->pChildren[2] = children[6];
            }
            else // neighbor is smaller, so we need to point to its ancestor of equal depth
              {
                Box* temp = neighbor;
                while (!(temp->depth == children[6]->depth)) temp = temp->pParent;
                children[6]->pChildren[i] = temp;
                neighbor->pChildren[2] = children[6];
              }
          }
          neighbor = iter->Next();
        }
        break;
      case 4:
        while (neighbor != iter->End()) {
          if (!isOverLimit(children[2], neighbor) || !isOverLimit(neighbor, children[2])) {
            if (children[2]->depth >= neighbor->depth) {
              children[2]->pChildren[i] = neighbor;
              if (children[2]->depth == neighbor->depth) neighbor->pChildren[5] = children[2];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[2]->depth)) temp = temp->pParent;
              children[2]->pChildren[i] = temp;
              neighbor->pChildren[5] = children[2];
            }
          }
          if (!isOverLimit(children[3], neighbor) || !isOverLimit(neighbor, children[3])) {
            if (children[3]->depth >= neighbor->depth) {
              children[3]->pChildren[i] = neighbor;
              if (children[3]->depth == neighbor->depth) neighbor->pChildren[5] = children[3];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[3]->depth)) temp = temp->pParent;
              children[3]->pChildren[i] = temp;
              neighbor->pChildren[5] = children[3];
            }
          }
          if (!isOverLimit(children[6], neighbor) || !isOverLimit(neighbor, children[6])) {
            if (children[6]->depth >= neighbor->depth) {
              children[6]->pChildren[i] = neighbor;
              if (children[6]->depth == neighbor->depth) neighbor->pChildren[5] = children[6];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[6]->depth)) temp = temp->pParent;
              children[6]->pChildren[i] = temp;
              neighbor->pChildren[5] = children[6];
            }
          }
          if (!isOverLimit(children[7], neighbor) || !isOverLimit(neighbor, children[7])) {
            if (children[7]->depth >= neighbor->depth) {
              children[7]->pChildren[i] = neighbor;
              if (children[7]->depth == neighbor->depth) neighbor->pChildren[5] = children[7];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              //equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[7]->depth)) temp = temp->pParent;
              children[7]->pChildren[i] = temp;
              neighbor->pChildren[5] = children[7];
            }
          }

          neighbor = iter->Next();
        }
        break;
      case 5:
        while (neighbor != iter->End()) {
          if (!isOverLimit(children[0], neighbor) || !isOverLimit(neighbor, children[0])) {
            if (children[0]->depth >= neighbor->depth) {
              children[0]->pChildren[i] = neighbor;
              if (children[0]->depth == neighbor->depth) neighbor->pChildren[4] = children[0];
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              //equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[0]->depth)) temp = temp->pParent;
              children[0]->pChildren[i] = temp;
              neighbor->pChildren[4] = children[0];
            }
          }
          if (!isOverLimit(children[1], neighbor) || !isOverLimit(neighbor, children[1])) {
            if (children[1]->depth >= neighbor->depth)
              {
                children[1]->pChildren[i] = neighbor;
                if (children[1]->depth == neighbor->depth) neighbor->pChildren[4] = children[1];
              } else {
              // neighbor is smaller, so we need to point to its
              // ancestor of equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[1]->depth)) temp = temp->pParent;
              children[1]->pChildren[i] = temp;
              neighbor->pChildren[4] = children[1];
            }
          }
          if (!isOverLimit(children[4], neighbor) || !isOverLimit(neighbor, children[4])) {
            if (children[4]->depth >= neighbor->depth) {
              children[4]->pChildren[i] = neighbor;
              if (children[4]->depth == neighbor->depth) {
                neighbor->pChildren[4] = children[4];
              }
            } else {
              // neighbor is smaller, so we need to point to its ancestor of
              // equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[4]->depth)) temp = temp->pParent;
              children[4]->pChildren[i] = temp;
              neighbor->pChildren[4] = children[4];
            }
          }
          if (!isOverLimit(children[5], neighbor) || !isOverLimit(neighbor, children[5])) {
            if (children[5]->depth >= neighbor->depth) {
              children[5]->pChildren[i] = neighbor;
              if (children[5]->depth == neighbor->depth) {
                neighbor->pChildren[4] = children[5];
              }
            } else {
              // neighbor is smaller, so we need to point to
              // its ancestor of equal depth
              Box* temp = neighbor;
              while (!(temp->depth == children[5]->depth)) {
                temp = temp->pParent;
              }
              children[5]->pChildren[i] = temp;
              neighbor->pChildren[4] = children[5];
            }
          }
          neighbor = iter->Next();
        }
        break;
      default:
        std::cerr << "Only six neighbors" << std::endl;
        exit(1);
      }
    }

    for (int i = 0; i < 8; ++i) {
      this->pChildren[i] = children[i];
      this->pChildren[i]->pParent = this;
      // add all of parent's Walls, Edges, and corners to each child,
      // will be filtered later in updatestatus()
      this->pChildren[i]->Walls.insert(
                                       this->pChildren[i]->Walls.begin(),
                                       this->Walls.begin(),
                                       this->Walls.end());
      this->pChildren[i]->Edges.insert(
                                       this->pChildren[i]->Edges.begin(),
                                       this->Edges.begin(),
                                       this->Edges.end());
      this->pChildren[i]->corners.insert(
                                         this->pChildren[i]->corners.begin(),
                                         this->corners.begin(), this->corners.end() );
      this->pChildren[i]->spheres.insert(
                                         this->pChildren[i]->spheres.begin(),
                                         this->spheres.begin(), this->spheres.end() );
    }
    this->isLeaf = false;

    return true;
  }
};  // class Box
