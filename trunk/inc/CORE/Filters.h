/****************************************************************************
 * Filters.h -- Floating-point filters
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
 * $Id: Filters.h,v 1.4 2006-03-03 16:53:46 exact Exp $
 ***************************************************************************/
#ifndef __CORE_FILTERS_H__
#define __CORE_FILTERS_H__

#include <iostream>

#include <CORE/BigFloat2.h>
#include <cmath>
#include <cfloat>

#if defined (_MSC_VER) || defined (__MINGW32__) // add support for MinGW
  #define finite(x)	_finite(x)
  #define ilogb(x)	(int)_logb(x)
#endif

#if defined(sun) || defined(__sun)
  #include <ieeefp.h>
#endif

CORE_BEGIN_NAMESPACE

/// \class DummyFilter
/// \brief a dummy filter, do nothing
class DummyFilter {
  typedef DummyFilter thisClass;
public:
#ifdef CORE_DEBUG_FILTER
  void dump() const {}
#endif
  bool is_ok() const { return false; }
  int sign() const { return 0; }
  long uMSB() const { return 0; }
  long lMSB() const { return 0; }
  template <typename T> void set(const T&) {}
  void neg(const thisClass&) {}
  void sqrt(const thisClass&) {}
  void cbrt(const thisClass&) {}
  void root(const thisClass&, unsigned long) {}
  void addsub(const thisClass&, const thisClass&, bool) {}
  void mul(const thisClass&, const thisClass&) {}
  void div(const thisClass&, const thisClass&) {}
};

extern bool fpFilterFlag;

/// turn floating-point filter on/off
inline bool setFpFilterFlag(bool f) {
  bool oldf = fpFilterFlag;
  fpFilterFlag = f;
  return oldf;
}

// constants
const int IEEE_DOUBLE_PREC = 52;
const double DBL_INFTY = ::ldexp(DBL_MAX, 1);
const double CORE_EPS = ::ldexp(1.0, -IEEE_DOUBLE_PREC);

// k-th root for double (using BigFloat for now)
inline double root(double x, unsigned long k) 
{ BigFloat r(x); r.root(x, k); return r.get_d(); }

/// \class filteredFp Filter.h
/// \brief filteredFp is a simple filtered floating point number.
///        It is based on the Burnikel-Funke-Schirra (BFS) filter scheme.
///        We do not use IEEE exception mechanism here.
template <typename Kernel = BigFloat>
class BfsFilter {
  double fpVal;         // approximate double value for some "real value"
  double maxAbs;        // if (|fpVal| > maxAbs * ind * 2^{-52}) then
  int ind;              // sign of value is sign(fpVal).  Else, don't know.
  // REFERENCE: Burnikel, Funke, Schirra (BFS) filter
  // Chee: in isOK(), you used the test "|fpVal| >= maxAbs * ind * 2^{-52}" 
  // which seems to be correct (i.e., not |fpVal| > maxAbs * ind * 2^{-52})
  typedef BfsFilter thisClass;
  typedef typename Kernel::ZT ZT;
  typedef typename Kernel::QT QT;
  typedef typename Kernel::FT FT;
public:
#ifdef CORE_DEBUG_FILTER
  void dump() const 
  { std::cerr<<"[fpVal,maxAbs,ind]="<<fpVal<<","<<maxAbs<<","<<ind<<std::endl; }
#endif
  bool is_ok() const 
  { return (fpFilterFlag&&finite(fpVal)&&(fabs(fpVal)>=maxAbs*ind*CORE_EPS)); }
  int sign() const 
  { return (fpVal == 0.0) ? 0 : (fpVal > 0.0 ? 1: -1); }
  long lMSB() const 
  { return long(ilogb(fabs(fpVal) - maxAbs*ind*CORE_EPS)); }
  long uMSB() const 
  { return long(ilogb(fabs(fpVal) + maxAbs*ind*CORE_EPS)+1); }
  double get_value() const 
  { return fpVal; }
  int get_r_prec() const
  { return 53; }
  int get_a_prec() const
  { return 53; }
  double r_approx(int prec) const
  { return fpVal; }
  double a_approx(int prec) const
  { return fpVal; }

