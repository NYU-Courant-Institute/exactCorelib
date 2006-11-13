/* ************************************************
   File: testMpfr.cpp

   Purpose:  To test the numerical output routines related to Mpfr.
	     These are considered very important functions
	     in our library, as the integrity of the
	     internal computation cannot be verified
	     unless we can exactly control the input and
	     output of numbers. 
   Usage:
        % testMpfr 			     -- do the default simple test
        % testMpfr 0			     -- do the default battery of test
        % testMpfr [prec] [expr] [digit] [string=""] -- do a single test

  // Q: WHAT DOES CORRECTNESS OF EVERY OUTPUT DIGIT MEAN?
  // A: We compute any relative approx x with some relative p bits of precision 
  //    Then printout any q digits of this approximation x;
  //    say x' is the printout.
  //
  //    PROPERTY 1: (No Misleading Digits)
  //    	First, suppose x' is in decimal notation.
  //    	Count the number of digits after the decimal point in x'.
  //    	Say d.  Note that d can be 0 (but we do not allow d<0).
  //    	Then we require |x - x'| <= 10^(-d)
  //
  //    	In case the output x' is in scientific notation,
  //    	with exponent of e (e.g., x'=1.234e-5, then d=3, e=-5)
  //    	Then we want 
  //    		|x - x'| <= 10^(e-d)
  //
  // 	PROPERTY 2: (Sufficient Accuracy)
  // 		Suppose x' has D digits starting from the most significant
  // 		digit (e.g., x'=123.456, then D=6, d=3).
  // 		An additional property must be fulfilled:
  //
  //    	If 10^q <= 2^p, then we want D=q
  //
  //    	(or D >= q, for a less strict interpretation of output)
  //
  //    REMARKS: 
  //    We use MPFR's bigfloat output routines which uses relative
  //    precision in its output digits.  E.g., if x=0.0012345, and
  //    you ask for 3 digits from MPFR, you get x'=0.00123, not 0.001.
  //
  //    We also use C++ streams' notion of "output precision", which
  //    also shows relative precision in digits: 
  //    E.g., 
  //	        cout << setprecision(6);
  //   		cout<<  -0.00123456;  //shows -0.00123456
  //            cout<<  -123456789.0; //shows -1.23457e+08
  //            cout<<  -12345600.0;  //shows -1.23456e+07

   Author: Jihun and Chee (Oct 2006)

   Since Core Library 2.0
   $Id: testMpfr.cpp,v 1.6 2006-11-13 20:10:43 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

// test(p, e, s="",d=0)
// 	will validate whether expression e, evaluated to p bits
// 	of relative precision, gives correct output of digits d. This correct
// 	output is s (but if s="", then we generate s ourselves. if d=0 d is conversion p bits to digits).
//
void test(int prec, Expr exp, int digit=0, string ans=string(""));

// countDigits(s, &D, &d, &e)
// 	where s is a string (in decimal notation or scientific notation)
//   D is the number of digits after the most significant digit of s.
//   d is similar, but counts number of digits after the decimal point in s.
//   e is the exponent in scientific notation, but e=0 in decimal notation.
//   E.g., s="123.456",  D=6, d=3, e=0.
//         s="-1.23456e-4", D=6, d=5, e=-4.
//         s="-0.12345e04", D=5, d=5, e=4.
//
void countDigits(string s, int * D, int *d, int *e);

// main routine
//
int main( int argc, char *argv[] ) {
  
  //Default simple test:
  int prec = 200;		     // prec=200
  int digit = 60;		     // prec=200 bits becomes 60 digits
  Expr exp = sqrt(Expr(7));          // exp = sqrt(7)
  string str=			     // this is the answer expected (60 digits)
"2.64575131106459059050161575363926042571025918308245018036833";
  //Expr exp = "7/22";               // exp = rational approx to Pi

  if (argc>1) prec=atoi(argv[1]);
  if (argc>2) exp=argv[2];
  if (argc>3) digit=atoi(argv[3]);
  if (argc>4) str=argv[3];

  if (prec>0) {// do default simple test or one input expression
    switch(argc) {
      case 1:
      case 5:
        test(prec, exp, digit, str);
        break;
      case 2:
      case 3:
        test(prec, exp);
        break;
      case 4:
        test(prec, exp, digit);
        break;
    }
  } else {     // else, do a battery of standard tests

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST ONE:
      // illustrating C++ stream' output precision:
      cout << "====== TEST ONE: cout precision" << endl;
      cout<< setprecision(6) << -0.00123456 << endl; //shows -0.00123456
      cout<< setprecision(6) << -123456789.0 << endl; //shows -1.23457e+08
      cout<< setprecision(6) << -12345600.0 << endl; //shows -1.23456e+07

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST TWO: rational expressions
      cout<< "====== TEST TWO: rational expressions" << endl;
      for (int i=10095; i<10100; i++)
	 test(i, Expr(i)/Expr(i+1));

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST THREE: squareroot expressions
      cout << "====== TEST THREE: sqrt expressions" << endl;
      for (int i=1095; i<1100; i++)
 	test(i, sqrt(Expr(i)+sqrt(Expr(i))));

      // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // TEST FOUR: example from nested Sqrt (see "progs/nestedSqrt")
      // 	The expression is E = sqrt[k]{(exp(k,2)-1)} - 2
      // 		where sqrt[k]{.} means we take sqrt() k times
      // 		and exp(k,x) means we raise to 2^x k times
      // 	Hence, |E| is very small.  We use k=4, to compare to Core1.
      cout << "====== TEST FOUR: nested sqrt" << endl;
      int prec=40;
      Expr E=2;
      int k=4;  // can change if you want higher precision test
      for (int i=0;i<k;i++)
                E = E*E;
	//cout << "E = SQRE(2, " << k << ") = " << E << endl;
	//cout << "    where SQRE(2,k)= SQRE(2,k-1)**2 \n";
        E = E-1;
        for (int i=0;i<k;i++)
                E = sqrt(E);
	//cout << "e = SQRT(E-1, " << k << ") = " << E << endl;
	//cout << "    where SQRT(F,k)= sqrt(log(F,k-1)) \n";
        E = E-2;

	// This is testing against the output from Core1
	//    (200 bits=60 relative digits)
	test(200, E, 60, 
        "-.00000190736227536746916983256469775415813708674303147237637217066");

  }//battery of tests

  return 0;

}//main

// testing routine
// 
void test(int prec, Expr exp, int digits, string ansstr){
  ostringstream oss;                      // oss is the string we print into

  if (digits == 0)
    digits = bits2digits(prec);         // convert prec (in bits) into digits
  cout<< "prec = " << prec << ", digits = " << digits << endl;
  prec += 4;				  // to guarantee prec relative digits output,
                                          // 4 more precision is needed

  setDefaultOutputDigits(digits, oss);    // display precision
  setDefaultOutputDigits(digits);
  BigFloat bf = exp.approx(prec, CORE_INFTY);           // compute to p relative bits

  cout<< "expression = " << exp << endl;
  cout<< "relative bit precision asked to guarantee = " << prec << endl;
  cout<< "mantissa bits of the result Mpfr variable = " << bf.get_prec() << endl;

  BigFloat2 bound = exp.rep()->appValue();

  // check if the relative precision is guaraunteed
  if (abs((exp - bf) / exp) > pow(Expr(2), -prec))
    cout << "ERROR!!! get_f is wrong" << std::endl;

  if ((bf - bound.getLeft()) * (bf - bound.getRight()) > 0 )
    cout << "ERROR!!! get_f is not contained in the boundary" << std::endl;

  if (abs((exp - bound.getLeft()) / exp) > pow(Expr(2), -prec))
    cout << "ERROR!!! approx lower bound is out of range" << std::endl;
  if (abs((exp - bound.getRight()) / exp) > pow(Expr(2), -prec))
    cout << "ERROR!!! approx upper bound is out of range" << std::endl;

  if (abs((bound.getLeft() - bound.getRight()) / exp) > pow(Expr(2), -prec))
    cout << "ERROR!!! approx upper bound - lower bound is out of range" << std::endl;

  // check if upper and lower bounds actually contain the exact value
  if ((exp - bound.getLeft()) * (exp - bound.getRight()) > 0 )
    cout << "ERROR!!! bounds do not contaion an exact value" << std::endl;

  oss.str("");
  oss << exp;                           // print into oss

  string str = oss.str();
  if (ansstr != ""){ // in case the correct answer is provided:
    if (ansstr != str){
    	cout << "ERROR!!! output string is not correct" << endl;
	core_error("Wrong Output", __FILE__, __LINE__, true);
    } else
    	cout << "CORRECT!!! output string equals provided answer" << endl;
  } else {
  int D=0; 			// D=number of significant digits
  int d=0; 			// d=number of digits beyond decimal point
  int e=0;			// value of exponent in scientific notation
  countDigits(str, &D, &d, &e);
  
  Expr diff = abs(exp - Expr(str, 10, CORE_INFTY));   // check the difference

  if (diff > pow(Expr(10), e-d)) {
	  				   // PROPERTY 1 says relative error,
					   //     diff <= 10^{e-d}
    cout << "ERROR!!! output string is not correct" << endl;
    cout << "printout string:"<< str << endl;
    cout << "original string:"<< exp << endl;
    core_error("Wrong Output", __FILE__, __LINE__, true);
  } else
    cout << "CORRECT!!! output string is correct" << endl;
  }

}//test

// Analyzes number string:
void countDigits(string str, int * D, int *d, int *e) {

  *D=0; 			// D=number of significant digits
  *d=0; 			// d=number of digits beyond decimal point
  *e=0;				// value of exponent in scientific notation
  bool dot=false;		// if decimal point is found  
  int j=0;			// j= position of most significant digit

  if ((str[0] == '+')|| (str[0] == '-')) j++;  // Takes care of sign
  if (str[j] == 0) j++;         // Takes care of 0.XXX (but not 00.123)
  				// CAVEAT: assuming there is at most one 0.

  for (size_t i = j; i < str.size(); i++) {// this loop counts D=number
    if (str[i] == 'e' || str[i] == 'E') {  //   of printout digits
      *e=atoi(str.substr(i+1,str.size()-i).c_str());
      break; } 
    if (str[i] == '.') dot=true;	   // found dot
  
    if (str[i] >= '0' && str[i] <= '9') {
      D++;
      if (dot) d++;
    }// else error!
  }//for
}
