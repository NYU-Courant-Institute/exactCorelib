/****************************************************************************
 * ExprRep.h -- Internal Representation for Expr
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
 * $Id: ExprRep.h,v 1.7 2006-05-05 21:06:22 exact Exp $
 ***************************************************************************/
#ifndef __CORE_EXPRREP_H__
#define __CORE_EXPRREP_H__

#include <CORE/CoreAux.h>
#include <CORE/poly/Poly.h>
#include <bitset>
#include <iostream>

CORE_BEGIN_NAMESPACE

#define PREC_MIN        MPFR_PREC_MIN
#define DEF_INIT_PREC   53L

/// \class ExprRepT
/// \brief represent an expression node in DAG
template <typename RootBd, typename Filter, typename Kernel>
class ExprRepT {
protected:
  ExprRepT() : m_nodeinfo(0), m_ref_counter(1)
  {} 
  virtual ~ExprRepT()
  { if (m_nodeinfo) delete m_nodeinfo; }

public: // public methods
  /// return the current precision (relative)
  prec_t get_prec() const
  { return appValue().get_prec(); }

  /// return sign
  sign_t get_sign() {
    // if filter works
    if (m_filter.is_ok()) return m_filter.sign();
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // do exact evaluation
    if (!flags().test(fSign)) { compute_sign(); flags().set(fSign); }
    return sign(); 
  }
  /// return uMSB
  msb_t get_uMSB() { 
    // if filter works
    if (m_filter.is_ok()) return m_filter.uMSB();
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // do exact evaluation
    if (!flags().test(fuMSB)) { compute_uMSB(); flags().set(fuMSB); }
    return uMSB(); 
  }
  /// return lMSB
  msb_t get_lMSB() {
    // if filter works
    if (m_filter.is_ok()) return m_filter.lMSB();
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // do exact evaluation
    if (!flags().test(flMSB)) { compute_lMSB(); flags().set(flMSB); }
    return lMSB(); 
  }
  /// return root bound
  RootBd& get_rootBd() { 
    if (!m_nodeinfo) init_nodeinfo();
    if (!flags().test(fRootBd)) { compute_rootBd(); flags().set(fRootBd); }
    return rootBd(); 
  }
  /// return approximated value w/ relative precision
  Kernel& r_approx(prec_t prec) {
    // if filter works and has enough precision
    //if (m_filter.is_ok() && m_filter.get_r_prec() < prec) 
    //  return m_filter.r_approx(prec);
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // do exact evaluation
    if (!flags().test(fInit) || (!is_exact() && get_prec() < prec)) {
      if (compute_r_approx(prec)) flags().set(fExact);
      flags().set(fInit);
    }
    return appValue();
  }
  /// return approximated value w/ absolute precision
  Kernel& a_approx(prec_t prec) {
    // if filter works and has enough precision
    //if (m_filter.is_ok() && m_filter.get_a_prec() < prec) 
    //  return m_filter.a_approx(prec);
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // do exact evaluation
    if (!flags().test(fInit) || (!is_exact() && get_prec() < abs2rel(prec))) {
      if (compute_a_approx(prec)) flags().set(fExact);
      flags().set(fInit);
    }
    return appValue();
  }
  /// check whether appValue() is exact
  bool is_exact() const
  { return flags().test(fExact); }

protected: 
  /// convert absolute precision to relative precision
  prec_t abs2rel(prec_t prec)
  { return std::max(long(prec) + get_uMSB(), long(PREC_MIN)); }
  /// convert relative precision to absolute precision
  prec_t rel2abs(prec_t prec)
  { return std::max(long(prec) - get_lMSB(), long(PREC_MIN)); }

  /// set flags
  void set_flags(const sign_t& s, const msb_t& u, const msb_t& l) {
    sign() = s; flags().set(fSign);
    uMSB() = u; flags().set(fuMSB);
    lMSB() = l; flags().set(flMSB);
  }