  /// from "Exact Geometric Predicates using Cascaded Computation" P176
  ///
  ///   An input value $x$ exactly representable by a double has the 
  ///   floating-point approximation $\tidle{x}=x$, the supremum 
  ///   $\tidle{x_{sup}}=|x|$ and the index 0. Otherwise, 
  ///   $\tidle{x}=round(x)$, the supremum 
  ///   $\tidle{x_{sup}}=|\tidle{x}|=|round(x)|$ and the index 1.
  void set(long value) { 
    fpVal = value; maxAbs = value > 0 ? value : (-value); 
    ind = (sizeof(long) > 4 && ceillg(value) >= 53) ? 1 : 0;
  }
  void set(unsigned long value) {
    fpVal = value; maxAbs = value; 
    ind = (sizeof(unsigned long) > 4 && ceillg(value) >= 53) ? 1 : 0;
  }
  void set(double value)
  { fpVal = value; maxAbs = fabs(value); ind = 0; }
  void set(const ZT& value) { 
    fpVal = value.get_d(); maxAbs = fabs(fpVal); 
    ind = value.uMSB() >= 53 ? 1 : 0; 
  }
  void set(const QT& value) { 
    fpVal = value.get_d(); maxAbs = fabs(fpVal); 
    ind = 1; //value.uMSB() >= 53 ? 1 : 0; // ??? denonimator has to be power of 2
  }
  void set(const FT& value) {
    fpVal = value.get_d(); maxAbs = fabs(fpVal); 
    ind = value.get_prec() >= 53 ? 1 : 0;
  }
  void set(const Kernel& value) {
    fpVal = value.get_d(); maxAbs = fabs(fpVal); 
    ind = value.get_prec() >= 53 ? 1 : 0;
  }

  // negation
  void neg(const thisClass& child)
  { fpVal = -child.fpVal; maxAbs = child.maxAbs; ind = child.ind; }

  // square root
  void sqrt(const thisClass& child) {
    if (child.fpVal > 0.0) {
      fpVal = ::sqrt(child.fpVal); maxAbs = child.maxAbs / child.fpVal * fpVal;
    } else {
      fpVal = 0.0; maxAbs = ::ldexp(::sqrt(child.maxAbs), 26);
    }
    ind = 1 + child.ind;
//    std::cerr << "sqrt: "; dump();
  }

  // cubic root
  void cbrt(const thisClass& child) {
    if (child.fpVal > 0.0) {
      fpVal = ::cbrt(child.fpVal); maxAbs = child.maxAbs / child.fpVal * fpVal;
    } else {
      fpVal = 0.0; maxAbs = ::ldexp(::cbrt(child.maxAbs), 18);
    }
    ind = 1 + child.ind;
  }

  // k-th root
  // TODO: root() function for double, using newton ??
  void root(const thisClass& child, unsigned long k) {
    if (child.fpVal > 0.0) {
      fpVal = CORE_NS::root(child.fpVal, k); 
      maxAbs = child.maxAbs / child.fpVal * fpVal;
    } else {
      fpVal = 0.0; 
      maxAbs = ::ldexp(CORE_NS::root(child.maxAbs, k), (IEEE_DOUBLE_PREC+k-1)/k);
    }
    ind = 1 + child.ind;
  }

  // addition/subtraction
  void addsub(const thisClass& f, const thisClass& s, bool is_add) {
    fpVal = is_add ? f.fpVal + s.fpVal : f.fpVal - s.fpVal;
    maxAbs = f.maxAbs + s.maxAbs;
    ind = 1 + (f.ind > s.ind ? f.ind : s.ind);
//    std::cerr << (is_add? "add: ": "sub: "); dump();
  }

  // multiplication
  void mul(const thisClass& f, const thisClass& s) {
    fpVal = f.fpVal * s.fpVal;
    maxAbs = f.maxAbs * s.maxAbs + DBL_MIN;
    ind = 1 + f.ind + s.ind; 
//    std::cerr << "mul: "; dump();
  }

  // division
  void div(const thisClass& f, const thisClass& s) {
    double xxx = fabs(s.fpVal) / s.maxAbs - (s.ind+1)*CORE_EPS;
    if (xxx > 0) {
      fpVal = f.fpVal / s.fpVal;
      maxAbs = (fabs(f.fpVal)/fabs(s.fpVal)+f.maxAbs/s.maxAbs)/xxx+DBL_MIN;
      ind = 1 + (f.ind > s.ind+1 ? f.ind : s.ind+1);
    } else {
      fpVal = DBL_INFTY;
      maxAbs = 0.0;
      ind = 0;
    }
//    std::cerr << "div: "; dump();
  }
};

CORE_END_NAMESPACE

#endif /*__CORE_FILTERS_H__*/
