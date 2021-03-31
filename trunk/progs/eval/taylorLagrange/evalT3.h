//////////////////////////////////////////////////////////////////
//
// evalT3.h
//		-- root search class, based on EVAL and Taylor expansion
//       using generalized Taylor forms with cubic convergence for f and f'
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALT3_H__
#define __CORE_EVALT3_H__

#include "evalTaylor.h"
#include "exactRange.h"

class evalT3 : public evalTaylor {
public:

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalT3(vector<real> v, interval I): evalTaylor(v,I) { name = "evalT3"; }
  evalT3(): evalTaylor() { name = "evalT3"; }

  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f over I using the cubic Taylor form
  //
	interval get_f (interval& I) {
  
    compute_Taylor_coefficients(I.m());
    real r = I.r();

    //
    // we split f(x) into
    //
    //  - the exact part        g_2(x) = c_0 + c_1 (x-m) + c_2 (x-m)^2      and
    //  - the remainder part    R_{g_2} (x) = \sum_{i=3}^n c_i (x-m)^i
    //
    
    //
    // get the exact range of g_2
    //
    interval gI = quadraticRange(c[0],c[1],c[2],r);
    
    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{g_2}(I) = r^3 [-1,1] S_{3,n},
    //
    // where S_{3,n} = \sum_{i=3}^n |c_i| r^{i-3}
    //
    // compute S_{3,n} using Horner summation
    //
	  real S = fabs(c[n]);   // assume n>2
	  for (int i=n-1; i>=3; i--) 
	    S = S * r + fabs(c[i]);

    //
    // return g_2(I) + r^3 [-1,1] S_{3,n}
    //
    S = r*r*r * S;
    gI.a -= S;
    gI.b += S;
    
    return( gI );
  }
  
  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f' over I using the cubic Taylor form
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
    //  - the exact part        g'_3(x) = c_1 + 2 c_2 (x-m) + 3 c_3 (x-m)^2      and
    //  - the remainder part    R_{g'_3} (x) = \sum_{i=4}^n i c_i (x-m)^{i-1}
    //
    
    //
    // get the exact range of g'_3
    //
    interval gI = quadraticRange(c[1],2*c[2],3*c[3],r);
    
    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{g'_3}(I) = r^3 [-1,1] S'_{3,n-1},
    //
    // where S'_{3,n-1} = \sum_{i=4}^n i |c_i| r^{i-4}
    //
    // compute S'_{3,n-1} using Horner summation
    //
	  real S = n*fabs(c[n]);    // assume n>3
	  for (int i=n-1; i>=4; i--)
	    S = S * r + i*fabs(c[i]);

    //
    // return g'_3(I) + r^3 [-1,1] S'_{3,n-1}
    //
    S = r*r*r * S;
    gI.a -= S;
    gI.b += S;
    
    return( gI );
  }
}; // evalT3 class
//
//////////////////////////////////////////////////////////////////
#endif
