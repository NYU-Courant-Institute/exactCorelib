/* **************************************
   File: mesh/ceval/ceval.cpp
   Description: 
   		This is the toplevel entry point for the CEVAL algorithm.
		It depends on the 
			algorithms.{h,cpp}
			predicates.{h.cpp}

   Usage:

   Author:  Narayan Kamath
   Date:    2010

   Since Core Library  Version 2.1
   $Id: CXY.cpp,v 1.3 2010/06/10 16:51:46 exact Exp $
 ************************************** */

#include <iostream>
#include <list>
#include <sys/time.h>

#include "ceval-defs.h"
#include "algorithm.h"
#include "predicates.h"

int main(int argc, char **argv) {
  // x^n - 2*a^2.x^2 + 4ax - 2
  // a = 2 n = 6
  //Polynomial<double> p = "1-200*x^2 + 6600*x^4 -84480*x^6 + 549120*x^8-2050048*x^10 + 4659200*x^12-6553600*x^14 + 5570560*x^16-2621440*x^18 + 524288*x^20";
  
  //Polynomial<PolyType> p = "x^5-1";

  // DEFAULT INPUT PARAMETERS:
  string sp="x^5-1";		// input polynomial as string
  double xmin=-10.0;
  double ymin=-10.0;
  double boxwidth=20;

  // PROCESS INPUT PARAMETERS:
  if (argc>1) sp=argv[1];
  if (argc>2) xmin=atof(argv[2]);
  if (argc>3) ymin=atof(argv[3]);
  if (argc>4) boxwidth=atof(argv[4]);

  Polynomial<PolyType> p = sp;		// convert string to polynomial
  cout << "Input Polynomial is:  " << p.toString() << endl;
  cout << "(xmin, ymin) = (" << xmin << ", " << ymin << ")\n";
  cout << "boxwidth = " << boxwidth << endl;

  // ymin and ymax are computed (because ceval only works with square boxes)
  double xmax=xmin+boxwidth;
  double ymax=ymin+boxwidth;

  // OTHER INITIALIZATIONS:
  struct timeval start;
  struct timeval end;

  double min_box_size_d = 0.0001;
  
  // Start Computation:

  Box *main = new Box(Complex(xmin, ymin), Complex(xmax, ymax));

  // Start run
  gettimeofday(&start, NULL);
  Predicates pred(p);
  Algorithm algo(pred, main, min_box_size_d, true, true);
  algo.Run();
  gettimeofday(&end, NULL);
  // End run

  cout << "Total time : "
	<< (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec)
	<< " micro seconds" << endl;

  cout << "--------------------------" << endl;
  cout << "TOTAL NUMBER :" << algo.output()->size() << endl;
  list<const Disk *>::const_iterator it = algo.output()->begin();
  while (it != algo.output()->end()) {
    cout << "m= " << (*it)->centre << ", r= " << (*it)->radius << endl;
    ++it;
  }
}
