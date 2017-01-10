/***************************************************
 * file: tCoreAux.cpp
 *
 * 	Testing some functionalities in CoreAux.cpp
 * 		stringToNumber<T>( string s)
 *		numberToString( T number)
 *	
 *	These are inverses for BigInt, BigRat
 *
 *
 * --Chee Yap (Jan 2017)
 * **************************************************
 *
 ***************************************************/


// levels should be set by LEV in Makefile:  
#define CORE_LEVEL 4
#include "CORE.h"

using namespace std;

void diag(string s, string ss, string cor, string err){
    if (s.compare(ss) == 0)
	cout << cor << endl;
    else
	cout << err << endl;
}


int main(int argc, char* argv[]) {

    int start=1022, finish=1025;
    for (int i=start; i<finish; i++){
    	cout << "int i     = " << CORE::numberToString(i) << endl;
	BigInt bi = BigInt(i);
    	cout << "  BigInt bi = " << CORE::numberToString(bi) << endl;
    	cout << "    T sqrt(bi) = " << CORE::numberToString(sqrt(bi)) << endl;
	BigRat br = BigRat(i);
    	cout << "  BigRat br = " << CORE::numberToString(br) << endl;
    	cout << "    T sqrt(br) = " << CORE::numberToString(sqrt(br)) << endl;
	BigFloat bf = BigFloat(i);
    	cout << "  BigFloat bf = " << CORE::numberToString(bf) << endl;
    	cout << "    T sqrt(bf) = " << CORE::numberToString(sqrt(bf)) << endl;
    }

    for (int i=start; i<finish; i++){
	string si = CORE::numberToString(i);
	int ii = CORE::stringToNumber<int>(si);
	string sii = CORE::numberToString(ii);
	diag(si,sii, "CORRECT! stringToNumber<int>(numberToString)",
		 "ERROR! stringToNumber<int>(numberToString)");
    }

    cout<< ">> BigInt: ========================================" << endl;
    for (int i=start; i<finish; i++){
	BigInt bi = BigInt(i*i*i);
	cout << " BigInt bi = " << bi << endl;
	string si = CORE::numberToString(bi);
	BigInt ii = CORE::stringToNumber<BigInt>(si);
	string sii = CORE::numberToString(ii);
	diag(si,sii, "CORRECT! stringToNumber<BigInt>(numberToString)",
		 "ERROR! stringToNumber<BigInt>(numberToString)");
    }

    cout<< ">> BigRat: ========================================" << endl;
    for (int i=start; i<finish; i++){
	BigRat br = BigRat(i,7);
	cout << " BigRat br(i,7) = " << br << endl;
	string sr = CORE::numberToString(br);
	br = CORE::stringToNumber<BigRat>(sr);
	string srr = CORE::numberToString(br);
	diag(sr,srr, "CORRECT! stringToNumber<BigRat>(numberToString)",
		 "ERROR! stringToNumber<BigRat>(numberToString)");
    }

    cout<< ">> BigFloat: ========================================" << endl;
    for (int i=start; i<finish; i++){
	BigFloat bf = BigFloat((double)i/7);
	cout << " BigFloat bf((double)i/7) = " << bf << endl;
	string sf = CORE::numberToString(bf);
	bf = CORE::stringToNumber<BigFloat>(sf);
	string sff = CORE::numberToString(bf);
	diag(sf,sff, "CORRECT! stringToNumber<BigFloat>(numberToString)",
		 "ERROR! stringToNumber<BigFloat>(numberToString)");
    }

}//main
