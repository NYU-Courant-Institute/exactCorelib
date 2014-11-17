#include "./SoftSubdivisionSearch.h"

SoftSubdivisionSearch::SoftSubdivisionSearch(Box* root, double e, int qType, unsigned int s):
  epsilon(e), QType(qType), ct(0), pRoot(root),
  freeCount(0), stuckCount(0), mixCount(0), mixSmallCount(0) {
  switch (QType) {
  case 1:
    PQ = new SeqQueue();
    break;
  case 0:
    PQ = new RandQueue(s);
    break;
  case 2:
    PQ = new DijkstraQueue();
    break;
  default:
    std::cerr << "Wrong QType" << std::endl;
    exit(1);
  }

  pRoot->updateStatus();
  insertNode(pRoot);
}

void SoftSubdivisionSearch::insertNode(Box* b) {
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
    if (b->width < epsilon) {
      ++mixSmallCount;
    }
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

bool SoftSubdivisionSearch::expand(Box* b) {
  if (b->split(epsilon)) {
    for (int i = 0; i < 8; ++i) {
      b->pChildren[i]->updateStatus();
      insertNode(b->pChildren[i]);
    }
    return true;
  }
  return false;
}

void SoftSubdivisionSearch::findPath(Box* boxA, Box* boxB) {
  toReset.clear();
  boxA->dist2Source = 0;
  BoxQueue* PQ = new DijkstraQueue();
  PQ->push(boxA);
  toReset.push_back(boxA);
  while (!PQ->empty()) {
    Box* current = PQ->extract();
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
              PQ->push(current->pChildren[i]);
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
      if (current == boxB && boxB->dist2Source == 0) {
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
            PQ->push(neighbor);
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
}

void SoftSubdivisionSearch::unionAdjacent(Box* b) {
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

Box* SoftSubdivisionSearch::findEnclosingFreeBox(double coordinate[3]) {
  Box* box = pRoot->getBox(coordinate[0], coordinate[1], coordinate[2]);
  while (box && !(box)->isFree()) {
    if (!expand(box)) {
      return NULL; // Does not have a free box for the given resolution
    }
    ++ct;
    box = box->getBox(coordinate[0], coordinate[1], coordinate[2]);
  }
  return box;
}

vector<Box*> SoftSubdivisionSearch::getCanonicalPath(Box* boxA, Box* boxB) {
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

// The SSS Framework as described in the RSS 2013 RCV Paper, section 7
vector<Box*> SoftSubdivisionSearch::softSubdivisionSearch(double alpha[3], double beta[3]) {
  vector<Box*> path;
  path.clear();

  // 1. Initialization
  Box* boxA = findEnclosingFreeBox(alpha);
  Box* boxB = findEnclosingFreeBox(beta);
  if (boxA == NULL || boxB == NULL) {
    return path;
  }

  // 2. Main
  if (QType == 0 || QType == 1) {
    while (!pSets->isConnect(boxA, boxB)) {
      if (PQ->empty()) {
        path.clear();
        return path;
      }
      Box* b = PQ->extract();
      // b might not be a leaf since it could already be split in
      // expand(Box* b), and PQ is not updated there
      if (b->isLeaf) {
        expand(b);
      }
      ++ct;
    }
  } else if (QType == 2) {
    findPath(boxA, boxB);
  }

  // 3. Compute Free Channel
  path = getCanonicalPath(boxA, boxB);
  return path;
}
