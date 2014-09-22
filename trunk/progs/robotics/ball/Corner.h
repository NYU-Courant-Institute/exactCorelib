#pragma once
#include "./Vector.h"
#include <math.h>
class Edge;

class Corner : public Vector {
 public:
  Edge* preEdge;
  Edge* nextEdge;
  // Constructor from (x,y,z):
  Corner(double xx, double yy, double zz) : Vector(xx, yy, zz) { }
  Corner(const Corner& c) : Vector(c) { }

  Corner operator+(const Corner& other) {
    return Corner(x + other.x, y + other.y, z + other.z);
  }
  Corner operator-(const Corner& other) {
    return Corner(x - other.x, y - other.y, z - other.z);
  }

  double distance(double x2, double y2, double z2) {
    return
      sqrt((x - x2) * (x - x2) +
           (y - y2) * (y - y2) +
           (z - z2) * (z - z2) );
  }
};
