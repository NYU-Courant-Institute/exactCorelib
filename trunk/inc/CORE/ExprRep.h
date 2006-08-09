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
 * $Id: ExprRep.h,v 1.10 2006-08-09 09:38:58 exact Exp $
 ***************************************************************************/
#ifndef __CORE_EXPRREP_H__
#define __CORE_EXPRREP_H__

#include <CORE/CoreAux.h>
#include <CORE/poly/Poly.h>
#include <CORE/poly/Sturm.h>
#include <bitset>
#include <iostream>

CORE_BEGIN_NAMESPACE

#define PREC_MIN        MPFR_PREC_MIN
#define DEF_INIT_PREC   53UL


/// \Pre-defined macros for user own operations
/// \Unary node
#define BEGIN_DEFINE_UNARY_NODE(cls_name)                     \
template <typename RootBd, typename Filter, typename Kernel>  \
class cls_name : public UnaryOpRepT<RootBd, Filter, Kernel> { \
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;           \
  typedef UnaryOpRepT<RootBd, Filter, Kernel> UnaryOpRep;     \
  using UnaryOpRep::child;                                    \
  using ExprRep::filter;                                      \
  using ExprRep::rootBd;                                      \
  using ExprRep::sign;                                        \
  using ExprRep::uMSB;                                        \
  using ExprRep::lMSB;                                        \
  using ExprRep::appValue;                                    \
  using ExprRep::abs2rel;                                     \
public:                                                       \
  cls_name(ExprRep* c) : UnaryOpRep(c)                        \
  { compute_filter(); compute_numtype(); }                    \
protected:

/// \Binary node
#define BEGIN_DEFINE_BINARY_NODE(cls_name)                    \
template <typename RootBd, typename Filter, typename Kernel>  \
class cls_name : public BinaryOpRepT<RootBd, Filter, Kernel> {\
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;           \
  typedef BinaryOpRepT<RootBd, Filter, Kernel> BinaryOpRep;   \
  using BinaryOpRep::first;                                   \
  using BinaryOpRep::second;                                  \
  using ExprRep::filter;                                      \
  using ExprRep::rootBd;                                      \
  using ExprRep::sign;                                        \
  using ExprRep::uMSB;                                        \
  using ExprRep::lMSB;                                        \
  using ExprRep::appValue;                                    \
  using ExprRep::abs2rel;                                     \
public:                                                       \
  cls_name(ExprRep* f, ExprRep* s, bool b = false)            \
   : BinaryOpRep(f, s, b)                                     \
  {compute_filter(); compute_numtype(); }                     \
protected:

/// \Knary node
#define BEGIN_DEFINE_KNARY_NODE(cls_name)                     \
template <typename RootBd, typename Filter, typename Kernel>  \
class cls_name : public KnaryOpRepT<RootBd, Filter, Kernel> { \
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;           \
  typedef KnaryOpRepT<RootBd, Filter, Kernel> KnaryOpRep;     \
  using KnaryOpRep::children;                                 \
  using ExprRep::filter;                                      \
  using ExprRep::rootBd;                                      \
  using ExprRep::sign;                                        \
  using ExprRep::uMSB;                                        \
  using ExprRep::lMSB;                                        \
  using ExprRep::appValue;                                    \
  using ExprRep::abs2rel;                                     \
public:                                                       \
  cls_name(const std::vector<ExprRep*>& c)                    \
   : KnaryOpRep(c)                                            \
  { compute_filter(); compute_numtype(); }                    \
protected:


#define END_DEFINE_UNARY_NODE };

#define BEGIN_DEFINE_RULE(fun_name)                           \
	virtual void fun_name() {

#define BEGIN_DEFINE_RULE_INSERT                              \
public:                                                       \
	void insert(ExprRep* c) {                             

#define BEGIN_DEFINE_RULE_FILTER                              \
	void compute_filter() {
#define BEGIN_DEFINE_RULE_NUMTYPE                             \
	void compute_numtype() {

#define BEGIN_DEFINE_RULE_SIGN                                \
	virtual bool compute_sign() {
#define BEGIN_DEFINE_RULE_UMSB                                \
	virtual bool compute_uMSB() {
#define BEGIN_DEFINE_RULE_LMSB                                \
	virtual bool compute_lMSB() {

#define BEGIN_DEFINE_RULE_R_APPROX                            \
	virtual bool compute_r_approx(prec_t prec) {
#define BEGIN_DEFINE_RULE_A_APPROX                            \
	virtual bool compute_a_approx(prec_t prec) {

#define BEGIN_DEFINE_RULE_ROOTBD                              \
	virtual void compute_rootbd(id_rootbd_t id) {

#define END_DEFINE_RULE }

#define DEFINE_UNARY_FUNCTION(fun_name, cls_name)             \
  template<typename T>                                        \
  Expr<T> fun_name(const Expr<T>& e)                          \
  { return new cls_name<T>(e.rep()); }				

#define DEFINE_BINARY_FUNCTION(fun_name, cls_name)            \
  template<typename T>                                        \
  Expr<T> fun_name(const Expr<T>& f, const Expr<T>& s)        \
  { return new cls_name<T>(f.rep(), s.rep()); }				

#define DEFINE_KNARY_FUNCTION(fun_name, cls_name)             \
  template<typename T>                                        \
  Expr<T> fun_name(const std::vector<Expr<T>>& e)             \
  { return new cls_name<T>(e);: }

/// \class ExprRepT
/// \brief represent an expression node in DAG
template <typename RootBd, typename Filter, typename Kernel>
class ExprRepT {
  typedef ExprRepT<RootBd, Filter, Kernel> thisClass;
  typedef RootBd* id_rootbd_t;
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
    if (filter().is_ok())
      return filter().sign();
    
    // initialize nodeinfo if necessary
    if (!m_nodeinfo)
      init_nodeinfo();
    else { 
      // if cache has sign
      if (flags().test(fSign))
        return sign();
      // if kernel has sign
      if (flags().test(fInit) && appValue().has_sign())
        return appValue().sgn();
    }
    // do exact evaluation
    if (!compute_sign()) 
      refine();
    
    flags().set(fSign);
    return sign(); 
  }
  /// return uMSB
  msb_t get_uMSB() { 
    // if filter works
    if (filter().is_ok())
      return filter().uMSB();
    // initialize nodeinfo if necessary
    if (!m_nodeinfo)
      init_nodeinfo();
    else {
      // if cache has uMSB
      if (flags().test(fuMSB))
        return uMSB();
      // if kernel is initialized
      if (flags().test(fInit))
        return appValue().uMSB();
    }
    // do exact evaluation
    if (!compute_uMSB())
      refine();
    
    flags().set(fuMSB);
    return uMSB(); 
  }
  /// return lMSB
  msb_t get_lMSB() {
    // if filter works
    if (filter().is_ok())
      return filter().lMSB();
    // initialize nodeinfo if necessary
    if (!m_nodeinfo)
      init_nodeinfo();
    else {
      // if cache has lMSB
      if (flags().test(flMSB))
        return lMSB();
      // if kernel is initialized
      if (flags().test(fInit))
        return appValue().lMSB();
    }
    // do exact evaluation
    if (!compute_lMSB())
      refine();
    
    flags().set(flMSB);
    return lMSB(); 
  }
  /// return root bound
  RootBd& get_rootBd(const id_rootbd_t id=0) { 
    if (!m_nodeinfo) 
      init_nodeinfo();

    rootBd().set_id(id == 0 ? rootBd().get_id():id);

    if (!rootBd().is_exact() || !flags().test(fRootBd)) {
      compute_rootBd(id);
      flags().set(fRootBd);
    }
    
    return rootBd(); 
  }
  /// return approximated value w/ relative precision
  Kernel& r_approx(prec_t prec) {
    // if filter works and has enough precision
    //if (filter().is_ok() && filter().get_r_prec() > prec)
    //{ return filter().get_r_prec(); }
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // do exact evaluation
    if (is_approx_needed(prec)) {
      if (compute_r_approx(prec)) flags().set(fExact);
      flags().set(fInit);
    }
    return appValue();
  }
  /// return approximated value w/ absolute precision
  Kernel& a_approx(prec_t prec) {
    // initialize nodeinfo if necessary
    if (!m_nodeinfo) init_nodeinfo();
    // if filter works and has enough precision
    //if (filter().is_ok() && filter().get_a_prec() > prec)
    //{ return filter().get_a_prec(); }
    // do exact evaluation
    if (is_approx_needed(abs2rel(prec))) {
      if (compute_a_approx(prec)) flags().set(fExact);
      flags().set(fInit);
    }
    return appValue();
  }
  /// check whether compute_a(r)_approx is needed
  bool is_approx_needed(prec_t prec)
  { return !flags().test(fInit) || (!is_exact() && get_prec() < prec); }

  /// check whether appValue() is exact
  bool is_exact() const
  { return flags().test(fExact); }

  void refine() {
    prec_t rootbd = get_rootBd().get_bound();
    
    for (prec_t prec=std::min(rootbd, DEF_INIT_PREC); prec<=rootbd; prec<<=1) {
      a_approx(prec);
      if (!appValue().has_zero()) {
        set_flags(appValue().sgn(), appValue().uMSB(), appValue().lMSB());
#ifdef CORE_DEBUG_ROOTBOUND
        std::cerr << "found sign =" << appValue().sgn() << std::endl;
#endif
        return;
      }
    }
#ifdef CORE_DEBUG_ROOTBOUND
    std::cerr << "root bound=" << rootBd().get_bound() << std::endl;
    rootBd().dump();
#endif
    set_flags(0, 0, MSB_MIN);
  }

protected: 
  /// convert absolute precision to relative precision
  prec_t abs2rel(prec_t prec)
  { return std::max(long(prec) + get_uMSB(), long(PREC_MIN)); }
  /// convert relative precision to absolute precision
  prec_t rel2abs(prec_t prec)
  { return std::max(long(prec) - get_lMSB(), long(PREC_MIN)); }

  /// set flags
  void set_flags(const sign_t& s, const msb_t& u, const msb_t& l) {
    assert(m_nodeinfo);
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
  virtual bool compute_sign()
  { assert(0); return false;}
  /// compute uMSB
  virtual bool compute_uMSB()
  { assert(0); return false;}
  /// compute lMSB
  virtual bool compute_lMSB()
  { assert(0); return false;}
  /// compute rootBd
  virtual void compute_rootBd(const id_rootbd_t id)
  {assert(0);}
  /// compute relative approximation (default)
  /// return true if the approximation can be represented exactly by MPFR) 
  virtual bool compute_r_approx(prec_t prec)
  { return compute_a_approx(rel2abs(prec)); } 
  /// compute absolute approximation (default)
  /// return true if the approximation can be represented exactly by MPFR) 
  virtual bool compute_a_approx(prec_t prec)
  { return compute_r_approx(abs2rel(prec)); }
public: 
  virtual size_t get_children_size()
  { return 0; }
  virtual thisClass* get_child(size_t t)
  { return 0; }

protected: 
  enum CachedFlags {
    fSign, fuMSB, flMSB, fRootBd, 
    fInit, fExact, numFlags
  };
  typedef std::bitset<numFlags> flag_t;

  const flag_t& flags() const { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_flags;
  }
  flag_t& flags()  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_flags;
  }

  const sign_t& sign() const { 
    assert(m_nodeinfo);
   return m_nodeinfo->m_sign;
  }
  sign_t& sign()  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_sign;
  }

  const msb_t& uMSB() const  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_uMSB;
  }
  msb_t& uMSB()  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_uMSB;
  }

  const msb_t& lMSB() const {
    assert(m_nodeinfo);
    return m_nodeinfo->m_lMSB;
  }
  msb_t& lMSB()  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_lMSB;
  }

  const RootBd& rootBd() const  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_rootBd;
  }
  RootBd& rootBd()  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_rootBd;
  }

  void new_nodeinfo() { m_nodeinfo = new NodeInfo(); }
public:
  Kernel& appValue()  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_appValue;
  }
  const Kernel& appValue() const  { 
    assert(m_nodeinfo);
    return m_nodeinfo->m_appValue;
  }
	
  Filter& filter() { return m_filter; }
  const Filter& filter() const { return m_filter; }
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
  const int get_ref() const
  { return m_ref_counter; }
private:
  int m_ref_counter;
};

/// \class ConstRepT 
/// \brief constant node
template <typename RootBd, typename Filter, typename Kernel, typename T>
class ConstRepT : public ExprRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef RootBd* id_rootbd_t;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
  using ExprRep::set_flags;
  using ExprRep::new_nodeinfo;
  using ExprRep::init_value;
  using ExprRep::init_nodeinfo;
protected:
  T value;
public:
  template <typename V>
  ConstRepT(const V& v) : value(v)
  { filter().set(v); }
  virtual ~ConstRepT() 
  {}

protected:
  virtual void init_nodeinfo() {
    new_nodeinfo();
    init_value(value);
  }

  virtual bool compute_sign()
  { sign() = sgn(value); return true; }
  virtual bool compute_uMSB()
  { uMSB() = ceillg(value); return true; }
  virtual bool compute_lMSB()
  { lMSB() = floorlg(value); return true; }
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().set(value); }
  virtual bool compute_r_approx(prec_t prec)
  { return appValue().set(value, prec); }

private:
  // generic version for BigInt, BigRat, Mpfr, BigFloat
  template <typename V> void init_flags(const V& v) 
  { set_flags(v.sgn(), v.uMSB(), v.lMSB()); }
  // specialized version for long
  void init_flags(long v) 
  { set_flags(sgn(v), ceillg(v), floorlg(v)); }
  // specialized version for unsigned long
  void init_flags(unsigned long v) 
  { set_flags(sgn(v), ceillg(v), floorlg(v)); }
  // specialized version for double
  void init_flags(double v) 
  { set_flags(sgn(v), ceillg(v), floorlg(v)); }
};

/// \class ConstRepT 
/// \brief constant node
template <typename RootBd, typename Filter, typename Kernel,typename NT>
class ConstPolyRepT : public ExprRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef RootBd* id_rootbd_t;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
protected:
  Kernel value;
  Sturm<NT> ss; ///< internal Sturm sequences
  BFInterval I; ///< current interval contains the real value
public:
  ConstPolyRepT(const Polynomial<NT>& p, int n) : ss(p) {
    I = ss.isolateRoot(n);
    // check whether n-th root exists
    if (I.first == 1 && I.second == 0) {
      core_error("CORE ERROR! root index out of bound", __FILE__, __LINE__, true);
      abort();
    }
    // refine initial interval to absolte error of 2^53
    I = ss.newtonRefine(I, 54);
    assert(I.first < I.second && I.first*I.second>=0);
    // we get an exact root
    if (I.first == I.second) value = I.first;
    else value.set(BigFloat2(I.first, I.second), 53);
    filter().set(value);
  }

