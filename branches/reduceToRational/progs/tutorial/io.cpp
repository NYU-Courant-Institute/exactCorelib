/* ************************************************
   File: io.cpp

   Purpose:  IO examples from Tutorial.  These illustrate
	     the interaction between three kinds of precision parameters:

		(1) the precision for evaluation of expressions,
			controlled by setDefaultPrecision(rel,abs)
		(2) the precision of output,
			controlled by setDefaultOutputDigits(n, os).
			This basically calls the std::setprecision(digits) of
			std::ostreams.setprecision(n).
		(3) the precision for reading inputs,
			controlled by setDefaultInputDigits(n)

		In addition, there is the choice between scientific
		and positional format for output. 
		
   Usage:
        % io

   Since Core Library 1.7
   $Id: io.cpp,v 1.3 2010/06/11 17:49:30 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE/CORE.h"

int main( int argc, char *argv[] ) {

  // This is the default setting:
  // 		setDefaultPrecision(60, INFTY);
  //setDefaultInputDigits(CORE_INFTY);

  double X("-1234.567890");
  std::cout << "  Let X=1234.567890\n";
  std::cout << "  Default Printout of X : \n    "
	<< X << std::endl;
  std::cout << "  Printout of X after setprecision(10) : \n    "
	<< std::setprecision(10) << X 
	<< "\n     -- so `output width' (=10 here) counts the decimal point\n";
  std::cout << "  Printout of X after setprecision(6): \n    "
	<< std::setprecision(6) << X
	<< "\n     -- so the default width is 6" << std::endl;

  setScientificFormat();

  std::cout << "  Printout of X after setScientificFormat(): \n    "
	<< X << "\n     -- the width remains set at 6 \n"
  	<< "     -- but decimal point in Scientific width is not counted! \n"
  	<< "     -- Note the rounding of the last digit\n";
  std::cout << "  Printout of X after setprecision(10) : \n    "
	<< std::setprecision(10) << X << std::endl;
  std::cout << "  Printout of X after setprecision(11) : \n    "
	<< std::setprecision(11) << X 
	<< "\n     -- an artificial zero digit is printed"  << std::endl;
  std::cout << "  Printout of X after setprecision(12) : \n    "
	<< std::setprecision(12) << X 
	<< "\n     -- another artificial zero digit!"  << std::endl;
  std::cout << "  Printout of X after setprecision(100) : \n    "
	<< std::setprecision(100) << X 
	<< "\n     -- Should print correctly"  << std::endl;
  std::cout << "  Printout of X after setprecision(200) : \n    "
	<< std::setprecision(200) << X 
	<< "\n     -- Should see print out errors!"  << std::endl;
  std::cout << "  FIX: set defInputDigits to INFTY before reading X \n";

  defInputDigits = CORE_INFTY;
  setDefaultPrecision(CORE_INFTY,800);

  // X = double("1234.567890");
  X=std::string("-1234.567890"); // why is this a problem for Core2?

  std::cout << "  printout X after setprecision(20) : \n    "
	<< std::setprecision(20) << X  << std::endl;
  std::cout << "  Try Again: printout X after setprecision(200) : \n    "
	<< std::setprecision(200) << X 
	<< "\n     -- Should see no printout errors!"  << std::endl;

  setPositionalFormat();
  std::cout << "  Printout of X after setPositionalFormat(): \n    "
	<< X << std::endl;
  std::cout << "  Printout of X after setprecision(11): \n    "
	<< std::setprecision(11) << X
	<< "\n     -- only one extra zero printed, as expected" << std::endl;
  std::cout << "  Printout of X after setprecision(5): \n    "
	<< std::setprecision(5) << X
	<< "\n     -- rounding to a 4-digit integer" << std::endl;
  std::cout << "  Printout of X after setprecision(4): \n    "
	<< std::setprecision(4) << X
	<< "\n     -- we are forced to go to scientific format" << std::endl;

  std::cout << "\n To see what happens if X were not exact, we next set Y = 1/3.\n"
	<< "    Here Y is an expression\n"
  	<< "    We evaluate Y relative precision 53 (15.9 digits) \n"
	<< "    by calling setDefaultPrecision(53,CORE_INFTY). \n";
  double one = 1;
  double three = 3;
  double Y = one / three;
  setDefaultPrecision(53, CORE_INFTY);

  std::cout << "  Output In Positional Format\n";
  std::cout << "    Cout precision 12:      " << std::setprecision(12) << Y << std::endl;
  std::cout << "    Cout precision 15:      " << std::setprecision(15) << Y << std::endl;
  std::cout << "    Cout precision 16:      " << std::setprecision(16) << Y << std::endl;
  std::cout << "        -- no guarantee that you will see more digits!" << std::endl;
  std::cout << "    Cout precision 17:      " << std::setprecision(17) << Y << std::endl;
  std::cout << "        -- you may see more because CORE has exceeded the requirements!" << std::endl;
  std::cout << "    Cout precision 18:      " << std::setprecision(18) << Y << std::endl;
  std::cout << "    Cout precision 19:      " << std::setprecision(19) << Y << std::endl;
  std::cout << "    Cout precision 20:      " << std::setprecision(20) << Y << std::endl;
  std::cout << "    Cout precision 21:      " << std::setprecision(21) << Y << std::endl;
  std::cout << "    Cout precision 22:      " << std::setprecision(22) << Y << std::endl;
  std::cout << "    Cout precision 23:      " << std::setprecision(23) << Y << std::endl;
  std::cout << "    Cout precision 24:      " << std::setprecision(24) << Y << std::endl;
  std::cout << "    Cout precision 25:      " << std::setprecision(25) << Y << std::endl;
  std::cout << "    Cout precision 26:      " << std::setprecision(26) << Y << std::endl;
  std::cout << "    Cout precision 27:      " << std::setprecision(27) << Y << std::endl;
  std::cout << "    Cout precision 28:      " << std::setprecision(28) << Y << std::endl;
  std::cout << "    Cout precision 29:      " << std::setprecision(29) << Y << std::endl;
  std::cout << "    Cout precision 30:      " << std::setprecision(30) << Y << std::endl;
  std::cout << "    Cout precision 31:      " << std::setprecision(31) << Y << std::endl;

  std::cout << "THE NUMBER OF OUTPUT DIGITS SHOULD STOP SOMEWHAT BEFORE 31 \n"
	<< "SINCE CORE EXCEEDS THE REQUESTED PRECISION BY A FACTOR of 2\n" ;

  std::cout << "To unsure that you really see 31 DIGITS, we must compute to \n"
	<< "102 relative bits of precision: by calling Y.approx(103, CORE_INFTY) \n";

  Y.approx(103, CORE_INFTY);
  std::cout << "    Cout precision 31:      " << std::setprecision(31) << Y << std::endl;
  std::cout << "    Cout precision 36:      " << std::setprecision(36) << Y << std::endl;
  std::cout << "    Cout precision 41:      " << std::setprecision(41) << Y << std::endl;
  std::cout << "    Cout precision 46:      " << std::setprecision(46) << Y << std::endl;
  std::cout << "    Cout precision 51:      " << std::setprecision(51) << Y << std::endl;

  std::cout << "Now, we do Y.approx(123, CORE_INFTY) \n";

  Y.approx(123, CORE_INFTY);
  std::cout << "    Cout precision 51:      " << std::setprecision(51) << Y << std::endl;
}
