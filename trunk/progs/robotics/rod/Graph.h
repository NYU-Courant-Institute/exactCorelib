#pragma once

#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include "./Box.h"

using namespace std;

extern double beta[3];

class distCmp {
 public:
  bool operator()(Box* a, Box* b) {
    return a->dist2Source > b->dist2Source;
  }
};


// min heap
template <typename CmpFunctor>
class distHeap {
 private:
  static void siftDown(vector<Box*>& bv, unsigned long i) {
    CmpFunctor cmp;
    unsigned int l = 2 * i + 1;
    unsigned int r = 2 * i + 2;
    unsigned long smallest = i;
    if (l < bv.size() && cmp(bv[i], bv[l])) {
      smallest = l;
    }
    if (r < bv.size() && cmp(bv[smallest], bv[r])) {
      smallest = r;
    }
    if (smallest != i) {
      Box* tmp = bv[smallest];
      bv[smallest] = bv[i];
      bv[i] = tmp;
      bv[smallest]->heapId = smallest;
      bv[i]->heapId = i;

      siftDown(bv, smallest);
    }
  }

 public:
  static void makeHeap(vector<Box*>& bv) {
    if (bv.size() <= 1) {
      return;
    }
    for (int i = 0; i < bv.size(); ++i) {
      bv[i]->heapId = i;
    }
    for (int i = (bv.size() -2) / 2; i >= 0; --i) {
      siftDown(bv, i);
    }
  }

  static void insert(vector<Box*>& bv, Box* b) {
    bv.push_back(b);
    int bid = bv.size() - 1;
    b->heapId = bid;
    decreaseKey(bv, b, b->dist2Source);
  }

  static void decreaseKey(vector<Box*>& bv, Box* b, double dist) {
    CmpFunctor cmp;
    assert(b->heapId >= 0);
    unsigned long bid = static_cast<unsigned long>(b->heapId);
    unsigned long pid = (bid - 1) / 2;
    assert(bv[bid] == b);
    assert(b->dist2Source >= dist);

    b->dist2Source = dist;
    while (bid > 0 && cmp(bv[pid], bv[bid])) {
      Box* tmp = bv[bid];
      bv[bid] = bv[pid];
      bv[pid] = tmp;
      bv[bid]->heapId = bid;
      bv[pid]->heapId = pid;

      bid = pid;
      pid = (bid - 1) / 2;
    }
  }

  static Box* extractMin(vector<Box*>& bv) {
    Box* minB = bv[0];
    bv[0] = bv.back();
    bv[0]->heapId = 0;
    minB->heapId = -1;
    bv.pop_back();
    siftDown(bv, 0);
    return minB;
  }
};

// won't work with std pq, as this comparison is not transitional!
class PQCmp3 {
 public:
  bool operator() (const Box* a, const Box* b) {
    // if depth diff bigger than 3, use depth as priority
    // if (abs(a->depth - b->depth) > 8)
    // {
    //   return a->depth > b->depth;
    // }
    // otherwise expand box closer to beta
    double distDiff =
      (a->x - beta[0]) * (a->x - beta[0]) +
      (a->y - beta[1]) * (a->y - beta[1]) +
      (a->z - beta[2]) * (a->z - beta[2]) -
      ((b->x - beta[0]) * (b->x - beta[0]) +
       (b->y - beta[1]) * (b->y - beta[1]) +
       (b->z - beta[2]) * (b->z - beta[2]));
    return distDiff > 0;
  }
};

class Path {
 public:
  static bool isNeighbor(Box* a, Box* b) {
    double dx = abs(a->x - b->x);
    double dy = abs(a->y - b->y);
    double dz = abs(a->z - b->z);
    double wa = a->width / 2;
    double wb = b->width / 2;
    return
      (abs(dx - (wa + wb)) < 0.001 && abs(dy - abs(wa - wb)) < 0.001 && abs(dz - abs(wa - wb)) < 0.001) ||
      (abs(dx - (wa - wb)) < 0.001 && abs(dy - abs(wa + wb)) < 0.001 && abs(dz - abs(wa - wb)) < 0.001) ||
      (abs(dx - (wa - wb)) < 0.001 && abs(dy - abs(wa - wb)) < 0.001 && abs(dz - abs(wa + wb)) < 0.001);
  }

