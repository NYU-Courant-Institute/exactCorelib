//////////////////////////////////////////////////////////////////
//
// evalLagrange.h
//		-- root search class, based on EVAL and recursve Lagrange interpolation
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALLAGRANGE_H__
#define __CORE_EVALLAGRANGE_H__

#include "eval.h"

class evalLagrange : public eval {
public:
  int N;                        // floor(d/3), where d is the degree of the input polynomial f
  vector< vector<real> > data;  // data values at the interval end and mid points

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalLagrange(vector<real> v, interval I): eval(v,I) { N = f.n/3; }
  evalLagrange(): eval() { N = f.n/3; }

  //////////////////////////////////////////////////////////////
  //
  // compute all values f^(3j)(m), j=0,1,2,...
  //
  void compute_derivatives(vector<real>& d, real m) {
    vector<real> cc = f.c;	// copy coefficients of f
    
    d.resize(N+1);

	  for (int k=0, idx=0; idx<=N; k+=3, idx++) {
			// cc[k..n] are the coefficients of f^{(k)}
			real sum = 0;                  // sum = Horner eval of f^{(k)}(m)
			for (int i=f.n; i>=k; i--) {
		    sum = sum * m + cc[i];
		    cc[i] *= (i-k) * (i-k-1) * (i-k-2);  // update cc
			}
      d[idx] = sum;
		}
  }
  
  //////////////////////////////////////////////////////////////
  //
  // split interval I into left and right half
  // and pass on the references to the data
  //
  interval split_left(interval& I) {
    interval I_left(I.a,I.m(),I.level+1);
    I_left.a_data = I.a_data;
    I_left.b_data = I.m_data;
    return I_left;
  }
  interval split_right(interval& I) {
    interval I_right(I.m(),I.b,I.level+1);
    I_right.a_data = I.m_data;
    I_right.b_data = I.b_data;
    return I_right;
  }
}; // evalLagrange class
//
//////////////////////////////////////////////////////////////////
#endif
