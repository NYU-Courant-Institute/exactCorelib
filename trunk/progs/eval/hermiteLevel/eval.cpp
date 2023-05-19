#include <cstdlib>	// needed

#include "base.h"
#include "evalT2min.h"
#include "evalT2.h"
#include "evalT3.h"
#include "evalT4.h"
#include "evalL3.h"
#include "evalL3cheap.h"
#include "evalL3ncheap.h"
#include "evalL4.h"
#include "evalL4cheap.h"
#include "evalH4.h"
#include "evalH4cheap.h"
#include "evalH4ncheap.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
// main program
//
// 	command line arguments:
//    first argument is a string that selects the method to be used for EVAL
//
//      first character selects the kind of method
//        t -> generalized Taylor form
//        l -> recursive Lagrange form
//        a -> runs all methods
//        A -> runs all methods, except for the minimal linear Taylor form
//
//      second character selects the degree of exact evaluation
//        2 -> linear (only for Taylor)
//        3 -> quadratic
//        4 -> cubic
//
//      optional third character
//        c -> for cheaper centered form evaluation of the remainder part (only for Lagrange)
//        m -> for minimal Taylor form (only for linear Taylor)
//
//      optional last character
//        v -> verbose output
//        w -> laconic output (only key data as csv)
//        r -> only running time output (in secs)
//        t -> only treesize output
//
//    (optional) second argument is a number N, which force EVAL to be called N times and to report
//        the average running time
//
//    first char can also choose utility for creating tables in ISSAC'23 
//        m -> utility for creating Table 5
//           second char l -> L3ncheap
//                       h -> H4ncheap
//           third  arg is int minlevel - first level to be tested
//           fourth arg is int maxlevel - last level to be tested
//           fifth  arg is int incr     - increment of level
//        n -> utility for creating Table 4
//           second arg is int N - number of runs
//
//    examples:
//
//      t3    -> generalized Taylor form with cubic convergence with normal output
//      l4cw  -> recursive Lagrange with quartic convergence and cheap remainder part with laconic output
//      at    -> reports the tree sizes for all methods
//      ar 10 -> reports the average running times (for 10 runs) for all methods
//
// 	  The data for the polynomial and the initial interval are assumed to be read from a file.
//
//    examples:
//
//      cat data/chebyshev020.pol data/chebyshev.int | ./eval t3v
//
//