  template <typename V>
  void init_value(const V& value) {
    // initialize approximated value
    if (this->appValue().set(value)) this->flags().set(fExact);
    // set flag
    flags().set(fInit);
  }
protected: // overridable methods
  /// initialize nodeinfo
  virtual void init_nodeinfo()
  { new_nodeinfo(); }
  /// compute sign
  virtual void compute_sign()
  {}
  /// compute uMSB
  virtual void compute_uMSB()
  {}
  /// compute lMSB
  virtual void compute_lMSB()
  {}
  /// compute rootBd
  virtual void compute_rootBd()
  {}
  /// compute relative approximation (default)
  /// return true if the approximation can be represented exactly by MPFR) 
  virtual bool compute_r_approx(prec_t prec)
  { return compute_a_approx(rel2abs(prec)); } 
  /// compute absolute approximation (default)
  /// return true if the approximation can be represented exactly by MPFR) 
  virtual bool compute_a_approx(prec_t prec)
  { return compute_r_approx(abs2rel(prec)); } 

protected: 
  enum CachedFlags {
    fSign, fuMSB, flMSB, fRootBd, 
    fInit, fExact, numFlags
  };
  typedef std::bitset<numFlags> flag_t;

  const flag_t& flags() const { return m_nodeinfo->m_flags; }
  flag_t& flags() { return m_nodeinfo->m_flags; }

  const sign_t& sign() const { return m_nodeinfo->m_sign; }
  sign_t& sign() { return m_nodeinfo->m_sign; }

  const msb_t& uMSB() const { return m_nodeinfo->m_uMSB; }
  msb_t& uMSB() { return m_nodeinfo->m_uMSB; }

  const msb_t& lMSB() const { return m_nodeinfo->m_lMSB; }
  msb_t& lMSB() { return m_nodeinfo->m_lMSB; }

  const RootBd& rootBd() const { return m_nodeinfo->m_rootBd; }
  RootBd& rootBd() { return m_nodeinfo->m_rootBd; }


  void new_nodeinfo() { m_nodeinfo = new NodeInfo(); }
public:
  Kernel& appValue() { return m_nodeinfo->m_appValue; }
  const Kernel& appValue() const { return m_nodeinfo->m_appValue; }
	
public:
  /// node information
  struct NodeInfo {
    flag_t  m_flags;    ///<- flags
    sign_t  m_sign;     ///<- sign
    msb_t   m_uMSB;     ///<- upper bound of Most Significant Bit
    msb_t   m_lMSB;     ///<- low bound of Most Significant Bit
    RootBd  m_rootBd;   ///<- root bound
    Kernel  m_appValue; ///<- apprixmated value
  };

  NodeInfo* m_nodeinfo; ///<- node information
  Filter    m_filter;   ///<- filter 

public: // reference counting
  void inc_ref() 
  { ++m_ref_counter; }
  void dec_ref()
  { if (--m_ref_counter == 0) delete this; }
private:
  int m_ref_counter;
};

/// \class ConstRepT 
/// \brief constant node
template <typename RootBd, typename Filter, typename Kernel, typename T>
class ConstRepT : public ExprRepT<RootBd, Filter, Kernel> {
protected:
  T value;
public:
  template <typename V>
  ConstRepT(const V& v) : value(v)
  { this->m_filter.set(v); }
  virtual ~ConstRepT() 
  {}

protected:
  virtual void init_nodeinfo() {
    this->new_nodeinfo();
    // initialize flags
    this->init_flags(value);
    // initialize approximated value
    this->init_value(value);
  }
  virtual void compute_rootBd()
  { this->rootBd().set(value); }
  virtual bool compute_a_approx(prec_t prec)
  { return this->appValue().set(this->value, prec); }

private:
  // generic version for BigInt, BigRat, Mpfr, BigFloat
  template <typename V> void init_flags(const V& v) 
  { this->set_flags(v.sgn(), v.uMSB(), v.lMSB()); }
  // specialized version for long
  void init_flags(long v) 
  { this->set_flags(sgn(v), ceillg(v), floorlg(v)); }
  // specialized version for unsigned long
  void init_flags(unsigned long v) 
  { this->set_flags(sgn(v), ceillg(v), floorlg(v)); }
  // specialized version for double
  void init_flags(double v) 
  { this->set_flags(sgn(v), ceillg(v), floorlg(v)); }
};

