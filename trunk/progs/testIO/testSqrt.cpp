/* ************************************************
   File: testSqrt.cpp

   Purpose:  Tests the accuracy of Expr::doubleValue() method.
   	     If e is an Expression, e.doubleValue() returns
	     a machine double which approximates e.

   Usage:
        % testSqrt [n=2] [k=0] [showFlag=false]

	All arguments are optional, with defaults shown.
	If there is any third argument at all, showFlag is set to
	true, and the output is "verbose".
	If k=0, we test the Expression e = sqrt(Expr(n)).
	Otherwise, we test the Expressions sqrt(i),
	for each i=n, n+1, ..., n+k-1.

	Here is how we test e = sqrt(Expr(i)).  Note that e is 
	the exact square root of n.  Let X = e.doubleValue().  
	We compare X against the machine evaluation of sqrt(i).  
	There are 3 possibile outcomes for this test:

	  (1) ERROR:    |machine sqrt(i) - X | > 4 * CORE_EPS * e
	  (2) WARNING:  |machine sqrt(i) - X | > 0 (but not an error)
	  (3) CORRECT:  |machine sqrt(i) - X | = 0 
	
	Here,  2 * CORE_EPS = 2^{-52} = 2.22045e-16 is the relative
	error.  This value is also known as the machine epsilon.  

	Why did we use 4 * CORE_EPS = 4.44089e-16? 
	Because the machine arithmetic may introduce an error of
	2 * CORE_EPS, and our doubleValue() may introduce an error of
	2 * CORE_EPS.  
	
	RESULTS: In a test of sqrt(i) (for i=2,...,1200),
	the worst relative error is achieved at i=66, 257, 1028.
	(i.e., just above powers of 2, as expected). 
	The largest relative error achieved is 4.43224e-16.
	For i around 16,000, the largest relative error is achieved
	at sqrt(16,385) with 4.44075e-16.  So our 4*CORE_EPS bound
	is essentially tight.

   BUG/FEATURE:
        -- doubleValue() does not check for overflow or underflows.
   	-- Expr::doubleValue() can result in error -- this will be fixed.
	-- It is possible to tighten Core Library's doubleValue() to
		that we avoid all WARNINGS altogether.  Perhaps we
		should do this.

   Author: Chee Yap (yap@cs.nyu.edu)
   Date:   Sept 28, 2003.  Revised, Mar 19, 2004.

   Since Core Library Version 1.6
   $Id: testSqrt.cpp,v 1.2 2006-08-09 09:57:45 exact Exp $
 ************************************************ */  

//# define CORE_DEBUG

#ifndef CORE_LEVEL
# define CORE_LEVEL 4
#endif

#include "CORE.h"

using namespace std;
using namespace CORE;

Expr test(double n, bool showFlag){

     Expr e	 = sqrt(Expr(n));	// this value is exact!
     double d	 = e.doubleValue();	// this value is what we want to check
     double dd	 = sqrt(n);		// this value is the IEEE approximation
     std::string s = e.toString(); //using default Output Precision
     Expr relerr(0.0);
  
     if (showFlag) {
     cout << endl;
     cout << "===================== SETPRECISION(15) ==================\n";
     cout << setprecision(15);
     cout << "   Expr:      sqrt(" << n << ") = " << e << endl;
     cout << "   doubleVal: sqrt(" << n << ") = " << d << endl;
     cout << "   Machine:   sqrt(" << n << ") = " << dd << endl;
     cout << "   STRING OUTPUT = " << s << endl;

     cout << "===================== SETPRECISION(14) ==================\n";
     cout << setprecision(14);
     cout << "   Expr:      sqrt(" << n << ") = " << e << endl;
     cout << "   doubleVal: sqrt(" << n << ") = " << d << endl;
     cout << "   Machine:   sqrt(" << n << ") = " << dd << endl;
     cout << "   ======= NOTE: the last three output should agree =====\n";
     cout << "===================== SELF-VALIDATION ===================\n";
     }

     /* **************** DO SELF-VALIDATION *********    */

     if (d != dd) {
        relerr = core_abs(Expr(d) - Expr(dd))/e;
        if ( (relerr > 4*Expr(CORE_EPS))  && showFlag ) {
          cout<<"   ERROR:    | doubleVal - MachineVal | = " << relerr
		  << " >  4 * CORE_EPS " << endl;
	} 
	else if (showFlag)
	  cout<< "   WARNING: | doubleVal - MachineVal | = " << relerr
		  << endl;
     } else if (showFlag)
	   cout << "   CORRECT:  doubleVal = MachineVal !! " << endl;

     return relerr;	// this ought to be bounded by 4*CORE_EPS
}

int main(int argc, char** argv)
{
   // DEFAULT ARGUMENTS
   double n = 2.0;
   int k = 0;
   bool showFlag = false;

   // USER SUPPLIED ARGUMENTS
   if (argc > 1) n = atoi(argv[1]);
   if (argc > 2) k =atoi(argv[2]);
   if (argc > 3) showFlag = true;

   // DO THE TEST
   Expr maxError(0.0), curError;
   double worsti = 0;

   if (k == 0) test(n, showFlag);
   else
	   for (double i=n; i<n+k; i++) {
		  curError = test(i, showFlag);
		  if (maxError < curError) {
			worsti = i;
		  	maxError = curError;
		  }

   	cout<< " =================\n SUMMARY:" << endl;
   	cout<< "    Maximum relative error, RelErr = " << maxError
	   	<< " achieved at sqrt(" << worsti << ")" << endl;
   	cout<< "    Allowed relative error, ErrBound = " << 4*CORE_EPS << endl;
   	if (maxError > 4 * Expr(CORE_EPS))
       		cout<< "    ERROR!  RelErr > ErrBound" << endl;
   	else
       		cout<< "    CORRECT!  RelErr <= ErrBound" << endl;
   }   
   return 0;
}
