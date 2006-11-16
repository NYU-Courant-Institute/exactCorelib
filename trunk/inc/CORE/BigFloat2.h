/****************************************************************************
 * BigFloat2.h -- Big Floating-point number class providing arbitrary precision
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
 * $Id: BigFloat2.h,v 1.15 2006-11-16 17:41:02 exact Exp $
 ***************************************************************************/
#ifndef __CORE_BIGFLOAT2_H__
#define __CORE_BIGFLOAT2_H__

#include <CORE/BigFloat.h>
#include <CORE/Policies.h>

CORE_BEGIN_NAMESPACE

// BUG:
// 
//  1. all fixed version and auto version have problems when one of input 
//     are output since the set_prec function will reset the input value.
//

/// \class BigFloat2
/// \brief BigFloat2 is a floating-point interval class 
class BigFloat2 {
public: // public typedefs
  typedef BigInt   ZT;
  typedef BigRat   QT;
  typedef BigFloat FT;

public: //private:
  FT   m_l;      ///<- lower bound
  FT   m_r;      ///<- upper bound
  bool m_exact;  ///<- exact flag (when it is true, m_l is the exact value
                 //                and m_r is undefined)

public:
  /// \name constructors (auto version)
  //@{
  /// default constructor
  BigFloat2() : m_l(0), m_exact(true)
  {}
  /// copy constructor
  BigFloat2(const BigFloat2& r) : m_l(r.m_l), m_r(r.m_r), m_exact(r.m_exact)
  {}
  /// constructor from two BigFloat
  BigFloat2(const BigFloat& l, const BigFloat& r) : m_l(l),m_r(r),m_exact(false)
  {}
  /// generic constructor for <tt>T</TT>
  template <typename T> BigFloat2(const T& v) : m_l(v), m_exact(true)
  {}
  /// construct for <tt>QT = BigRat</tt>
  BigFloat2(const QT& q) 
  { set(q, DOUBLE_PREC); }
  /// construct for <tt>char*</tt> (no implicit conversion)
  explicit BigFloat2(const char* str, int base = 10) 
  { set(str, base); }
  /// construct for <tt>std::string</tt> (no implicit conversion)
  explicit BigFloat2(const std::string& str, int base = 10) 
  { set(str, base); }
  //@}

