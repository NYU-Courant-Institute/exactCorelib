#include "vor2d.h"
#include "vor_qt.h"
#include "vor_box.h"
#include "Corner.h"
#include "Edge.h"
#include "Object.h"
#include "Point.h"

using vor2d::vor_box;
using vor2d::vor_qt;
using vor2d::Corner;
using vor2d::Edge;
using vor2d::Feature;
using vor2d::Object;

void draw_box(const vor_box& box) {
  double cx = box.center()[0];
  double cy = box.center()[1];
  double w = box.width();

  glColor3f(0.7, 0.7, 0.7); // Gray.
  glLineWidth(1.0);
  glBegin(GL_LINE_LOOP);
  glVertex2d(cx - w, cy - w);
  glVertex2d(cx - w, cy + w);
  glVertex2d(cx + w, cy + w);
  glVertex2d(cx + w, cy - w);
  glEnd();
}

void draw_box_rec(const vor_box& box) {
  draw_box(box);
  if (!box.is_leaf()) {
    vor_box** children = box.children();
    for (int i = 0; i < box.num_children(); i++) {
      draw_box_rec(*children[i]);
    }
  }
}

void draw_edge(const Edge& edge) {
  const Point2d s_pos = edge.source()->position();
  const Point2d d_pos = edge.dest()->position();
  glColor3f(0.0, 0.0, 0.0); // Black.
  glLineWidth(3.0);
  glBegin(GL_LINES);
  glVertex2d(s_pos[0], s_pos[1]);
  glVertex2d(d_pos[0], d_pos[1]);
  glEnd();
}

void draw_corner(const Corner& corner) {
  const Point2d point = corner.position();
  glColor3f(0.0, 0.0, 0.0); // Black.
  glPointSize(7.0);
  glBegin(GL_POINTS);
  glVertex2d(point[0], point[1]);
  glEnd();
}
