/****************************************************************
 file: nestedSqrt.cc
 purpose:
	To illustrate a very high precision inequality comparison
	This example is taken from the SODA'98 paper of
	Burnikel, Fleischer, Mehlhorn and Schirra
	(which describes the BFMS root bound)
 author: Chee Yap and Zilin (July 2001)
 usage:
	% nestedSqrt [N] [Flag]
	
	-- where the two optional arguments have the following meaning:
	-- N is the nesting depth of the sqrt expression
		N = 4 is the default
	-- Flag is used to turn on or off the incremental evaluation flag
		Flag = 1 to turn on (this is the default)
		Flag = 0 to turn off incremental evaluation 

 NOTES:
	There is little reason to turn off the incremental evaluation flag.
	To give an example of how incremental evaluation can speed up the
	evaluation, try the following two commands in this directory:

		% make timing N=8 IncFlag=0
		% make timing N=8 IncFlag=1

	On our machine, these two tests takes 30.9 and 1 second, respectively.

 Since CORE Library version 1.4
 $Id: nestedSqrt.cpp,v 1.1 2006-03-07 04:51:25 exact Exp $
****************************************************************/

#ifndef CORE_LEVEL
#define CORE_LEVEL 3
#endif

#include "CORE.h"

int main(int argc, char* argv[])
{
	std::cout.precision(40);
	setDefaultRelPrecision(20);
	setDefaultAbsPrecision(CORE_posInfty);

        double E = 2;
        int i,k;
	k = 4;
	if (argc == 1) {
	   std::cout << "Default nesting depth is 4; you can provide the\n"
	        << "  nesting depth in the command line, e.g., nestedSqrt 6\n";
	} else {
           k = atoi(argv[1]);
	   if (argc == 3) {
		int IncFlag = atoi(argv[2]);
		//if (IncFlag == 0) 
		//	setIncrementalEvalFlag(false);
		//else
		//	setIncrementalEvalFlag(true);
	   }
	}

        for (i=0;i<k;i++)
                E = E*E;
	std::cout << "E = SQRE(2, " << k << ") = " << E << std::endl;
	std::cout << "    where SQRE(2,k)= SQRE(2,k-1)**2 \n";
        E = E-1;
        for (i=0;i<k;i++)
                E = sqrt(E);
	std::cout << "e = SQRT(E-1, " << k << ") = " << E << std::endl;
	std::cout << "    where SQRT(F,k)= sqrt(log(F,k-1)) \n";
        E = E-2;
	std::cout << "e - 2 = " << E << std::endl;
	std::cout << "    N.B. e - 2 should a very small negative number\n";
        return 0;
}
