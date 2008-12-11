/****************************************************************************
 * RootBounds.h -- Constructive root bounds
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
 * $Id: RootBounds.h,v 1.9 2008-12-11 21:24:17 exact Exp $
 ***************************************************************************/
#ifndef __CORE_ROOTBOUNDS_H__
#define __CORE_ROOTBOUNDS_H__

#include <iostream>
#include <sstream>
#include <string>
#include <limits>

CORE_BEGIN_NAMESPACE

/// Dummy Root Bound (minimal root bound class)
template <int bound = 1000>
class DummyRootBd {
  typedef DummyRootBd thisClass;
public:
#ifdef CORE_DEBUG_ROOTBOUND
  void dump() const {}
#endif
  bool is_constructive() const { return false; }
  unsigned long get_bound() const { return bound; }
  template <typename T> void set(const T& value) {}
  void neg(const thisClass& child) {}
  void root(const thisClass& child, unsigned long k) {}
  void addsub(const thisClass& f, const thisClass& s) {}
  void mul(const thisClass& f, const thisClass& s) {}
  void div(const thisClass& f, const thisClass& s) {}
};

const double log_5 = std::log(double(5)) / std::log(double(2));
const unsigned long Inf = std::numeric_limits<unsigned long>::infinity();

template <typename Kernel = BigFloat>
class BfmsskRootBd {
  typedef BfmsskRootBd thisClass;
  typedef typename Kernel::ZT ZT;
  typedef typename Kernel::QT QT;
  typedef typename Kernel::FT FT;

  bool m_visit;
  unsigned long d;
  unsigned long u25;
  unsigned long l25;
  unsigned long v2p;
  unsigned long v2m;
  unsigned long v5p;
  unsigned long v5m;

  //return the ceil of log_2(5^x)
  unsigned long ceilLg5(const unsigned long x) {
    return static_cast<unsigned long>(std::ceil(log_5 * x));
  }
  bool is_bounded(const unsigned long x) {
    return x >= std::numeric_limits<unsigned long>::min() ||
	   x <= std::numeric_limits<unsigned long>::max();
  }
public:
  BfmsskRootBd() : m_visit(false), d(1), u25(0), l25(0), v2p(0), v2m(0), v5p(0), v5m(0) {}
  void dump() const {}
  unsigned long get_deg()
  { return d; }
  void set_visit(bool visit)
  { m_visit = visit; }
  bool get_visit()
  { return m_visit; }
  bool is_constructive() const
  { return true; }
  unsigned long get_bound(unsigned long deg) {
    unsigned long bound;

    if (is_bounded(v2p) || is_bounded(v2m))
      bound = Inf;
    else
      bound = l25 + u25 * (deg - 1UL) - (v2p - v2m) - ceilLg5(v5p - v5m);
    if (is_bounded(bound))
      bound = Inf;

    return std::max(bound, 1UL);
  }
  void set(long value) 		{ set(ZT(value)); }
  void set(unsigned long value) { set(ZT(value)); }
  void set(double value) 	{ set(FT(value)); }
  void set(const ZT& value) {
    if (value == 0)
      return;

    ZT temp(value);
	  
    v2p = temp.get_k_exp(temp, 2UL);
    v5p = temp.get_k_exp(temp, 5UL);

    v2m = v5m = 0;
    
    u25 = temp.ceillg();
    l25 = 0;
  }
  void set(const QT& value) {
    if (value == 0)
      return;

    ZT num = value.num();
    ZT den = value.den();

    v2p = num.get_k_exp(num, 2UL);
    v2m = num.get_k_exp(den, 2UL);

    v5p = num.get_k_exp(num, 5UL);
    v5m = num.get_k_exp(den, 5UL);

    u25 = num.ceillg();
    l25 = den.ceillg();
  }
  void set(const FT& value) {
    ZT x; exp_t e = value.get_z_exp(x);
    if (e >= 0) {
      v2p = e;
    } else { 
      v2m = e;
    }

    v5p = v5m = 0;

    u25 = x.ceillg();
    l25 = 0;
  }
  void set(const Kernel& value) {
    if (value.sgn() >= 0)
      set(value.getLeft());
    else
      set(value.getRight());
  }
  void neg(const thisClass& child) {
    u25 = child.u25; l25 = child.l25;
    v2p = child.v2p; v2m = child.v2m;
    v5p = child.v5p; v5m = child.v5m;
  }

