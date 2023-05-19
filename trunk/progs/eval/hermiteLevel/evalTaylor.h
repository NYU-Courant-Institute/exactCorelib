//////////////////////////////////////////////////////////////////
//
// evalTaylor.h
//		-- root search class, based on EVAL and Taylor expansion
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALTAYLOR_H__
#define __CORE_EVALTAYLOR_H__

#include "eval.h"

class evalTaylor : public eval {
public:
  int n;              // degree of the input polynomial f(x)
  vector<real> c;     // coefficients c_i of the Taylor expansion of f(x) about m

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalTaylor(vector<real> v, interval I): eval(v,I) { n = f.n; c.resize(n+1); }
  evalTaylor(): eval() { n = f.n; c.resize(n+1); }

  //////////////////////////////////////////////////////////////
  //
  // compute coefficients c_i of the Taylor expansion of f(x) about m,
  // so that f(x) = \sum_{i=0}^n c_i (x-m)^i
  //
  void compute_Taylor_coefficients(real m) {
    vector<real> cc = f.c;	// copy coefficients of f
    
	  for (int k=0; k<=n; k++) {
			// cc[k..n] are the coefficients of f^{(k)}/k!
			real sum = 0; // sum = Horner eval of f^{(k)}(m)/k!
			for (int i=n; i>=k; i--) {
		    sum = sum * m + cc[i];
		    cc[i] = (cc[i] * (i-k)) / (1+k); // update cc
			}
			c[k] = sum;
		}
  }

  //////////////////////////////////////////////////////////////
  //
  // split interval I into left and right half
  //
  interval split_left(interval& I) {
    return interval(I.a,I.m(),I.level+1);
  }
  interval split_right(interval& I) {
    return interval(I.m(),I.b,I.level+1);
  }
}; // evalTaylor class
//
//////////////////////////////////////////////////////////////////
#endif
