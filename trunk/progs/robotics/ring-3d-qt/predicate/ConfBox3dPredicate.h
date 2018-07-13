#pragma once

#include "subdivision/ConfBox3d.h"
#include "subdivision/Status.h"

class ConfBox3d;

class ConfBox3dPredicate {
 public:
  Status classification(ConfBox3d* b, Circle3d cir);

  void checkCollisionDetectionFeatureSet(ConfBox3d* b);
  void checkVoronoiFeatureSet(ConfBox3d *b);

  double findClearance(ConfBox3d *b, Circle3d cir);
  void findClearance2(ConfBox3d *b, Circle3d cir);
};