  static vector<Box*> bfsShortestPath(Box* a, Box* b) {
    unordered_set<Box*> visited;
    vector<Box*> fringe;
    fringe.push_back(a);
    visited.insert(a);
    cout << a->boxId <<"\t" << a->x << "\t" << a->y << "\t" << a->z << "\t" << a->width << endl;
    int begin = 0;
    while (begin < fringe.size()) {
      Box* c = fringe[begin];
      begin++;
      /* for (int j = 0; j < Box::boxes.size(); j++) { */
        /* Box* n = Box::boxes[j]; */
        /* if (n->status == Box::FREE && n->isLeaf && visited.find(n) == visited.end() && isNeighbor(n, c)) { */
          /* cout << n->boxId <<"\t" << n->x << "\t" << n->y << "\t" << n->z << "\t" << n->width << "\t" << "prev: " << c->boxId << endl; */
          /* n->prev = c; */
          /* fringe.push_back(n); */
          /* visited.insert(n); */
        /* } */
      for (int j = 0; j < 6; ++j) {
        BoxIter* iter = new BoxIter(c, j);
        Box* n = iter->First();
        while (n && n != iter->End()) {
          if (c->boxId == 162) {
            cout << "162: " << j << " " << n->boxId <<"\t" << n->x << "\t" << n->y << "\t" << n->z << "\t" << n->width << endl;
          }
          if (n->getStatus() == Box::FREE && visited.find(n) == visited.end()) {
            n->prev = c;
          cout << n->boxId <<"\t" << n->x << "\t" << n->y << "\t" << n->z << "\t" << n->width << "\t" << "prev: " << c->boxId << endl;
            fringe.push_back(n);
            visited.insert(n);
          }
          n = iter->Next();
        }
      }
    }

    vector<Box*> path;
    cout << b->boxId <<"\t" << b->x << "\t" << b->y << "\t" << b->z << "\t" << b->width << endl;
    path.push_back(b);
    while (path.back()->prev) {
      path.push_back(path.back()->prev);
    }
    /* return path.size() == 1 ? fringe : path; */
    return path;
  }

  static vector<Box*> dijkstraShortestPath(Box* a, Box* b) {
    a->dist2Source = 0;
    vector<Box*> bv;
    distHeap<distCmp>::insert(bv, a);
    while (bv.size()) {
      Box* current = distHeap<distCmp>::extractMin(bv);
      current->visited = true;
      if (current == b) {
        break;
      }

      for (int i = 0; i < 6; ++i) {
        BoxIter* iter = new BoxIter(current, i);
        Box* neighbor = iter->First();

        while (neighbor && neighbor != iter->End()) {
          if (!neighbor->visited && neighbor->status == Box::FREE) {
            double dist2pre =
              sqrt((current->x - neighbor->x) * (current->x - neighbor->x) +
                   (current->y - neighbor->y) * (current->y - neighbor->y) +
                   (current->z - neighbor->z) * (current->z - neighbor->z) );
            double dist2src = dist2pre + current->dist2Source;

            if (neighbor->dist2Source == -1) {
              neighbor->prev = current;
              neighbor->dist2Source = dist2src;
              distHeap<distCmp>::insert(bv, neighbor);
            } else if (neighbor->dist2Source > dist2src) {
              neighbor->prev = current;
              distHeap<distCmp>::decreaseKey(bv, neighbor, dist2src);
            }
          }
          neighbor = iter->Next();
        }
      }
    }

    vector<Box*> path;
    path.push_back(b);
    while (path.back()->prev) {
      path.push_back(path.back()->prev);
    }
    return path;
  }
};
