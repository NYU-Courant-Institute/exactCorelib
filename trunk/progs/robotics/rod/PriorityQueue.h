#pragma once
#include "Box.h"
#include <queue>
#include <vector>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include "Graph.h"
#include <math.h>
#include <unordered_set>
#include "UnionFind.h"

using namespace std;

class PQCmp {
 public:
  bool operator() (const Box* a, const Box* b) {
    //use depth for now
    if (a->depth > b->depth) {
      return true;
    }
    //if same depth, expand box created earlier first
    if (a->depth == b->depth) {
      return a->priority > b->priority;
    }
    return false;
  }
};


class BoxQueue {
 public:

  BoxQueue(void) { }

  virtual void push(Box* b) = 0;

  virtual Box* extract() = 0;

  virtual bool empty() = 0;

  virtual int size() = 0;

  virtual void clear() = 0;

  virtual void process(Box* b) {
    if (b->getStatus() == Box::MIXED) {
      push(b);
    }
  }

  ~BoxQueue(void) { }
};

class SeqQueue : public BoxQueue {
 private:
  priority_queue<Box*, vector<Box*>, PQCmp> PQ;
 public:
  void push(Box* b) {
    PQ.push(b);
  }

  Box* extract() {
    Box* r = PQ.top();
    PQ.pop();
    return r;
  }

  bool empty() {
    return PQ.empty();
  }

  int size() {
    return PQ.size();
  }

  void clear() {
    PQ = priority_queue<Box*, vector<Box*>, PQCmp>();
  }
};

class RandQueue : public BoxQueue
{
 private:
  list<Box*> L;
  unsigned int Qseed;

 public:
 RandQueue(unsigned int s): Qseed(s) {
   srand( Qseed );
 }

  void push(Box* b) {
    L.push_back(b);
  }

  Box* extract() {
    unsigned long i = static_cast<unsigned long>(rand()) % L.size();
    list<Box*>::iterator iter = L.begin();
    advance(iter, i);
    Box* r = *iter;
    L.erase(iter);
    return r;
  }

  bool empty() {
    return L.empty();
  }

  int size() {
    return L.size();
  }

  void clear() {
    L.clear();
  }

};

class DijkstraQueue : public BoxQueue
{
 private:
  vector<Box*> bv;
  double alpha[3];
  double beta[3];
  unordered_set<Box*> sourceSet;
  unordered_set<Box*> fringe;

  bool hasNeighborInSourceSet(Box* b) {
    for (int j = 0; j < 6; ++j) {
      BoxIter* iter = new BoxIter(b, j);
      Box* n = iter->First();
      while (n && n != iter->End()) {
        if (sourceSet.find(n) != sourceSet.end()) {
          /* b->prev = n; */
          return true;
        }
        n = iter->Next();
      }
    }
    return false;
  }

  void recursiveAddToSourceSet(Box* b) {
    sourceSet.insert(b);
    for (int j = 0; j < 6; ++j) {
      BoxIter* iter = new BoxIter(b, j);
      Box* n = iter->First();
      while (n && n != iter->End()) {
        if (n->getStatus() == Box::FREE) {
          if (sourceSet.find(n) == sourceSet.end()) {
            /* n->prev = b; */
            recursiveAddToSourceSet(n);
          }
        } else if (n->getStatus() == Box::MIXED) {
          if (fringe.find(n) == fringe.end()) {
            fringe.insert(n);
            push(n);
          }
        }
        n = iter->Next();
      }
    }
  }

 public:

  DijkstraQueue(double alpha[3], double beta[3]) {
    this->alpha[0] = alpha[0];
    this->alpha[1] = alpha[1];
    this->alpha[2] = alpha[2];

    this->beta[0] = beta[0];
    this->beta[1] = beta[1];
    this->beta[2] = beta[2];
  }


  void push(Box* b) {
    distHeap<PQCmp3>::insert(bv, b);
  }

  Box* extract() {
    Box* b = distHeap<PQCmp3>::extractMin(bv);
    return b;
  }

  bool empty() {
    return bv.empty();
  }

  int size() {
    return bv.size();
  }

  void clear() {
    bv.clear();
  }

  void process(Box* b) {
    if (b->getStatus() == Box::FREE) {
      if ((sourceSet.empty() && b->getBox(alpha[0], alpha[1], alpha[2]) == b) ||
          hasNeighborInSourceSet(b)) {
        recursiveAddToSourceSet(b);
      }
    }
  }
};
