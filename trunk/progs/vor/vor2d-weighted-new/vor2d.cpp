#include "vor_qt.h"
#include "vor_box.h"
#include "vor2d.h"
#include "Corner.h"
#include "Edge.h"
#include "Graphics.h"
#include "Object.h"
#include "Point.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <png.h>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

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

// Idea from http://forum.openframeworks.cc/t/scroll-wheel-event/593/3.
#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

namespace po = boost::program_options;
using namespace std;

using vor2d::vor_box;
using vor2d::vor_qt;
using vor2d::Corner;
using vor2d::Edge;
using vor2d::Feature;
using vor2d::Object;

// Global parameters.
const int window_width = 1024;
const double abs_eps = 1.0d / (1 << 7);
double geom_eps;
bool interactive_mode = false;

int scene_width = window_width;
const double scale_factor = 1.5;
double scale = 1.0;
int sx = 0;
int sy = 0;

// Global variables.
vor_qt* tree;
queue<vor_box*> subdiv;
queue<vor_box*> construct;
vector<vor_box*> vor_edge_boxes;
vector<vor_box*> vor_vert_boxes;
vector<Object*> objects;
bool show_grid = true;
bool save_image;
bool display_image;
string input_file_name;

// Set input options.
// See http://www.boost.org/doc/libs/1_41_0/doc/html/program_options.
void init_options(int argc, char* argv[]) {
  const string input_file_arg = "input_file_name";

  // Set non-positional options.
  po::options_description desc("Voronoi diagram options");
  desc.add_options()
    ("help", "Print this help message.")
    ("geps", po::value<double>(&geom_eps)->default_value(1.0), "Geometric epsilon.")
    ("save", po::value<bool>(&save_image)->default_value(false), "Save an image of the construction.")
    ("display", po::value<bool>(&display_image)->default_value(true), "Display the consturcted Voronoi diagram.")
    ("input_file_name", po::value<string>(&input_file_name), "Input file name.");

  // Set positional options.
  po::positional_options_description p_desc;
  p_desc.add("input_file_name", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p_desc).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    exit(1);
  }

  if (!vm.count("input_file_name")) {
    interactive_mode = true;
  }
}

void initialize(string input_file_name) {
  const string title_prefix = "2D subdivision-based Voronoi diagram";

  // Initialize global variables.
  tree = new vor_qt(2 /* dimension */, 2.0 /* width */);

  // Initialize graphics.
  if (display_image) {
    // Set up antialiasing.
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Set up window.
    glutInitWindowSize(window_width, window_width);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutCreateWindow((title_prefix + (input_file_name.length() > 0 ? (" - " + input_file_name) : "")).c_str());
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    // Other.
    GLUI_Master.set_glutMouseFunc(Mouse);
  }
}

void cleanup() {
  delete tree;
}

void display() {
  if(!display_image) {
    return;
  }
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_box_rec(*tree->root(), show_grid);
  
  // Draw all features.
  Corner* c;
  Edge* e;
  vector<Feature*>* features = tree->root()->get_features();
  for (auto it = features->begin(); it < features->end(); ++it) {
    // This is a hack found on StackOverflow to handle C++'s lack of "instanceof".
    // TODO: Use typeid or some other functionality?
    c = dynamic_cast<Corner*>(*it);
    if (c != nullptr) {
      draw_corner(*c);
    } else {
      e = dynamic_cast<Edge*>(*it);
      draw_edge(*e);
    }
  }

  // Save image if applicable.
  if (save_image) {
    save_png();
  }
  
  glutSwapBuffers();
}

// Based on http://www.labbookpages.co.uk/software/imgProc/libPNG.html
// and http://zarb.org/~gc/html/libpng.html.
// 4 used as a constant denotes the number of components in "RGBA".
void save_png() {
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep* row_ptrs = (png_bytep*) malloc(sizeof(png_bytep*) * window_width);
  png_bytep row_ptr = NULL;
  string filename = input_file_name + (show_grid ? "-grid" : "") + ".png";
  FILE* file = fopen(filename.c_str(), "wb");
  GLubyte* data = (GLubyte*) malloc(sizeof(GLubyte) * 4 * window_width * window_width);

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);

  for (int i = 0; i < window_width; i++) {
    row_ptrs[i] = (png_bytep) malloc(sizeof(png_bytep) * 4 * window_width);
  }
  if (!file || !info_ptr || !row_ptrs) {
    cout << "Warning: Unable to write file.\n";
    return;
  }
  png_init_io(png_ptr, file);
  glReadPixels(0, 0, window_width, window_width, GL_RGBA, GL_UNSIGNED_BYTE, data);

  // Write header.
  png_set_IHDR(png_ptr, info_ptr, window_width, window_width,
	       8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
	       PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);
  
  // Write image.
  for (int x = 0; x < window_width; x++) {
    row_ptr = row_ptrs[window_width - (x + 1)];
    for (int y = 0; y < window_width * 4; y++) {
      row_ptr[y] = data[window_width * 4 * x + y];
    }
  }
  png_write_rows(png_ptr, row_ptrs, window_width);
  png_write_end(png_ptr, NULL);

  // Cleanup.
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp) NULL);

  for (int i = 0; i < window_width; i++) {
    free(row_ptrs[i]);
  }
  free(row_ptrs);
  free(data);
  fclose(file);
}

