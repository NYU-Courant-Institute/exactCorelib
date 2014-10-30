#include "vor_qt.h"
#include "vor_box.h"
#include "vor2d.h"
#include "Corner.h"
#include "Edge.h"
#include "Graphics.h"
#include "Object.h"
#include "Point.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <boost/algorithm/string.hpp>

#ifdef __CYGWIN32__
#include "glui.h"
#endif
#ifdef _WIN32
#include <gl/glui.h>
#endif
#ifdef __APPLE__
#include "glui.h"
#endif
#ifdef __linux__
#include <GL/glui.h>
#endif

using namespace std;

using vor2d::vor_box;
using vor2d::vor_qt;
using vor2d::Corner;
using vor2d::Edge;
using vor2d::Feature;
using vor2d::Object;

void parse();
void run();

// Global variables.
vor_qt* tree;
queue<vor_box*> unprocessed;

void initialize() {
  // Initialize global variables.
  tree = new vor_qt(2 /* dimension */, 1.0 /* width */);

  // Set up antialiasing.
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  // Set up window.
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("");
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void cleanup() {
  delete tree;
}

void display () {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_box_rec(*tree->root());
  parse();
  glutSwapBuffers();
}

int main(int argc, char* argv[]) {
  // Initialize GUI and global variables.
  glutInit(&argc, argv);
  initialize();

  glutDisplayFunc(display);
  glutMainLoop();
  
  run();
  cleanup();
}

string get_line(ifstream& ifs) {
  string s = "";

  do {
    if (ifs.eof()) {
      cout << "End of file encountered.\n";
      exit(1);
    }
    std::getline(ifs, s);
    boost::algorithm::trim(s);
  } while (s.length() == 0 || s[0] == '#'); // Skip comment lines.

  return s;
}

void parse() {
  int num_points;
  int point_count;
  int num_objects;
  int vert;
  int x, y;
  ifstream ifs("test_input", std::ifstream::in);

  cout << "Parse\n";

  // 1. Parse points.
  stringstream ss(get_line(ifs));
  ss.seekg(0);
  ss >> num_points;
  vector<double> px(num_points);
  vector<double> py(num_points);
  for (int i = 0; i < num_points; i++) {
    stringstream ss(get_line(ifs));
    ss.seekg(0);
    ss >> px[i] >> py[i];
  }

  // 2. Parse objects.	  
  ss.str(get_line(ifs));
  ss.seekg(0);
  ss >> num_objects;
  double weight;
  vector<Object*> objects(num_objects);
  for (int i = 0; i < num_objects; i++) {
    ss.str(get_line(ifs));
    ss.seekg(0);

    // Parse metric information.
    if (ss.peek() == 'w') {
      ss.seekg(1);
      ss >> weight;
      ss.str(get_line(ifs));
      ss.seekg(0);
    } else {
      weight = 1.0;
    }

    vector<Corner*> verts;
    vector<Edge*> edges;
    while (!ss.eof()) {
      ss >> vert;
      const Point2d point(px[vert] / (WINDOW_WIDTH / 2) - 1, py[vert] / (WINDOW_WIDTH / 2) - 1);
      Corner* corner = new Corner(point);
      draw_corner(*corner);
      if (verts.empty() || !(*corner == *verts[0])) {
	verts.push_back(corner);
	if (verts.size() > 1) {
	  Edge* edge = new Edge(verts[verts.size() - 2], verts[verts.size() - 1]);
	  edges.push_back(edge);
	  draw_edge(*edge);
	}
      } else if (verts.size() > 2) {
	// Close the polygon.
	Edge* edge = new Edge(verts[verts.size() - 1], verts[0]);
	edges.push_back(edge);
	draw_edge(*edge);
      }
    }
  }

  ifs.close();
}

void run() {
  // Subdivision phase.
  // Construction phase.
}