  ConstPolyRepT(const Polynomial<NT>& p, const BFInterval& II) : ss(p), I(II) {
    BFVecInterval v;
    ss.isolateRoots(I.first, I.second, v);
    I = v.front();
    if (v.size() != 1) {
      core_error("CORE ERROR! root index out of bound", __FILE__, __LINE__, true);
      abort();
    }
    // refine initial interval to absolte error of 2^53
    I = ss.newtonRefine(I, 54);
    assert(I.first < I.second && I.first*I.second>=0);
    // we get an exact root
    if (I.first == I.second) value.set(I.first, 53);
    else value.set(BigFloat2(I.first, I.second), 53);
    filter().set(value);
  }

  virtual ~ConstPolyRepT() 
  {}

protected:
  virtual bool compute_sign()
  { sign() = value.sgn(); return true; }
  virtual bool compute_uMSB()
  { uMSB() = value.uMSB(); return true; }
  virtual bool compute_lMSB()
  { lMSB() = value.lMSB(); return true; }
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().set(value); }
  virtual bool compute_a_approx(prec_t prec) {
    I = ss.newtonRefine(I, prec+1);
    assert(I.first < I.second && I.first*I.second >= 0);
    if (I.first == I.second) value.set(I.first, prec);
    else value.set(BigFloat2(I.first, I.second), prec);
    return appValue().set(value);
  }
};

/// \class UnaryOpRepT
/// \brief unary operation expression node
template <typename RootBd, typename Filter, typename Kernel>
class UnaryOpRepT : public ExprRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
public:
  UnaryOpRepT(ExprRep* c) : child(c)
  { child->inc_ref(); }
  virtual ~UnaryOpRepT()
  { child->dec_ref(); }
  virtual size_t get_children_size()
  { return 1; }
  virtual ExprRep* get_child(size_t t)
  { return child; }

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
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef UnaryOpRepT<RootBd, Filter, Kernel> UnaryOpRep;
  typedef RootBd* id_rootbd_t;
  using UnaryOpRep::child;
  using UnaryOpRep::check_exact;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
  using ExprRep::abs2rel;
public:
  NegRepT(ExprRep* c) : UnaryOpRep(c) 
  { filter().neg(child->filter()); }
  virtual ~NegRepT() 
  {}
protected:
  /* notice here we use lazy evaluation */
  virtual bool compute_sign() 
  { sign() = -child->get_sign(); return true;}
  virtual bool compute_uMSB() 
  { uMSB() = child->get_uMSB(); return true;}
  virtual bool compute_lMSB() 
  { lMSB() = child->get_lMSB(); return true;}
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().neg(child->get_rootBd(id)); }
  virtual bool compute_r_approx(prec_t prec) {
    return check_exact(appValue().neg(child->r_approx(prec), prec));
  } 
  virtual bool compute_a_approx(prec_t prec) {
    return this->check_exact(appValue().neg(child->a_approx(prec), abs2rel(prec)));
  }
};

