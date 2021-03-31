#include <cstdlib>	// needed 

#include "base.h"
#include "evalL4cheap.h"

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
// 		option = 2:  -- no  further options until now --
// 	   argv[2] = prec (int)
// 	   argv[3] = eps (int)
//
int main (int argc, char *argv[]) {
  mpf_set_default_prec(my_mpf_precision);

  if (argc == 1) {	// if no arguments,
		evalL4cheap eL4cheap;	//   then initialize eL4cheap from cin file
		eL4cheap.EVAL();	
		eL4cheap.stats(20);
		return 0;
  }

  int option = atoi(argv[1]);
  int prec = (argc > 2)?  atoi(argv[2]) : 5;	// output precision
  int eps = (argc > 3)?  atoi(argv[3]) : 2;	// root precision

  switch(option) {
	case 0: {
	  evalL4cheap eL4cheap(0);
  	eL4cheap.test();
		break;}
	case 1: {
	  vector<real> v = {-1, -1, 1};
	  evalL4cheap eL4cheap(v, interval(-2,2));
	  eL4cheap.EVAL(eps);
	  eL4cheap.stats(prec);
		break;}
	default:
	  cout << "Option not implemented yet" << endl;
  }//switch
//  return option;
  return 0;
}//main
//
//////////////////////////////////////////////////////////////////////////
