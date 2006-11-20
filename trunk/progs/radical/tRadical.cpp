/*************************************************************
 * File: tRadical.cpp
 * Purpose:
 * 	Verify that the basic functions of radical(n,m) which
 * 	constructs the m-th root of n is working.
 *
 * Usage:   
 * 		> ./tRadical [n=3] [m=3] [prec=40]
 *
 * 	where the default values are shown above.
 * 	If n<0, then we do a battery of self-tests and
 * 	verify that the result is correct.
 *
 * Restriction/Bugs:
 * 	n must be an natural number (BigInt or int)
 * 	m must be >= 1
 * 	
 * 	If m is too big, this test will be really slow.
 * 	Currently, 
 * 	        > ./tRadical 123 30
 * 	is reasonably "real time" but
 * 	        > ./tRadical 123 40
 * 	takes about 2 minutes on a Pentium 1.4 GHz laptop.
 * 	Time is somewhat less sensitive to large n.
 *
 * Author: Chee Yap, Vikram Sharma
 * Date:  May 22, 2003
 *
 * Since Core Library version 1.6
 *************************************************************/

#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

// Default number type is NT=BigInt
#if defined _NT_INT
  typedef int NT;
#elif defined _NT_LON
  typedef long NT;
#elif defined _NT_EXP
  typedef Expr NT;
#elif defined _NT_RAT
  typedef BigRat NT;
#elif defined _NT_BF
  typedef BigFloat NT;
#elif defined _NT_BIGINT
  typedef BigInt NT;
#else
  typedef BigInt NT;
#endif


//template Expr::Expr(const Polynomial<NT>& p, int n);
//template ConstPolyRep<NT>;

int main(int argc , char ** argv) {
  int outputPrec;
  int eps;
  
  NT n = 3;
  if (argc > 1) {
    n = atoi(argv[1]);
  }

  int m = 3;
  if (argc > 2) {
    m = atoi(argv[2]);
  }

  outputPrec =  40;	// default output precision
  if (argc > 3) {
    outputPrec = atoi(argv[3]);
  }

  eps = (int)(outputPrec*log(10.0)/log(2.0)); 
  cerr << "outputPrec = " << outputPrec << ", eps = " << eps << endl;
 
  setDefaultOutputDigits(outputPrec);

  if (n >= 0) {
     Expr mthrootn = radical(n,m);
     mthrootn.approx(1000);
     cout << "The " << m << "th root of "<< n << " is "
	     << mthrootn.approx() << endl;
   
     Expr p = power(mthrootn, m);
     cout << "The " << m << "-th power of " << mthrootn << " is " << endl;
     cout << "    P = " << p.approx() << endl;
   
     if(p == Expr(n))
       cout << "CORRECT! P is equal to " << n << endl; 
     else
       cout << "ERROR! P is NOT equal to " << n << endl; 
     
  } else { // if n < 0, do the automatic test
     Expr r, rr;
     if ((radical(NT(1), 1000) == 1) && (radical(NT(1), 1) ==1)) 
	   cout << "CORRECT! for radical(1,k), k=1, 1000" << endl;
	else 
	   cout << "ERROR! for radical(1,k), k=1, 1000" << endl;
     if ((radical(NT(2), 1) == 2) && (radical(NT(12345), 1) == 12345)) 
	   cout << "CORRECT! for radical(m,1), m=2, 12345" << endl;
	else 
	   cout << "ERROR! for radical(m,1), m=2, 12345" << endl;
     for (int i=1; i<11; i++) {
        r = radical(NT(2), i);
	rr = radical(NT(789), i);
	if ((power(r,i) != 2) || (power(rr,i) != 789)){
	   cout << "ERROR! for radical(m,i), (m=2,789), i = " << i << endl;
	} else {
	   cout << "CORRECT! for radical(m,i), (m=2,789), i = " << i << endl;
	}
     }
     for (int i=1000; i<1011; i++) {
        r = radical(NT(i), 7);
	rr = radical(NT(i), 20);
	if ((power(r,7) != i) || (power(rr,20) != i)){
	   cout << "ERROR! for radical(i,k), (k=7,20), i = " << i << endl;
	} else {
	   cout << "CORRECT! for radical(i,k), (k=7,20), i = " << i << endl;
	}
    }
/*
#ifdef _NT_BF
     BigFloat bf(283243346, 0, -4);
     BigFloat bf2(1234, 0, -5);

     for (int i=0; i<100; i++) {
	     bf += bf2;
	     Expr e = radical(bf, 2);
	     Expr p = power(e, 2);
	     cout << "bf=" << bf << endl;
	     cout << "e=" << e << endl;
	     cout << "p=" << p << endl;
	     if (Expr(bf) == p) {
		     cout << "CORRECT!" << endl;
	     } else {
		     cout << "ERROR!" << endl;
	     }
     }
#endif
*/  }
}
