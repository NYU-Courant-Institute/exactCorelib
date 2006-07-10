/****************************************************************************
 * CoreAux.h -- Auxilliary functions for the Core Library
 *
 * Core Library Version 2.0, March 2006
 * Copyright (c) 1995-2006 Exact Computation Project
 * All rights reserved.
 *
 * This file is part of Core Library (http://cs.nyu.edu/exact/core); you 
 * may redistribute it under the terms of the Q Public License version 1.0.
 * See the file LICENSE.QPL distributed with Core Library.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * WWW URL: http://cs.nyu.edu/exact/core
 * Email: exact@cs.nyu.edu
 *
 * $Id: CoreAux.h,v 1.9 2006-07-10 13:46:55 exact Exp $
 ***************************************************************************/
#ifndef __CORE_COREAUX_H__
#define __CORE_COREAUX_H__

#include <cmath>
#include <iostream>

CORE_BEGIN_NAMESPACE

/// Writes out an error or warning message in the local file CORE_DIAGFILE
/** If last argument (err) is TRUE, then this is considered an error
 *  (not just warning).  In this case, the message is also printed in
 *  std::cerr, using std::perror().
 *  */
void core_error(std::string msg, std::string file, int lineno, bool err);

/// This is for debugging messages
inline void core_debug(std::string msg){
  std::cout << __FILE__ << "::" << __LINE__ << ": " << msg
            << std::endl;
}

// help inline functions for long
inline sign_t sgn(long v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
msb_t ceillg(long v);
msb_t floorlg(long v);

// help inline functions for unsigned long
inline sign_t sgn(unsigned long v)
{ return v==0 ? 0 : 1; }
msb_t ceillg(unsigned long v);
msb_t floorlg(unsigned long v);

// help inline functions for double
inline sign_t sgn(double v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline msb_t ceillg(double v)
{ return ilogb(v)+1; }
inline msb_t floorlg(double v)
{ return ilogb(v); }

/// template function returns the absolute value
template <class T>
inline const T core_abs(const T& a) {
  return ((a < T(0)) ? -a : a);
}

template <class T>
inline  void core_swap(T& a, T& b) {
  T tmp;
  tmp = a;
  a = b;
  b = tmp;
}

//#ifdef CORE_OLDNAMES 
/// \addtogroup GlobalBackCompatiableFunctions
//@{
inline bool isDivisible(int x, int y) { return x % y == 0; }
inline bool isDivisible(long x, long y) { return x % y == 0; }
inline int div_exact(int x, int y) { return x/y; }
inline long div_exact(long x, long y) { return x/y; }
inline long ceilLg(long a) { return ceilLg(BigInt(a)); }
inline long ceilLg(int a) { return ceilLg(BigInt(a)); }
inline long sign(long a) { return a==0 ? 0 : a > 0 ? 1 : -1; }
inline int sign(int a) { return a==0 ? 0 : a > 0 ? 1 : -1; }
inline long abs(long x) { return (x>=0) ? x : (-x); }
inline int abs(int x) { return (x>=0) ? x : (-x); }
long gcd(long m, long n) {
  if (m == 0)
    return core_abs(n);
  if (n == 0)
    return core_abs(m);
  long p = core_abs(m);
  long q = core_abs(n);
  if (p<q)
    core_swap(p, q);
  while (q>0) {
    long r = p % q;
    p = q;
    q = r;
  }
  return p;
}
int gcd(int m, int n) {
  if (m == 0)
    return core_abs(n);
  if (n == 0)
    return core_abs(m);
  int p = core_abs(m);
  int q = core_abs(n);
  if (p<q)
    core_swap(p, q);
  while (q>0) {
    int r = p % q;
    p = q;
    q = r;
  }
  return p;
}
//@}
//#endif

CORE_END_NAMESPACE

#endif /*__CORE_COREAUX_H__*/
