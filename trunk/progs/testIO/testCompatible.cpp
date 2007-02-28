/* ************************************************
   File: testCompatible.cpp

   Purpose:  Tests if two decimal strings, are they compatible?
   		Strings can be in scientific or positional notation

   Usage:
        % testCompatible
	% testCompatible [string1] [string2] [compatible]
	% testCompatible 12.34 1234e-2 true

   Theory: Here is the definition of compatibility
     For any decimal string SX, it has a nominal value X
     and also an uncertainty UX.

     E.g., SX= 1.234 then X=1.234 and UX=0.001
     E.g., SX= 1200e-2 then X=12.00 and UX=0.01
     E.g., SX= 12 then X=12 and UX=1

 	The string SX then defines an interval [X-UX, X+UX].
 
     When we say two strings, SX and SY are compatible, we mean
     that the corresponding intervals overlap.

     To decide compatibility of SX and SY, we first compute
     X, UX, Y, UY.  Wlog, let X <= Y.  Then
     SX and SY are compatible iff
           X + UX >= Y - UY.
     REMARK: we could try to work with the stricter notion with
     	   X + UX > Y - UY
     but it might cause trouble with some of our other implementations.

     E.g., 1.200 is compatible with 1.19999 and 1.201
     E.g., 1.234 is compatible with 1.235 and 1.233

   Author: Jihun and Chee Yap

   Since Core Library 2.0
   $Id: testCompatible.cpp,v 1.3 2007-02-28 19:47:06 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

//////////////////////////////////////////////////
// test routine
//////////////////////////////////////////////////
void test (string& strIn, string& strAns, bool answer) {
  if (isCompatible(strIn, strAns) == answer)
    cout << "CORRECT!!! isCompatible(" << strIn << ", "
	    << strAns << ") = " << answer << endl;
  else {
    cout << "ERROR!!! isCompatible(" << strIn << ", "
	    << strAns << ") <> " << answer << endl;
    string digitIn, digitAns;
    int exponentIn, exponentAns;

    getDigits(strIn, digitIn, exponentIn);
    getDigits(strAns, digitAns, exponentAns);
    
    cout << "Input String = " << strIn << endl;
    cout << "\tUncertainty =" << getUncertainty(strIn) << endl;
    cout << "\tDigits =" << digitIn << endl;
    cout << "\tExponents =" << exponentIn << endl;
    cout << "Answer String = " << strAns << endl;
    cout << "\tUncertainty =" << getUncertainty(strAns) << endl;
    cout << "\tDigits =" << digitAns << endl;
    cout << "\tExponents =" << exponentAns << endl;
  }
}	

//////////////////////////////////////////////////
// main
//////////////////////////////////////////////////
int main( int argc, char *argv[] ) {

  string stringIn = "123.456";
  string stringAns = "123.45555";

  string digitIn = "123456";
  string digitIn_;

  int exponentIn = -3;
  int exponentIn_;

  int uIn = -3;
  int uAns = -5;

  if (getUncertainty(stringIn) != uIn ||
      getUncertainty(stringAns) != uAns)
	  cout << "ERROR getUncertainty is wrong" << endl;
  else
	  cout << "CORRECT getUncertainty" << endl;

  
  getDigits(stringIn, digitIn_, exponentIn_);
  if (digitIn != digitIn_ || exponentIn != exponentIn_)
	  cout << "ERROR getDigits is wrong" << endl;
  else
	  cout << "CORRECT getDigits" << endl;
  
  bool answer=true;

  stringIn = "1.23400e-5";
  stringAns = "12.3399e-6";
  answer = true;
  test(stringIn, stringAns, answer);

  stringIn =  "1.23400e-5";
  stringAns = "1.23398e-5";
  answer = true;// if we use our current "weak notion" of compatible
  		// if we use the strong notion, it would be incompatible
  test(stringIn, stringAns, answer);
  stringAns = "1.23397e-5";
  answer = false;
  test(stringIn, stringAns, answer);

  stringIn = "+12";
  stringAns = "13";
  answer = true;
  test(stringIn, stringAns, answer);
  stringIn = "-12";
  answer = false;
  test(stringIn, stringAns, answer);
  stringAns = "-15";
  answer = false;
  test(stringIn, stringAns, answer);
  stringAns = "-13";
  answer = true;
  test(stringIn, stringAns, answer);

  stringIn = "+00012.5";
  stringAns = "013";
  answer = true;
  test(stringIn, stringAns, answer);

  stringIn = "0";
  stringAns = "1";
  answer = true;
  test(stringIn, stringAns, answer);

  stringIn = "0.0";
  stringAns = "0.12";
  answer = false;
  test(stringIn, stringAns, answer);

  if (argc>1) {
    stringIn=argv[1];
    if (argc>2) stringAns=argv[2];
    if (argc>3) answer=atoi(argv[3]);
    test(stringIn, stringAns, answer);
  }
}//main