  void root(thisClass& child, unsigned long k) {
    if (child.v2p + ceilLg5(child.v5p) + child.u25 >=
        child.v2m + ceilLg5(child.v5m) + child.l25) {
      unsigned long vtilda2 = child.v2p + (k - 1UL) * child.v2m;
      v2p = vtilda2 / k;
      v2m = child.v2m;
      unsigned long vmod2;
      if (is_bounded(v2p))
        vmod2 = Inf;
      else
        vmod2 = vtilda2 - k * v2p; // == vtilda2 % k
      unsigned long vtilda5 = child.v5p + (k - 1UL) * child.v5m;
      v5p = vtilda5 / k;
      v5m = child.v5m;
      unsigned long vmod5;
      if (is_bounded(v5p))
        vmod5 = Inf;
      else
        vmod5 = vtilda5 - k * v5p; // == vtilda5 % k

      u25 = (child.u25 + (k - 1UL) * child.l25 + vmod2 + ceilLg5(vmod5) + 1UL) / k;
      l25 = child.l25;
    } else {
      unsigned long vtilda2 = (k - 1UL) * child.v2p + child.v2m;
      v2p = child.v2p;
      v2m = vtilda2 / k;
      unsigned long vmod2;
      if (is_bounded(v2m))
        vmod2 = Inf;
      else
        vmod2 = vtilda2 - k * v2m; // == vtilda2 % k
      unsigned long vtilda5 = (k - 1UL) * child.v5p + child.v5m;
      v5p = child.v5p;
      v5m = vtilda5 / k;
      unsigned long vmod5;
      if (is_bounded(v5m))
        vmod5 = Inf;
      else
        vmod5 = vtilda5 - k * v5m; // == vtilda5 % k

      u25 = child.u25;
      l25 = ((k - 1UL) * child.u25 + child.l25 + vmod2 + ceilLg5(vmod5) + 1UL) / k;
    }
  }
  void addsub(thisClass& f, thisClass& s) {
    v2p = std::min(f.v2p + s.v2m, f.v2m + s.v2p);
    v2m = f.v2m + s.v2m;
    v5p = std::min(f.v5p + s.v5m, f.v5m + s.v5p);
    v5m = f.v5m + s.v5m;

    if (is_bounded(v2p) || is_bounded(v5p))
      u25 = Inf;
    else
      u25 = 1UL + std::max(
  	    f.v2p + s.v2m - v2p + ceilLg5(f.v5p + s.v5m - v5p) + f.u25 + s.l25,
	    f.v2m + s.v2p - v2p + ceilLg5(f.v5m + s.v5p - v5p) + f.l25 + s.u25); 
    l25 = f.l25 + s.l25;
  }
  void mul(thisClass& f,thisClass& s) {
    v2p = f.v2p + s.v2p;
    v2m = f.v2m + s.v2m;
    v5p = f.v5p + s.v5p;
    v5m = f.v5m + s.v5m;
    u25 = f.u25 + s.u25;
    l25 = f.l25 + s.l25;
  }
  void div(thisClass& f, thisClass& s) { 
    v2p = f.v2p + s.v2m;
    v2m = f.v2m + s.v2p;
    v5p = f.v5p + s.v5m;
    v5m = f.v5m + s.v5p;
    u25 = f.u25 + s.l25;
    l25 = f.l25 + s.u25;
  }
};

/// BFMSS Root Bound
template <typename Kernel = BigFloat>
class BfmssRootBd {
  typedef BfmssRootBd thisClass;
  typedef thisClass* id_rootbd_t;
  typedef typename Kernel::ZT ZT;
  typedef typename Kernel::QT QT;
  typedef typename Kernel::FT FT;
  bool    m_visit;
  unsigned long d_e;
  unsigned long u_e;
  unsigned long l_e;
public:
  BfmssRootBd() : m_visit(false), d_e(1) {}
  void dump() const 
  { std::cout<<"[d_e,u_e,l_e]="<<d_e<<","<<u_e<<","<<l_e<<std::endl; }
  
