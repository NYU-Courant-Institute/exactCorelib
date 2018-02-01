/* fpOverflow.cpp
 *
 * 	Taken from:
 *	https://stackoverflow.com/questions/15655070/how-to-detect-double-precision-floating-point-overflow-and-underflow
 *
 *	This program runs a for-loop for n times with this consequence:
 *
 *		For n=7, the overflow flag is turned on, but the
 *			underflow flag is not turned on.
 *		For n larger than 7, both the
 *			overflow and underflow flags are turned on.
 *		For n smaller than 7, both the
 *			overflow and underflow flags are not turned on.
 *		
 *	This program relies on the library <fenv> which has 
 *		functions to test the IEEE754 exceptions flags.
 *
 *	Note that
 *		POSIX, C99, C++11 have <fenv.h>
 *			and C++11 has <cfenv>)
 *
 *	When compiling, be sure to give "-std=c++11" flag to g++.
 *
 *	-- Chee (Jan 2018)
 *
 *	Funny BUG: if you compile this using "make fpOverflow", the
 *		the compiler includes all the usual compiler
 *		flags (which is largely unnecessary for this program),
 *		and the resulting executable is actually wrong: it will give
 *		wrong output.  But when you
 *		compile by using only the necessary flags
 *		(using "make fgOverflowX"),
 *		the problem goes away.
 ****************************************************/
#include <cfenv>
#include <iostream>

int main(int argc, char* argv[]) {

    int n= 7;
    if (argc > 1) n = atoi(argv[1]);

    std::feclearexcept(FE_OVERFLOW);
    std::feclearexcept(FE_UNDERFLOW);

    double overflowing_var = 1000;
    double underflowing_var = 0.01;

    std::cout << "Overflow flag before: "
		<< (bool)std::fetestexcept(FE_OVERFLOW) << std::endl;
    std::cout << "Underflow flag before: "
		<< (bool)std::fetestexcept(FE_UNDERFLOW) << std::endl;

    for(int i = 0; i < n; ++i) {
        overflowing_var *= overflowing_var;
        underflowing_var *= underflowing_var;
    }

    std::cout << "Overflow flag after: "
		<< (bool)std::fetestexcept(FE_OVERFLOW) << std::endl;
    std::cout << "Underflow flag after: "
		<< (bool)std::fetestexcept(FE_UNDERFLOW) << std::endl;
}
