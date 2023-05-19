//////////////////////////////////////////////////////////////////
//
// evalL3.h
//		-- root search class, based on EVAL and recursve Lagrange interpolation
//       using recursive Lagrange forms with cubic and quadratic convergence for f and f'
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALL3_H__
#define __CORE_EVALL3_H__

#include "evalLagrange.h"
#include "exactRange.h"

class evalL3 : public evalLagrange {
public:
  real d00, d01, d02;           // coefficients of h_0(x)
  real T;                       // remainder estimate T_{3,n}

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalL3(vector<real> v, interval I): evalLagrange(v,I) { name = "evalL3"; }
  evalL3(): evalLagrange() { name = "evalL3"; }

  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f over I using recursive Lagrange forms with cubic convergence
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

    //
    // compute derivative values at m
    //
    vector<real> dm;
    compute_derivatives(dm, I.m());
    data.push_back(dm);
    I.m_data = data.size()-1;

    real r = I.r();

    //
    // we split f(x) into
    //
    //  - the exact part        h_0(x) = d_{0,0} + d_{0,1} (x-m) + d_{0,2} (x-m)^2,
    //
    //                   where  d_{0,0} = f(m),
    //                          d_{0,1} = (f(b)-f(a)) / (2r),
    //                          d_{0,2} = (f(b)-2f(m)+f(a)) / (2r^2),         and
    //
    //  - the remainder part    R_{h_0} (x) = f(x) - h_0(x),
    //
    //                   whose range is approximated with iterative Lagrange interpolation
    //
    
    //
    // get the exact range of h_0
    //
    real fa = (data[I.a_data])[0];      // f(a)
    real fm = (data[I.m_data])[0];      // f(m)
    real fb = (data[I.b_data])[0];      // f(b)

    d00 = fm;
    d01 = (fb-fa)/(2*r);
    d02 = (fb-2*fm+fa)/(2*r*r);
    
    interval h0I = quadraticRange( d00,d01,d02, r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{h_0}(I) = [-1,1] T_{3,n},
    //
    // where T_{3,n} = \sum_{j=1}^{d/3} |h_j(I)| \Omega_3^j
    //
    // compute T_{3,n} using Horner summation
    //
    real Omega3 = sqrt((real)3)/27 * r*r*r;
    T = 0;
    for (int j=N; j>=1; j--) {
      //
      // recall that  h_j(x) = d_{j,0} + d_{j,1} (x-m) + d_{j,2} (x-m)^2,
      //
      //                   where  d_{j,0} = f^(3j)(m),
      //                          d_{j,1} = (f^(3j)(b)-f^(3j)(a)) / (2r),
      //                          d_{j,2} = (f^(3j)(b)-2f^(3j)(m)+f^(3j)(a)) / (2r^2)
      //
      real dfa = (data[I.a_data])[j];      // f^(3j) (a)
      real dfm = (data[I.m_data])[j];      // f^(3j) (m)
      real dfb = (data[I.b_data])[j];      // f^(3j) (b)
      
      real dj0 = dfm;
      real dj1 = (dfb-dfa)/(2*r);
      real dj2 = (dfb-2*dfm+dfa)/(2*r*r);
      
      //
      // get the exact range of h_j
      //
      interval hjI = quadraticRange( dj0,dj1,dj2, r);
      
      //
      // add |h_j(I)| to current T and multiply with \Omega_3
      //
      T = ( T + max(fabs(hjI.a),fabs(hjI.b)) ) * Omega3;
    }

    //
    // return h_0(I) + [-1,1] T_{3,n}
    //
    h0I.a -= T;
    h0I.b += T;
    
    return( h0I );
  }
  
  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f' over I using recursive Lagrange forms with quadratic convergence
  //
  // NOTE: this routine assumes that "get_f(I)" has been called before
  //       so that the coefficients d_{0,*} of h_0(x) and the remainder
  //       size T_{3,n} are already available
  //
	interval get_df(interval& I) {
    real r = I.r();

    //
    // get the exact range of h_0'(x) = d_{0,1} + 2 d_{0,2} (x-m)
    //
    interval h0I = linearRange(d01,2*d02,r);

    //
    // return h_0'(I) + 3*sqrt(3)/r [-1,1] T_{3,n}
    //
    T = 3*sqrt((real)3) * T / r;
    h0I.a -= T;
    h0I.b += T;
    
    return( h0I );
  }
}; // evalL3 class
//
//////////////////////////////////////////////////////////////////
#endif
