#include <cstdlib>	// needed 

#include "base.h"
#include "evalL3.h"

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
		evalL3 eL3;	//   then initialize eL3 from cin file
		eL3.EVAL();	
		eL3.stats(20);
		return 0;
  }

  int option = atoi(argv[1]);
  int prec = (argc > 2)?  atoi(argv[2]) : 5;	// output precision
  int eps = (argc > 3)?  atoi(argv[3]) : 2;	// root precision

  switch(option) {
	case 0: {
	  evalL3 eL3(0);
  	eL3.test();
		break;}
	case 1: {
	  vector<real> v = {-1, -1, 1};
	  evalL3 eL3(v, interval(-2,2));
	  eL3.EVAL(eps);
	  eL3.stats(prec);
		break;}
	default:
	  cout << "Option not implemented yet" << endl;
  }//switch
//  return option;
  return 0;
}//main
//
//////////////////////////////////////////////////////////////////////////
