#include "./Box.h"

class Predicate {
 public:
  virtual void classify(Box* b) = 0;
};

class BoxPredicate : public Predicate {
 public:
  void classify(Box* b) {
    cout << Box::r0 << endl;
    if (b->status != Box::UNKNOWN) {
      return;
    }

    double outerDomain = Box::r0 + b->rB;
    double innerDomain = Box::r0 > b->rB ? Box::r0 - b->rB : 0;
    for (list<Corner*>::iterator it = b->corners.begin(); it != b->corners.end(); ) {
      Corner* c = *it;
      double distCorner = c->distance(b->x, b->y, b->z);
      if (distCorner <= innerDomain) {
        b->status = Box::STUCK;
        return;
      } else if (distCorner <= outerDomain ) {
        b->status = Box::MIXED;
        ++it;
      } else {
        it = b->corners.erase(it);
      }
    }

    for (list<Edge*>::iterator it = b->Edges.begin(); it != b->Edges.end(); ) {
      Edge* e = *it;
      double distEdge = e->distance(b->x, b->y, b->z);
      if (distEdge < innerDomain) {
        b->status = Box::STUCK;
        return;
      } else if (distEdge <= outerDomain) {
        b->status = Box::MIXED;
        ++it;
      } else {
        it = b->Edges.erase(it);
      }
    }

    for (list<Wall*>::iterator it = b->Walls.begin(); it != b->Walls.end(); ) {
      Wall* w = *it;
      double distWall = w->distance(b->x, b->y, b->z);
      if (distWall < innerDomain) {
        b->status = Box::STUCK;
        return;
      }
      else if (distWall <= outerDomain) {
        b->status = Box::MIXED;
        ++it;
      }
      else {
        it = b->Walls.erase(it);
      }
    }

    for (list<Sphere*>::iterator it = b->spheres.begin(); it != b->spheres.end(); ) {
      Sphere* s = *it;
      double distSphere = s->distance(b->x, b->y, b->z);
      if (distSphere < innerDomain) {
        b->status = Box::STUCK;
        return;
      } else if (distSphere <= outerDomain) {
        b->status = Box::MIXED;
        ++it;
      } else {
        it = b->spheres.erase(it);
      }
    }

    if (b->corners.empty() && b->Edges.empty() && b->Walls.empty() && b->spheres.empty()) {
      if (!b->pParent) {
        b->status = Box::FREE;
      } else {
        b->status = checkChildStatus(b->pParent, b->x, b->y, b->z);
      }
    }

  }

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
