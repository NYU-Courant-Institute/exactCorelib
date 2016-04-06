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

// TODO: Convert magic numbers into global parameters.

void draw_vor_seg(const vor_seg& seg) {
  glColor3f(1.0, 0.0, 0.0); // Red.
  glLineWidth(3.0);
  glBegin(GL_LINES);
  glVertex2d(seg.p_[0], seg.p_[1]);
  glVertex2d(seg.q_[0], seg.q_[1]);
  glEnd();
}

void draw_box(const vor_box& box, bool show_grid) {
  double cx = box.center()[0];
  double cy = box.center()[1];
  double hw = box.width() / 2;

  // Draw mesh boundaries.
  if (show_grid) {
    glColor3f(0.7, 0.7, 0.7); // Gray
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2d(cx - hw, cy - hw);
    glVertex2d(cx - hw, cy + hw);
    glVertex2d(cx + hw, cy + hw);
    glVertex2d(cx + hw, cy - hw);
    glEnd();
  }

  if (box.is_degen()) {
    glColor3f(0, 0, 1.0); // Blue
    glBegin(GL_POLYGON);
    glVertex2d(cx - hw, cy - hw);
    glVertex2d(cx - hw, cy + hw);
    glVertex2d(cx + hw, cy + hw);
    glVertex2d(cx + hw, cy - hw);
    glEnd();
  }

  // Display Voronoi segments.
  for (vor_seg* seg : *box.get_segments()) {
    draw_vor_seg(*seg);
  }
}

void draw_box_rec(const vor_box& box, bool show_grid) {
  draw_box(box, show_grid);
  if (!box.is_leaf()) {
    vor_box** children = box.children();
    for (int i = 0; i < box.num_children(); i++) {
      draw_box_rec(*children[i], show_grid);
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
