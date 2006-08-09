/****************************************************************
 File: nestedSqrtX.ccp
 Purpose:
 	Generalization of nestedSqrt.cpp,
		where the base number "x" can be arbitrary.
	Experiment to demonstrate the speedup effects of the
		k-ary rootbounds from Yap and Pion

	For any real x > 0, and natural number k, we test the sign of
	the following two expressions:

		E0 = E0(x,k) = SQRT( SQRE( x, k), k) - x
		E1 = E1(x,k) = SQRT( SQRE( x, k) + 1, k) - x

	where SQRE( x, k) = SQRE( x, k-1)**2    when k>0, and SQRE( x, 0)= x,
	and   SQRT( x, k) = sqrt( SQRT( x, k-1) when k>0, and SQRT( x, 0)= x,

	Clearly E0 is identically zero.
	For large k, E1 is a very small positive number.

 Author: Chee Yap (Sep 30, 2002)
 Usage:
	% nestedSqrtX [k=4] [x="2.25"]
	
	-- where the two optional arguments have the following meaning:
	-- k is the nesting depth of the sqrt expression
		k = 4 is the default
	-- x is the base number in decimal.
		x = 2.25 is the default

 Since CORE Library Version 1.5
 $Id: nestedSqrtX.cpp,v 1.2 2006-08-09 10:08:47 exact Exp $
****************************************************************/

#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include "CORE.h"

int main(int argc, char* argv[])
{
   std::cout << "================== NestedSqrtX ================" << std::endl;
   std::cout << "===============================================" << std::endl;

   // PRECISION PARAMETERS
   // ========================================================================
	std::cout.precision(40);		// printout precision
	setDefaultRelPrecision(20);		// default relative precision
	setDefaultAbsPrecision(CORE_posInfty);	// turn off absolute precision

   // COMMAND LINE ARGUMENTS
   // ========================================================================
        double E, x;
        int i,k;
	if (argc == 1) {
	   k = 4;
	   std::cout << "   Nesting depth is 4 (default)" << std::endl;
	} else {
           k = atoi(argv[1]);
	   std::cout << "   Nesting depth is " << k << std::endl;
	}
	if (argc == 3) {
		x = argv[2];
	        std::cout << "   x = " << x << std::endl;
	} else {
		x = "2.25";			// default base value
	        std::cout << "   x = " << x << " (default) " << std::endl;
	}

   // EVALUATION OF EXPRESSION E1 
   // ========================================================================
	E = x;
        for (i=0;i<k;i++)
                E = E*E;
	std::cout << "   E = SQRE(" << x << ", " << k << ") \n";
	std::cout << "     = " << E << std::endl;
	std::cout << "       where SQRE(x,k)= SQRE(x,k-1)**2 and SQRE(x,0)=x\n";
        double E1 = E+1;
        for (i=0;i<k;i++)
                E1 = sqrt(E1);
        E1 = E1 - x;
	if (E1>0) {
	  setScientificFormat();
	  std::cout << "   E1 = SQRT(E + 1, " << k << ") - x \n";
	  std::cout << "      = " << E1 << std::endl;
	  std::cout << "        (N.B. E1 should be very small positive number)\n";
	  std::cout << "        ( lg(E1) ~ " <<E1.rep()->get_uMSB()<< ")"<< std::endl;
	} else 
	  std::cerr  << "   ERROR!! E1 is not positive " << std::endl;

   // EVALUATION OF EXPRESSION E0
   // ========================================================================
        for (i=0;i<k;i++)
                E = sqrt(E);
        E = E-x;
	if (E == 0) {
	std::cout << "   E0 = SQRT(E, " << k << ") - x \n";
	std::cout << "      = " << E << std::endl;
	std::cout << "        (CORRECT!! E0 is identically zero)\n";
	} else
	  std::cerr  << "   ERROR!! E0 is not zero " << std::endl;

   std::cout << "===============================================" << std::endl;
   return 0;
}