  /// \name constructors (fixed version)
  //@{
  /// generic constructor with specified precision (reduce to call set())
  template <typename T> BigFloat2(const T& v, prec_t prec) 
  { set(v, prec); }
  /// constructor for <tt>char*</tt>
  BigFloat2(const char* str, int base, prec_t prec) 
  { set(str, base, prec); }
  /// constructor for <tt>std::string</tt>
  BigFloat2(const std::string& str, int base, prec_t prec) 
  { set(str, base, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>char</tt>
  BigFloat2(char i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned char</tt>
  BigFloat2(unsigned char i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>short</tt>
  BigFloat2(short i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned short</tt>
  BigFloat2(unsigned short i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>int</tt>
  BigFloat2(int i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned int</tt>
  BigFloat2(unsigned int i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>long</tt>
  BigFloat2(long i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned long</tt>
  BigFloat2(unsigned long i, exp_t e, prec_t prec) 
  { set_2exp(i, e, prec); }
  //@}

public: 
  /// \name precision accessors
  //@{
  /// return current precision
  prec_t get_prec() const 
  { return is_exact()?m_l.get_prec():std::max(m_l.get_prec(),m_r.get_prec()); }
  /// set current precision
  void set_prec(prec_t prec)
  { m_l.set_prec(prec); if (!is_exact()) m_r.set_prec(prec); }
  //@}

  /// \name exactness accessors
  //@{
  /// return exactness
  bool is_exact() const
  { return m_exact; };
  /// set exactness
  void set_exact(bool b)
  { m_exact = b; }
  //@}

  /// \name assignment functions (raw version)
  //@{
  /// assignment function for <tt>BigFloat2</tt>
  bool r_set(const BigFloat2& x)
  { return _set_f<RawArithmeticPolicy>(x); }
  /// assignment function for <tt>char*</tt>
  bool r_set(const char* x, int base = 10)
  { return _set_str<RawArithmeticPolicy>(x, base); }
  /// assignment function for <tt>std::string</tt>
  bool r_set(const std::string& x, int base = 10)
  { return _set_str<RawArithmeticPolicy>(x.c_str(), base); }
  /// generic assignment function for <tt>T</tt>
  template <typename T> bool r_set(const T& x)
  { return _set<RawArithmeticPolicy, T>(x); }
  /// set value to be \f$i*2^e\f$ for <tt>int</tt>
  bool r_set_2exp(int i, exp_t e)
  { return _set_2exp_si<RawArithmeticPolicy>(long(i), e); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned int</tt>
  bool r_set_2exp(unsigned int i, exp_t e)
  { return _set_2exp_ui<RawArithmeticPolicy>((unsigned long)(i), e); }
  /// set value to be \f$i*2^e\f$ for <tt>long</tt>
  bool r_set_2exp(long i, exp_t e)
  { return _set_2exp_si<RawArithmeticPolicy>(i, e); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned long</tt>
  bool r_set_2exp(unsigned long i, exp_t e)
  { return _set_2exp_ui<RawArithmeticPolicy>(i, e); }
  //@}

  /// \name assignment functions (fixed version)
  //@{
  /// assignment function for <tt>BigFloat2</tt>
  bool set(const BigFloat2& x, prec_t prec)
  { return _set_f<FixedArithmeticPolicy>(x, prec); }
  /// assignment function for <tt>char*</tt>
  bool set(const char* x, int base, prec_t prec)
  { return _set_str<FixedArithmeticPolicy>(x, base, prec); }
  /// assignment function for <tt>std::string</tt>
  bool set(const std::string& x, int base, prec_t prec)
  { return _set_str<FixedArithmeticPolicy>(x.c_str(), base, prec); }
  /// assignment function for <tt>T</tt>
  template <typename T> bool set(const T& x, prec_t prec)
  { return _set<FixedArithmeticPolicy, T>(x, prec); }

  /// set value to be \f$i*2^e\f$ for <tt>int</tt>
  bool set_2exp(int i, exp_t e, prec_t prec)
  { return _set_2exp_si<FixedArithmeticPolicy>(long(i), e, prec); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned int</tt>
  bool set_2exp(unsigned int i, exp_t e, prec_t prec)
  { return _set_2exp_ui<FixedArithmeticPolicy>((unsigned long)(i), e, prec); }
  /// set value to be \f$i*2^e\f$ for <tt>long</tt>
  bool set_2exp(long i, exp_t e, prec_t prec)
  { return _set_2exp_si<FixedArithmeticPolicy>(i, e, prec); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned long</tt>
  bool set_2exp(unsigned long i, exp_t e, prec_t prec)
  { return _set_2exp_ui<FixedArithmeticPolicy>(i, e, prec); }
  //@}

  /// \name assignment functions (auto version)
  //@{
  /// assignment function for <tt>BigFloat2</tt>
  bool set(const BigFloat2& x)
  { return _set_f<AutoArithmeticPolicy>(x); }
  /// assignment function for <tt>char*</tt>
  bool set(const char* x, int base = 10)
  { return _set_str<AutoArithmeticPolicy>(x, base); }
  /// assignment function for <tt>std::string</tt>
  bool set(const std::string& x, int base = 10)
  { return _set_str<AutoArithmeticPolicy>(x.c_str(), base); }
  /// assignment function for <tt>T</tt>
  template <typename T> bool set(const T& x)
  { return _set<AutoArithmeticPolicy, T>(x); }

  /// set value to be \f$i*2^e\f$ for <tt>int</tt>
  bool set_2exp(int i, exp_t e)
  { return _set_2exp_si<AutoArithmeticPolicy>(long(i), e); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned int</tt>
  bool set_2exp(unsigned int i, exp_t e)
  { return _set_2exp_ui<AutoArithmeticPolicy>((unsigned long)(i), e); }
  /// set value to be \f$i*2^e\f$ for <tt>long</tt>
  bool set_2exp(long i, exp_t e)
  { return _set_2exp_si<AutoArithmeticPolicy>(i, e); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned long</tt>
  bool set_2exp(unsigned long i, exp_t e)
  { return _set_2exp_ui<AutoArithmeticPolicy>(i, e); }
  //@}

  /// \name arithmetic functions (raw version)
  //@{
  /// negation for <tt>BigFloat2</tt>
  bool r_neg(const BigFloat2& x)
  { return _neg_f<RawArithmeticPolicy>(x); }
  /// negation for <tt>T</tt>
  template <typename T> bool r_neg(const T& x)
  { return _neg<RawArithmeticPolicy, T>(x); }

  /// square root for <tt>BigFloat2</tt>
  bool r_sqrt(const BigFloat2& x)
  { return _sqrt_f<RawArithmeticPolicy>(x); }
  /// square root for <tt>T</tt>
  template <typename T> bool r_sqrt(const T& x)
  { return _sqrt<RawArithmeticPolicy, T>(x); }

  /// cubic root for <tt>BigFloat2</tt>
  bool r_cbrt(const BigFloat2& x)
  { return _cbrt_f<RawArithmeticPolicy>(x); }
  // /// cubic root for <tt>T</tt>
  // template <typename T> bool r_cbrt(const T& x)
  // { return _cbrt<RawArithmeticPolicy, T>(x); }

  /// k-th root for <tt>BigFloat2</tt>
  bool r_root(const BigFloat2& x, unsigned long k)
  { return _root_f<RawArithmeticPolicy>(x, k); }
  // /// k-th root for <tt>T</tt>
  // template <typename T> bool r_root(const T& x, unsigned long k)
  // { return _root<RawArithmeticPolicy, T>(x, k); }

  /// addition/subtraction for <tt>BigFloat2</tt>
  bool r_addsub(const BigFloat2& x, const BigFloat2& y, bool is_add)
  { return is_add ? r_add(x, y) : r_sub(x, y); }
  /// addition/subtraction for <tt>BigFloat2, T</tt>
  template <typename T>
  bool r_addsub(const BigFloat2& x, const T& y, bool is_add)
  { return is_add ? r_add(x, y) : r_sub(x, y); }
  /// addition/subtraction for <tt>T, BigFloat2</tt>
  template <typename T>
  bool r_addsub(const T& x, const BigFloat2& y, bool is_add)
  { return is_add ? r_add(x, y) : r_sub(x, y); }

  /// addition for <tt>BigFloat2+BigFloat2</tt>
  bool r_add(const BigFloat2& x, const BigFloat2& y)
  { return _add_f<RawArithmeticPolicy>(x, y); }
  /// addition for <tt>BigFloat2+T</tt>
  template <typename T> bool r_add(const BigFloat2& x, const T& y)
  { return _add<RawArithmeticPolicy, T>(x, y); }
  /// addition for <tt>T+BigFloat2</tt>
  template <typename T> bool r_add(const T& x, const BigFloat2& y)
  { return _add<RawArithmeticPolicy, T>(x, y); }

  /// subtraction for <tt>BigFloat2-BigFloat2</tt>
  bool r_sub(const BigFloat2& x, const BigFloat2& y)
  { return _sub_f<RawArithmeticPolicy>(x, y); }
  /// subtraction for <tt>BigFloat2-T</tt>
  template <typename T> bool r_sub(const BigFloat2& x, const T& y)
  { return _sub<RawArithmeticPolicy, T>(x, y); }
  /// subtraction for <tt>T-BigFloat2</tt>
  template <typename T> bool r_sub(const T& x, const BigFloat2& y)
  { return _sub<RawArithmeticPolicy, T>(x, y); }

  /// multiplication for <tt>BigFloat2*BigFloat2</tt>
  bool r_mul(const BigFloat2& x, const BigFloat2& y)
  { return _mul_f<RawArithmeticPolicy>(x, y); }
  /// multiplication for <tt>BigFloat2*T</tt>
  template <typename T> bool r_mul(const BigFloat2& x, const T& y)
  { return _mul<RawArithmeticPolicy, T>(x, y); }
  /// multiplication for <tt>T*BigFloat2</tt>
  template <typename T> bool r_mul(const T& x, const BigFloat2& y)
  { return _mul<RawArithmeticPolicy, T>(x, y); }

  /// division for <tt>BigFloat2/BigFloat2</tt>
  bool r_div(const BigFloat2& x, const BigFloat2& y)
  { return _div_f<RawArithmeticPolicy>(x, y); }
  /// division for <tt>BigFloat2/T</tt>
  template <typename T> bool r_div(const BigFloat2& x, const T& y)
  { return _div<RawArithmeticPolicy, T>(x, y); }
  /// division for <tt>T/BigFloat2</tt>
  template <typename T> bool r_div(const T& x, const BigFloat2& y)
  { return _div<RawArithmeticPolicy, T>(x, y); }
  //@}

  /// \name arithmetic functions (fixed version)
  //@{
  /// negation for <tt>BigFloat2</tt>
  bool neg(const BigFloat2& x, prec_t prec)
  { return _neg_f<FixedArithmeticPolicy>(x, prec); }
  /// negation for <tt>T</tt>
  template <typename T> bool neg(const T& x, prec_t prec)
  { return _neg<FixedArithmeticPolicy, T>(x, prec); }

  /// square root for <tt>BigFloat2</tt>
  bool sqrt(const BigFloat2& x, prec_t prec = getDefaultBFradicalPrec())
  { return _sqrt_f<FixedArithmeticPolicy>(x, prec); }
  /// square root for <tt>T</tt>
  template <typename T> bool sqrt(const T& x, prec_t prec = getDefaultBFradicalPrec())
  { return _sqrt<FixedArithmeticPolicy, T>(x, prec); }

  /// cubic root for <tt>BigFloat2</tt>
  bool cbrt(const BigFloat2& x, prec_t prec = getDefaultBFradicalPrec())
  { return _cbrt_f<FixedArithmeticPolicy>(x, prec); }
  // /// cubic root for <tt>T</tt>
  // template <typename T> bool cbrt(const T& x, prec_t prec = getDefaultBFradicalPrec())
  // { return _cbrt<FixedArithmeticPolicy, T>(x, prec); }

  /// k-th root for <tt>BigFloat2</tt>
  bool root(const BigFloat2& x, unsigned long k, prec_t prec = getDefaultBFradicalPrec())
  { return _root_f<FixedArithmeticPolicy>(x, k, prec); }
  // /// k-th root for <tt>T</tt>
  // template <typename T> 
  // bool root(const T& x, unsigned long k, prec_t prec = getDefaultBFradicalPrec())
  // { return _root<FixedArithmeticPolicy, T>(x, k, prec); }

  /// addition/subtraction for <tt>BigFloat2</tt>
  bool addsub(const BigFloat2& x, const BigFloat2& y, prec_t prec, bool is_add)
  { return is_add ? add(x, y, prec) : sub(x, y, prec); }
  /// addition/subtraction for <tt>BigFloat2, T</tt>
  template <typename T>
  bool addsub(const BigFloat2& x, const T& y, prec_t prec, bool is_add)
  { return is_add ? add(x, y, prec) : sub(x, y, prec); }
  /// addition/subtraction for <tt>T, BigFloat2</tt>
  template <typename T>
  bool addsub(const T& x, const BigFloat2& y, prec_t prec, bool is_add)
  { return is_add ? add(x, y, prec) : sub(x, y, prec); }

  /// addition for <tt>BigFloat2+BigFloat2</tt>
  bool add(const BigFloat2& x, const BigFloat2& y, prec_t prec)
  { return _add_f<FixedArithmeticPolicy>(x, y, prec); }
  /// addition for <tt>BigFloat2+T</tt>
  template <typename T> bool add(const BigFloat2& x, const T& y, prec_t prec)
  { return _add<FixedArithmeticPolicy, T>(x, y, prec); }
  /// addition for <tt>T+BigFloat2</tt>
  template <typename T> bool add(const T& x, const BigFloat2& y, prec_t prec)
  { return _add<FixedArithmeticPolicy, T>(x, y, prec); }

  /// subtraction for <tt>BigFloat2-BigFloat2</tt>
  bool sub(const BigFloat2& x, const BigFloat2& y, prec_t prec)
  { return _sub_f<FixedArithmeticPolicy>(x, y, prec); }
  /// subtraction for <tt>BigFloat2-T</tt>
  template <typename T> bool sub(const BigFloat2& x, const T& y, prec_t prec)
  { return _sub<FixedArithmeticPolicy, T>(x, y, prec); }
  /// subtraction for <tt>T-BigFloat2</tt>
  template <typename T> bool sub(const T& x, const BigFloat2& y, prec_t prec)
  { return _sub<FixedArithmeticPolicy, T>(x, y, prec); }

  /// multiplication for <tt>BigFloat2*BigFloat2</tt>
  bool mul(const BigFloat2& x, const BigFloat2& y, prec_t prec)
  { return _mul_f<FixedArithmeticPolicy>(x, y, prec); }
  /// multiplication for <tt>BigFloat2*T</tt>
  template <typename T> bool mul(const BigFloat2& x, const T& y, prec_t prec)
  { return _mul<FixedArithmeticPolicy, T>(x, y, prec); }
  /// multiplication for <tt>T*BigFloat2</tt>
  template <typename T> bool mul(const T& x, const BigFloat2& y, prec_t prec)
  { return _mul<FixedArithmeticPolicy, T>(x, y, prec); }

  /// division for <tt>BigFloat2/BigFloat2</tt>
  bool div(const BigFloat2& x, const BigFloat2& y, prec_t prec = getDefaultBFdivPrec())
  { return _div_f<FixedArithmeticPolicy>(x, y, prec); }
  /// division for <tt>BigFloat2/T</tt>
  template <typename T> 
  bool div(const BigFloat2& x, const T& y, prec_t prec = getDefaultBFdivPrec())
  { return _div<FixedArithmeticPolicy, T>(x, y, prec); }
  /// division for <tt>T/BigFloat2</tt>
  template <typename T> 
  bool div(const T& x, const BigFloat2& y, prec_t prec = getDefaultBFdivPrec())
  { return _div<FixedArithmeticPolicy, T>(x, y, prec); }
  //@}

  /// \name arithmetic functions (auto version)
  /// NOTE: no auto version for sqrt, cbrt, root, div
  //@{
  /// negation for <tt>BigFloat2</tt>
  bool neg(const BigFloat2& x)
  { return _neg_f<AutoArithmeticPolicy>(x); }
  /// negation for <tt>T</tt>
  template <typename T> bool neg(const T& x)
  { return _neg<AutoArithmeticPolicy, T>(x); }

  /// addition/subtraction for <tt>BigFloat2</tt>
  bool addsub(const BigFloat2& x, const BigFloat2& y, bool is_add)
  { return is_add ? add(x, y) : sub(x, y); }
  /// addition/subtraction for <tt>BigFloat2, T</tt>
  template <typename T>
  bool addsub(const BigFloat2& x, const T& y, bool is_add)
  { return is_add ? add(x, y) : sub(x, y); }
  /// addition/subtraction for <tt>T, BigFloat2</tt>
  template <typename T>
  bool addsub(const T& x, const BigFloat2& y, bool is_add)
  { return is_add ? add(x, y) : sub(x, y); }

  /// addition for <tt>BigFloat2+BigFloat2</tt>
  bool add(const BigFloat2& x, const BigFloat2& y)
  { return _add_f<AutoArithmeticPolicy>(x, y); }
  /// addition for <tt>BigFloat2+T</tt>
  template <typename T> bool add(const BigFloat2& x, const T& y)
  { return _add<AutoArithmeticPolicy, T>(x, y); }
  /// addition for <tt>T+BigFloat2</tt>
  template <typename T> bool add(const T& x, const BigFloat2& y)
  { return _add<AutoArithmeticPolicy, T>(x, y); }

  /// subtraction for <tt>BigFloat2-BigFloat2</tt>
  bool sub(const BigFloat2& x, const BigFloat2& y)
  { return _sub_f<AutoArithmeticPolicy>(x, y); }
  /// subtraction for <tt>BigFloat2-T</tt>
  template <typename T> bool sub(const BigFloat2& x, const T& y)
  { return _sub<AutoArithmeticPolicy, T>(x, y); }
  /// subtraction for <tt>T-BigFloat2</tt>
  template <typename T> bool sub(const T& x, const BigFloat2& y)
  { return _sub<AutoArithmeticPolicy, T>(x, y); }

  /// multiplication for <tt>BigFloat2*BigFloat2</tt>
  bool mul(const BigFloat2& x, const BigFloat2& y)
  { return _mul_f<AutoArithmeticPolicy>(x, y); }
  /// multiplication for <tt>BigFloat2*T</tt>
  template <typename T> bool mul(const BigFloat2& x, const T& y)
  { return _mul<AutoArithmeticPolicy, T>(x, y); }
  /// multiplication for <tt>T*BigFloat2</tt>
  template <typename T> bool mul(const T& x, const BigFloat2& y)
  { return _mul<AutoArithmeticPolicy, T>(x, y); }
  //@}

public:
  /// \name conversion functions
  //@{
  /// return double value
  double get_d() const {
    return m_l.get_d();
  }
  /// return the string representation
  std::string get_str(size_t digits = 0, int base = 10) const {
    if (is_exact()) {
      return m_l.get_str(digits, base);
    } else {
      long bits = abs_diam().get_exp();
      
      // round up to validate bits
      long valprec = m_l.get_exp() - bits;
     
      if (valprec <= 0) {
        valprec = bits;
      }
      if (valprec < 2) valprec = 2;

      size_t valid_digits = (size_t)(valprec*log(2.0)/log(double(base)));
      
      if (digits > 0U && digits < valid_digits)
        valid_digits = digits;
      if (valid_digits < 2)
        valid_digits = 2;
  
      return m_l.get_str(valid_digits, base);
    }
  }
  /// return <tt>BigInt</tt> value
  ZT get_z() const
  { return m_l.get_z(); }
  /// return <tt>BigRat</tt> value
  QT get_q() const {
    return m_l.get_q();
  }
  /// return <tt>BigFloat</tt> value
  FT get_f() const {
    if (is_exact()) 
      return m_l;
    else {
      //BigFloat2 ret = (*this);
      //return ret.centerize();

      // get validate bits
      long bits = abs_diam().get_exp();
      // round up to validate bits
      FT result(m_l);
      //std::cout << "\nbits=" << bits << std::endl;

      long valprec = m_l.get_exp() - bits;
      // jihun 2006: in case of m_r.get_exp() > m_l.get_exp(),
      // e.g. m_r = 0.1 * 2^-426, m_l = 0.1 * 2^-423, bits = -423
      // I dont know how to deal with this.
      if (valprec <= 0) {
        valprec = bits;
        if (valprec < 0) valprec -= valprec;
      }
      if (valprec < 2) valprec = 2;

      result.prec_round(valprec);
/*
      std::cout << "\n   m_r's prec=" << m_r.get_prec() << std::endl;
      std::cout << "result's prec=" << result.get_prec() << std::endl;
      std::cout << "   m_l's prec=" << m_l.get_prec() << std::endl;

      std::cout << "\n   m_r=" << m_r << std::endl;
      std::cout << "result=" << result << std::endl;
      std::cout << "   m_l=" << m_l << std::endl;
*/
      return result;
    }
  }
  //@}

  /// \name miscellaneous functions
  //@{
  /// swap function
  void swap(BigFloat2 &rhs)
  { m_l.swap(rhs.m_l); m_r.swap(rhs.m_r); std::swap(m_exact, rhs.m_exact); }
  /// return absolute diameter
  FT abs_diam() const
  { FT diam; abs_diam(diam); return diam; }
  /// get absolute diameter, return 0 if exact
  int abs_diam(FT& diam) const {
    if (is_exact()) {
      return diam.set(0);
    } else {
      diam.set_prec(get_prec()); return diam.sub(m_r, m_l); 
    }
  }
  void makeCeilExact() { 
    if (is_exact()) return;  
    m_l = m_r; m_r = 0; m_exact = true;
  }
  void makeFloorExact() { 
    if (is_exact()) return;  
    m_r = 0; m_exact = true;
  }
  void makeExact() { 
    set_exact(true);
  }
  BigFloat getLeft() const {
    return m_l;
  }
  BigFloat getRight() const {
    if (is_exact())
      return m_l;
    else
      return m_r;
  }
  BigFloat centerize() {
    if (is_exact()) return m_l;  
    m_l.add(m_l, m_r);
    m_l.div_2exp(m_l, 1);
    m_r = 0; m_exact = true;
    return m_l;
  }
  /// check whether contains zero
  bool has_zero() const
  { return is_exact() ? (m_l.sgn()==0) : (m_l.sgn()<=0 && m_r.sgn()>=0); } 
  bool isZeroIn() const
  { return has_zero(); }
  bool has_sign() const
  { return !has_zero(); }
  /// return sign
  int sgn() const {
    if (is_exact()) {
      return m_l.sgn();
    } else if (m_l.sgn() > 0) {
      return 1;
    } else if (m_r.sgn() < 0) {
      return -1;
    } else {
#ifndef NDEBUG
      std::cerr << "BigFloat2 Warning: cannot get correct sign!" << std::endl;
      std::cerr << "m_l:m_r=" << m_l << ":" << m_r << std::endl;
#endif
      return 0;
    }
  }
  /// return upper bound of MSB
  long uMSB() const
  { return is_exact() ? m_l.uMSB() : m_r.uMSB(); }
  /// return lower bound of MSB
  long lMSB() const
  { return m_l.lMSB(); }
  //@}

  /// \name helper functions
  //@{
  /// set to be zero
  void set_zero()
  { m_l.set(0); m_exact = true; }
  /// set to be \f$[-\infty, +\infty]\f$
  void set_inf()
  { m_l.set_neg_inf(); m_r.set_pos_inf(); m_exact = false; }
  /// set to be NaN
  void set_nan()
  { m_l.set_nan(); m_exact = true; }
  bool is_integer() const
  { return is_exact() && m_l.is_integer(); }
  //@}

public: // C++ style operators
  /// \name unary, increment, decrement operators
  //@{ 
  /// unary plus operator
  BigFloat2 operator+() const
  { return BigFloat2(*this); }
  /// unary negation operator
  BigFloat2 operator-() const
  { BigFloat2 r; r.neg(*this); return r; }
  /// prefix increment operator
  BigFloat2& operator++()
  { *this += 1; return *this; }
  /// postfix increment operator
  BigFloat2 operator++(int)
  { BigFloat2 r(*this); ++(*this); return r; }
  /// prefix decrement operator
  BigFloat2& operator--()
  { *this -= 1; return *this; }
  /// postfix decrement operator
  BigFloat2 operator--(int)
  { BigFloat2 r(*this); --(*this); return r; }
  //@}

  /// \name assignment and compound assignment operators (call auto version)
  //@{
  /// assignment operator for <tt>BigFloat2</tt>
  BigFloat2& operator=(const BigFloat2& rhs)
  { if (&rhs != this) set(rhs); return *this; }
  /// generic assignment operator for <tt>T</tt>
  template <typename T> BigFloat2& operator=(const T& rhs)
  { set(rhs); return *this; }
  /// generic compound assignment operator <tt>+=</tt>
  template <typename T> BigFloat2& operator+=(const T& rhs) 
  { BigFloat2 t; t.add(*this, rhs); t.swap(*this); return *this; }
  /// generic compound assignment operator <tt>-=</tt>
  template <typename T> BigFloat2& operator-=(const T& rhs) 
  { BigFloat2 t; t.sub(*this, rhs); t.swap(*this); return *this; }
  /// generic compound assignment operator <tt>*=</tt>
  template <typename T> BigFloat2& operator*=(const T& rhs) 
  { BigFloat2 t; t.mul(*this, rhs); t.swap(*this); return *this; }
  /// generic compound assignment operator <tt>/=</tt>
  template <typename T> BigFloat2& operator/=(const T& rhs) 
  { BigFloat2 t; t.div(*this, rhs); t.swap(*this); return *this; }
  //@}

#ifdef CORE_OLDNAMES
  /// \name back-compatiable functions
  //@{
  /// Has Exact Division
  static bool hasExactDivision() { return false; }
  /// sign function
  /** \note This is only the sign of the mantissa, it can be taken to be
      the sign of the BigFloat2 only if !(isZeroIn()). */
  int sign() const { return sgn(); }
  /// check whether contains zero
  /** \return true if contains zero, otherwise false */
  bool isZeroIn() const { return has_zero(); }
  /// set value from <tt>const char*</tt> (base = 10)
  void fromString(const char* s) { set(s, 10); }
  /// convert to <tt>std::string</tt> (base = 10)
  std::string toString() const { return get_str(); }
  /// convert to <tt>std::string</tt> (base = 10)
  std::string str() const { return toString(); }
  /// return int value
  int intValue() const { return static_cast<int>(get_d()); }
  /// return long value
  int longValue() const { return static_cast<long>(get_d()); }
  /// return float value
  float floatValue() const { return static_cast<float>(get_d()); }
  /// return double value
  double doubleValue() const { return get_d(); }
  /// return BigInt value
  BigInt BigIntValue() const { return get_z(); }
  /// return BigRat value
  BigRat BigRatValue() const { return get_q(); }
  //@}
#endif
private:
  // assignment
  template <template <typename, typename, typename> class Policy>
  bool _set_f(const BigFloat2& x, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy>
  bool _set_str(const char* x, int base, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _set(const T& x, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy>
  bool _set_2exp_si(long, exp_t e, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy>
  bool _set_2exp_ui(unsigned long, exp_t e, prec_t prec = 0);

  // negation
  template <template <typename, typename, typename> class Policy>
  bool _neg_f(const BigFloat2& x, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _neg(const T& x, prec_t prec = 0);
  
  // square root
  template <template <typename, typename, typename> class Policy>
  bool _sqrt_f(const BigFloat2& x, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _sqrt(const T& x, prec_t prec = 0);
  
  // cubic root
  template <template <typename, typename, typename> class Policy>
  bool _cbrt_f(const BigFloat2& x, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _cbrt(const T& x, prec_t prec = 0);
  
  // k-th root
  template <template <typename, typename, typename> class Policy>
  bool _root_f(const BigFloat2& x, unsigned long k, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _root(const T& x, unsigned long k, prec_t prec = 0);
  
  // addition
  template <template <typename, typename, typename> class Policy>
  bool _add_f(const BigFloat2& x, const BigFloat2& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _add(const BigFloat2& x, const T& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _add(const T& x, const BigFloat2& y, prec_t prec = 0);

  // subtraction 
  template <template <typename, typename, typename> class Policy>
  bool _sub_f(const BigFloat2& x, const BigFloat2& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _sub(const BigFloat2& x, const T& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _sub(const T& x, const BigFloat2& y, prec_t prec = 0);

  // multiplication
  template <template <typename, typename, typename> class Policy>
  bool _mul_f(const BigFloat2& x, const BigFloat2& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _mul(const BigFloat2& x, const T& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _mul(const T& x, const BigFloat2& y, prec_t prec = 0);

  // division
  template <template <typename, typename, typename> class Policy>
  bool _div_f(const BigFloat2& x, const BigFloat2& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _div(const BigFloat2& x, const T& y, prec_t prec = 0);
  template <template <typename, typename, typename> class Policy, typename T>
  bool _div(const T& x, const BigFloat2& y, prec_t prec = 0);
};

/// \addtogroup BigFloat2ArithmeticOperators
//@{
/// BigFloat2 + BigFloat2
inline BigFloat2 operator+(const BigFloat2& x, const BigFloat2& y)
{ BigFloat2 r; r.add(x, y); return r; }
/// BigFloat2 + T
//template <typename T> inline BigFloat2 operator+(const BigFloat2& x, const T& y)
//{ BigFloat2 r; r.add(x, y); return r; }
/// T + BigFloat2
//template <typename T> inline BigFloat2 operator+(const T& x, const BigFloat2& y)
//{ BigFloat2 r; r.add(x, y); return r; }

/// BigFloat2 - BigFloat2
inline BigFloat2 operator-(const BigFloat2& x, const BigFloat2& y)
{ BigFloat2 r; r.sub(x, y); return r; }
/// BigFloat2 - T
template <typename T> inline BigFloat2 operator-(const BigFloat2& x, const T& y)
{ BigFloat2 r; r.sub(x, y); return r; }
/// T - BigFloat2
template <typename T> inline BigFloat2 operator-(const T& x, const BigFloat2& y)
{ BigFloat2 r; r.sub(x, y); return r; }

/// BigFloat2 * BigFloat2
inline BigFloat2 operator*(const BigFloat2& x, const BigFloat2& y)
{ BigFloat2 r; r.mul(x, y); return r; }
/// BigFloat2 * T
template <typename T> inline BigFloat2 operator*(const BigFloat2& x, const T& y)
{ BigFloat2 r; r.mul(x, y); return r; }
/// T * BigFloat2
template <typename T> inline BigFloat2 operator*(const T& x, const BigFloat2& y)
{ BigFloat2 r; r.mul(x, y); return r; }

/// BigFloat2 / BigFloat2 (w/ default precision getDefaultBFdivPrec())
inline BigFloat2 operator/(const BigFloat2& x, const BigFloat2& y)
{ BigFloat2 r; r.div(x, y); return r; }
/// BigFloat2 / T (w/ default precision getDefaultBFdivPrec())
template <typename T> inline BigFloat2 operator/(const BigFloat2& x, const T& y)
{ BigFloat2 r; r.div(x, y); return r; }
/// T / BigFloat2 (w/ default precision getDefaultBFdivPrec())
template <typename T> inline BigFloat2 operator/(const T& x, const BigFloat2& y)
{ BigFloat2 r; r.div(x, y); return r; }
//@}

/// \addtogroup BigFloat2IostreamOperators
//@{
/// istream operator for <tt>BigFloat2</tt>
inline std::istream& operator>>(std::istream& is, BigFloat2& x)
{ BigFloat2::FT tmp; is >> tmp; x.set(tmp); return is; }
/// ostream operator for <tt>BigFloat2</tt>
inline std::ostream& operator<<(std::ostream& os, const BigFloat2& x)
{ return os << x.get_str(get_output_precision()); }
//@}

/// \addtogroup BigFloat2GlobalFunctions
//@{
/// square root
inline BigFloat2 sqrt(const BigFloat2& x, prec_t prec = getDefaultBFradicalPrec())
{ BigFloat2 r; r.sqrt(x, prec); return r; }
/// cubic root
inline BigFloat2 cbrt(const BigFloat2& x, prec_t prec = getDefaultBFradicalPrec())
{ BigFloat2 r; r.cbrt(x, prec); return r; }
/// k-th root
inline BigFloat2 root(const BigFloat2& x, unsigned long k, prec_t prec = getDefaultBFradicalPrec())
{ BigFloat2 r; r.root(x, k, prec); return r; }
//@}

// include inline functions (private)
#include <CORE/BigFloat2.inl>
/*
// convert any type to BigFloat
template <typename T>
inline BigFloat2 ToBigFloat2(const T& x, prec_t r = defRelPrec) 
{ std::cout << "r=" << r << std::endl; return BigFloat2(x, r); }
*/
inline int sign(const BigFloat2& x) 
{ return x.sgn(); }

#ifdef CORE_OLDNAMES 
/// \addtogroup BigFloat2BackCompatiableFunctions
//@{
//@}
#endif

CORE_END_NAMESPACE

#endif /*__CORE_BIGFLOAT2_H__*/