/// \class UnaryOpRepT
/// \brief unary operation expression node
template <typename RootBd, typename Filter, typename Kernel>
class UnaryOpRepT : public ExprRepT<RootBd, Filter, Kernel> {
public:
  UnaryOpRepT(ExprRepT<RootBd, Filter, Kernel>* c) : child(c)
  { child->inc_ref(); }
  virtual ~UnaryOpRepT()
  { child->dec_ref(); }
protected:
  /// check whether the operation and child node are both exact
  bool check_exact(bool ret)
  { return ret && child->is_exact(); }
  ExprRepT<RootBd, Filter, Kernel>* child; /// <- pointer to the child node
};

/// \class NegRepT
/// \brief negation node
template <typename RootBd, typename Filter, typename Kernel>
class NegRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
public:
  NegRepT(ExprRepT<RootBd, Filter, Kernel>* c) 
    : UnaryOpRepT<RootBd, Filter, Kernel>(c) 
  { this->m_filter.neg(this->child->m_filter); }
  virtual ~NegRepT() 
  {}
protected:
  /* notice here we use lazy evaluation */
  virtual void compute_sign() 
  { this->sign() = -this->child->get_sign(); }
  virtual void compute_uMSB() 
  { this->uMSB() = this->child->get_uMSB(); }
  virtual void compute_lMSB() 
  { this->lMSB() = this->child->get_lMSB(); }
  virtual void compute_rootBd()
  { this->rootBd().neg(this->child->get_rootBd()); }
  virtual bool compute_r_approx(prec_t prec) {
    return this->check_exact(this->appValue().neg(this->child->r_approx(prec), prec));
  } 
  virtual bool compute_a_approx(prec_t prec) {
    return this->check_exact(this->appValue().neg(this->child->a_approx(prec), this->abs2rel(prec)));
  }
};

/// \class SqrtRepT
/// \brief square root node
template <typename RootBd, typename Filter, typename Kernel>
class SqrtRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
public:
  SqrtRepT(ExprRepT<RootBd, Filter, Kernel>* c)
    : UnaryOpRepT<RootBd, Filter, Kernel>(c) 
  { this->m_filter.sqrt(this->child->m_filter); }
  virtual ~SqrtRepT() 
  {}
protected:
  virtual void compute_sign() 
  { this->sign() = this->child->get_sign(); }
  virtual void compute_uMSB() 
  { this->uMSB() = (this->child->get_uMSB()+1) >> 1; }
  virtual void compute_lMSB() 
  { this->lMSB() = this->child->get_lMSB() >> 1; }
  virtual void compute_rootBd()
  { this->rootBd().root(this->child->get_rootBd(), 2); }
  virtual bool compute_r_approx(prec_t prec) {
    return this->check_exact(this->appValue().sqrt(this->child->r_approx(prec*2), prec));
  }
};

/// \class CbrtRepT
/// \brief cubic root node
template <typename RootBd, typename Filter, typename Kernel>
class CbrtRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
public:
  CbrtRepT(ExprRepT<RootBd, Filter, Kernel>* c)
    : UnaryOpRepT<RootBd, Filter, Kernel>(c) 
  { this->m_filter.cbrt(this->child->m_filter); }
  virtual ~CbrtRepT() 
  {}
protected:
  virtual void compute_sign() 
  { this->sign() = this->child->get_sign(); }
  virtual void compute_uMSB() 
  { this->uMSB() = (this->child->get_uMSB()+2)/3; }
  virtual void compute_lMSB() 
  { this->lMSB() = this->child->get_lMSB()/3; }
  virtual void compute_rootBd()
  { this->rootBd().root(this->child->get_rootBd(), 3); }
  virtual bool compute_r_approx(prec_t prec) {
    return this->check_exact(this->appValue().cbrt(this->child->r_approx(prec*3), prec));
  }
};

/// \class RootRepT
/// \brief k-th root node
template <typename RootBd, typename Filter, typename Kernel>
class RootRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
public:
  RootRepT(ExprRepT<RootBd, Filter, Kernel>* c, unsigned long k)
    : UnaryOpRepT<RootBd, Filter, Kernel>(c), m_k(k) 
  { this->m_filter.root(this->child->m_filter, k); }
  virtual ~RootRepT() 
  {}