/// \class SqrtRepT
/// \brief square root node
template <typename RootBd, typename Filter, typename Kernel>
class SqrtRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef UnaryOpRepT<RootBd, Filter, Kernel> UnaryOpRep;
  typedef RootBd* id_rootbd_t;
  using UnaryOpRep::child;
  using UnaryOpRep::check_exact;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
  using ExprRep::abs2rel;
public:
  SqrtRepT(ExprRep* c) : UnaryOpRep(c) 
  { filter().sqrt(child->filter()); }
  virtual ~SqrtRepT() 
  {}
protected:
  virtual bool compute_sign() 
  { sign() = child->get_sign(); return true;}
  virtual bool compute_uMSB() 
  { uMSB() = (child->get_uMSB()+1) >> 1; return true;}
  virtual bool compute_lMSB() 
  { lMSB() = child->get_lMSB() >> 1; return true;}
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().root(child->get_rootBd(id), 2); }
  virtual bool compute_r_approx(prec_t prec) {
    return check_exact(appValue().sqrt(child->r_approx(prec*2), prec));
  }
};

/// \class CbrtRepT
/// \brief cubic root node
template <typename RootBd, typename Filter, typename Kernel>
class CbrtRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef UnaryOpRepT<RootBd, Filter, Kernel> UnaryOpRep;
  typedef RootBd* id_rootbd_t;
  using UnaryOpRep::child;
  using UnaryOpRep::check_exact;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
  using ExprRep::abs2rel;
