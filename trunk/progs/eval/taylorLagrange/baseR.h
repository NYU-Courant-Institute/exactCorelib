//////////////////////////////////////////////////////////////////////////
//
// base.h
//		-- base class
//
//////////////////////////////////////////////////////////////////////////

#ifndef __CORE_BASE_H__
#define __CORE_BASE_H__

#include <iostream>
#include <gmp.h>
#include <gmpxx.h>

typedef mpz_class natural;

//
// choose rational or bigfloat arithmetic
//

#define RATIONAL

#ifdef RATIONAL
  typedef mpq_class real;
  //
  // provide sqrt function for rational arithmetics,
  // which temporarily switches to bigfloat arithmetic
  //
  real sqrt(real x) {
    if ( x == 3 )
      return (1.7320508075688773);
    mpf_class xf(x);
    xf = sqrt(xf);
    real xq(xf);
    return xq;
  }
#else
  typedef mpf_class real;
#endif


//
// bigfloat precision
//
int my_mpf_precision = 1024;

//
// provide absolute value and sign function for mpq or mpf 
//
real fabs(real x) {
	if (x > 0) return x;
	return -x;
}

real sgn(real x) {
	if (x > 0) return  1;
	if (x < 0) return -1;
	return 0;
}

//
// provide min and max function for mpq or mpf 
//
real min(real x1, real x2) {
  if (x1 < x2) return x1;
  return x2;
}

real max(real x1, real x2) {
  if (x1 > x2) return x1;
  return x2;
}

//
// Debugging
//
void cerror(std::string msg){
    std::cerr << "ERROR: " << msg;
}
void cwarning(std::string msg){
    std::cerr << "WARNING: " << msg;
}

#endif
//
//////////////////////////////////////////////////////////////////////////