int main(int argc, char* argv[]) {
  init_options(argc, argv);

  glutInit(&argc, argv);
  initialize(input_file_name);

  if (input_file_name.length() > 0) {
    parse(input_file_name);
  }

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
  Object* o;
  vor_box* root = tree->root();
  ifstream ifs(input, std::ifstream::in);

  if (!ifs) {
    cout << "File " << input << " not found.\n";
    exit(1);
  }

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
  for (int i = 0; i < num_objects; i++) {
    double inv_weight = 1.0;
    double a, b, c;
    ss.str(get_line(ifs));
    ss.seekg(0);

    // Parse metric information.
    // Parse multiplicative weight.
    if (ss.peek() == 'w') {
      ss.seekg(1);
      ss >> inv_weight;
      
      // Verify that the weight is positive.
      if (inv_weight <= 0.0) {
	cout << "Error: multiplicative weight must be positive.";
	exit(1);
      }
      o = new Object(1.0 / inv_weight);
      ss.str(get_line(ifs));
      ss.seekg(0);
    } else if (ss.peek() == 'm') { // Parse anisotropic metric parameters.
      ss.seekg(1);
      // For matrices of the form:
      // [a b]
      // [b c]
      ss >> a >> b >> c;
      o = new Object(a, b, c);
      ss.str(get_line(ifs));
      ss.seekg(0);
      
      // Verify that the matrix is positive definite
      // by checking that its two principal minors are positive.
      if (a <= 0 || (a * c - b * b) <= 0) {
        cout << "Error: metric matrix is not positive definite.\n";
        exit(1);
      }
    } else {
      o = new Object(1.0 / inv_weight);
    }

    objects.push_back(o);
    vector<Corner*> verts;
    int poly_verts = 0;
    while (!ss.eof()) {
      if (++poly_verts > num_points) {
	cout << "Parse error.\n";
	exit(1);
      }

      ss >> vert;
      const Point2d point(2 * px[vert] / window_width - 1, 2 * py[vert] / window_width - 1);
      Corner* corner = new Corner(point, o);
      if (verts.empty() || !(*corner == *verts[0])) {
        o->add_feature(corner);
	root->add_feature(corner);
        verts.push_back(corner);
        if (verts.size() > 1) {
          Edge* edge = new Edge(verts[verts.size() - 2], verts[verts.size() - 1], o);
          o->add_feature(edge);
	  root->add_feature(edge);
        }
      } else if (verts.size() > 2) {
        // Close the polygon.
        Edge* edge = new Edge(verts[verts.size() - 1], verts[0], o);
        o->add_feature(edge);
	root->add_feature(edge);
      }
    }
    root->add_object(o);
  }
  
  ifs.close();
}

void Mouse(int button, int state, int x, int y) {
  if (state == GLUT_UP) {
    // cout << x << " " << y << " " << button << "\n";
    if (button == GLUT_LEFT_BUTTON) {
      show_grid = !show_grid;
    } else if (button == GLUT_WHEEL_UP) {
      scale *= scale_factor;
    } else if (button == GLUT_WHEEL_DOWN) {
      scale = (scale > scale_factor) ? (scale / scale_factor) : 1.0;
    }
    display(); // Rerenders based on any click.
  }
}

void enqueue_children(vor_box* box) {
  box->smooth_split();
  vor_box** children = box->children();
  for (int i = 0; i < box->num_children(); i++) {
    subdiv.push(children[i]);
  }
}

// TODO: Improve this to handle degenerate input.
#define MAX_OBJECTS_FOR_CONSTRUCTION 3
#define MK_SCALE 1.0
#define JC_SCALE 3.0
#define INT_SCALE 3.0
void run() {
  // Subdivision phase.
  subdiv.push(tree->root());
  while (!subdiv.empty()) {
    vor_box* box = subdiv.front();
    subdiv.pop();
    double radius = box->radius();
    double num_obj = box->num_objects();

    assert(num_obj > 0);
    if (num_obj == 1) {
      continue;
    }

    if (box->width() < abs_eps) {
      cout << "Warning: absolute epsilon reached.\n";
      box->set_degen(true);
      continue;
    }
    
    if (num_obj > MAX_OBJECTS_FOR_CONSTRUCTION 
	|| radius > box->clearance()
	|| radius > geom_eps // TODO: Make sure this isn't off by a multiplicative factor of 2.
	|| !box->cpv()) {
      enqueue_children(box);
    } else if (num_obj == 2) {
      construct.push(box); // TODO: Don't use "construct" queue.
      vor_edge_boxes.push_back(box);
    } else { // num_obj == 3
      // TODO: Avoid iterating through all vertex boxes.
      bool int_vert_boxes = false;
      for (auto it = vor_vert_boxes.begin(); it != vor_vert_boxes.end(); ++it) {
      	if (box->scaled_intersect(**it, INT_SCALE)) {
      	  int_vert_boxes = true;
      	  break;
      	}
      }

      if (false /*int_vert_boxes || !(box->cjc(JC_SCALE) && box->cmk(MK_SCALE))*/) {
      	enqueue_children(box);
      	continue;
      } else {
	construct.push(box);
      	vor_vert_boxes.push_back(box);
      }
    }
  }

  // Construction phase.
  while (!construct.empty()) {
    vor_box* box = construct.front();
    construct.pop();

    // Check that boxes used for construction are leaves.
    // Due to smooth splitting they may not be.
    if (box->is_leaf()) {
      box->gen_vertices();
    } else {
      for (int i = 0; i < box->num_children(); i++) {
	construct.push(box->children()[i]);
      }
    }
  }

  // Display.
  display();

  // Print statistics.
  cout << "\nTotal splits: " << tree->splits() << "\n";
}