public:
  CbrtRepT(ExprRep* c) : UnaryOpRep(c) 
  { filter().cbrt(child->filter()); }
  virtual ~CbrtRepT() 
  {}
protected:
  virtual bool compute_sign() 
  { sign() = child->get_sign(); return true;}
  virtual bool compute_uMSB() 
  { uMSB() = (child->get_uMSB()+2)/3; return true;}
  virtual bool compute_lMSB() 
  { lMSB() = child->get_lMSB()/3; return true;}
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().root(child->get_rootBd(id), 3); }
  virtual bool compute_r_approx(prec_t prec) {
    return check_exact(appValue().cbrt(child->r_approx(prec*3), prec));
  }
};

/// \class RootRepT
/// \brief k-th root node
template <typename RootBd, typename Filter, typename Kernel>
class RootRepT : public UnaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef UnaryOpRepT<RootBd, Filter, Kernel> UnaryOpRep;
  typedef RootBd* id_rootbd_t;
  using UnaryOpRep::child;
  using UnaryOpRep::check_exact;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
  using ExprRep::abs2rel;
public:
  RootRepT(ExprRep* c, unsigned long k) : UnaryOpRep(c), m_k(k) 
  { filter().root(child->filter(), k); }
  virtual ~RootRepT() 
  {}
