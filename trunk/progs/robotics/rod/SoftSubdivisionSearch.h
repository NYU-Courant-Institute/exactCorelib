/* **************************************
   File: SoftSubdivisionSearch.h

   Description: The Main module of the SSS Framework

   HISTORY: July, 2013: Surin Ahn

   Since Core Library  Version 2.1
   $Id: $
   ************************************** */

#pragma once
#include <iostream>
#include "./Box.h"
#include "./UnionFind.h"
#include "./PriorityQueue.h"
#include "./Predicate.h"

using namespace std;

class SoftSubdivisionSearch {
 private:
  UnionFind* pSets;
  double epsilon;
  int QType;
  int ct;
  BoxQueue* PQ;
  vector<Box*> toReset;
  double alpha[3];
  double beta[3];
  Predicate* predicate;

  BoxQueue* getPriorityQueue();

  void insertNode(Box* b);

  bool expand(Box* b);

  void unionAdjacent(Box* b);

  Box* findEnclosingFreeBox(double coordinate[3]);

  vector<Box*> getCanonicalPath(Box* boxA, Box* boxB);

  // find path using simple heuristic:
  // use distance to beta as key in PQ, see dijkstraQueue
  void findPath(Box* boxA, Box* boxB);

 public:
  Box* pRoot;
  int freeCount;
  int stuckCount;
  int mixCount;
  int mixSmallCount;

  SoftSubdivisionSearch(Box* root, double e, int qType, unsigned int s, double alpha[3], double beta[3]);

  int getNumSubdivisions() {
    return ct;
  }

  vector<Box*> softSubdivisionSearch();

  ~SoftSubdivisionSearch(void) { }
};
