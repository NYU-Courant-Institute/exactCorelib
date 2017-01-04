
/***************************************************
 *	file: coreid.cpp
 *
 * 	We use C++'s typeinfo system to tell us the "real" type
 * 	of Core numbers and other objects:
 *
 *		     cout << "Type T = " << typeid(T).name() << endl;
 *
 * 	This is useful for debugging.
 * 	Here are the outputs in each CORE_LEVEL:
 *
 * 	CORE_LEVEL 1:
 *		>>>  Core Number Types:
 *		Test type int = i
 *		Test type long = l
 *		Test type float = f
 *		Test type double = N4CORE13DoubleWrapperE
 *		>>>
 *		Test type BigInt = l
 *		Test type BigRat = N4CORE13DoubleWrapperE
 *		Test type BigFloat = N4CORE13DoubleWrapperE
 *		Test type BFInterval = St4pairIN4CORE8BigFloatES1_E
 *		Test type BigFloat2 = N4CORE9BigFloat2E
 *		Test type Expr = N4CORE13DoubleWrapperE
 *
 * 	CORE_LEVEL 2:
 *		>>>  Core Number Types:
 *		Test type int = i
 *		Test type long = N4CORE6BigIntE
 *		Test type float = f
 *		Test type double = N4CORE8BigFloatE
 *		>>>
 *		Test type BigInt = N4CORE6BigIntE
 *		Test type BigRat = N4CORE6BigRatE
 *		Test type BigFloat = N4CORE8BigFloatE
 *		Test type BFInterval = St4pairIN4CORE8BigFloatES1_E
 *		Test type BigFloat2 = N4CORE9BigFloat2E
 *		Test type Expr = N4CORE8BigFloatE
 *
 *
 * 	CORE_LEVEL 3:
 *		>>>  Core Number Types:
 *		Test type int = i
 *		Test type long = N4CORE6BigIntE
 *		Test type float = f
 *		Test type double = N4CORE5ExprTINS_19BfmsskRootBd_doubleINS_9BigFloat2EEENS_9BfsFilterIS2_EES2_EE
 *		>>>
 *		Test type BigInt = N4CORE6BigIntE
 *		Test type BigRat = N4CORE6BigRatE
 *		Test type BigFloat = N4CORE8BigFloatE
 *		Test type BFInterval = St4pairIN4CORE8BigFloatES1_E
 *		Test type BigFloat2 = N4CORE9BigFloat2E
 *		Test type Expr = N4CORE5ExprTINS_19BfmsskRootBd_doubleINS_9BigFloat2EEENS_9BfsFilterIS2_EES2_EE
 *
 * 	CORE_LEVEL 4:
 *		>>>  Core Number Types:
 *		Test type int = i
 *		Test type long = l
 *		Test type float = f
 *		Test type double = d
 *		>>>
 *		Test type BigInt = N4CORE6BigIntE
 *		Test type BigRat = N4CORE6BigRatE
 *		Test type BigFloat = N4CORE8BigFloatE
 *		Test type BFInterval = St4pairIN4CORE8BigFloatES1_E
 *		Test type BigFloat2 = N4CORE9BigFloat2E
 *		Test type Expr = N4CORE5ExprTINS_19BfmsskRootBd_doubleINS_9BigFloat2EEENS_9BfsFilterIS2_EES2_EE
 *
 *	--Chee Yap (Jan 2017)
 ***************************************************/


#include<typeinfo>


// levels should be set by LEV in Makefile:  
//   Jan2017: the current Makefile seems not to set this right...
#define CORE_LEVEL 4
#include "CORE.h"

using namespace std;

template<typename T>
	void test(string msg = "") {
		cout << "Test type " 
		     << msg << " = "
		     << typeid(T).name()
		     << endl;
	}

int main(int argc, char* argv[]) {


  cout << ">>>  Core Number Types:" << endl;
  test<int>("int");
  test<long>("long");
  test<float>("float");
  test<double>("double");

  cout << ">>> " << endl;
  test<BigInt>("BigInt");
  test<BigRat>("BigRat");
  test<BigFloat>("BigFloat");
  test<BFInterval>("BFInterval");
  test<BigFloat2>("BigFloat2");
  test<Expr>("Expr");

}//main
