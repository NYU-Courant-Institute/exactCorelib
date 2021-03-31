#include <cstdlib>	// needed 

#include "base.h"
#include "evalT2min.h"
#include "evalT2.h"
#include "evalT3.h"
#include "evalT4.h"
#include "evalL3.h"
#include "evalL3cheap.h"
#include "evalL4.h"
#include "evalL4cheap.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
// main program
//
// 	command line arguments:
//    first argument is a string that selects the method to be used for EVAL
//
//      first character selects the kind of method
//
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
  if ( argv[1][0] == 'a' )
    method = 0;
  if ( argv[1][0] == 'A' ) {
    method = 0;
    skipET2min = true;
  }
  if (method < 0)
    return -1;
  
  if (m > 1) {
    if ( argv[1][1] == '2' )
      method += 0;
    if ( argv[1][1] == '3' )
      method += 1;
    if ( argv[1][1] == '4' )
      method += 4;
  }

  if ( (m > 2) && ( (method == 4) || (method == 7) ) && ( argv[1][2] == 'c' ) )
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
  eval* e = new evalT2min();
  vector<real> c = e->f.c;
  interval I = e->I0;
  double timeET2, timeEL3cheap;
  for (int i=1; i<=8; i++) {
    if ((method == 0) || (method == i)) {
      if ( skipET2min && (i==1) )
        cout << "-, ";
      else {
        switch(i) {
          case 2: e = new evalT2(c,I);       break;
          case 3: e = new evalT3(c,I);       break;
          case 4: e = new evalL3(c,I);       break;
          case 5: e = new evalL3cheap(c,I);  break;
          case 6: e = new evalT4(c,I);       break;
          case 7: e = new evalL4(c,I);       break;
          case 8: e = new evalL4cheap(c,I);  break;
        }
        for (int j=0; j<N; j++)
    		  e->EVAL();	
    		e->stats(20);
    
        if (method == 0)
          if (runtime) {
            if (i==2)
              timeET2 = e->time.count();
            if (i==5)
              timeEL3cheap = e->time.count();
          }
            
          if (i<8)
            cout << ", ";
          else {
            if (runtime)
              cout << ", " << timeET2 / timeEL3cheap;
            cout << endl;
          }
      }
    }
  }

  return 0;
} //main
//
//////////////////////////////////////////////////////////////////////////
