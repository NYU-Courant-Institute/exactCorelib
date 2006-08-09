/* ************************************************
   File: testIO.cpp

   Purpose:  To test the numerical output routines.
	     These are considered very important functions
	     in our library, as the integrity of the
	     internal computation cannot be verified
	     unless we can exactly control the input and
	     output of numbers. 
   Usage:
        % testIO

   Author: Chee Yap (yap@cs.nyu.edu)

   Since Core Library 1.4
   $Id: testIO.cpp,v 1.2 2006-08-09 09:55:39 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

int main( int argc, char *argv[] ) {

//  CORE_init(8);	// This method is recommended in all CORE programs.
		// The parameter 8 sets the std::cout precision to 8.
  setDefaultOutputDigits(8);

  double one = 1.0;
  double three = 3.0;
  double one_third =  one / three;

  std::cout << "OUTPUT FOR THE Expr VALUE 1.0/3.0\n";
  std::cout << "  computed to relative precision 120: \n" ;
  setDefaultPrecision(120, CORE_posInfty);

  std::cout << "  (1) Output In Positional Notation\n";
  std::cout << "    Default std::cout precision (=8): " << one_third << std::endl;
  std::cout << "    Cout precision 12:      " << std::setprecision(12) << one_third << std::endl;
  std::cout << "    Cout precision 36:      " << std::setprecision(36) << one_third << std::endl;
  std::cout << "    Cout precision 37:      " << std::setprecision(37) << one_third << std::endl;
  std::cout << "    Cout precision 38:      " << std::setprecision(38) << one_third << std::endl;
  std::cout << "    Cout precision 39:      " << std::setprecision(39) << one_third << std::endl;
  std::cout << "    Cout precision 40:      " << std::setprecision(40) << one_third << std::endl;
  std::cout << "    Cout precision 41:      " << std::setprecision(41) << one_third << std::endl;
  std::cout << "    Cout precision 42:      " << std::setprecision(42) << one_third << std::endl;
  std::cout << "    Cout precision 43:      " << std::setprecision(43) << one_third << std::endl;
  std::cout << "    Cout precision 44:      " << std::setprecision(44) << one_third << std::endl;
  std::cout << "    Cout precision 46:      " << std::setprecision(46) << one_third << std::endl;
  std::cout << "    Cout precision 48:      " << std::setprecision(48) << one_third << std::endl;
  std::cout << "  N.B. 120 bits = 36.123 digits.  Above, we have asked to see \n";
  std::cout << "  more and more bits of the computed value.  The output will indeed \n";
  std::cout << "  show more and more bits, until all significant bits are used up.\n";
  std::cout << "  This should happen AFTER 37 digits but hopefully before 48 digits\n";

  std::cout << "  (2) Output In Scientific Notation\n";
  // presently scientific manipulator is not supported by g++
  	std::cout << std::setprecision(10);
  	std::cout.setf(std::ios::scientific, std::ios::floatfield);
  std::cout << "    Default Cout precision:" << one_third << std::endl;
  std::cout << "    Cout precision 12:  " << std::setprecision(12) << one_third << std::endl;
  std::cout << "    Cout precision 24:  " << std::setprecision(24) << one_third << std::endl;
  std::cout << "    Cout precision 25:  " << std::setprecision(25) << one_third << std::endl;
  std::cout << "    Cout precision 36:  " << std::setprecision(36) << one_third << std::endl;
  std::cout << "    Cout precision 48:  " << std::setprecision(48) << one_third << std::endl;

  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout << std::setprecision(10);

  std::cout << "  (3) Default Print Digits (a Core library parameter)\n";
  std::cout.precision(6);
  std::cout << "Output precision: " << std::cout.precision() 
       << ", precision [r=67, a=INFTY]" << std::endl;
  std::cout << "    one_third = " << one_third << std::endl << std::endl; 

  std::cout.precision(20);
  std::cout << "Output precision: " << std::cout.precision() 
       << ", precision [r=67, a=INFTY]" << std::endl;
  std::cout << "    one_third = " << one_third << std::endl << std::endl;

  std::cout.precision(30);
  std::cout << "The precision is not sufficient for 30 decimal digits!" << std::endl;
  std::cout << "Output precision: " << std::cout.precision() 
       << ", precision [r=67, a=INFTY]" << std::endl;
  std::cout << "one_third = " << one_third  << std::endl << std::endl;

  std::cout << "Set the precision to [r=100, a=INFTY] to get it right: " << std::endl;
  setDefaultPrecision(100, CORE_posInfty);
  std::cout << "Output precision: " << std::cout.precision() 
       << ", precision [r=100, a=INFTY]" << std::endl;
  std::cout << "one_third = " << one_third << std::endl << std::endl; 

  // test rounding and etc...
  setDefaultPrecision(54, CORE_posInfty);
  std::cout.precision(16);
  std::cout << "Precisions: [54, INFTY] (simulating machine double precision) \n"
       << "           This print out 16 digits..." << std::endl;
  std::cout.precision(10);
  double e0 = 0.01234;
  double e1 = 0.00000000012345;
  double e2 = 0.0000000001234567890123;
  double e3 = 1.2;
  double e4 = 123.456;
  double e5 = 123456789;
  double e6 = 123456789.0123456;
// debugging:
  double e7 = BigFloat("1234567890123.456");
  double e8 = 1234567890123456.0;
  double e9 = 0.023;
  double e10 = 123456789.50001;
  double e11 = 123456789.49999;
  double e12 = 199999999.50001;
  double e13 = 999999999.50001;
    
  double ne0 = -.01234;
  double ne1 = -0.00000000012345;
  double ne2 = -0.0000000001234567890123;
  double ne3 = -1.2;
  double ne4 = -123.456;
  double ne5 = -123456789;
  double ne6 = -123456789.0123456;
  double ne7 = -1234567890123.456;
  double ne8 = -1234567890123456.0;       
  double ne9 = -0.023;

  std::cout << " e0 ( 0.01234                   ) : " << e0 << std::endl;
  std::cout << " e1 ( 0.00000000012345          ) : " << e1 << std::endl;
  std::cout << " e2 ( 0.0000000001234567890123  ) : " << e2 << std::endl;
  std::cout << " e3 ( 1.2                       ) : " << e3 << std::endl;
  std::cout << " e4 ( 123.456                   ) : " << e4 << std::endl;
  std::cout << " e5 ( 123456789                 ) : " << e5 << std::endl;
  std::cout << " e6 ( 123456789.0123456         ) : " << e6 << std::endl;
  std::cout << " e7 ( 1234567890123.456         ) : " << e7 << std::endl;
  std::cout << " e8 ( 1234567890123456.0        ) : " << e8 << std::endl;
  std::cout << " e9 ( 0.023                     ) : " << e9 << std::endl;
#if CORE_LEVEL == 1
  std::cout << " e9 ( 0.023                     ) : " << std::setprecision(30) <<e9 << std::endl;
#endif
  std::cout << " e10 ( 123456789.50001          ) : " << e10 << std::endl;
  std::cout << " e11 ( 123456789.49999          ) : " << e11 << std::endl;
  std::cout << " e12 ( 199999999.50001          ) : " << e12 << std::endl;
  std::cout << " e13 ( 999999999.50001          ) : " << e13 << std::endl;
  std::cout << " ne0 ( -.01234                  ) : " << ne0 << std::endl;
  std::cout << " ne1 ( -0.00000000012345        ) : " << ne1 << std::endl;
  std::cout << " ne2 ( -0.0000000001234567890123) : " << ne2 << std::endl;
  std::cout << " ne3 ( -1.2                     ) : " << ne3 << std::endl;
  std::cout << " ne4 ( -123.456                 ) : " << ne4 << std::endl;
  std::cout << " ne5 ( -123456789               ) : " << ne5 << std::endl;
  std::cout << " ne6 ( -123456789.0123456       ) : " << ne6 << std::endl;
  std::cout << " ne7 ( -1234567890123.456       ) : " << ne7 << std::endl;
  std::cout << " ne8 ( -1234567890123456.0      ) : " << ne8 << std::endl;
  std::cout << " ne9 ( -0.023                   ) : " << ne9 << std::endl;
  std::cout << std::endl;

#if CORE_LEVEL == 3
/*
  // test Reals
  std::cout << "test reals" << std::endl;
  BigRat R(1, 3);
  Real R1("1/3");
  //std::cout << "error before here?" << std::endl;//BigRat R2("1/3");
  BigFloat B(R);
  std::cout.precision(8);
  Real Q = R;
  Real X = B;

  std::cout << "BigRat Real Q(1/3): " << Q << std::endl;
  std::cout << "Real R1(\"1/3\"): " << R1 << std::endl;
  std::cout << "BigFloat Real X(1/3): " << X << std::endl;
*/
  // test square root
  std::cout.precision(10);
  setDefaultPrecision(35, CORE_posInfty); // bug?
  double two_over_three = BigRat(2, 3);
  std::cout << "sqrt(2/3) = " << sqrt(two_over_three) << std::endl << std::endl;;
  
  // show the differnce between the use of string and double constant.
  std::cout.precision(20);
  setDefaultPrecision(67, CORE_posInfty);

  defInputDigits = 20;
  double exact1 = BigFloat("0.023");
  double inexact1 = 0.023; // this can not be represented in banary exactly.

  double exact2 = BigFloat("1234.567e-5");
  double inexact2 = 1234.567e-5;

  std::cout << "The literal double constant is not always exact:" << std::endl; 
  std::cout << "construct from double constant - (0.023): " << inexact1 << std::endl;
  std::cout << "construct from string - (0.023): " << exact1 << std::endl;
  std::cout << "construct from double constant - (1234.567e-5): " << inexact2 << std::endl;
  std::cout << "construct from string - (1234.567e-5): " << exact2 << std::endl;

  // =============================
  BigRat RR(100010, 100010);
  std::cout << "100010/100010 = " << RR << std::endl;
  // =============================

  std::cout.precision(20);
  setDefaultPrecision(67, CORE_posInfty);
  double r1 = "0.123456789";
  std::cout << "r1 = " << r1 << std::endl;
  double r2("0.1234567890123456789", CORE_posInfty);
  std::cout << "r2 = " << std::setprecision(24) << r2 << ", read in exactly" << std::endl;
  double r3("0.1234567890123456789", 9);
  std::cout << "r3 = " << std::setprecision(24) << r3 << ", read in with precision 10^{-9}" << std::endl;
