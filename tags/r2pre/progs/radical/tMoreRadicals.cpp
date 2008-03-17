/*************************************************************
 * File: tMoreRadical.cpp
 * Purpose:
 * 	Test some interesting identities involving radicals.
 *
 * Usage:   
 * 		> ./tMoreRadicals [prec=40]
 *
 * 	where the default values are shown above.
 *
 * Restriction/Bugs:
 * 	Currently, n must be an natural number (BigInt or int)
 *
 * Author: Chee Yap, Vikram Sharma
 * Date:  August 15, 2003
 * Core Library version 1.6
 *************************************************************/

#include "CORE.h"

using namespace CORE;
using namespace std;

typedef BigInt NT;

//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  outputPrec =  40;	// default output precision
  if (argc >= 4) {
    outputPrec = atoi(argv[3]);
  }

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  std::cerr << "outputPrec = " << outputPrec << ", eps = " << eps << std::endl;
 
  setDefaultOutputDigits(outputPrec);


  /* **************************************************
   * Example 1 -- from Ramanujan (see \cite{mehlhorn:rasc:03})
   * 
   *   3 sqrt( X ) - cuberoot(2) - cuberoot(20) + cuberoot(25) = 0.
   *
   *        where   X  =  cuberoot(5) - cuberoot(4).
   *
   ************************************************** */
     Expr X, R;

	X = radical(NT(5), 3) - radical(NT(4), 3);
	R = 3*sqrt(X)  - radical(NT(2), 3)
		- radical(NT(20),3) + radical(NT(25),3);

	if (R != 0) {
	   cout <<
	   "ERROR! 3 sqrt( cr(5)-cr(4) ) - cr(2) - cr(20) + cr(25) != 0"
	   << endl;
	   cout << "       Its value is " << R << endl;
	} else {
	   cout <<
	   "CORRECT! 3 sqrt( cr(5)-cr(4) ) - cr(2) - cr(20) + cr(25) == 0"
	   << endl;
	}
  /* **************************************************
   * Example 2 (Ramanujan -- see Yap, Chap.15)
   *   cuberoot(cuberoot(2)-1)  =
   *   		cuberoot(1/9) - cuberoot(2/9) + cuberoot(4/9).
   ************************************************** */
}
