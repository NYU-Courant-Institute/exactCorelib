//////////////////////////////////////////////////////////////////
//
// evalT2min.h
//		-- root search class, based on EVAL and Taylor expansion
//       using minimal Taylor forms with quadratic convergence for f and f'
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALT2MIN_H__
#define __CORE_EVALT2MIN_H__

#include "evalTaylor.h"
#include "exactRange.h"

class evalT2min : public evalTaylor {
public:

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalT2min(vector<real> v, interval I): evalTaylor(v,I) { name = "evalT2min"; }
  evalT2min(): evalTaylor() { name = "evalT2min"; }

  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f over I using the minimial quadratic Taylor form
  //
	interval get_f (interval& I) {
  
    compute_Taylor_coefficients(I.m());
    real r = I.r();

    //
    // we split f(x) into
    //
    //  - the exact part        g_1(x) = c_0 + c_1 (x-m)      and
    //  - the remainder part    R_{g_1} (x) = 1/2 f''(xi) (I-m)^2,  xi \in I
    //
    
    //
    // get the exact range of g_1
    //
    interval gI = linearRange(c[0],c[1],r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{g_1}(I) = 1/2 f''(I) (I-m)^2,
    //
    // where (I-m)^2 = [0,r^2]
    //
    // compute f''(I) using Horner summation
    //
	  interval ddfI = interval(n*(n-1)*f.c[n]);   // assume n>1
	  for (int i=n-1; i>=2; i--) 
	    ddfI = ddfI * I + i*(i-1)*f.c[i];

    ddfI = ddfI * interval(0,r*r/2);

    //
    // return g_1(I) + []R_{g_1}(I)
    //
    return( gI + ddfI );
  }
  
  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f' over I using the minimal quadratic Taylor form
  //
	interval get_df(interval& I) {
    real r = I.r();

    //
    // we split f'(x) = \sum_{i=1}^n i c_i (x-m)^{i-1} into
    //
    //  - the exact part        g'_2(x) = c_1 + 2 c_2 (x-m)      and
    //  - the remainder part    R_{g'_2} (x) = 1/2 f''(xi) (I-m)^2,  xi \in I
    //
    
    //
    // get the exact range of g'_2
    //
    interval gI = linearRange(c[1],2*c[2],r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{g'_2}(I) = 1/2 f'''(I) (I-m)^2,
    //
    // where (I-m)^2 = [0,r^2]
    //
    // compute f'''(I) using Horner summation
    //
	  interval dddfI = interval(n*(n-1)*(n-2)*f.c[n]);   // assume n>2
	  for (int i=n-1; i>=3; i--) 
	    dddfI = dddfI * I + i*(i-1)*(i-2)*f.c[i];

    dddfI = dddfI * interval(0,r*r/2);

    //
    // return g'_2(I) + []R_{g'_2}(I)
    //
    return( gI + dddfI );
  }
}; // evalT2'min class
//
//////////////////////////////////////////////////////////////////
#endif