protected:
  virtual bool compute_sign() 
  { sign() = child->get_sign(); return true;}
  virtual bool compute_uMSB() 
  { uMSB() = (child->get_uMSB()+m_k-1)/m_k; return true;}
  virtual bool compute_lMSB() 
  { lMSB() = child->get_lMSB()/m_k; return true;}
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().root(child->get_rootBd(id), m_k); }
  virtual bool compute_r_approx(prec_t prec) {
    return check_exact(appValue().root(child->r_approx(prec*m_k), m_k, prec));
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
  virtual size_t get_children_size()
  { return 2; }
  virtual ExprRep* get_child(size_t i)
  { return i==0 ? first : second; } 
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
  typedef BinaryOpRepT<RootBd, Filter, Kernel> BinaryOpRep;
  typedef RootBd* id_rootbd_t;
  using BinaryOpRep::first; 
  using BinaryOpRep::second; 
  using ExprRep::filter;
  using ExprRep::rootBd;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::abs2rel;
  using ExprRep::set_flags;
public:
  AddSubRepT(ExprRep* f, ExprRep* s, bool b = false) : BinaryOpRep(f, s, b)
  {filter().addsub(first->filter(),second->filter(), is_add);}
  virtual ~AddSubRepT() 
  {}
protected:
  virtual bool compute_sign() {
    sign_t sf = first->get_sign();
    sign_t ss = second->get_sign();
    if (!is_add) ss = -ss;
    if (sf == 0) // first operand is zero
      sign() = ss;
    else if (ss == 0) // second operand is zero
      sign() = sf;
    else if (sf == ss) // same sign
      sign() = sf;
    else { // different sign
     if (first->get_lMSB() > second->get_uMSB())
        sign() = sf;
      else if (first->get_uMSB() < second->get_lMSB())
        sign() = ss ;
      else // unknown sign!
        return false;
    }
    return true;
  }
  virtual bool compute_uMSB() {
    sign_t sf = first->get_sign();
    sign_t ss = second->get_sign();
    if (!is_add) ss = -ss;
    if (sf == 0) // first operand is zero
      uMSB() = second->get_uMSB();
    else if (ss == 0) // second operand is zero
      uMSB() = first->get_uMSB();
    else {
      msb_t uf = first->get_uMSB();
      msb_t us = second->get_uMSB();
      uMSB() = std::max(uf, us);
      if (sf == ss) uMSB() += 1;
    }
    return true;
  }
  virtual bool compute_lMSB() {
    sign_t sf = first->get_sign();
    sign_t ss = second->get_sign();
    if (!is_add) ss = -ss;
    if (sf == 0) // first operand is zero
      lMSB() = second->get_lMSB();
    else if (ss == 0) // second operand is zero
      lMSB() = first->get_lMSB();
    else {
      msb_t lf = first->get_lMSB();
      msb_t ls = second->get_lMSB();
      if (sf == ss) {// same sign
        if (lf == ls)
          lMSB() = lf + 1;
        else
          lMSB() = std::max(lf, ls);
      } else { // different sign
        if (lf > second->get_uMSB())
          lMSB() = lf - 1;
        else if (ls > first->get_uMSB())
          lMSB() = ls - 1;
        else // unknown lMSB()!
          return false;
      }
    }
    return true;
  }
  virtual void compute_rootBd(const id_rootbd_t id)
  {rootBd().addsub(first->get_rootBd(id),second->get_rootBd(id));}
  virtual bool compute_a_approx(prec_t prec) {
    return this->check_exact(appValue().addsub(first->a_approx(prec+2), second->a_approx(prec+2), abs2rel(prec+1), is_add)); 
  }
};