protected:
  virtual void compute_sign() 
  { this->sign() = this->child->get_sign(); }
  virtual void compute_uMSB() 
  { this->uMSB() = (this->child->get_uMSB()+m_k-1)/m_k; }
  virtual void compute_lMSB() 
  { this->lMSB() = this->child->get_lMSB()/m_k; }
  virtual void compute_rootBd()
  { this->rootBd().root(this->child->get_rootBd(), m_k); }
  virtual bool compute_r_approx(prec_t prec) {
    return this->check_exact(this->appValue().root(this->child->r_approx(prec*m_k), m_k, prec));
  }
private:
  unsigned long m_k;
};

/// \class BinaryOpRepT
/// \brief binary operation expression node
template <typename RootBd, typename Filter, typename Kernel>
class BinaryOpRepT : public ExprRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
public:
  BinaryOpRepT(ExprRep* f, ExprRep* s, bool is_self) : first(f), second(s)
  { if (!is_self) first->inc_ref(); second->inc_ref(); }
  virtual ~BinaryOpRepT()
  { first->dec_ref(); second->dec_ref(); }
  /// check whether the operation and child nodes are both exact
  bool check_exact(bool ret)
  { return ret && first->is_exact() && second->is_exact(); }
protected:
  ExprRep* first;  /// <- pointer to the first child node
  ExprRep* second; /// <- pointer to the second child node
};

/// \class AddSubRepT
/// \brief add/sub node
template <typename RootBd, typename Filter, typename Kernel, bool is_add>
class AddSubRepT : public BinaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
public:
  AddSubRepT(ExprRep* f, ExprRep* s, bool b = false)
   : BinaryOpRepT<RootBd, Filter, Kernel>(f, s, b)
  {this->m_filter.addsub(this->first->m_filter,this->second->m_filter, is_add);}
  virtual ~AddSubRepT() 
  {}
protected:
  virtual void compute_sign() {
    sign_t sf = this->first->get_sign();
    sign_t ss = this->second->get_sign();
    if (!is_add) ss = -ss;
    if (sf == 0) // first operand is zero
      this->sign() = ss;
    else if (ss == 0) // second operand is zero
      this->sign() = sf;
    else if (sf == ss) // same sign
      this->sign() = sf;
    else { // different sign
      if (this->first->get_lMSB() > this->second->get_uMSB())
        this->sign() = sf;
      else if (this->first->get_uMSB() < this->second->get_lMSB())
        this->sign() = ss ;
      else // unknown sign!
        refine();
    }
  }
  virtual void compute_uMSB() {
    sign_t sf = this->first->get_sign();
    sign_t ss = this->second->get_sign();
    if (!is_add) ss = -ss;
    if (sf == 0) // first operand is zero
      this->uMSB() = this->second->get_uMSB();
    else if (ss == 0) // second operand is zero
      this->uMSB() = this->first->get_uMSB();
    else {
      msb_t uf = this->first->get_uMSB();
      msb_t us = this->second->get_uMSB();
      this->uMSB() = std::max(uf, us);
      if (sf == ss) this->uMSB() += 1;
    }
  }
  virtual void compute_lMSB() {
    sign_t sf = this->first->get_sign();
    sign_t ss = this->second->get_sign();
    if (!is_add) ss = -ss;
    if (sf == 0) // first operand is zero
      this->lMSB() = this->second->get_lMSB();
    else if (ss == 0) // second operand is zero
      this->lMSB() = this->first->get_lMSB();
    else {
      msb_t lf = this->first->get_lMSB();
      msb_t ls = this->second->get_lMSB();
      if (sf == ss) {// same sign
        if (lf == ls)
          this->lMSB() = lf + 1;
        else
          this->lMSB() = std::max(lf, ls);
      } else { // different sign
        if (lf > this->second->get_uMSB())
          this->lMSB() = lf - 1;
        else if (ls > this->first->get_uMSB())
          this->lMSB() = ls - 1;
        else // unknown lMSB()!
          refine();
      }
    }
  }
  virtual void compute_rootBd()
  {this->rootBd().addsub(this->first->get_rootBd(),this->second->get_rootBd());}
  virtual bool compute_a_approx(prec_t prec) {
    return this->check_exact(this->appValue().addsub(this->first->a_approx(prec+2), this->second->a_approx(prec+2), this->abs2rel(prec+1), is_add)); 
  } 
