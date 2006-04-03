/****************************************************************************
 * Expr.h -- EGC number class providing guarranteed precision
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
 * $Id: Expr.h,v 1.11 2006-04-03 20:39:39 exact Exp $
 ***************************************************************************/
#ifndef __CORE_EXPR_H__
#define __CORE_EXPR_H__

#include <CORE/ExprRep.h>

CORE_BEGIN_NAMESPACE

/// \class ExprT
/// Kernel -- internal representation 
template <typename RootBd, typename Filter, typename Kernel>
class ExprT {
public: // public typedefs
  typedef RootBd     RootBdT;
  typedef Filter     FilterT;
  typedef Kernel     KernelT;

  typedef typename Kernel::ZT ZT;
  typedef typename Kernel::QT QT;
  typedef typename Kernel::FT FT;
  typedef Kernel              KT;

private: // private typedefs
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef ConstRepT<RootBd, Filter, Kernel, long> ConstLongRep;
  typedef ConstRepT<RootBd, Filter, Kernel, unsigned long> ConstULongRep;
  typedef ConstRepT<RootBd, Filter, Kernel, double> ConstDoubleRep;
  typedef ConstRepT<RootBd, Filter, Kernel, ZT> ConstZTRep;
  typedef ConstRepT<RootBd, Filter, Kernel, QT> ConstQTRep;
  typedef ConstRepT<RootBd, Filter, Kernel, FT> ConstFTRep;
  //typedef ConstRepT<RootBd, Filter, Kernel, KT> ConstKTRep;
  typedef NegRepT<RootBd, Filter, Kernel> NegRep;
  typedef SqrtRepT<RootBd, Filter, Kernel> SqrtRep;
  typedef CbrtRepT<RootBd, Filter, Kernel> CbrtRep;
  typedef RootRepT<RootBd, Filter, Kernel> RootRep;
  typedef AddSubRepT<RootBd, Filter, Kernel, true> AddRep;
  typedef AddSubRepT<RootBd, Filter, Kernel, false> SubRep;
  typedef MulRepT<RootBd, Filter, Kernel> MulRep;
  typedef DivRepT<RootBd, Filter, Kernel> DivRep;

public:
  ExprT() : m_rep(new ConstLongRep(0L)) {}
  ExprT(char c) : m_rep(new ConstLongRep(long(c))) {}
  ExprT(unsigned char uc) : m_rep(new ConstULongRep((unsigned long)(uc))) {}
  ExprT(short s) : m_rep(new ConstLongRep(long(s))) {}
  ExprT(unsigned short us) : m_rep(new ConstULongRep((unsigned long)(us))) {}
  ExprT(int i) : m_rep(new ConstLongRep(long(i))) {}
  ExprT(unsigned int ui) : m_rep(new ConstULongRep((unsigned long)(ui))) {}
  ExprT(long l) : m_rep(new ConstLongRep(l)) {}
  ExprT(unsigned long ul) : m_rep(new ConstULongRep(ul)) {}
  ExprT(float f) : m_rep(new ConstDoubleRep(double(f))) {}
  ExprT(double d) : m_rep(new ConstDoubleRep(d)) {}
  ExprT(const ZT& z) : m_rep(new ConstZTRep(z)) {}
  ExprT(const FT& f) : m_rep(new ConstFTRep(f)) {}
  ExprT(const QT& q) 
  { FT f; (f.set(q)==0)?(m_rep=new ConstFTRep(f)):(m_rep=new ConstQTRep(q)); }
  ExprT(const KT& k) : m_rep(new ConstFTRep(k.get_f())) {}
  ExprT(const char* s, prec_t prec = get_def_input_digits()) 
  { construct_from_string(s, prec); }
  ExprT(const std::string& s, prec_t prec = get_def_input_digits()) 
  { construct_from_string(s.c_str(), prec); }
public:
  ExprT(ExprRep* r) : m_rep(r) {}
  ExprT(const ExprT& r) : m_rep(r.m_rep) { m_rep->inc_ref(); }
  ~ExprT() { m_rep->dec_ref(); }
  ExprT& operator=(const ExprT& r) {
    if (&r != this) { m_rep->dec_ref(); m_rep = r.m_rep; m_rep->inc_ref(); }
    return *this;
  }
public:
  ExprT& operator+=(const ExprT& e)
  { m_rep = new AddRep(m_rep, e.m_rep, true); return *this; }
  ExprT& operator-=(const ExprT& e)
  { m_rep = new SubRep(m_rep, e.m_rep, true); return *this; }
  ExprT& operator*=(const ExprT& e)
  { m_rep = new MulRep(m_rep, e.m_rep, true); return *this; }
  ExprT& operator/=(const ExprT& e)
  { m_rep = new DivRep(m_rep, e.m_rep, true); return *this; }

