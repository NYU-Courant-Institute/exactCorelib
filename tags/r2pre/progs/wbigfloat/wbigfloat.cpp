/* **************************************
   File: wbigfloat.cpp

   Purpose: This is a simple example using weak bigfloat number type
	to control output precision of four aritmetic operations
        - addition, subtraction, multiplication, division
        set_wbf_mode(bool b) turns on and off the weak bigfloat mode.
        set_wbf_prec(prec p) specifies the desired output precision.

        Example:

        set_wbf_prec(100);
        A = B*C;

        In weak bigfloat mode, the precision of A will be set to 100, such that
        the accuracy of multiplication is guaranteed to 100-bit relative precision.

   Since CORE Library Version 2.0
   $Id: wbigfloat.cpp,v 1.1 2007-10-22 19:34:38 exact Exp $
************************************** */
#define CORE_LEVEL 2

#include "CORE/CORE.h"

using namespace std;

int main(int argc, char** argv) {
  set_wbf_mode(true);
  
  BigFloat x(1), y(3), z;

  set_wbf_prec(3);

  z = x / y;
  cout << z.get_prec() << endl;
  cout << z << endl;

  set_wbf_prec(10);
  z = x / y;
  cout << z.get_prec() << endl;
  cout << z << endl;
}
