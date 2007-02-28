/* ************************************************
   File: testCompatible.cpp

   Purpose:  Given two decimal strings, are they compatible?

   Usage:
        % testCompatible

   To Do: Make these routines "self-validating".

   Author: Chee Yap (yap@cs.nyu.edu)

   Since Core Library 1.4
   $Id: testCompatible.cpp,v 1.1 2007-02-28 19:02:58 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

void test (string& strIn, string& strAns, bool answer) {
  if (isCompatible(strIn, strAns) == answer)
    cout << "CORRECT!!! isCompatible(" << strIn << ", "
	    << strAns << ") = " << answer << endl;
  else {
    string digitIn, digitAns;
    int exponentIn, exponentAns;

    getDigits(strIn, digitIn, exponentIn);
    getDigits(strAns, digitAns, exponentAns);
    
    cout << "ERROR!!! isCompatible is wrong" << endl;
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

int main( int argc, char *argv[] ) {

  BigInt temp;
  temp.pow(10, 0);
  cout << "temp = " << temp << std::endl;

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
	  cout << "ERROR getUncertainty is wrong" << std::endl;
  else
	  cout << "CORRECT getUncertainty" << std::endl;

  
  getDigits(stringIn, digitIn_, exponentIn_);
  if (digitIn != digitIn_ || exponentIn != exponentIn_)
	  cout << "ERROR getDigits is wrong" << std::endl;
  else
	  cout << "CORRECT getDigits" << std::endl;
  
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

  if (argc>1) stringIn=argv[1];
  if (argc>2) stringAns=argv[2];
  if (argc>3) answer=atoi(argv[3]);

  test(stringIn, stringAns, answer);
}//main