  ExprT operator+() const 
  { return ExprT(*this); }
  ExprT operator-() const 
  { return ExprT(new NegRep(m_rep)); }

  ExprT& operator++()
  { *this += 1; return *this; }
  ExprT operator++(int)
  { ExprT r(*this); ++(*this); return r; }
  ExprT& operator--()
  { *this -= 1; return *this; }
  ExprT operator--(int)
  { ExprT r(*this); --(*this); return r; }

  /// addition
  friend ExprT operator+(const ExprT& e1, const ExprT& e2)
  { return ExprT(new AddRep(e1.rep(), e2.rep())); }
  template <typename T>
  friend ExprT operator+(const ExprT& e1, const T& v)
  { return ExprT(new AddRep(e1.rep(), ExprT(v).rep())); }
  template <typename T>
  friend ExprT operator+(const T& v, const ExprT& e2)
  { return ExprT(new AddRep(ExprT(v).rep(), e2.rep())); }
  /// subtraction
  friend ExprT operator-(const ExprT& e1, const ExprT& e2)
  { return ExprT(new SubRep(e1.rep(), e2.rep())); }
  template <typename T>
  friend ExprT operator-(const ExprT& e1, const T& v)
  { return ExprT(new SubRep(e1.rep(), ExprT(v).rep())); }
  template <typename T>
  friend ExprT operator-(const T& v, const ExprT& e2)
  { return ExprT(new SubRep(ExprT(v).rep(), e2.rep())); }
  /// multiplication
  friend ExprT operator*(const ExprT& e1, const ExprT& e2)
  { return ExprT(new MulRep(e1.rep(), e2.rep())); }
  template <typename T>
  friend ExprT operator*(const ExprT& e1, const T& v)
  { return ExprT(new MulRep(e1.rep(), ExprT(v).rep())); }
  template <typename T>
  friend ExprT operator*(const T& v, const ExprT& e2)
  { return ExprT(new MulRep(ExprT(v).rep(), e2.rep())); }
  /// division
  friend ExprT operator/(const ExprT& e1, const ExprT& e2)
  { return ExprT(new DivRep(e1.rep(), e2.rep())); }
  template <typename T>
  friend ExprT operator/(const ExprT& e1, const T& v)
  { return ExprT(new DivRep(e1.rep(), ExprT(v).rep())); }
  template <typename T>
  friend ExprT operator/(const T& v, const ExprT& e2)
  { return ExprT(new DivRep(ExprT(v).rep(), e2.rep())); }
  /// square root
  friend ExprT sqrt(const ExprT& e)
  { return ExprT(new SqrtRep(e.rep())); }
  /// cube root
  friend ExprT cbrt(const ExprT& e)
  { return ExprT(new CbrtRep(e.rep())); }
  /// kth-root
  friend ExprT root(const ExprT& e, unsigned long k)
  { return ExprT(new RootRep(e.rep(), k)); }
  
  /// compare function
  int cmp(const ExprT& e) const
  { return m_rep == e.m_rep ? 0 : SubRep(m_rep, e.m_rep).get_sign(); }

