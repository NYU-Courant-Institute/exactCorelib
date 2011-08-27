/**
 * main.cpp
 * driving program for modified-miranda algorithm
 * 
 * July 18, 2011
 */

#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <iostream>
#include <tclap/CmdLine.h>

#include "algorithm.h"
#include "tmp-display.h"
#include "benchmark.h"

using namespace std;

void startGlutLoop(int argc, char **argv);

// statistical variable
unsigned int largest_gen = 0;
unsigned int c0_excluded = 0;
unsigned int jacobian_excluded = 0;
unsigned int mk_excluded = 0;

// command line arguments (tclap install required)
// ValueArg parameters:
// 1:flag     2: name     3: description  4: required or not  5: default  6: optional visitor
TCLAP::ValueArg<string> f_xy ("f", "fxy", "first curve of the system", false, "", "string");
TCLAP::ValueArg<string> g_xy ("g", "gxy", "second curve of the system", false, "", "string");
TCLAP::ValueArg<string> x_minimum("x", "x_min", "Lower bound of x range", false, "-2", "string");
TCLAP::ValueArg<string> x_maximum("c", "x_max", "Upper bound of x range", false, " 2", "string");
TCLAP::ValueArg<string> y_minimum("y", "y_min", "Lower bound of y range", false, "-2", "string");
TCLAP::ValueArg<string> y_maximum("u", "y_max", "Upper bound of y range", false, " 2", "string");
TCLAP::ValueArg<string> Poly ("p", "poly",  "file name of the polynomial", false, "", "string");
TCLAP::ValueArg<string> min_box_size("m", "minsize", "minimum size of box", false, "0.0001", "string");
TCLAP::ValueArg<string> max_box_size("M", "maxsize", "maximum size of box", false, "0.1",    "string");
TCLAP::ValueArg<string> max_generation("r", "maxgen", "maximum generation", false, "15", "string");


int main(int argc, char **argv) {

  // use DoubleWrapper for arithmetics
  typedef IntervalT<DoubleWrapper> Interval;
  typedef BoxT<DoubleWrapper> Box;
  typedef BiPoly<DoubleWrapper> poly;

  // queues that used for display
  std::vector<const Box *> output;
  std::vector<const Box *> ambiguous;
  std::vector<const Box *> exclude;

  // default values for arguments, all arguments can be 
  // specified in Makefile

  // This is the maximum number of ambiguous boxes to be printed
  unsigned int max_ambiguous_box = 20;

  DoubleWrapper x_min = -2, x_max = 2, y_min = -2, y_max = 2;
  DoubleWrapper min_size = 0.0001;
  DoubleWrapper max_size = 0.01;
  int max_gen = 15;
  string fxy_str = "y - (x-1)^2 - 1";
  string gxy_str = "x - 1";
  poly fxy;
  poly gxy;

  try {
    TCLAP::CmdLine cmd("MK test", ' ', "1.0");
    cmd.add(Poly);
    cmd.add(x_minimum);
    cmd.add(x_maximum);
    cmd.add(y_minimum);
    cmd.add(y_maximum);
    cmd.add(min_box_size);
    cmd.add(max_box_size);
    cmd.add(f_xy);
    cmd.add(g_xy);
    cmd.add(max_generation);

    // parse the arguments
    cmd.parse(argc, argv);
  }
  catch (TCLAP::ArgException &e) {
    cerr << "Error : " << e.error() << endl;
    cerr << "Processing arg : " << e.argId() << endl;
    return -1;
  }
  
  if((f_xy.getValue() != "") && (g_xy.getValue() != "")) {
    fxy_str = f_xy.getValue();
    gxy_str = g_xy.getValue();
    fxy = fxy.getbipoly(fxy_str);
    gxy = gxy.getbipoly(gxy_str);
  }
  else if(Poly.getValue() != "") {
    benchmark::GetBiPoly(Poly.getValue().c_str(), &fxy, &gxy);
  }
  else {
    cout << "you should either specify a file name or provide 2 bipolynomials!" << endl;
    return -1;
  }

  x_min = x_minimum.getValue();
  x_max = x_maximum.getValue();
  y_min = y_minimum.getValue();
  y_max = y_maximum.getValue();
  min_size = min_box_size.getValue();
  max_size = max_box_size.getValue();
  max_gen = atoi(max_generation.getValue().c_str());
  // initial box construction
  Interval x_range(x_min, x_max);
  Interval y_range(y_min, y_max);
  Box *const box = new Box(0, x_range, y_range);


  struct timeval start;
  struct timeval end;
  // start time
  gettimeofday(&start, NULL);
  
  // get the algorithm running
  MKPredicates<DoubleWrapper> *const pred = 
    new MKPredicates<DoubleWrapper>(fxy, gxy, min_size, max_size, max_gen);
  Algorithm::Run<DoubleWrapper>(*pred, box, 
      &output, &ambiguous, &exclude);

  // end time
  gettimeofday(&end, NULL);

  unsigned int num_includes = output.size();      // statistic collections
  cout << endl << "Output regions: " << endl;
  for(unsigned int i = 0; i < output.size(); i++) {
    const Box *b = output[i];
    cout << "X: " << b->x_range << " , Y: " << b->y_range << endl;
  }

  
  unsigned int num_ambiguous = ambiguous.size();   // statistic collections
  
  cout << "Ambiguous regions: " << endl;
  for(unsigned int i = 0; i < ambiguous.size(); i++) {
    if(i > max_ambiguous_box)
	    break;
    const Box *b = ambiguous[i];
    cout << "X: " << b->x_range << " , Y: " << b->y_range << endl;
  }
  

  unsigned int num_excludes = exclude.size();  // statistic collections

  cout << endl;
  cerr << "The total time taken was = " <<
      (end.tv_sec - start.tv_sec)*1000000 + 
      (end.tv_usec - start.tv_usec) << " micro seconds" << endl;

  cout << endl << "statistic results: " << endl;
  cout << "output regions: " << num_includes << endl;
  cout << "ambiguous regions: " << num_ambiguous << endl; 
  cout << "exclusion regions: " << num_excludes << endl; 
  cout << "maximum generation: " << largest_gen << endl;
  cout << endl;
/*
  Box *first = new Box(0, Interval(-5, 5), Interval(-3, 3));
  Box *second = first->Dilate(2);
  cout << "first box y left: " << first->y_range.getL() << endl;;
  cout << "second box y left: " << second->y_range.getL() << endl;
*/
  // display preparation
  Box *b_display = new Box(0, x_range, y_range);
  display_funcs::SetDisplayParams(b_display, &exclude, &output, &ambiguous);
  // draw plot
  startGlutLoop(argc, argv);
  delete b_display;
  delete pred;


  return 0;
}

void startGlutLoop(int argc, char **argv) {
  cout << "-------------Graphic----------------"<<endl;
  cout << "--------Press ESC to exit-----------"<<endl;

  // Various open GL related things.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MK       (Press ESC to exit)");
  display_funcs::ClearBackground();
  glutReshapeFunc(display_funcs::ReshapeHandler);
  glutDisplayFunc(display_funcs::DisplayHandler);
  glutKeyboardFunc(display_funcs::KeyHandler);
  glutMainLoop();
}

