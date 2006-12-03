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
 * $Id: Expr.h,v 1.24 2006-12-03 18:52:06 exact Exp $
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
  typedef SumOpRepT<RootBd, Filter, Kernel> SumRep;
  typedef ProdOpRepT<RootBd, Filter, Kernel> ProdRep;
  typedef PiRepT<RootBd, Filter, Kernel> PiRep;
  typedef ERepT<RootBd, Filter, Kernel> ERep;

public:
  ExprT() : m_rep(new ConstLongRep(0L, NODE_NT_INTEGER)) {}
  ExprT(char c) : m_rep(new ConstLongRep(long(c), NODE_NT_INTEGER)) {}
  ExprT(unsigned char uc) : m_rep(new ConstULongRep((unsigned long)(uc), NODE_NT_INTEGER)) {}
  ExprT(short s) : m_rep(new ConstLongRep(long(s), NODE_NT_INTEGER)) {}
  ExprT(unsigned short us) : m_rep(new ConstULongRep((unsigned long)(us), NODE_NT_INTEGER)) {}
  ExprT(int i) : m_rep(new ConstLongRep(long(i), NODE_NT_INTEGER)) {}
  ExprT(unsigned int ui) : m_rep(new ConstULongRep((unsigned long)(ui), NODE_NT_INTEGER)) {}
  ExprT(long l) : m_rep(new ConstLongRep(l, NODE_NT_INTEGER)) {}
  ExprT(unsigned long ul) : m_rep(new ConstULongRep(ul, NODE_NT_INTEGER)) {}
  ExprT(float f) : m_rep(new ConstDoubleRep(double(f), NODE_NT_DYADIC)) {}
  ExprT(double d) : m_rep(new ConstDoubleRep(d, NODE_NT_DYADIC)) {}
  ExprT(const ZT& z) : m_rep(new ConstZTRep(z, NODE_NT_INTEGER)) {}
  ExprT(const FT& f) : m_rep(new ConstFTRep(f, NODE_NT_DYADIC)) {}
  ExprT(const QT& q) { 
    FT f;
    (f.set(q)==0)?
    (m_rep=new ConstFTRep(f, NODE_NT_DYADIC)):
    (m_rep=new ConstQTRep(q, NODE_NT_RATIONAL));
  }
  ExprT(const KT& k) : m_rep(new ConstFTRep(k.get_f(), NODE_NT_DYADIC)) {}
  ExprT(const char* s, int base = 10, prec_t prec = getDefaultInputDigits()) 
  { construct_from_string(s, base, prec); }
  ExprT(const std::string& s, int base = 10, prec_t prec =getDefaultInputDigits()) 
  { construct_from_string(s.c_str(), base, prec); }
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
  /// pi
  friend ExprT pi()
  { return ExprT(new PiRep()); }
  /// e
  friend ExprT e()
  { return ExprT(new ERep()); }
  /// radical -- alternative name for root(n,k)
  template<class NT>
  friend ExprT radical(const NT& n, unsigned long k) {
    assert(n>=0 && k>=1);
    /*  This code is slower because root calls MPFR
     *  while this code uses our own Newton iteration.
    if (n==0 || n == 1 || k ==1) return n;
    Polynomial<NT> Q(k);
    Q.setCoeff(0, -n);
    Q.setCoeff(k, 1);
    return rootOf(Q);
    */
    return root(ExprT(n),k); //Jihun:this version is very slow.root bound becomes extremely large
  }

  /// helper function for constructing Polynomial node (n-th node)
  template <class NT>
  friend ExprT rootOf(const Polynomial<NT>& p, int n = 0) {
    return ExprT(new ConstPolyRepT<RootBd, Filter, Kernel, NT>(p, n));
  }
  /// helper function for constructing Polynomial node witb BFInterval
  template <class NT>
  friend ExprT rootOf(const Polynomial<NT>& p, const BFInterval& I) {
    return ExprT(new ConstPolyRepT<RootBd, Filter, Kernel, NT>(p, I));
  }
  /// helper function for constructing Polynomial node with pair of BigFloats
  template <class NT, class T>
  friend ExprT rootOf(const Polynomial<NT>& p, const T& x, const T& y) {
    return ExprT(new ConstPolyRepT<RootBd, Filter, Kernel, NT>(p, BFInterval(x, y)));
  }

  friend ExprT power(const ExprT& e, long k) {
    if (k==0)  return 1;
    else if (k==1) return e;
    else {
      bool sign = true;
      if (k < 0) 
      { sign = false; k = -k; }
      std::vector<ExprRep*> c;
      ProdRep* newRep = new ProdRep(c);
      for (long i=0; i < k; i++)
        newRep->insert (e.rep());
      if (sign)
        return ExprT(newRep);
      else
        return ExprT(1) / ExprT(newRep);
    }
  }
  friend ExprT pow(const ExprT& e, long k) {
    return power(e,k);
  }

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
    if (p->sign()) os << p->approx(defRelPrec,defAbsPrec); else os << "0"; return os;
  }
  std::string toString() {
    ExprT* p = const_cast<ExprT*>(this);
    if (p->sign()) return p->approx(defRelPrec,defAbsPrec).get_str(); else return "0";
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
    else if (r_prec == CORE_INFTY)
      return a_approx(a_prec).get_f();
    else 
      return a_approx( std::min(a_prec, m_rep->rel2abs(r_prec)) ).get_f();
  }
  /// return integer value 
  int intValue() const {
    ExprT* p = const_cast<ExprT*>(this);
    p->a_approx(2);
    return (int)m_rep->appValue().get_d();
  }
  /// return long value 
  long longValue() const
  { return (long)m_rep->appValue().get_d(); }
  /// return float value 
  float floatValue() const
  { return (float)m_rep->appValue().get_d(); }
  /// return double value 
  double doubleValue() const {
    ExprT* p = const_cast<ExprT*>(this);
    p->r_approx(52);
    return m_rep->appValue().get_d();
  }
  /// return BigInt value 
  BigInt BigIntValue() const
  { return m_rep->appValue().get_z(); }
  /// return BigRat value 
  BigRat BigRatValue() const
  { return m_rep->appValue().get_q(); }
  /// return BigFloatValue
  FT BigFloatValue() const
  { return m_rep->appValue().get_f(); }
  /// return BigFloat2Value
  KT BigFloat2Value() const
  { return m_rep->appValue(); }
  /// double interval
  void doubleInterval(double& lb, double& ub) {
    ExprT* p = const_cast<ExprT*>(this);
    p->r_approx(52);
    KT interval = p->BigFloat2Value();
    lb = interval.getLeft().get_d(BF_RNDD);
    ub = interval.getRight().get_d(BF_RNDU);
  }

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

  /// Debug Help Functions
  void debug(int mode, int level, int depthLimit) {
    std::cout << "-------- Expr debug() -----------" << std::endl;
    std::cout << "rep = " << rep() << std::endl;
    if (mode == LIST_MODE)
      rep()->debugList(level, depthLimit);
    else if (mode == TREE_MODE)
      rep()->debugTree(level, 0, depthLimit);
    else
      core_error("unknown debugging mode", __FILE__, __LINE__, false);
    std::cout << "---- End Expr debug(): " << std::endl;
  }
  
