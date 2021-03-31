//////////////////////////////////////////////////////////////////
//
// evalT2.h
//		-- root search class, based on EVAL and Taylor expansion
//       using generalized Taylor forms with quadratic convergence for f and f'
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALT2_H__
#define __CORE_EVALT2_H__

#include "evalTaylor.h"
#include "exactRange.h"

class evalT2 : public evalTaylor {
public:

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalT2(vector<real> v, interval I): evalTaylor(v,I) { name = "evalT2"; }
  evalT2(): evalTaylor() { name = "evalT2"; }

  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f over I using the quadratic Taylor form
  //
	interval get_f (interval& I) {
  
    compute_Taylor_coefficients(I.m());
    real r = I.r();

    //
    // we split f(x) into
    //
    //  - the exact part        g_1(x) = c_0 + c_1 (x-m)      and
    //  - the remainder part    R_{g_1} (x) = \sum_{i=2}^n c_i (x-m)^i
    //
    
    //
    // get the exact range of g_1
    //
    interval gI = linearRange(c[0],c[1],r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{g_1}(I) = r^2 [-1,1] S_{2,n},
    //
    // where S_{2,n} = \sum_{i=2}^n |c_i| r^{i-2}
    //
    // compute S_{2,n} using Horner summation
    //
	  real S = fabs(c[n]);   // assume n>1
	  for (int i=n-1; i>=2; i--) 
	    S = S * r + fabs(c[i]);

    //
    // return g_1(I) + r^2 [-1,1] S_{2,n}
    //
    S = r*r * S;
    gI.a -= S;
    gI.b += S;
    
    return( gI );
  }
  
  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f' over I using the quadratic Taylor form
  //
  // NOTE: this routine assumes that "get_f(I)" has been called before
  //       so that the coefficients c_i of the Taylor expansion of f(x) 
  //       about m are already available; otherwise the line
  //
  //          compute_Taylor_coefficients(I.m());
  //
  //       should be added at the beginning of this routine
  //
	interval get_df(interval& I) {
    real r = I.r();

    //
    // we split f'(x) = \sum_{i=1}^n i c_i (x-m)^{i-1} into
    //
    //  - the exact part        g'_2(x) = c_1 + 2 c_2 (x-m)      and
    //  - the remainder part    R_{g'_2} (x) = \sum_{i=3}^n i c_i (x-m)^{i-1}
    //
    
    //
    // get the exact range of g'_2
    //
    interval gI = linearRange(c[1],2*c[2],r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{g'_2}(I) = r^2 [-1,1] S'_{2,n-1},
    //
    // where S'_{2,n-1} = \sum_{i=3}^n i |c_i| r^{i-3}
    //
    // compute S'_{2,n-1} using Horner summation
    //
	  real S = n*fabs(c[n]);    // assume n>2
	  for (int i=n-1; i>=3; i--)
	    S = S * r + i*fabs(c[i]);

    //
    // return g'_2(I) + r^2 [-1,1] S'_{2,n-1}
    //
    S = r*r * S;
    gI.a -= S;
    gI.b += S;
    
    return( gI );
  }
}; // evalT2 class
//
//////////////////////////////////////////////////////////////////
#endif
