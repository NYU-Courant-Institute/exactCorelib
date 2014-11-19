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

void Mouse(int button, int state, int x, int y);
void parse(string input);
void run();

// Global variables.
vor_qt* tree;
queue<vor_box*> subdiv;
queue<vor_box*> construct;
vector<Object*> objects;
bool show_grid = true;

void initialize(string input_file_name) {
  const string title_prefix = "2D subdivision-based Voronoi diagram - ";

  // Initialize global variables.
  tree = new vor_qt(2 /* dimension */, 4.0 /* width */);

  // Set up antialiasing.
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  // Set up window.
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_WIDTH);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutCreateWindow((title_prefix + input_file_name).c_str());
  glClearColor(1.0, 1.0, 1.0, 1.0);

  // Other.
  GLUI_Master.set_glutMouseFunc(Mouse);
}

void cleanup() {
  delete tree;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_box_rec(*tree->root(), show_grid);
  
  // Draw all corners.
  vector<Corner*>* corners = tree->root()->get_corners();
  for (auto it = corners->begin(); it < corners->end(); ++it) {
    draw_corner(**it);
  }

  // Draw all edges.
  vector<Edge*>* edges = tree->root()->get_edges();
  for (auto it = edges->begin(); it < edges->end(); ++it) {
    draw_edge(**it);
  }
  
  glutSwapBuffers();
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "No input file.\n";
    exit(1);
  }
  
  glutInit(&argc, argv);
  initialize(argv[1]);
  parse(argv[1]);
  glutDisplayFunc(display);
  run();
  glutMainLoop();
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

void parse(string input) {
  int num_points;
  int point_count;
  int num_objects;
  int vert;
  int x, y;
  vor_box* root = tree->root();
  ifstream ifs(input, std::ifstream::in);

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
  double inv_weight;
  for (int i = 0; i < num_objects; i++) {
    ss.str(get_line(ifs));
    ss.seekg(0);

    // Parse metric information.
    if (ss.peek() == 'w') {
      ss.seekg(1);
      ss >> inv_weight;
      ss.str(get_line(ifs));
      ss.seekg(0);
    } else {
      inv_weight = 1.0;
    }
    Object* o = new Object(1.0 / inv_weight);
    objects.push_back(o);

    vector<Corner*> verts;
    while (!ss.eof()) {
      ss >> vert;
      const Point2d point(px[vert] / (WINDOW_WIDTH / 2) - 1, py[vert] / (WINDOW_WIDTH / 2) - 1);
      Corner* corner = new Corner(point, o);
      if (verts.empty() || !(*corner == *verts[0])) {
	o->add_feature(corner);
	root->add_corner(corner);
	verts.push_back(corner);
	if (verts.size() > 1) {
	  Edge* edge = new Edge(verts[verts.size() - 2], verts[verts.size() - 1], o);
	  o->add_feature(edge);
	  root->add_edge(edge);
	}
      } else if (verts.size() > 2) {
	// Close the polygon.
	Edge* edge = new Edge(verts[verts.size() - 1], verts[0], o);
	o->add_feature(edge);
	root->add_edge(edge);
      }
    }
    root->add_object(o);
  }

  ifs.close();
}

void Mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    show_grid = !show_grid;
    display();
  }
}

#define MAX_OBJECTS_FOR_CONSTRUCTION 3
void run() {
  // Subdivision phase.
  subdiv.push(tree->root());
  while (!subdiv.empty()) {
    vor_box* box = subdiv.front();
    subdiv.pop();
    double radius = box->radius();
    double num_obj = box->num_objects();

    if (num_obj > 1) {
      if (box->width() > ABS_EPS 
	  && (num_obj > MAX_OBJECTS_FOR_CONSTRUCTION || box->clearance() < 2 * radius || radius > GEOM_EPS)) {
	box->smooth_split();
	vor_box** children = box->children();
	for (int i = 0; i < box->num_children(); i++) {
	  subdiv.push(children[i]);
	}
      } else {
	construct.push(box);
      }
    }
  }

  // Construction phase.
  while (!construct.empty()) {
    vor_box* box = construct.front();
    construct.pop();
    box->gen_vertices();
  }

  // Display.
  display();
}