/// \class MulRepT
/// \brief multiplication node
template <typename RootBd, typename Filter, typename Kernel>
class MulRepT : public BinaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef BinaryOpRepT<RootBd, Filter, Kernel> BinaryOpRep;
  typedef RootBd* id_rootbd_t;
  using BinaryOpRep::first; 
  using BinaryOpRep::second; 
  using ExprRep::filter;
  using ExprRep::rootBd;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
public:
  MulRepT(ExprRep* f, ExprRep* s, bool b = false) : BinaryOpRep(f, s, b)
  { filter().mul(first->filter(), second->filter()); }
  virtual ~MulRepT() 
  {}
protected:
  virtual bool compute_sign() 
  { sign() = first->get_sign() * second->get_sign(); return true;}
  virtual bool compute_uMSB() 
  { uMSB() = first->get_uMSB() + second->get_uMSB(); return true;}
  virtual bool compute_lMSB() 
  { lMSB() = first->get_lMSB() + second->get_lMSB(); return true;}
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().mul(first->get_rootBd(id), second->get_rootBd(id)); }
  virtual bool compute_r_approx(prec_t prec) {
   return check_exact(appValue().mul(first->r_approx(prec+2), second->r_approx(prec+2), prec+1));
  }
};

/// \class DivRepT
/// \brief division node
template <typename RootBd, typename Filter, typename Kernel>
class DivRepT : public BinaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef BinaryOpRepT<RootBd, Filter, Kernel> BinaryOpRep;
  typedef RootBd* id_rootbd_t;
  using BinaryOpRep::first; 
  using BinaryOpRep::second; 
  using ExprRep::filter;
  using ExprRep::rootBd;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
public:
  DivRepT(ExprRep* f, ExprRep* s, bool b = false) : BinaryOpRep(f, s, b)
  { filter().div(first->filter(), second->filter()); }
  virtual ~DivRepT() 
  {}
