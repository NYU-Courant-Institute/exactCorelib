#pragma once
#include <math.h>

// SSE3 is two 3-vectors, the second vector represents rotation, with all of its components between -1 and +1, and one of the component need to be +1 or -1
class SSE3 {
 public:
  Vector translation;
  Vector rotation;
  // Constructor from (xx, yy, zz):

  SSE3(double xx, double yy, double zz, double aa = 0, double bb = 0, double cc = 1) {
    translation = Vector(xx, yy, zz);
    rotation = Vector(aa, bb, cc);
  }

  SSE3(const Vector& trans, const Vector& rot = Vector(0, 0, 1)) {
    translation = trans;
    rotation = rot;
  }

  bool split(double epsilon) {
    return false;
  }

  /* Vector operator+(const Vector& other) { */
    /* return Vector(x + other.x, y + other.y, z + other.z); */
  /* } */
  /* Vector operator-(const Vector& other) { */
    /* return Vector(x - other.x, y - other.y, z - other.z); */
  /* } */
  /* double operator*(const Vector& other) { */
    /* return x * other.x + y * other.y + z * other.z; */
  /* } */
  /* Vector operator*(const double alpha) { */
    /* return Vector(alpha * x, alpha * y, alpha * z); */
  /* } */

  /* double distance(double x2, double y2, double z2) { */
    /* return */
      /* sqrt((x - x2) * (x - x2) + */
           /* (y - y2) * (y - y2) + */
           /* (z - z2) * (z - z2) ); */
  /* } */
  /* double distance(Vector other) { */
    /* return distance(other.x, other.y, other.z); */
  /* } */
};
