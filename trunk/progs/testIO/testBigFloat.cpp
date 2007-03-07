/* ************************************************
   File: testBigFloat.cpp

   Purpose:  Tests I/O for BigFloat and BigFloat2

   		Strings can be in scientific or positional notation

   Usage:
        % testBigFloat

   Author: Jihun and Chee Yap

   Since Core Library 2.0
   $Id: testBigFloat.cpp,v 1.1 2007-03-07 20:26:25 exact Exp $
 ************************************************ */  

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

using namespace std;

using namespace CORE;

//////////////////////////////////////////////////
// test
//////////////////////////////////////////////////

/*
template <class T1, class T2>
void test(T1 ans, T2 unknown, string msg1 = "", msg2 = "") {
 if (ans != unknown) {
	 cout << "ERROR!!! " << msg1 << endl;
	 cout << "  answer : " << ans << endl;
	 cout << "  unknown : " << unknown << endl;
 }
 else if (coretest_verbose) {
	 cout << "CORRECT!!! " << msg2 << endl;
	 cout << "  answer : " << ans << endl;
	 cout << "  unknown : " << unknown << endl;
 }
 coretest_error = true;
}
*/	

//////////////////////////////////////////////////
// main
//////////////////////////////////////////////////
int main( int argc, char *argv[] ) {

  // Global unit test variables:
  coretest_error=false;
  coretest_verbose=false;

  if (argc>1) coretest_verbose = true;
  
  // get exponent:
  BigFloat f("0"); 		// f=0
  core_test(0, f.get_exp(), "f.get_exp() is wrong");
  			// REMARK: this is really only a convention.
  f = BigFloat("1"); 		// f=1
  core_test(1, f.get_exp(), "f.get_exp() is wrong");
  f = BigFloat("0.5"); 		// f=1/2
  core_test(0, f.get_exp(), "f.get_exp() is wrong");
  f = BigFloat("0.125"); 	// f=1/8
  core_test(-2, f.get_exp(), "f.get_exp() is wrong");
  f = BigFloat("1.125");	 // f=9/8
  core_test(1, f.get_exp(), "f.get_exp() is wrong");
  
  // get mantissa:
  f = BigFloat("31459");
  BigInt z;
  f.get_z_exp(z);
  core_test("31459", z.get_str(), "f.get_z_exp() is wrong");
  //
  
  // BigFloat2 string input:
  BigFloat2 f2("0.0007891", 10); 
  core_test("0.0007891", f2.get_str(), "BigFloat string input is wrong");
  //
  
  f = "0.0625";
  f2 = BigFloat2(f);
  core_test(".0625", f2.get_str(), "BigFloat2.get_str is wrong");

  // BigFloat2 string input:
  core_test(".0007891", f2.get_str(4), "BigFloat2 string input is wrong");
  //

  if (coretest_error = false)
    cout << "CORRECT!!! all test are passed" << endl;

  return 0;
}//main
