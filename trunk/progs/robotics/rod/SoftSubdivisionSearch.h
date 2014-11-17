/* **************************************
   File: SoftSubdivisionSearch.h

   Description:

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once
#include <iostream>
#include "./Box.h"
#include "./UnionFind.h"
#include "./PriorityQueue.h"

extern int freeCount;
extern int stuckCount;
extern int mixCount;
extern int mixSmallCount;

using namespace std;

class SoftSubdivisionSearch {
 private:
  void insertNode(Box* b) {
    switch (b->getStatus()) {
    case Box::FREE:
      new Set(b);
      unionAdjacent(b);
      ++freeCount;
      break;
    case Box::STUCK:
      ++stuckCount;
      break;
    case Box::MIXED:
      ++mixCount;
      if (b->width < epsilon)
        ++mixSmallCount;
      PQ->push(b);
      break;
    case Box::UNKNOWN:
      std::cout << "UNKNOWN not handled" << std::endl;
      break;
    default:
      std::cerr << "Wrong Status" << std::endl;
      exit(1);
    }
  }

 public:
  UnionFind* pSets;
  BoxQueue* PQ;
  Box* pRoot;
  double epsilon;
  int QType;

  SoftSubdivisionSearch(Box* root, double e, int qType, unsigned int s):
  pRoot(root), epsilon(e), QType(qType) {
    switch (QType) {
    case 1:
      PQ = new seqQueue();
      break;
    case 0:
      PQ = new randQueue(s);
      break;
    case 2:
      PQ = new dijkstraQueue();
      break;
    default:
      std::cerr << "Wrong QType" << std::endl;
      exit(1);
    }

    pRoot->updateStatus();
    insertNode(pRoot);
  }

  Box* getBox(Box* root, double x, double y, double z) {
    if (x > root->x + root->width / 2 || x < root->x - root->width / 2
        || y > root->y + root->width / 2 || y < root->y - root->width / 2
        || z > root->z + root->width / 2 || z < root->z - root->width / 2) {
      return 0;
    }

    Box* b = root;
    while (!b->isLeaf) {
      double dx = x - b->x;
      double dy = y - b->y;
      double dz = z - b->z;

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

  Box* getBox(double x, double y, double z) {
    return getBox(pRoot, x, y, z);
  }

  bool expand(Box* b) {
    if (!b->split(epsilon)) {
      return false;
    }
    for (int i = 0; i < 8; ++i) {
      b->pChildren[i]->updateStatus();
      insertNode(b->pChildren[i]);
    }

    return true;
  }

  bool expand() {
    while (!PQ->empty()) {
      Box* b = PQ->extract();
      // b might not be a leaf since it could already be split in
      // expand(Box* b), and PQ is not updated there
      if (b->isLeaf && b->split(epsilon)) {
        assert(b->status == Box::MIXED);
        for (int i = 0; i < 8; ++i) {
          b->pChildren[i]->updateStatus();
          insertNode(b->pChildren[i]);
        }
        return true;
      }
    }
    return false;
  }

  void unionAdjacent(Box* b) {
    for (int i = 0; i < 6; ++i) {
      BoxIter* iter = new BoxIter(b, i);
      Box* neighbor = iter->First();
      while (neighbor && neighbor != iter->End()) {
        if (neighbor->status == Box::FREE) {
          pSets->Union(b, neighbor);
        }
        neighbor = iter->Next();
      }
    }
  }

  Box* findEnclosingFreeBox(double coordinate[3], int& expandCounter) {
    Box* box = getBox(coordinate[0], coordinate[1], coordinate[2]);
    while (box && !(box)->isFree()) {
      if (!expand(box)) {
        return NULL; // Does not have a free box for the given resolution
      }
      ++expandCounter;
      box = getBox(box, coordinate[0], coordinate[1], coordinate[2]);
    }
    return box;
  }

  vector<Box*> optimizePath(Box* boxA, Box* boxB, vector<Box*> toReset) {
    boxA->prev = NULL;
    vector<Box*> path;
    path.clear();
    path.push_back(boxB);
    while (path.back()->prev) {
      path.push_back(path.back()->prev);
    }
    for (unsigned int i = 0; i < toReset.size(); ++i) {
      toReset[i]->prev = NULL;
    }
    vector<Box*> dijkstraShortestPath = Graph::dijkstraShortestPath(boxA, boxB);
    if (dijkstraShortestPath.back() == boxA) {
      path = dijkstraShortestPath;
    } else {
      cerr << "Something went wrong in the dijkstra path generation algorithm, defaulting to subdivision generated path" << endl;
    }
    return path;
  }

  vector<Box*> toReset;

  // find path using simple heuristic:
  // use distance to beta as key in PQ, see dijkstraQueue
  bool findPath(Box* a, Box* b, int& ct) {
    bool isPath = false;
    toReset.clear();
    a->dist2Source = 0;
    dijkstraQueue PQ;
    PQ.push(a);
    toReset.push_back(a);
    while (!PQ.empty()) {
      Box* current = PQ.extract();
      current->visited = true;

      // if current is MIXED, try expand it and push the children that is
      // ACTUALLY neighbors of the source set (set containing alpha) into the
      // PQ again
      if (current->status == Box::MIXED) {
        if (expand(current)) {
          ++ct;
          for (int i = 0; i < 8; ++i) {
            // go through neighbors of each child to see if it's in source set
            // if yes, this child go into the PQ
            bool isNeighborOfSourceSet = false;
            Box * prev = NULL;
            for (int j = 0; j < 6 && !isNeighborOfSourceSet; ++j) {
              BoxIter* iter = new BoxIter(current->pChildren[i], j);
              Box* n = iter->First();
              while (n && n != iter->End()) {
                if (n->dist2Source == 0) {
                  isNeighborOfSourceSet = true;
                  prev = n;
                  break;
                }
                n = iter->Next();
              }
            }
            if (isNeighborOfSourceSet) {
              switch (current->pChildren[i]->getStatus()) {
                //if it's FREE, also insert to source set
              case Box::FREE:
                current->pChildren[i]->dist2Source = 0;
                current->pChildren[i]->prev = prev;
                // fallthrough
              case Box::MIXED:
                PQ.push(current->pChildren[i]);
                toReset.push_back(current->pChildren[i]);
                break;
              case Box::STUCK:
                cerr << "inside FindPath: STUCK case not treated" << endl;
                break;
              case Box::UNKNOWN:
                cerr << "inside FindPath: UNKNOWN case not treated" << endl;
                break;
              default:
                std::cerr << "Wrong Status" << std::endl;
                exit(1);
              }
            }
          }
        }
      } else {
        // if current is not MIXED, then must be FREE
        // go through its neighbors and add FREE and MIXED ones to PQ
        // also add FREE ones to source set
        //found path!
        if (current == b && b->dist2Source == 0) {
          isPath = true;
          break;
        }
        for (int i = 0; i < 6; ++i) {
          BoxIter* iter = new BoxIter(current, i);
          Box* neighbor = iter->First();
          while (neighbor && neighbor != iter->End()) {
            if (!neighbor->visited && neighbor->dist2Source == -1 &&
                (neighbor->status == Box::FREE || neighbor->status == Box::MIXED)) {
              if (neighbor->status == Box::FREE) {
                neighbor->dist2Source = 0;
                neighbor->prev = current;
              }
              PQ.push(neighbor);
              toReset.push_back(neighbor);
            }
            neighbor = iter->Next();
          }
        }
      }
    }

    // these two fields are also used in dijkstraShortestPath
    // need to reset
    for (unsigned int i = 0; i < toReset.size(); ++i) {
      toReset[i]->visited = false;
      toReset[i]->dist2Source = -1;
    }

    return isPath;
  }

  vector<Box*> softSubdivisionSearch(double alpha[3], double beta[3], int& ct) {
    vector<Box*> path;
    path.clear();
    Box* boxA = findEnclosingFreeBox(alpha, ct);
    Box* boxB = findEnclosingFreeBox(beta, ct);
    if (boxA == NULL || boxB == NULL) {
      return path;
    }
    bool noPath = false;
    if (QType == 0 || QType == 1) {
      while (!noPath && !pSets->isConnect(boxA, boxB)) {
        if (!expand()) {
          noPath = true;
        }
        ++ct;
      }
    } else if (QType == 2) {
      noPath = !findPath(boxA, boxB, ct);
    }
    path = optimizePath(boxA, boxB, toReset);
    return path;
  }
  ~SoftSubdivisionSearch(void) { }
};
