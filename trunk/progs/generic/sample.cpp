/************************************************************
  file: sample.cc
  purpose:
	a simple sample CORE program
  CORE Library 
  $Id: sample.cpp,v 1.1 2006-03-07 04:51:24 exact Exp $
************************************************************/
 
#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

// Important: place all standard header files BEFORE "CORE.h"
#include "CORE.h"

int main(int argc, char* argv[]) {

  double x = 1.44;	// Inexact input; C++ literal input precision

  defInputDigits = 20;  // This should be better than C++ precision
  double y = "1.44";	// Use of string constructor 

  defInputDigits = CORE_INFTY; // force exact input 
  double z = "1.44";	// z is represented as a rational number
  
  int p = setDefaultOutputDigits(); // p = defOutputDigits
  std::cout << "    C++'s Default Printout Digits is " << p << std::endl;
  std::cout << "    CORE Default Printout Digits is " << defOutputDigits << std::endl;

  std::cout << "    x = 1.44;  // standard C++ literal input" << std::endl;
  std::cout << "Printout of x: " << x << std::endl;
  std::cout << "      sqrt(x): " << sqrt(x) << std::endl;
  std::cout << "==================================================" << std::endl;

  std::cout << std::setprecision(6) ;
  std::cout << "    y = \"1.44\". // string literal; defInputDigits = 20\n";
  std::cout << "    Set CORE Printout Digits to 6 " << std::endl;
  std::cout << "Printout of y: " << y << std::endl;
  std::cout << "      sqrt(y): " << sqrt(y) << std::endl;
  std::cout << "==================================================" << std::endl;

  std::cout << std::setprecision(6) ;
  std::cout << "    z = \"1.44\". // string literal; defInputDigits = Infinite\n";
  std::cout << "    Set CORE Printout Digits to 6 " << std::endl;
  std::cout << "Printout of z: " << z << std::endl;
  std::cout << "      sqrt(z): " << sqrt(z) << std::endl;
  std::cout << "==================================================" << std::endl;
  std::cout << "    Above, x, y and z look the same."
	<< "\n    To see that they are not the same, we force more"
	<< "\n    precision in evaluation and in output:" << std::endl;

  std::cout << "---Set defRelPrec 133 bits = 40 digits"
	<< "\n ---Set defPrintDigits to 40 " << std::endl;
  defRelPrec = 133; // this gives 40 digits of precision
  setDefaultOutputDigits(40);

  std::cout << "sqrt(x) = " << sqrt(x) << std::endl;
  std::cout << "sqrt(y) = " << sqrt(y) << std::endl;
  std::cout << "sqrt(z) = " << sqrt(z) << std::endl;
  std::cout << "==================================================" << std::endl;

  std::cout << "    Even though sqrt(z) looks like 1.2, the only way to"
	<< "\n   verify this is to do a comparison with 1.2" << std::endl;

  double sqrtz = "1.2";  // exact input

  if (sqrt(z) == sqrtz)
	std::cout << "CORRECT!  sqrt(z) = 1.2 exactly" << std::endl;
  else
	std::cout << "ERROR!  sqrt(z) != 1.2 " << std::endl;
  std::cout << "==================================================" << std::endl;

  return (0);
}
