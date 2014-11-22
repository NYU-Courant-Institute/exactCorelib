#include "./SoftSubdivisionSearch.h"

// TODO: Let the constructor accept alpha and beta as configurations of the robot
SoftSubdivisionSearch::SoftSubdivisionSearch(Box* root, double e, int qType, unsigned int s, double alpha[3], double beta[3]):
  epsilon(e), QType(qType), ct(0), pRoot(root),
  freeCount(0), stuckCount(0), mixCount(0), mixSmallCount(0) {
  this->alpha[0] = alpha[0];
  this->alpha[1] = alpha[1];
  this->alpha[2] = alpha[2];

  this->beta[0] = beta[0];
  this->beta[1] = beta[1];
  this->beta[2] = beta[2];
  switch (QType) {
  case 1:
    PQ = new SeqQueue();
    break;
  case 0:
    PQ = new RandQueue(s);
    break;
  case 2:
    PQ = new DijkstraQueue(alpha, beta);
    break;
  default:
    std::cerr << "Wrong QType" << std::endl;
    exit(1);
  }
  predicate = new BoxPredicate();

  predicate->classify(pRoot);
  // pRoot->updateStatus();
  insertNode(pRoot);
}

void SoftSubdivisionSearch::insertNode(Box* b) {
  PQ->process(b);
  switch (b->getStatus()) {
  case Box::FREE:
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
      predicate->classify(b->pChildren[i]);
      // b->pChildren[i]->updateStatus();
      insertNode(b->pChildren[i]);
    }
    return true;
  }
  return false;
}

void SoftSubdivisionSearch::unionAdjacent(Box* b) {
  if (b->getStatus() != Box::FREE) {
    cerr << "Cannot union boxes that are mixed or stuck" << endl;
    exit(1);
  }
  if (b->pSet == 0) {
    new Set(b);
  }
  for (int i = 0; i < 6; ++i) {
    BoxIter* iter = new BoxIter(b, i);
    Box* neighbor = iter->First();
    while (neighbor && neighbor != iter->End()) {
      if (neighbor->status == Box::FREE) {
        if (neighbor->pSet == 0) {
          new Set(neighbor);
        }
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

// The SSS Framework as described in the RSS 2013 RCV Paper, section 7
vector<Box*> SoftSubdivisionSearch::softSubdivisionSearch() {
  Box::boxes.clear();
  Box::boxIdCounter = 0;
  vector<Box*> path;
  path.clear();

  // 1. Initialization
  Box* boxA = findEnclosingFreeBox(alpha);
  Box* boxB = findEnclosingFreeBox(beta);
  if (boxA == NULL || boxB == NULL) {
    return path;
  }

  // 2. Main
  while (!pSets->isConnect(boxA, boxB)) {
    if (PQ->empty()) {
      path.clear();
      return path;
    }
    Box* b = PQ->extract();
    // b might not be a leaf since it could already be split in
    // expand(Box* b), and PQ is not updated there
    if (b->isLeaf && b->getStatus() == Box::MIXED) {
      expand(b);
    }
    ++ct;
  }

  // 3. Compute Free Channel
  // path = getCanonicalPath(boxA, boxB);
  path = Path::bfsShortestPath(boxA, boxB);
  for (int i = 0; i < Box::boxes.size(); i++) {
    Box* b = Box::boxes[i];
    if (b->getStatus() == Box::FREE) {
      cout << b->boxId << "\t" << b->x << "\t" << b->y << "\t" << b->z << "\t" << b->width << endl;
    }
  }
  return path;
}