int main (int argc, char *argv[]) {
  mpf_set_default_prec(my_mpf_precision);

  if (argc == 1) {    // if no arguments, do nothing
    return -1;
  }

  int m = strlen(argv[1]);
  int method = -1;
  bool skipET2min = false;

  //
  // parse command line arguments to choose the correct method
  //
  if ( argv[1][0] == 't' )
    method = 2;
  if ( argv[1][0] == 'l' )
    method = 3;
  if ( argv[1][0] == 'h' )
    method = 5;
  if ( argv[1][0] == 'a' )
    method = 0;
  if ( argv[1][0] == 'A' ) {
    method = 0;
    skipET2min = true;
  }
  // utility for creating Table 4
  if ( argv[1][0] == 'n' ) {
    laconic = true;
    int N = 1;
    if (argc >= 2) {
      N = atoi(argv[2]);
    }
    eval* e = new evalL3cheap();
    vector<real> c = e->f.c;
    interval I = e->I0;
    double timeEL3cheap, timeEH4cheap;
    double sigma = 0.0;
    for (int i = 0; i < N; i++) {
      e = new evalL3cheap(c,I);
      e->EVAL();
      timeEL3cheap = e->time.count();
      e = new evalH4cheap(c,I);
      e->EVAL();
      timeEH4cheap = e->time.count();
      sigma += timeEL3cheap/timeEH4cheap;
    }
    cout << "deg = " << e->f.n << " sigma = " << sigma/(double)N << endl;
    return 0;
  }
  // utility for creating Table 5
  if ( argv[1][0] == 'm' ) {
    char method = argv[1][1];
    laconic = true;
    int N = 2;
    int minlevel;
    int maxlevel;
    int incr = 1;
    if (argc >= 3) {
      minlevel = atoi(argv[2]);
      maxlevel = atoi(argv[3]);
      if (argc >= 4) incr = atoi(argv[4]);
    }
    eval* e = new evalL3ncheap();
    vector<real> c = e->f.c;
    interval I = e->I0;
    double timeEL3ncheap = 0.0;
    for (int level = minlevel; level <= maxlevel; level+=incr) {
      timeEL3ncheap = 0.0;
      for (int i = 0; i < N; i++) {
        if (method == 'l')
          e = new evalL3ncheap(c,I,level);
        if (method == 'h')
          e = new evalH4ncheap(c,I,level);
        e->EVAL();
        timeEL3ncheap += e->time.count();
      }
      cout << "level = " << level << " time = " << timeEL3ncheap/N << endl;
    }
    return 0;
  }
  if (method < 0)
    return -1;

  if (m > 1) {
    if ( argv[1][1] == '2' )
      method += 0;
    if ( argv[1][1] == '3' )
      method += 2;
    if ( argv[1][1] == '4' )
      method += 5;
  }
	
  if ( (m > 2) && ( (method == 5) || (method == 8) || (method == 10) ) && ( argv[1][2] == 'c' ) )
    method += 1;
  if ( (m > 2) && ( method == 2 ) && ( argv[1][2] == 'm' ) )
    method -= 1;

  //
  // set the output mode
  //
  if ( argv[1][m-1] == 'v' )
    verbose = true;
  if ( argv[1][m-1] == 'w' )
    laconic = true;
  if ( argv[1][m-1] == 'r' ) {
    laconic = true;
    runtime = true;
  }
  if ( argv[1][m-1] == 't' ) {
    laconic = true;
    treesize = true;
  }

  int N = 1;
  if (argc >= 3)
    N = atoi(argv[2]);

  //
  // execute the appropriate (or all) variant of eval
  //
  eval* e = new evalT2();
  vector<real> c = e->f.c;
  interval I = e->I0;
  double timeEL3cheap, timeEL3ncheap, timeEH4cheap, timeEH4ncheap;
  for (int i=1; i<=11; i++) {
	    if ((method == 0) || (method == i)) {
      if ( skipET2min && (i==1) )
        cout << "-, ";
      else {
        switch(i) {
          case 2: e = new evalL3cheap(c,I);  break;
          case 3: e = new evalL4cheap(c,I);  break;
          case 4: e = new evalL3ncheap(c,I,10); 	break; // **Level 10 Convergence**
          case 5: e = new evalL3ncheap(c,I,20); 	break; // **Level 15 Convergence**
          case 6: e = new evalL3ncheap(c,I,40); 	break; // **Level 20 Convergence**
          case 7: e = new evalH4(c,I);      break;
          case 8: e = new evalH4cheap(c,I); break;
          case 9: e = new evalH4ncheap(c,I,10); break; // **Level 10 Convergence**
          case 10: e = new evalH4ncheap(c,I,15); break; // **Level 15 Convergence**
          case 11: e = new evalH4ncheap(c,I,20); break; // **Level 20 Convergence**
        }
        for (int j=0; j<N; j++)
    	    e->EVAL();	
    	  e->stats(20);
        if (method == 0)
          if (runtime) {
            if (i==2) 
              timeEL3cheap = e->time.count();
            if (i==5)
              timeEL3ncheap = e->time.count();
            if (i==8)
              timeEH4cheap = e->time.count();
            if (i==10)
              timeEH4ncheap = e->time.count();
          }  
          if (i<11)
            cout << ", ";
          else {
            if (runtime)
              cout << ", " << timeEL3cheap / timeEH4cheap;
              cout << ", " << timeEL3cheap / timeEH4ncheap;
              cout << ", " << timeEL3cheap / timeEL3ncheap;
            cout << endl;
          }
  }}}

  return 0;
} //main
//
//////////////////////////////////////////////////////////////////////////
