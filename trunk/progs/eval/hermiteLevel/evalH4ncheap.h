//////////////////////////////////////////////////////////////////
//
// evalL3ncheap.h
//		-- root search class, based on EVAL and recursive Hermite interpolation
//       up to level lev (default 15) using recursive Hermite forms with cubic and quartic convergence
//       for f and f' and estimating of the range of the recursive Hermite interpolants
//       h_j with the centered form
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVALH4NCHEAP_H__
#define __CORE_EVALH4NCHEAP_H__

#include "evalHermite.h"
#include "exactRange.h"

class evalH4ncheap : public evalHermite {
public:
  int lev;                      // level of convergence
  real d00, d01, d02, d03;      // coefficients of h_0(x)
  real T;                       // remainder estimate T_{4,lev}

  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //
  evalH4ncheap(vector<real> v, interval I, int lev): evalHermite(v,I) {
    name = "evalH'4n";
    this->lev = min(N+1,lev);
  }
  evalH4ncheap(vector<real> v, interval I): evalHermite(v,I) { 
    name = "evalH'4n"; 
    this->lev = min(N+1,15);
  }
  evalH4ncheap(int lev): evalHermite() {
    name = "evalH'4n";
    this->lev = min(N+1,lev);
  }
  evalH4ncheap(): evalHermite() { 
    name = "evalH'4n"; 
    this->lev = min(N+1,15);
  }

  //////////////////////////////////////////////////////////////
  //
  // compute all values f^{(4j)}(m) for j=0,...,lev-1
  //
  void compute_derivatives_here(vector<real>& d, real m) {
    vector<real> cc = f.c; // copy coefficients of f

    d.resize(2*lev);

    for (int k=0, idx=0; idx<=2*(lev-1); k+=4, idx+=2) {
      // cc[k..n] are the coefficients of f^{(k)}
      real sum = 0;                // sum = Horner val of f^{(k)}(m)
      for (int i=f.n; i>=k; i--) {
        sum = sum * m + cc[i];
        cc[i] *= (i-k); // update cc
      }
      d[idx] = sum;

      sum = 0;                     // sum = Horner val of f^{(k+1)}(m)
      for (int i=f.n; i>=k+1; i--) {
        sum = sum * m + cc[i];
        cc[i] *= (i-k-1) * (i-k-2) * (i-k-3); // update cc
      }
      d[idx+1] = sum;
    }
  }

  //////////////////////////////////////////////////////////////
  //
  // estimate the range of f over I using recursive Hermite interpolation with quartic convergence 
  // up to level lev
  //
  interval get_f (interval& I) {
    // ! Change compute_derivatives so it only calculates the needed data
    // compute derivative values at a, unless inherited from mother interval
    //
    if (I.a_data < 0) {
      vector<real> da;
      compute_derivatives_here(da, I.a);
      data.push_back(da);
      I.a_data = data.size()-1;
    }

    //
    // compute derivative values at b, unless inherited from mother interval
    //
    if (I.b_data < 0) {
      vector<real> db;
      compute_derivatives_here(db, I.b);
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
    real  fa = (data[I.a_data])[0]; // f(a)
    real  fb = (data[I.b_data])[0]; // f(b)
    real dfa = (data[I.a_data])[1]; // f'(a)
    real dfb = (data[I.b_data])[1]; // f'(b)

    real  sigma = (fa+fb)/4;
    real dsigma = (dfa+dfb)/4;
    real  delta = (fb-fa)/4/r;
    real ddelta = (dfb-dfa)/4;

    d00 = 2*sigma - r*ddelta; 
    d01 = 3*delta - dsigma;
    d02 = ddelta/r;
    d03 = (dsigma - delta)/rr;

    interval h0I = cubicRange(d00, d01, d02, d03, r);

    //
    // an estimate for the range of the remainder part is
    //
    //    []R_{h_0}(I) = [-1,1] T'_{4,n},
    //
    // where T'_{4,lev} = \sum_{j=1}^{d/4} |[]h_j(I)| \Omega_4^j
    // and the range of h_j is estimated with the centered form evaluation
    //
    // compute T'_{4,lev} using Horner summation
    //    
    real Omega4 = rr*rr/24;
    // 
    // compute f^(4*lev)(I) directly 
    // by computing coefficients of f^(4*lev)
    // then using direct evaluation
    //
    interval tail_estimate = f.evalDiff(I, 4*lev);
    T = Omega4 * max(fabs(tail_estimate.a), fabs(tail_estimate.b));
    for (int j=lev-1; j>=1; j--) {
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
      // add |[]h_j(I)| = |d_{j,0}| + r|d_{j,1}| + r^2|d_{j,2}| + r^3|d_{j,3}| to current T and multiply with \Omega_4
      //
      T = ( T + ((fabs(dj3)*r+fabs(dj2))*r+fabs(dj1))*r+fabs(dj0) ) * Omega4;
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

    interval h0I = quadraticRange(d01, 2*d02, 3*d03, r);

    T = 128 * T / (9*r);
    h0I.a -= T;
    h0I.b += T;

    return (h0I);
  }

  interval split_left(interval& I) override {
    vector<real> dm;
    compute_derivatives_here(dm, I.m());
    data.push_back(dm);
    I.m_data = data.size()-1;

    interval I_left(I.a, I.m(), I.level+1);
    I_left.a_data = I.a_data;
    I_left.m_data = I.m_data;
    return I_left;
  }
}; // evalH4ncheap class
#endif