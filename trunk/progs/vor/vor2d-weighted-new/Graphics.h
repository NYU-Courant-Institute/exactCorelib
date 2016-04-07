#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "vor2d.h"
#include "vor_qt.h"
#include "vor_box.h"
#include "Corner.h"
#include "Edge.h"
#include "Object.h"
#include "Point.h"

using vor2d::vor_box;
using vor2d::vor_qt;
using vor2d::vor_seg;
using vor2d::Corner;
using vor2d::Edge;
using vor2d::Feature;
using vor2d::Object;

class Graphics {
public:
  Graphics(const Point2d& ctr, double scale);
  void draw_vor_seg(const vor_seg& seg);
  void draw_box(const vor_box& box, bool show_grid);
  void draw_box_rec(const vor_box& box, bool show_grid);
  void draw_edge(const Edge& edge);
  void draw_corner(const Corner& corner);
  void update(const Point2d& ctr, double scale);
  void update(double scale);
  double mxi(double x);
  double myi(double y);
  double mx(double x);
  double my(double y);

private:
  Point2d ctr_;
  double scale_;
};

#endif // GRAPHICS_H
