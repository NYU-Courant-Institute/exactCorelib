#include "./ConfBox3d.h"

void ConfBox3dPredicate::classify(ConfBox3d* b) {
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
