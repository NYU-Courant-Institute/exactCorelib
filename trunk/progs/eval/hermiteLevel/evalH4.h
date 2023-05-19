//////////////////////////////////////////////////////////////////
//
// evalH4.h
//		-- root search class, based on EVAL and recursive Hermite interpolation
//       using recursive Hermite forms with quartic and cubic convergence for f and f'
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALH4_H__
#define __CORE_EVALH4_H__

#include "evalHermite.h"
#include "exactRange.h"

class evalH4 : public evalHermite {
public:
  real d00, d01, d02, d03;      // coefficients of h_0(x)
  real T;                       // remainder estimate T_{4,n}

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalH4(vector<real> v, interval I): evalHermite(v,I) { name = "evalH4"; }
  evalH4(): evalHermite() { name = "evalH4"; }

  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f over I using recursive Hermite forms with quartic convergence
  //
	interval get_f (interval& I) {  
    //
    // compute derivative values at a, unless inherited from mother interval
    //
    if (I.a_data < 0) {
      vector<real> da;
      compute_derivatives(da, I.a);
      data.push_back(da);
      I.a_data = data.size()-1;
    }

    //
    // compute derivative values at b, unless inherited from mother interval
    //
    if (I.b_data < 0) {
      vector<real> db;
      compute_derivatives(db, I.b);
      data.push_back(db);
      I.b_data = data.size()-1;
    }

    real r = I.r();
    real rr = r*r;

    //
    // we split f(x) into
    //
    //  - the exact part        h_0(x) = d_{0,0} + d_{0,1} (x-m) + d_{0,2} (x-m)^2 + d_{0,3} (x-m)^3,
    //
    //                   where  d_{0,0} = (f(a)+f(b)) / 2 - r (f'(b)-f'(a)) / 4,
    //                          d_{0,1} = 3 (f(b)-f(a)) / (4r) - (f'(a)+f'(b)) / 4,
    //                          d_{0,2} = (f'(b)-f'(a)) / (4r),
    //                          d_{0,3} = (f'(a)+f'(b)) / (4r^2) - (f(b)-f(a)) / (4r^3),         and
    //
    //  - the remainder part    R_{h_0} (x) = f(x) - h_0(x),
    //
    //                   whose range is approximated with iterative Hermite interpolation
    //
    
    //
    // get the exact range of h_0
    //
    real  fa = (data[I.a_data])[0];      // f(a)
    real  fb = (data[I.b_data])[0];      // f(b)
    real dfa = (data[I.a_data])[1];      // f'(a)
    real dfb = (data[I.b_data])[1];      // f'(b)

    real  sigma = (fa+fb)/4;
    real dsigma = (dfa+dfb)/4;
    real  delta = (fb-fa)/4/r;
    real ddelta = (dfb-dfa)/4;
    
    d00 = 2*sigma - r*ddelta;
    d01 = 3*delta - dsigma;
    d02 = ddelta/r;
    d03 = (dsigma - delta)/rr;

    interval h0I = cubicRange( d00,d01,d02,d03, r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{h_0}(I) = [-1,1] T_{4,n},
    //
    // where T_{4,n} = \sum_{j=1}^{d/4} |h_j(I)| \Omega_4^j
    //
    // compute T_{4,n} using Horner summation
    //
    real Omega4 = rr*rr/24;
    T = 0;
    for (int j=N; j>=1; j--) {
      //
      // recall that   h_j(x) = d_{j,0} + d_{j,1} (x-m) + d_{j,2} (x-m)^2 + d_{j,3} (x-m)^3,
      //
      //       where  d_{j,0} = (f^(4j)(a)+f^(4j)(b)) / 2 - r (f^(4j+1)(b)-f^(4j+1)(a)) / 4,
      //              d_{j,1} = 3 (f^(4j)(b)-f^(4j)(a)) / (4r) - (f^(4j+1)(a)+f^(4j+1)(b)) / 4,
      //              d_{j,2} = (f^(4j+1)(b)-f^(4j+1)(a)) / (4r),
      //              d_{j,3} = (f^(4j+1)(a)+f^(4j+1)(b)) / (4r^2) - (f^(4j)(b)-f^(4j)(a)) / (4r^3),         and
      //
      real  fa = (data[I.a_data])[2*j];       // f^(4j)(a)
      real  fb = (data[I.b_data])[2*j];       // f^(4j)(b)
      real dfa = (data[I.a_data])[2*j+1];     // f^(4j+1)(a)
      real dfb = (data[I.b_data])[2*j+1];     // f^(4j+1)(b)
    
      real  sigma = (fa+fb)/4;
      real dsigma = (dfa+dfb)/4;
      real  delta = (fb-fa)/4/r;
      real ddelta = (dfb-dfa)/4;
    
      real dj0 = 2*sigma - r*ddelta;
      real dj1 = 3*delta - dsigma;
      real dj2 = ddelta/r;
      real dj3 = (dsigma - delta)/rr;
      
      //
      // get the exact range of h_j
      //
      interval hjI = cubicRange( dj0,dj1,dj2,dj3, r);

      //
      // add |h_j(I)| to current T and multiply with \Omega_4
      //
      T = ( T + max(fabs(hjI.a),fabs(hjI.b)) ) * Omega4;
    }

    //
    // return h_0(I) + [-1,1] T_{4,n}
    //
    h0I.a -= T;
    h0I.b += T;

    return( h0I );
  }
  
  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f' over I using recursive Hermite forms with cubic convergence
  //
  // NOTE: this routine assumes that "get_f(I)" has been called before
  //       so that the coefficients d_{0,*}  of h_0(x) and the remainder
  //       size T_{4,n} are already available
  //
  interval get_df(interval& I) {
    real r = I.r();

    //
    // get the exact range of h_0'(x) = d_{0,1} + 2 d_{0,2} (x-m) + 3 d_{0,3} (x-m)^2
    //
    interval h0I = quadraticRange( d01,2*d02,3*d03, r);

    //
    // return h_0'(I) + 8/9*sqrt(3)/r [-1,1] T_{4,n}
    //
    T = 128 * T / (9*r);            
    h0I.a -= T;
    h0I.b += T;
    
    return( h0I );
  }
}; // evalH4 class
//
//////////////////////////////////////////////////////////////////
#endif
