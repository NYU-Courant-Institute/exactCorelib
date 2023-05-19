#include <cstdlib> // needed

#include "base.h"
#include "evalL3ncheap.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
// main program
//
// 	command line arguments:
// 	   If there are no arguments, it is assumed that we will
// 		read a polynomial from a file (e.g., text1.txt)
// 	   Else:
// 	   argv[1] = option (int)
// 		option = 0:  run unit tests
// 		option = 1:  find roots of the golden ratio in [-2,2]
//    option = 2:  read polynomial from file and run with specified level of convergence
// 		option = 3:  -- no  further options until now --
// 	   argv[2] = prec (int)
// 	   argv[3] = eps (int)
//
int main (int argc, char *argv[]) {
  mpf_set_default_prec(my_mpf_precision);

  if (argc == 1) { // if no arguments
    evalL3ncheap eL3ncheap; // then initialize eL3ncheap from cin file
    eL3ncheap.EVAL();
    eL3ncheap.stats(20);
    return 0;
  }

  int option = atoi(argv[1]);
  int prec = (argc > 2)? atoi(argv[2]) : 5; // output precision (default 5)
  int eps = (argc > 3)? atoi(argv[3]) : 2; // root precision (default 2)
  int lvl = (argc > 4)? atoi(argv[4]) : 3; // level of convergence (default 3)

  switch(option) {
    case 0: {
      evalL3ncheap eL3ncheap(0);
      eL3ncheap.test();
      break;
    }
    case 1: {
      vector<real> v = {-1, -1, 1};
      evalL3ncheap eL3ncheap(v, interval(-2,2), lvl);
      eL3ncheap.EVAL(eps);
      eL3ncheap.stats(prec);
      break;
    }
    case 2: {
      evalL3ncheap eL3ncheap(lvl); // then initialize eL3ncheap from cin file
      eL3ncheap.EVAL(eps);
      eL3ncheap.stats(prec);
      break;
    }
    default:
      cout << "Option not implemented yet" << endl;
  } // end switch
  // return
  return 0;
} // end main
