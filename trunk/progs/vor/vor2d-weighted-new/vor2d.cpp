#include "vor_qt.h"
#include "vor_box.h"
#include "vor2d.h"
#include "Corner.h"
#include "Edge.h"
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

// using std::cout;
// using std::ifstream;
// using std::queue;
// using std::string;
// using std::stringstream;

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
  tree = new vor_qt(2 /* dimension */, 1.0 /* width */);
}

void cleanup() {
  delete tree;
}

void draw_rect() {
  glColor3f(0.0, 0.0, 1.0);
  glRectf(-0.5, -0.75, 0.75, 0.5);
}

void display () {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_rect();
  glutSwapBuffers();
}

int main(int argc, char* argv[]) {
  initialize();

  // Initialize GUI.
  glutInit(&argc, argv);
  glutInitWindowSize(1024, 1024);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("");

  // GLUI* glui = GLUI_Master.create_glui("", 0, 1024, 1024);
  // glui->set_main_gfx_window(windowID);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glutDisplayFunc(display);
  glutMainLoop();
  
  parse();
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

  // 1. Parse points.
  stringstream ss(get_line(ifs));
  ss.seekg(0);
  ss >> num_points;
  vector<Point2d> points(num_points);
  for (int i = 0; i < num_points; i++) {
    stringstream ss(get_line(ifs));
    ss.seekg(0);
    ss >> x >> y;
    Point2d point(x, y);
    points.push_back(point);
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
    } else {
      weight = 1.0;
    }

    vector<Corner*> verts;
    vector<Edge*> edges;
    while (!ss.eof()) {
      ss >> vert;
      Corner* corner = new Corner(points[vert]);
      if (verts.empty() || !(*corner == *verts[0])) {
	verts.push_back(corner);
	if (verts.size() > 1) {
	  edges.push_back(new Edge(verts[verts.size() - 2], verts[verts.size() - 1]));
	}
      } else if (verts.size() > 2) {
	// Close the polygon.
	edges.push_back(new Edge(verts[verts.size() - 1], verts[0]));
      }
    }
  }

  ifs.close();
}

void run() {
  // Subdivision phase.
  // Construction phase.
}
