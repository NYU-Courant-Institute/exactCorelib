/* ************************************************
   File: testMpfr.cpp

   Purpose:  To test the numerical output routines related to Mpfr.
	     These are considered very important functions
	     in our library, as the integrity of the
	     internal computation cannot be verified
	     unless we can exactly control the input and
	     output of numbers. 

  // Q: WHAT DOES CORRECTNESS OF EVERY OUTPUT DIGIT MEAN?
  // A: We compute any relative approx x with some relative p bits of precision 
  //    Then printout any q digits of this approximation x;
  //    say x' is the printout.
  //
  //    PROPERTY 1:
  //    	Count the number of digits after the decimal point.
  //    	Say d, in x'.  Note that d can be 0.
  //    	Then |x - x'| <= 10^(-d)
  //
  //    	In case the output x' is in scientific notation,
  //    	with exponent of e (e.g., if x'=1.23e-5, then e=-5)
  //    	Then we want 
  //    		|x - x'| <= 10^(e-d)
  //
  // 	PROPERTY 2:
  // 		Suppose x' has D digits starting from the most significant
  // 		digit (e.g., x'=123.456, then D=6, d=3).
  // 		An additional property must be fulfilled:
  //
  //    	If 10^q <= 2^p, then we want D=q
  //    		(or D >= q, for a less strict interpretation)
  //
   Usage:
        % testMpfr

   Author: Jihun and Chee 

   Since Core Library 2.0
   $Id: testMpfr.cpp,v 1.1 2006-10-30 22:39:50 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

void test(int prec, Expr exp, string ans=string(""));

int main( int argc, char *argv[] ) {
  
  int prec = 200;		          // prec is   p=100
  Expr exp = sqrt(Expr(7));               // exp = sqrt(7)
  //Expr exp = "7/22";               // exp = sqrt(7)

  if (argc>1) prec=atoi(argv[1]);
  if (argc>2) exp=argv[2];

  if (prec>0)
  	test(prec, exp);
  else {//do a battery of standard tests

       // example from nested Sqrt:
       int prec=40;
       //std::cout.precision(40);
       //setDefaultRelPrecision(20);

       Expr E=2;
       int k=4;
       for (int i=0;i<k;i++)
                E = E*E;
	//std::cout << "E = SQRE(2, " << k << ") = " << E << std::endl;
	//std::cout << "    where SQRE(2,k)= SQRE(2,k-1)**2 \n";
        E = E-1;
        for (int i=0;i<k;i++)
                E = sqrt(E);
	//std::cout << "e = SQRT(E-1, " << k << ") = " << E << std::endl;
	//std::cout << "    where SQRT(F,k)= sqrt(log(F,k-1)) \n";
        E = E-2;

	// This is testing against the output from Core1
	//    (200 bits=60 digits)
	test(200, E,
	  "-.00000190736227536746916983256469775415813708674303147237637");
%-.00000190736227536746916983256469775415813708674303147237637
%-.0000019073622753674691698325646977541581370867430314723763721706
%-.00000190736227536746916983256469775415813708674303147237637217065539

	  for (int i=1080; i<1100; i++)
		  test(i, sqrt(i+sqrt(i)));
  }
  return 0;

}//main

void test(int prec, Expr exp, string ansstr){
  ostringstream oss;                      // oss is the string we print into

  int digits = bits2digits(prec);         // digits is q=30
  cout<< "digits = " << digits << endl;

  setDefaultOutputDigits(digits, oss);    // display precision
  setDefaultOutputDigits(digits);
  exp.approx(prec, CORE_INFTY);           // compute to p relative bits
  cout<< "expression = " << exp << endl;

  oss.str("");
  oss << exp;                           // print into oss

  string str = oss.str();
  if (ansstr != ""){ // in case we know what the correct answer is
    if (ansstr != str)
    	cout << "ERROR(1)!!! output string is not correct" << std::endl;
  } else {
  int D=0; 				// D=number of significant digits
  int d=0; 				// d=number of digits beyond decimal point
  bool dot=false;			// if decimal point is found  
  int e=0;				// value of exponent in scientific notation
  int j=0;				// j= position of most significant digit

  if ((str[0] == '+')|| (str[0] == '-')) j++;  // Takes care of sign
  if (str[j] == 0) j++;                // Takes care of 0.XXX

  for (size_t i = j; i < str.size(); i++) {// this loop counts D=number
    if (str[i] == 'e' || str[i] == 'E') {  //   of printout digits
      e=atoi(str.substr(i+1,str.size()-i).c_str());
      break; }                             // 
    if (str[i] == '.') dot=true;	   // found dot
  
    if (str[i] >= '0' && str[i] <= '9') {
      D++;
      if (dot) d++;
    }
  }
  
  Expr diff = abs(exp - Expr(str, 10, CORE_INFTY));   // check the difference

  if (diff > pow(Expr(10), e-d)) {
	  				   // PROPERTY 1 says relative error,
					   //     diff <= 10^{e-d}
    cout << "ERROR(1)!!! output string is not correct" << std::endl;
    cout << "printout string:"<< str << std::endl;
    cout << "original string:"<< exp << std::endl;
  } else
    cout << "CORRECT(1)!!! output string is correct" << std::endl;
  }

}//test
