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
 * $Id: RootBounds.h,v 1.7 2006-09-22 11:56:18 exact Exp $
 ***************************************************************************/
#ifndef __CORE_ROOTBOUNDS_H__
#define __CORE_ROOTBOUNDS_H__

#include <iostream>

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
#ifdef CORE_DEBUG_ROOTBOUND
  void dump() const 
  { std::cout<<"[d_e,u_e,l_e]="<<d_e<<","<<u_e<<","<<l_e<<std::endl; }
#endif
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
