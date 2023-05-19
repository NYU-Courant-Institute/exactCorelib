//////////////////////////////////////////////////////////////////
//
// evalHermite.h
//		-- root search class, based on EVAL and recursive Hermite interpolation
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALHERMITE_H__
#define __CORE_EVALHERMITE_H__

#include "eval.h"

class evalHermite : public eval {
public:
  int N;                        // floor(d/4), where d is the degree of the input polynomial f
  vector< vector<real> > data;  // data values at the interval end and mid points

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalHermite(vector<real> v, interval I): eval(v,I) { N = f.n/4; }
  evalHermite(): eval() { N = f.n/4; }

  //////////////////////////////////////////////////////////////
  //
  // compute all values f^(j)(m), j=0,1,4,5,8,9,...
  //
  void compute_derivatives(vector<real>& d, real m) {
    vector<real> cc = f.c;	// copy coefficients of f
    
    d.resize(2*(N+1));

    for (int k=0, idx=0; idx<=2*N; k+=4, idx+=2) {
	  // cc[k..n] are the coefficients of f^{(k)}
	    real sum = 0;                  // sum = Horner eval of f^{(k)}(m)
      for (int i=f.n; i>=k; i--) {
        sum = sum * m + cc[i];
        cc[i] *= (i-k);  // update cc
      }
      d[idx] = sum;

	  // cc[k+1..n] are the coefficients of f^{(k+1)}
	    sum = 0;                  // sum = Horner eval of f^{(k+1)}(m)
      for (int i=f.n; i>=k+1; i--) {
        sum = sum * m + cc[i];
        cc[i] *= (i-k-1) * (i-k-2) * (i-k-3);  // update cc
      }
      d[idx+1] = sum;
    }
  }
  
  //////////////////////////////////////////////////////////////
  //
  // split interval I into left and right half
  // and pass on the references to the data
  //
  virtual interval split_left(interval& I) {
    //
    // compute derivative values at m
    //
    vector<real> dm;
    compute_derivatives(dm, I.m());
    data.push_back(dm);
    I.m_data = data.size()-1;
  
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
}; // evalHermite class
//
//////////////////////////////////////////////////////////////////
#endif