  friend bool operator==(const ExprT& x, const ExprT& y)
  { return x.cmp(y) == 0; }
  friend bool operator!=(const ExprT& x, const ExprT& y)
  { return x.cmp(y) != 0; }
  friend bool operator>=(const ExprT& x, const ExprT& y)
  { return x.cmp(y) >= 0; }
  friend bool operator<=(const ExprT& x, const ExprT& y)
  { return x.cmp(y) <= 0; }
  friend bool operator<(const ExprT& x, const ExprT& y)
  { return x.cmp(y) < 0; }
  friend bool operator>(const ExprT& x, const ExprT& y)
  { return x.cmp(y) > 0; }

  friend std::istream& operator>>(std::istream& is, ExprT& x)
  { FT val; is >> val; if (is) x = val; return is; }
  friend std::ostream& operator<<(std::ostream& os, const ExprT& x) {
    ExprT* p = const_cast<ExprT*>(&x);
    if (p->sign()) os << (p->r_approx(60)).get_f(); else os << "0"; return os;
  }
public: // public methods
  /// return relative approximation
  KT& r_approx(prec_t prec)
  { return m_rep->r_approx(prec); }
  /// return absolute approximation
  KT& a_approx(prec_t prec)
  { return m_rep->a_approx(prec); }
  
  /// return approximation \f$[r, \infty]\f$ or \f$[\infty, a]\f$
  FT approx(prec_t r_prec = defRelPrec, prec_t a_prec = defAbsPrec) {
    if (a_prec == CORE_INFTY)
      return r_approx(r_prec).get_f();
    else // if (r_prec == CORE_INFTY)
      return a_approx(a_prec).get_f();
  }
  /// return BigFloatValue
  FT BigFloatValue() 
  { return m_rep->appValue().get_f(); }

  /// return sign (dirty cast)
  sign_t sign() const
  { return const_cast<ExprT*>(this)->m_rep->get_sign(); }
  /// return upper bound of MSB (dirty cast)
  msb_t uMSB() const
  { return const_cast<ExprT*>(this)->m_rep->get_uMSB(); }
  /// return lower bound of MSB (dirty cast)
  msb_t lMSB() const
  { return const_cast<ExprT*>(this)->m_rep->get_lMSB(); }
  /// absolute value
  ExprT abs() const 
  { return sign() >= 0 ? +(*this) : -(*this); }

  /// return internal rep
  ExprRep* rep() const
  { return m_rep; }

private:
  void construct_from_string(const char* str, prec_t prec) {
    if (strchr(str, '/') != 0 || is_infty(prec))
      m_rep = new ConstQTRep(QT(str));
    else
      m_rep = new ConstFTRep(FT(str, prec));
  }
private:
  ExprRep* m_rep; ///<- internal representation
}; // end if ExprT

CORE_END_NAMESPACE

///////////////////////////////////////////////////////////////////////////
// Definition of Expr
///////////////////////////////////////////////////////////////////////////

#include <CORE/RootBounds.h>
#include <CORE/Filters.h>

CORE_BEGIN_NAMESPACE

// BFMSS root bound + BFS filter + BigFloat2
typedef ExprT<BfmssRootBd<BigFloat2>, BfsFilter<BigFloat2>, BigFloat2> Expr;

// BFMSS root bound + Dummy filter + BigFloat2
//typedef ExprT<BfmssRootBd<BigFloat2>, DummyFilter, BigFloat2> Expr;

// Dummy root bound + Dummy filter + BigFloat2
//typedef ExprT<BfmssRootBd<BigFloat2>, DummyRootBd<10>, BigFloat> Expr;

// include inline functions for ExprRep
#include <CORE/ExprRep.inl>

/// absolute value
inline Expr abs(const Expr& x) {
  return x.abs();
}
/// absolute value (same as abs)
inline Expr fabs(const Expr& x) {
  return abs(x);
}

CORE_END_NAMESPACE

#endif // __CORE_EXPR_H__