private:
  void refine() {
    if (this->m_filter.is_ok()) {
      this->set_flags(this->m_filter.sign(), this->m_filter.uMSB(), this->m_filter.lMSB());
    }
    // we initially set sign as if the value is zero;
    Kernel value;
    if (this->rootBd().is_constructive()) this->compute_rootBd();
    msb_t rootbd = this->rootBd().get_bound();
    for (int prec=std::min(rootbd, DEF_INIT_PREC); prec<=rootbd; prec<<=1) {
      value.addsub(this->first->a_approx(prec+2), this->second->a_approx(prec+2), this->abs2rel(prec+1), is_add); 
      if (!value.has_zero()) {
#ifdef CORE_DEBUG_ROOTBOUND
        std::cerr << "found sign =" << value.sgn() << std::endl;
#endif
        this->set_flags(value.sgn(), value.uMSB(), value.lMSB());
        return;
      }
    }
#ifdef CORE_DEBUG_ROOTBOUND
    std::cerr << "root bound=" << this->rootBd().get_bound() << std::endl;
    this->rootBd().dump();
#endif
    this->set_flags(0, 0, MSB_MIN);
  }
};

/// \class MulRepT
/// \brief multiplication node
template <typename RootBd, typename Filter, typename Kernel>
class MulRepT : public BinaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef BinaryOpRepT<RootBd, Filter, Kernel> BinaryOpRep;
  using BinaryOpRep::first; 
  using BinaryOpRep::second; 
  using ExprRep::m_filter;
  using ExprRep::rootBd;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
public:
  MulRepT(ExprRep* f, ExprRep* s, bool b = false)
    : BinaryOpRepT<RootBd, Filter, Kernel>(f, s, b)
  //{ this->m_filter.mul(this->first->m_filter, this->second->m_filter); }
  { m_filter.mul(first->m_filter, second->m_filter); }
  virtual ~MulRepT() 
  {}
protected:
  virtual void compute_sign() 
  //{ this->sign() = this->first->get_sign() * this->second->get_sign(); }
  { sign() = first->get_sign() * second->get_sign(); }
  virtual void compute_uMSB() 
  //{ this->uMSB() = this->first->get_uMSB() + this->second->get_uMSB(); }
  { uMSB() = first->get_uMSB() + second->get_uMSB(); }
  virtual void compute_lMSB() 
  //{ this->lMSB() = this->first->get_lMSB() + this->second->get_lMSB(); }
  { lMSB() = first->get_lMSB() + second->get_lMSB(); }
  virtual void compute_rootBd()
  //{ this->rootBd().mul(this->first->get_rootBd(), this->second->get_rootBd()); }
  { rootBd().mul(first->get_rootBd(), second->get_rootBd()); }
  virtual bool compute_r_approx(prec_t prec) {
    //return this->check_exact(this->appValue().mul(this->first->r_approx(prec+2), this->second->r_approx(prec+2), prec+1));
    return check_exact(appValue().mul(first->r_approx(prec+2), second->r_approx(prec+2), prec+1));
  }
};

/// \class DivRepT
/// \brief division node
template <typename RootBd, typename Filter, typename Kernel>
class DivRepT : public BinaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
public:
  DivRepT(ExprRep* f, ExprRep* s, bool b = false)
    : BinaryOpRepT<RootBd, Filter, Kernel>(f, s, b)
  { this->m_filter.div(this->first->m_filter, this->second->m_filter); }
  virtual ~DivRepT() 
  {}
protected:
  virtual void compute_sign() 
  { this->sign() = this->first->get_sign() * this->second->get_sign(); }
  virtual void compute_uMSB() 
  { this->uMSB() = this->first->get_uMSB() - this->second->get_lMSB(); }
  virtual void compute_lMSB() 
  { this->lMSB() = this->first->get_lMSB() - this->second->get_uMSB(); }
  virtual void compute_rootBd()
  { this->rootBd().div(this->first->get_rootBd(), this->second->get_rootBd()); }
  virtual bool compute_r_approx(prec_t prec) {
    return this->check_exact(this->appValue().div(this->first->r_approx(prec+2), this->second->r_approx(prec+2), prec+1));
  }
};

CORE_END_NAMESPACE

#endif /*__CORE_EXPRREP_H__*/