protected:
  virtual bool compute_sign() 
  { sign() = first->get_sign() * second->get_sign(); return true;}
  virtual bool compute_uMSB() 
  { uMSB() = first->get_uMSB() - second->get_lMSB(); return true;}
  virtual bool compute_lMSB() 
  { lMSB() = first->get_lMSB() - second->get_uMSB(); return true;}
  virtual void compute_rootBd(const id_rootbd_t id)
  { rootBd().div(first->get_rootBd(id), second->get_rootBd(id)); }
  virtual bool compute_r_approx(prec_t prec) {
    return check_exact(appValue().div(first->r_approx(prec+2), second->r_approx(prec+2), prec+1));
  }
};

/// \class KnaryOpRepT
/// \brief k-nary operation expression node
template <typename RootBd, typename Filter, typename Kernel>
class KnaryOpRepT : public ExprRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
public:
  KnaryOpRepT() {}
  KnaryOpRepT(const std::vector<ExprRep*>& c, bool is_self = false) : children(c)
  {
    for (size_t i=0; i<children.size(); i++) {
      children[i]->inc_ref();
	}
	if (children.size()>0 && is_self) children[0]->dec_ref();
  }
  virtual ~KnaryOpRepT() {
    for (size_t i=0; i<children.size(); i++) 
      children[i]->dec_ref();
  }
  virtual size_t get_children_size()
  { return children.size(); }
  virtual ExprRep* get_child(size_t i)
  { return children[i]; } 
protected:
  std::vector<ExprRep*> children;  /// <- vector of pointers to children nodes
};

/// \class SumRepT
/// \brief summation node
template <typename RootBd, typename Filter, typename Kernel>
class SumOpRepT : public KnaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef SumOpRepT<RootBd, Filter, Kernel> SumOpRep;
  typedef AddSubRepT<RootBd, Filter, Kernel, true> AddOpRep;
  typedef KnaryOpRepT<RootBd, Filter, Kernel> KnaryOpRep;
  typedef RootBd* id_rootbd_t;
  using KnaryOpRep::children;
  using ExprRep::filter;
  using ExprRep::rootBd;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::abs2rel;
public:
  SumOpRepT(ExprRep* first, ExprRep* second, bool is_self = false) {
    filter().set(0L);
    insert(first, is_self);
    insert(second);
  }    
    
  SumOpRepT(const std::vector<ExprRep*>& c, bool is_self = false) : KnaryOpRep(c, is_self)  {
    compute_filter();
  }
  virtual ~SumOpRepT() {}
  
  void insert(ExprRep* c, bool is_self = false) {
    children.push_back(c);
    if (!is_self) c->inc_ref();
    filter().addsub(filter(), c->filter(), true);
  }
protected:
  void compute_filter () {
    filter().set(0L);
    for(size_t i=0; i<children.size(); i++)
      filter().addsub(filter(), children[i]->filter(), true);
  }

  virtual bool compute_sign() {
    return false;
  }
  virtual bool compute_uMSB() {
    uMSB() = children[0]->get_uMSB();
    for(size_t i = 1; i < children.size(); i++) {
      uMSB() = std::max(uMSB(), children[i]->get_uMSB()) + 1;
    }
    return true;
  }
  virtual bool compute_lMSB() {
    return false;
  }
  virtual void compute_rootBd(const id_rootbd_t id) {
    rootBd().addsub(children[0]->get_rootBd(id), children[1]->get_rootBd(id));
    for (size_t i = 2; i < children.size(); i++) {
      rootBd().addsub(rootBd(), children[i]->get_rootBd(id));
    }
  }
  virtual bool compute_a_approx(prec_t prec) {
    int n = ceillg(children.size()) + 1;
    Kernel Value;
    appValue().set(0);

    bool exact = true;
    for (size_t i=0; i<children.size(); i++) {
       bool r = Value.add(appValue(), children[i]->a_approx(prec+n), abs2rel(prec+n));
       if (!children[i]->is_exact() || !r) exact = false;
       Value.swap(appValue());
    }
    return exact;
  }
public:
  bool is_self_mergable(prec_t absprec = DEF_INIT_PREC) {
    if (this->get_ref() > 1) return false;
    if (!is_approx_needed(abs2rel(absprec))) return false;

    return true;
  }
}; 