  std::string dump() { 
    std::ostringstream oss;
    oss <<"[d_e,u_e,l_e]="<<d_e<<","<<u_e<<","<<l_e;
    return oss.str();
  }
public:
  unsigned long get_deg()
  { return d_e; }
  void set_visit(bool visit)
  { m_visit = visit; }
  bool get_visit()
  { return m_visit; }
  bool is_constructive() const
  { return true; }
  /// BFMSS root bound could be zero, so set it to be 1 in that case
  unsigned long get_bound(unsigned long deg) const
  { return std::max(l_e + (deg - 1) * u_e, 1UL); }
  void set(long value)
  { u_e = ceillg(value); l_e = 0; }
  void set(unsigned long value)
  { u_e = ceillg(value); l_e = 0; }
  void set(double value)
  { set(FT(value)); }
  void set(const ZT& value)
  { u_e = value.ceillg(); l_e = 0; }
  void set(const QT& value)
  { u_e = value.num().ceillg(); l_e = value.den().ceillg(); }
  void set(const FT& value) {
    ZT x; exp_t e = value.get_z_exp(x);
    if (e >= 0) { // convert to integer
      x.mul_2exp(x, e); set(x);
    } else { // convert to rational
      QT q; q.div_2exp(x, -e); set(q);
    }
  }
  void set(const Kernel& value) {
    if (value.sgn() >= 0)
      set(value.getLeft());
    else
      set(value.getRight());
  }
  void neg(const thisClass& child) {
    u_e = child.u_e; l_e = child.l_e;
  }
  void root(thisClass& child, unsigned long k) {
    if (child.u_e >= child.l_e) {
      u_e = (child.u_e + (k-1)*child.l_e + (k-1)) / k;
      l_e = child.l_e;
    } else {
      u_e = child.u_e;
      l_e = ((k-1)*child.u_e + child.l_e + (k-1)) / k;
    }
    d_e = k;
  }
  void addsub(thisClass& f, thisClass& s) {
    u_e = std::max(f.u_e + s.l_e, f.l_e + s.u_e) + 1; 
    l_e = f.l_e + s.l_e;
  }
  void mul(thisClass& f,thisClass& s) {
    u_e = f.u_e + s.u_e;
    l_e = f.l_e + s.l_e;
  }
  void div(thisClass& f, thisClass& s) { 
    u_e = f.u_e + s.l_e;
    l_e = f.l_e + s.u_e;
  }
};

/// Minimum Root Bound (root bound class which taking minimum of two root bounds)
template <typename RootBd1, typename RootBd2>
class MinRootBd{
  typedef MinRootBd thisClass;
  RootBd1 m_rootBd1;
  RootBd2 m_rootBd2;
public:
  void dump() const 
  { m_rootBd1.dump(); m_rootBd2.dump(); }
  bool is_constructive() const 
  { return m_rootBd1.is_constructive() || m_rootBd2.is_constructive(); }
  unsigned long get_bound() const 
  { return std::min(m_rootBd1.get_bound(), m_rootBd2.get_bound()); }
  template <typename T> void set(const T& value) 
  { m_rootBd1.set(value), m_rootBd2.set(value); }
  void neg(const thisClass& child) 
  { m_rootBd1.neg(child), m_rootBd2.neg(child); }
  void root(const thisClass& child, unsigned long k)
  { m_rootBd1.root(child, k), m_rootBd2.root(child, k); }
  void addsub(const thisClass& f, const thisClass& s)
  { m_rootBd1.addsub(f, s), m_rootBd2.addsub(f, s); }
  void mul(const thisClass& f, const thisClass& s)
  { m_rootBd1.mul(f, s), m_rootBd2.mul(f, s); }
  void div(const thisClass& f, const thisClass& s)
  { m_rootBd1.div(f, s), m_rootBd2.div(f, s); }
};

CORE_END_NAMESPACE

#endif /*__CORE_ROOTBOUNDS_H__*/
