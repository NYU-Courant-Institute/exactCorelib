#pragma once
#include "./ConfBox3d.h"
#include "./Status.h"

class ConfBox3d;

class ConfBox3dPredicate {
 public:
  void classify(ConfBox3d* b);
  Status checkChildStatus(ConfBox3d* b, double x, double y, double z);

};
