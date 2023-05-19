//////////////////////////////////////////////////////////////////
//
// exactRange.h
//    -- provides exact range functions for linear, quadratic, and cubic polynomials
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EXACTRANGE_H__
#define __CORE_EXACTRANGE_H__

#include "base.h"
#include "interval.h"

//////////////////////////////////////////////////////////////////
//
// compute the exact range of the linear polynomial
//
//   g(x) = c0 + c1*(x-m)
//
// over the interval [m-r,m+r]
//
interval linearRange(real c0, real c1, real r) {
  real ga = c0 - r*c1;       // g(a)
  real gb = c0 + r*c1;       // g(b)
  
  interval I(ga,gb);

  return(I);
}

//////////////////////////////////////////////////////////////////
//
// compute the exact range of the quadratic polynomial
//
//   g(x) = c0 + c1*(x-m) + c2*(x-m)^2
//
// over the interval [m-r,m+r]
//
interval quadraticRange(real c0, real c1, real c2, real r) {
  real ga = (c2*r - c1)*r + c0;       // g(a)
  real gb = (c2*r + c1)*r + c0;       // g(b)
  
  interval I(ga,gb);
  
  if ( fabs(c1) < 2*fabs(c2)*r )
    if (c2 > 0)
      I.a = c0 - c1*c1/4/c2;
    else
      I.b = c0 - c1*c1/4/c2;
      
  return(I);
}

//////////////////////////////////////////////////////////////////
//
// compute the exact range of the cubic polynomial
//
//   g(x) = c0 + c1*(x-m) + c2*(x-m)^2 + c3*(x-m)^3
//
// over the interval [m-r,m+r]
//
interval cubicRange(real c0, real c1, real c2, real c3, real r) {
  if (c3 == 0)
    return quadraticRange(c0,c1,c2,r);
    
  real ga = ((-c3*r + c2)*r - c1)*r + c0;       // g(a)
  real gb = (( c3*r + c2)*r + c1)*r + c0;       // g(b)
  
  interval I(ga,gb);
  real Delta = c2*c2 - 3*c1*c3;
  
  if (Delta > 0) {
    real L = sgn(c3) * (c1 + 3*c3*r*r);
    real R = 2*fabs(c2)*r;
    if (L > R) {
      if (fabs(c2) < 3*fabs(c3)*r) {
        real xm = - (c2 - sqrt(Delta)) / (3*c3);
        real xp = - (c2 + sqrt(Delta)) / (3*c3);
        I.a = min( I.a, ((c3*xm + c2)*xm + c1)*xm + c0 );   // replace I.a with g(x-), if necessary
        I.b = max( I.b, ((c3*xp + c2)*xp + c1)*xp + c0 );   // replace I.b with g(x+), if necessary
      }
    }
    else if (L > -R)
      if (c2 > 0) {
        real xm = - (c2 - sqrt(Delta)) / (3*c3);
        I.a = ((c3*xm + c2)*xm + c1)*xm + c0;     // replace I.a with g(x-)
      }
      else if (c2 < 0) {
        real xp = - (c2 + sqrt(Delta)) / (3*c3);
        I.b = ((c3*xp + c2)*xp + c1)*xp + c0;     // replace I.b with g(x+)
      }
  }
      
  return(I);
}
//
//////////////////////////////////////////////////////////////////
#endif