/// \class SumRepT
/// \brief summation node
template <typename RootBd, typename Filter, typename Kernel>
class MulSumOpRepT : public KnaryOpRepT<RootBd, Filter, Kernel> {
  typedef ExprRepT<RootBd, Filter, Kernel> ExprRep;
  typedef KnaryOpRepT<RootBd, Filter, Kernel> KnaryOpRep;
  typedef RootBd* id_rootbd_t;
  using KnaryOpRep::children;
  using ExprRep::filter;
  using ExprRep::sign;
  using ExprRep::uMSB;
  using ExprRep::lMSB;
  using ExprRep::appValue;
  using ExprRep::rootBd;
public:
  MulSumOpRepT(const std::vector<ExprRep*>& c, bool is_self = false) : KnaryOpRep(c, is_self)  {
      compute_filter();
  }
  virtual ~MulSumOpRepT() {}
  
  void insert(ExprRep* c, bool is_self = false) {
    children.push_back(c);
    if (!is_self) c->inc_ref();
    filter().mul(filter(), c->filter());
  }
protected:
  void compute_filter () {
    filter().set(1L);
    for(size_t i=0; i<children.size(); i++)
      filter().mul(filter(), children[i]->filter());
  }

  virtual bool compute_sign() {
    sign_t tmpsgn = children[0]->get_sign();
    for(size_t i=1; i<children.size(); i++) {
      tmpsgn *= children[i]->get_sign();
    }
    sign() = tmpsgn;
    return true;
  }
  virtual bool compute_uMSB() {
    msb_t tmpuMSB = children[0]->get_uMSB();
    for(size_t i=1; i<children.size(); i++) {
      tmpuMSB += children[i]->get_uMSB();
    }
    uMSB() = tmpuMSB;
    return true;
  }
  virtual bool compute_lMSB() {
    msb_t tmplMSB = children[0]->get_lMSB();
    for(size_t i=1; i<children.size(); i++) {
      tmplMSB += children[i]->get_lMSB();
    }
    lMSB() = tmplMSB;
    return true;
  }
  virtual void compute_rootBd(const id_rootbd_t id) {
    rootBd().mul(children[0]->get_rootBd(id), children[1]->get_rootBd(id));
    for (size_t i=2; i<children.size(); i++) {
      rootBd().mul(rootBd(), children[i]->get_rootBd(id));
    }
  }
  virtual bool compute_r_approx(prec_t prec) {
    int n = ceillg(children.size()) + 1;
    Kernel Value;
    appValue().set(1);
    
    bool exact = true;
    for (size_t i=0; i<children.size(); i++) {
       bool r = Value.mul(appValue(), children[i]->r_approx(prec+n), prec+n);
       if (!children[i]->is_exact() || !r) exact = false;
       Value.swap(appValue());
    }
    return exact;
  }
}; 
/*
/// \class NegRepT macro version
/// \brief negation node
BEGIN_DEFINE_UNARY_NODE(NegRepT)
  BEGIN_DEFINE_RULE_FILTER
    filter().neg(child->m_filter);
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_NUMTYPE
//    this->_numType = this->child->_numType;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_SIGN
    sign() = -child->get_sign(); return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_UMSB
    uMSB() = child->get_uMSB(); return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_LMSB
    lMSB() = child->get_lMSB(); return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_R_APPROX
    return check_exact(appValue().neg(child->r_approx(prec), prec));
  END_DEFINE_RULE
    
  BEGIN_DEFINE_RULE_A_APPROX
    return check_exact(appValue().neg(child->a_approx(prec), abs2rel(prec)));
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_ROOTBD
    rootBd().neg(child->get_rootBd());
  END_DEFINE_RULE

END_DEFINE_UNARY_NODE

/// \class DivRepT macro version
/// \brief division node
BEGIN_DEFINE_BINARY_NODE(DivRepT)
  BEGIN_DEFINE_RULE_FILTER
    filter().div(first->filter(), second->filter());
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_NUMTYPE
//    this->_numType = this->child->_numType;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_SIGN
    sign() = first->get_sign() * second->get_sign(); return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_UMSB
    uMSB() = first->get_uMSB() - second->get_lMSB(); return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_LMSB
    lMSB() = first->get_lMSB() - second->get_uMSB(); return true;
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_ROOTBD
    rootBd().div(first->get_rootBd(), second->get_rootBd());
  END_DEFINE_RULE

  BEGIN_DEFINE_RULE_R_APPROX
    return check_exact(appValue().div(first->r_approx(prec+2), second->r_approx(prec+2), prec+1));
  END_DEFINE_RULE
END_DEFINE_UNARY_NODE
*/

CORE_END_NAMESPACE

#endif /*__CORE_EXPRREP_H__*/