#ifdef INTERACTIVE
  double r4;
  setDefaultInputDigitsision(5);
  std::cout << "r4 = ";
  std::cin >> r4;
  std::cout << "r4 = " << r4 << ", with an error less than 10^{-5}" << std::endl;
  double r5;
  setDefaultInputDigits(CORE_posInfty);
  std::cout << "r5 = ";
  std::cin >> r5;
  std::cout << "r5 = " << r5 << ", without any error in reading-in" << std::endl;
#endif // INTERACTIVE

double q = BigRat(1, 3);
setDefaultAbsPrecision(67); // about 20 digits
std::cout << "q = " << std::setprecision(10) << q << ", in 10 digits" << std::endl;
std::cout.precision(30); // default to output 30 digits.
std::cout.setf(std::ios::scientific, std::ios::floatfield); // use scientific notation
std::cout << "q = " << q << ", in scientific notation." << std::endl;
// std::cout.setf(0, std::ios::floatfield); // reset the format to default.
setPositionalFormat();
std::cout << "q = " << q << ", in positional notation." << std::endl;

// scientific format

  double s1 = 1.234e-1;
  std::cout << "s1 = 1.234e-1; in machine precision, s1 =" << s1 << std::endl;

  defInputDigits = 20;
  double s2 = "1.234e-1";
  std::cout << "s2 = \"1.234e-1\"; with defInputDigits = 20, , s2 =" << s2 << std::endl;

// rational format

  double a1 = 123/456;
  std::cout << "a1 = 123/456; in machine precision, a1 =" << a1 << std::endl;

  defInputDigits = 20;
  double a2 = "123/456";
  std::cout << "a2 = \"123/456\"; with defInputDigits = 20, a2 = " << a2 << std::endl;

  defInputDigits = CORE_INFTY;
  double a3 = "123/456";
  std::cout << "a3 = \"123/456\"; with defInputDigits = INFTY, a3 = " << a3 << std::endl;


#endif //LEVEL 3

  return 0;

}//main