private:
  void construct_from_string(const char* str, int base, prec_t prec) {
    if (strchr(str, '/') != 0)
      m_rep = new ConstQTRep(QT(str), NODE_NT_RATIONAL);
    else if (strchr(str, '.') != 0 && is_infty(prec))
      m_rep = new ConstQTRep(QT(construct_rat(str).c_str()), NODE_NT_RATIONAL);
    else if (is_infty(prec))
      m_rep = new ConstFTRep(FT(str), NODE_NT_RATIONAL);
    else
      m_rep = new ConstFTRep(FT(str, base, prec), NODE_NT_RATIONAL);
  }

  const std::string construct_rat (const char* str) {
    std::string s(str);
    std::string d1,d2,exp;
    
    int dot = s.find('.');
    int e = s.find('e');
    d1 = s.substr(0, dot);

    if (d1=="0") d1="";
    if (d1=="+0" || d1=="-0") d1.erase(1,1); 

    d2 = s.substr(dot+1, s.size()-dot-1);
    exp = s.substr(e+1, s.size()-e-1);

    std::string num = d1+d2;
    std::string den(d2.size()+1, '0');
    den[0]='1';

    return num + std::string("/") + den;
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

/// absolute value
inline Expr abs(const Expr& x) {
  return x.abs();
}
/// absolute value (same as abs)
inline Expr fabs(const Expr& x) {
  return abs(x);
}

inline sign_t sign(const Expr& x) {
  return x.sign();
}

inline double Todouble(const Expr& e, prec_t r = defRelPrec, prec_t a=defAbsPrec) {
  Expr* p = const_cast<Expr*>(&e);
  if (p->sign()) {
    p->approx(r,a);
    return e.doubleValue();
  } else
    return 0;
}

/// convert Expr to BigFloat2
inline BigFloat2 ToBigFloat2(const Expr& e, prec_t r = defRelPrec, prec_t a=defAbsPrec) {
  Expr* p = const_cast<Expr*>(&e);
  if (p->sign()) {
    p->approx(r,a);
    return e.BigFloat2Value();
  } else
    return BigFloat2(0);
}

inline BigInt ToBigInt(const Expr& e, prec_t r = defRelPrec, prec_t a=defAbsPrec) {
  Expr* p = const_cast<Expr*>(&e);
  if (p->sign()) {
    p->approx(r,a);
    return e.BigIntValue();
  } else
    return 0;
}

inline BigRat ToBigRat(const Expr& e, prec_t r = defRelPrec, prec_t a=defAbsPrec) {
  Expr* p = const_cast<Expr*>(&e);
  if (p->sign()) {
    p->approx(r,a);
    return e.BigRatValue();
  } else
    return 0;
}

inline bool isDivisible(const Expr& x, const Expr& y) {
  Expr e = x/y;
  return ((e - ToBigInt(e, CORE_INFTY, 2)) == 0);
}

inline Expr gcd(const Expr& x, const Expr& y) {
  return 1;
}

inline Expr div_exact(const Expr& x, const Expr& y) {
  return (x/y).approx(CORE_INFTY, 2);
}

inline BigInt floor(const Expr& x) {
  BigInt r = ToBigInt(x, CORE_INFTY, 2);
  if (x - r >= 0)
    return r;
  else
    return --r;
}

inline BigInt ceil(const Expr& x) {
  return -floor(-x);
}

inline long floorLg(const Expr& x) {
  if (x < 1)
    return -ceilLg(ceil(1/x));
  else
    return floorLg(floor(x));
}

inline long ceilLg(const Expr& x) {
  if (x < 1)
    return -floorLg(floor(1/x));
  else
    return ceilLg(ceil(x));
}
CORE_END_NAMESPACE
#endif // __CORE_EXPR_H__
