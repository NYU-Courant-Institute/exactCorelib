/****************************************************************************
 * BigFloat.h -- A C++ wrapper class for MPFR mpfr
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
 * $Id: BigFloat.h,v 1.3 2006-03-01 01:04:05 exact Exp $
 ***************************************************************************/
#ifndef __BIGFLOAT_H__
#define __BIGFLOAT_H__

#include <CORE/Mpfr.h>
#include <string>
#include <iostream>
#include <cassert>
#include <cmath>

/* Known Issues:

  1. mpfr_set_str() returns 0 if the string is a valid number, otherwise 
     returns -1. So you cannot get the exactness of the results.

  2. C++ style operators didn't work now since I haven't implement automatic
     version functions.
  
  3. when implementing automatic version, need notice the input could be the
     output, in that case, set_prec will reset the input value. 
 */
/* subtraction */
/* TODO: need take more care of rounding mode */
inline int mpfr_z_sub(mpfr_ptr z, mpz_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ int r = mpfr_sub_z(z, y, x, rnd); mpfr_neg(z, z, rnd); return -r; }
inline int mpfr_q_sub(mpfr_ptr z, mpq_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ int r = mpfr_sub_q(z, y, x, rnd); mpfr_neg(z, z, rnd); return -r; }

/* remove trailing zeros (by limbs) */
void mpfr_remove_trailing_zeros(mpfr_t x);
/* C++-style input of mpfr */
std::istream& operator>> (std::istream &, mpfr_ptr);
/* convert mpfr to string */
std::string mpfr2str(mpfr_srcptr mp, size_t ndigits, int base, bool fixed, rnd_t rnd = MPFR_RND);
  
#ifdef CORE_BEGIN_NAMESPACE
CORE_BEGIN_NAMESPACE
#endif

// constant of default precision for integer, IEEE single and double
const size_t INT_PREC = sizeof(int)*8;
const size_t SINGLE_PREC = 24;
const size_t DOUBLE_PREC = 53;
 
const prec_t MPFR_DEF_SQRT_PREC = DOUBLE_PREC;
const prec_t MPFR_DEF_CBRT_PREC = DOUBLE_PREC;
const prec_t MPFR_DEF_ROOT_PREC = DOUBLE_PREC;
const prec_t MPFR_DEF_DIV_PREC = DOUBLE_PREC;

#ifndef CORE_DISABLE_REFCOUNTING
  typedef RcMpfr BigFloatBase;
#else
  typedef Mpfr BigFloatBase;
#endif

/// \class BigFloat BigFloat.h
/// \brief BigFloat is a wrapper class of <tt>mpfr</tt> in MPFR
class BigFloat : public BigFloatBase {
  typedef BigFloatBase base_cls;
public: // public typedefs
  typedef BigInt ZT;
  typedef BigRat QT;
public:
  /// \name constructors (auto version)
  //@{
  /// default constructor
  BigFloat() : base_cls() {}
  /// copy constructor
  BigFloat(const BigFloat& rhs) : base_cls(rhs) {}
  /// constructor for <tt>int</tt> (use INT_PREC by default)
  BigFloat(int i, rnd_t rnd = MPFR_RND)
    : base_cls(static_cast<long>(i), INT_PREC, rnd) {}
  /// constructor for <tt>unsigned int</tt> (use INT_PREC by default)
  BigFloat(unsigned int i, rnd_t rnd = MPFR_RND)
    : base_cls(static_cast<unsigned long>(i), INT_PREC, rnd) {}
  /// constructor for <tt>long</tt> (use INT_PREC by default)
  BigFloat(long i, rnd_t rnd = MPFR_RND)
    : base_cls(i, INT_PREC, rnd) {}
  /// constructor for <tt>unsigned long</tt> (use INT_PREC by default)
  BigFloat(unsigned long i, rnd_t rnd = MPFR_RND)
    : base_cls(i, INT_PREC, rnd) {}
  /// constructor for <tt>double</tt> (use DOUBLE_PREC by default)
  BigFloat(double i, rnd_t rnd = MPFR_RND)
    : base_cls(i, DOUBLE_PREC, rnd) {}
  /// constructor for <tt>BigInt</tt> 
  BigFloat(const BigInt& x, rnd_t rnd = MPFR_RND)
    : base_cls(x.mp(), count_prec(x), rnd) {}
  /// constructor for <tt>BigRat</tt> (use DOUBLE_PREC by default)
  BigFloat(const BigRat& x, rnd_t rnd = MPFR_RND)
    : base_cls(x.mp(), count_prec(x), rnd) {}
  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit BigFloat(const char* s, int base = 10, rnd_t rnd = MPFR_RND)
    : base_cls(s, base, count_prec(s), rnd) {}
  /// constructor for <tt>char</tt> (no implicit conversion)
  explicit BigFloat(const std::string& s, int base = 10, rnd_t rnd = MPFR_RND)
    : base_cls(s.c_str(), base, count_prec(s.c_str()), rnd) {}
  //@}

  /// \name constructors (fixed version)
  //@{
  /// constructor for <tt>BigFloat</tt> with specified precision
  BigFloat(const BigFloat& rhs, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(rhs, prec, rnd) {}
  /// constructor for <tt>short</tt> with specified precision
  BigFloat(int i, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(static_cast<long>(i), prec, rnd) {}
  /// constructor for <tt>unsigned short</tt> with specified precision
  BigFloat(unsigned int i, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(static_cast<unsigned long>(i), prec, rnd) {}
  /// constructor for <tt>long</tt> with specified precision
  BigFloat(long i, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(i, prec, rnd) {}
  /// constructor for <tt>unsigned long</tt> with specified precision
  BigFloat(unsigned long i, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(i, prec, rnd) {}
  /// constructor for <tt>double</tt> with specified precision
  BigFloat(double i, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(i, prec, rnd) {}
  /// constructor for <tt>BigInt</tt> with specified precision
  BigFloat(const BigInt& x, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(x.mp(), prec, rnd) {}
  /// constructor for <tt>BigRat</tt> with specified precision
  BigFloat(const BigRat& x, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(x.mp(), prec, rnd) {}
  /// constructor for <tt>char*</tt> with specified precision
  explicit BigFloat(const char* s, int base, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(s, base, prec, rnd) {}
  /// constructor for <tt>std::string</tt> with specified precision
  explicit BigFloat(const std::string& s,int b,prec_t prec,rnd_t rnd=MPFR_RND)
    : base_cls(s.c_str(), b, prec, rnd) {}

  /// constructor with value \f$i*2^e\f$ for <tt>int</tt>
  BigFloat(int i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(static_cast<long>(i), e, prec, rnd) {}
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned int</tt>
  BigFloat(unsigned int i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(static_cast<unsigned long>(i), e, prec, rnd) {}
  /// constructor with value \f$i*2^e\f$ for <tt>long</tt>
  BigFloat(long i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(i, e, prec, rnd) {}
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned long</tt>
  BigFloat(unsigned long i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
    : base_cls(i, e, prec, rnd) {}
  //@}

public:
  /// \name precision accessors
  //@{
  /// return current precision
  prec_t get_prec() const
  { return mpfr_get_prec(mp()); }
  /// set current precision
  void set_prec(prec_t prec)
  { mpfr_set_prec(mp(), prec); }
  //@}

  /// \name exponent accessors
  //@{
  /// return exponent
  exp_t get_exp() const
  { return sgn() ? mpfr_get_exp(mp()) : 0; }
  /// set exponent (never need?)
  void set_exp(exp_t e)
  { mpfr_set_exp(mp(), e); }
  //@}

public:
  /// \name assignment functions (raw version)
  //@{
  /// assignment functions for <tt>BigFloat</tt>
  int set(const BigFloat& rhs, rnd_t rnd = MPFR_RND)
  { return mpfr_set(mp(), rhs.mp(), rnd); }
  /// assignment functions for <tt>int</tt>
  int set(int i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si(mp(), i, rnd); }
  /// assignment functions for <tt>unsigned int</tt>
  int set(unsigned int i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui(mp(), i, rnd); }
  /// assignment functions for <tt>long</tt>
  int set(long i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si(mp(), i, rnd); }
  /// assignment functions for <tt>unsigned long</tt>
  int set(unsigned long i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui(mp(), i, rnd); }
  /// assignment functions for <tt>double</tt>
  int set(double i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_d(mp(), i, rnd); }
  /// assignment functions for <tt>BigInt</tt>
  int set(const BigInt& x, rnd_t rnd = MPFR_RND)
  { return mpfr_set_z(mp(), x.mp(), rnd); }
  /// assignment functions for <tt>BigRat</tt>
  int set(const BigRat& x, rnd_t rnd = MPFR_RND) // use DOUBLE_PREC
  { return mpfr_set_q(mp(), x.mp(), rnd); }
  /// assignment functions for <tt>char*</tt>
  int set(const char* str, int base = 10, rnd_t rnd = MPFR_RND)
  { return mpfr_set_str(mp(), str, base, rnd); }
  /// assignment functions for <tt>std::string</tt>
  int set(const std::string& str, int base = 10, rnd_t rnd = MPFR_RND)
  { return mpfr_set_str(mp(), str.c_str(), base, rnd); }

  /// set value to be \f$i*2^e\f$ for <tt>int</tt>
  int set_2exp(int i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si_2exp(mp(), i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned int</tt>
  int set_2exp(unsigned int i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui_2exp(mp(), i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>long</tt>
  int set_2exp(long i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si_2exp(mp(), i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned long</tt>
  int set_2exp(unsigned long i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui_2exp(mp(), i, e, rnd); }
  //@}

public:
  /// \name arithmetic functions -- addition/subtraction (raw version)
  //@{
  /// addition/subtraction for <tt>BigFloat</tt>
  int addsub(const BigFloat& x, const BigFloat& y, bool isadd, rnd_t rnd = MPFR_RND)
  { return isadd ? add(x, y, rnd) : sub(x, y, rnd); }
  /// addition/subtraction for <tt>BigFloat, T</tt>
  template <typename T>
  int addsub(const BigFloat& x, const T& y, bool isadd, rnd_t rnd = MPFR_RND)
  { return isadd ? add(x, y, rnd) : sub(x, y, rnd); }
  /// addition/subtraction for <tt>T, BigFloat</tt>
  template <typename T>
  int addsub(const T& x, const BigFloat& y, bool isadd, rnd_t rnd = MPFR_RND)
  { return isadd ? add(x, y, rnd) : sub(x, y, rnd); }
  //@}

  /// \name arithmetic functions -- addition (raw version)
  //@{
  /// addition for <tt>BigFloat+BigFloat</tt>
  int add(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_add(mp(), x.mp(), y.mp(), rnd); }
  /// addition for <tt>BigFloat+int</tt>
  int add(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return add(x, static_cast<long>(y), rnd); }
  /// addition for <tt>BigFloat+unsigned int</tt>
  int add(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return add(x, static_cast<unsigned long>(y), rnd); }
  /// addition for <tt>BigFloat+long</tt>
  int add(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_si(mp(), x.mp(), y, rnd); }
  /// addition for <tt>BigFloat+unsigned long</tt>
  int add(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_ui(mp(), x.mp(), y, rnd); }
  /// addition for <tt>BigFloat+double</tt>
  int add(const BigFloat& x, double y, rnd_t rnd = MPFR_RND)
  { return add(x, BigFloat(y), rnd); }
  /// addition for <tt>BigFloat+BigInt</tt>
  int add(const BigFloat& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_z(mp(), x.mp(), y.mp(), rnd); }
  /// addition for <tt>BigFloat+BigRat</tt>
  int add(const BigFloat& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_q(mp(), x.mp(), y.mp(), rnd); }
  template <typename T> 
  int add(const T& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return add(y, x, rnd); }
  //@}

  /// \name arithmetic functions -- subtraction (raw version)
  //@{
  /// subtraction for <tt>BigFloat-BigFloat</tt>
  int sub(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub(mp(), x.mp(), y.mp(), rnd); }
  /// subtraction for <tt>BigFloat-int</tt>
  int sub(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return sub(x, static_cast<long>(y), rnd); }
  /// subtraction for <tt>BigFloat-unsigned int</tt>
  int sub(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return sub(x, static_cast<unsigned long>(y), rnd); }
  /// subtraction for <tt>BigFloat-long</tt>
  int sub(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_si(mp(), x.mp(), y, rnd); }
  /// subtraction for <tt>BigFloat-unsigned long</tt>
  int sub(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_ui(mp(), x.mp(), y, rnd); }
  /// subtraction for <tt>BigFloat-double</tt>
  int sub(const BigFloat& x, double y, rnd_t rnd = MPFR_RND)
  { return sub(x, BigFloat(y), rnd); }
  /// subtraction for <tt>BigFloat-BigInt</tt>
  int sub(const BigFloat& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_z(mp(), x.mp(), y.mp(), rnd); }
  /// subtraction for <tt>BigFloat-BigRat</tt>
  int sub(const BigFloat& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_q(mp(), x.mp(), y.mp(), rnd); }
  /// subtraction for <tt>int-BigFloat</tt>
  int sub(int x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return sub(static_cast<long>(x), y, rnd); }
  /// subtraction for <tt>unsigned int-BigFloat</tt>
  int sub(unsigned int x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return sub(static_cast<unsigned long>(x), y, rnd); }
  /// subtraction for <tt>long-BigFloat</tt>
  int sub(long x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_sub(mp(), x, y.mp(), rnd); }
  /// subtraction for <tt>unsigned long-BigFloat</tt>
  int sub(unsigned long x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_sub(mp(), x, y.mp(), rnd); }
  /// subtraction for <tt>double-BigFloat</tt>
  int sub(double x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return sub(BigFloat(x), y, rnd); }
  /// subtraction for <tt>BigInt-BigFloat</tt>
  int sub(const BigInt& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_z_sub(mp(), x.mp(), y.mp(), rnd); }
  /// subtraction for <tt>BigRat-BigFloat</tt>
  int sub(const BigRat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_q_sub(mp(), x.mp(), y.mp(), rnd); }
  //@}
  
  /// \name arithmetic functions -- multiplication (raw version)
  //@{
  /// multiplication for <tt>BigFloat*BigFloat</tt>
  int mul(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul(mp(), x.mp(), y.mp(), rnd); }
  /// multiplication for <tt>BigFloat*int</tt>
  int mul(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return mul(x, static_cast<long>(y), rnd); }
  /// multiplication for <tt>BigFloat*unsigned int</tt>
  int mul(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mul(x, static_cast<unsigned long>(y), rnd); }
  /// multiplication for <tt>BigFloat*long</tt>
  int mul(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_si(mp(), x.mp(), y, rnd); }
  /// multiplication for <tt>BigFloat*unsigned long</tt>
  int mul(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_ui(mp(), x.mp(), y, rnd); }
  /// multiplication for <tt>BigFloat*double</tt>
  int mul(const BigFloat& x, double y, rnd_t rnd = MPFR_RND)
  { return mul(x, BigFloat(y), rnd); }
  /// multiplication for <tt>BigFloat*BigInt</tt>
  int mul(const BigFloat& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_z(mp(), x.mp(), y.mp(), rnd); }
  /// multiplication for <tt>BigFloat*BigRat</tt>
  int mul(const BigFloat& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_q(mp(), x.mp(), y.mp(), rnd); }
  template <typename T> 
  int mul(const T& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mul(y, x, rnd); }
  //@}

  /// \name arithmetic functions -- division (raw version)
  //@{
  /// division for <tt>BigFloat/BigFloat</tt>
  int div(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_div(mp(), x.mp(), y.mp(), rnd); }
  /// division for <tt>BigFloat/int</tt>
  int div(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return div(x, static_cast<long>(y), rnd); }
  /// division for <tt>BigFloat/unsigned int</tt>
  int div(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return div(x, static_cast<unsigned long>(y), rnd); }
  /// division for <tt>BigFloat/long</tt>
  int div(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_si(mp(), x.mp(), y, rnd); }
  /// division for <tt>BigFloat/unsigned long</tt>
  int div(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_ui(mp(), x.mp(), y, rnd); }
  /// division for <tt>BigFloat/double</tt>
  int div(const BigFloat& x, double y, rnd_t rnd = MPFR_RND)
  { return div(x, BigFloat(y), rnd); }
  /// division for <tt>BigFloat/BigInt</tt>
  int div(const BigFloat& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_z(mp(), x.mp(), y.mp(), rnd); }
  /// division for <tt>BigFloat/BigRat</tt>
  int div(const BigFloat& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_q(mp(), x.mp(), y.mp(), rnd); }
  /// division for <tt>int/BigFloat</tt>
  int div(int x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return div(static_cast<long>(x), y, rnd); }
  /// division for <tt>unsigned int/BigFloat</tt>
  int div(unsigned int x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return div(static_cast<unsigned long>(x), y, rnd); }
  /// division for <tt>long/BigFloat</tt>
  int div(long x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_div(mp(), x, y.mp(), rnd); }
  /// division for <tt>unsigned long/BigFloat</tt>
  int div(unsigned long x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_div(mp(), x, y.mp(), rnd); }
  /// division for <tt>double/BigFloat</tt>
  int div(double x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return div(BigFloat(x), y, rnd); }
  /// division for <tt>BigInt/BigFloat</tt>
  int div(const BigInt& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return div(BigFloat(x), y, rnd); }
  /// division for <tt>BigRat/BigFloat</tt> (BigRat will be converted)
  int div(const BigRat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return div(BigFloat(x), y, rnd); }
  //@}

  /// \name square root functions (raw version)
  //@{
  /// square root function for <tt>BigFloat</tt>
  int sqrt(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt(mp(), x.mp(), rnd); }
  /// square root function for <tt>int</tt>
  int sqrt(int x, rnd_t rnd = MPFR_RND)
  { return sqrt(static_cast<long>(x), rnd); }
  /// square root function for <tt>unsigned int</tt>
  int sqrt(unsigned int x, rnd_t rnd = MPFR_RND)
  { return sqrt(static_cast<unsigned long>(x), rnd); }
  /// square root function for <tt>long</tt>
  int sqrt(long x, rnd_t rnd = MPFR_RND)
  { assert(x>=0); return mpfr_sqrt_ui(mp(), x, rnd); }
  /// square root function for <tt>unsigned long</tt>
  int sqrt(unsigned long x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_ui(mp(), x, rnd); }
  /// square root function for <tt>double</tt>
  int sqrt(double x, rnd_t rnd = MPFR_RND)
  { return sqrt(BigFloat(x), rnd); }
  //@}

  /// \name power functions (raw version)
  //@{
  /// power function for <tt>BigFloat^BigFloat</tt>
  int pow(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow(mp(), x.mp(), y.mp(), rnd); }
  /// power function for <tt>BigFloat^int</tt>
  int pow(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return pow(x, static_cast<long>(y), rnd); }
  /// power function for <tt>BigFloat^unsigned int</tt>
  int pow(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return pow(x, static_cast<unsigned long>(y), rnd); }
  /// power function for <tt>BigFloat^long</tt>
  int pow(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_si(mp(), x.mp(), y, rnd); }
  /// power function for <tt>BigFloat^unsigned long</tt>
  int pow(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_ui(mp(), x.mp(), y, rnd); }
  /// power function for <tt>BigFloat^BigInt</tt>
  int pow(const BigFloat& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_z(mp(), x.mp(), y.mp(), rnd); }
  /// power function for <tt>unsigned long^BigFloat</tt>
  int pow(unsigned long x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_pow(mp(), x, y.mp(), rnd); }
  /// power function for <tt>unsigned long^unsigned long</tt>
  int pow(unsigned long x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_pow_ui(mp(), x, y, rnd); }
  //@}

  /// \name other arithmetic functions (raw version)
  //@{
  /// square
  int sqr(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqr(mp(), x.mp(), rnd); }
  /// cubic root
  int cbrt(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { return mpfr_cbrt(mp(), x.mp(), rnd); }
  /// kth root
  int root(const BigFloat& x, unsigned long k, rnd_t rnd = MPFR_RND)
  { return mpfr_root(mp(), x.mp(), k, rnd); }
  /// negation
  int neg(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { return mpfr_neg(mp(), x.mp(), rnd); }
  /// absolute value
  int abs(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { return mpfr_abs(mp(), x.mp(), rnd); }
  //@}
  
  /// \name shift functions
  //@{
  /// left shift for <tt>int</tt>
  int mul_2exp(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return mul_2exp(x, static_cast<long>(y), rnd); }
  /// left shift for <tt>unsigned int</tt>
  int mul_2exp(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mul_2exp(x, static_cast<unsigned long>(y), rnd); }
  /// left shift for <tt>long</tt>
  int mul_2exp(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2si(mp(), x.mp(), y, rnd); }
  /// left shift for <tt>unsigned long</tt>
  int mul_2exp(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2ui(mp(), x.mp(), y, rnd); }
  /// right shift for <tt>int</tt>
  int div_2exp(const BigFloat& x, int y, rnd_t rnd = MPFR_RND)
  { return div_2exp(x, static_cast<long>(y), rnd); }
  /// right shift for <tt>unsigned int</tt>
  int div_2exp(const BigFloat& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return div_2exp(x, static_cast<unsigned long>(y), rnd); }
  /// right shift for <tt>long</tt>
  int div_2exp(const BigFloat& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2si(mp(), x.mp(), y, rnd); }
  /// right shift for <tt>unsigned long</tt>
  int div_2exp(const BigFloat& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2ui(mp(), x.mp(), y, rnd); }
  //@}

  /// \name comparison functions
  //@{
  /// compare with <tt>BigFloat</tt>
  int cmp(const BigFloat& x) const
  { return mpfr_cmp(mp(), x.mp()); }
  /// compare with <tt>int</tt>
  int cmp(int x) const
  { return cmp(static_cast<long>(x)); }
  /// compare with <tt>unsigned int</tt>
  int cmp(unsigned int x) const
  { return cmp(static_cast<unsigned long>(x)); }
  /// compare with <tt>long</tt>
  int cmp(long x) const
  { return mpfr_cmp_si(mp(), x); }
  /// compare with <tt>unsigned long</tt>
  int cmp(unsigned long x) const
  { return mpfr_cmp_ui(mp(), x); }
  /// compare with <tt>double</tt>
  int cmp(double x) const
  { return mpfr_cmp_d(mp(), x); }
  /// compare with <tt>BigInt</tt>
  int cmp(const BigInt& x) const
  { return mpfr_cmp_z(mp(), x.mp()); }
  /// compare with <tt>BigRat</tt>
  int cmp(const BigRat& x) const
  { return mpfr_cmp_q(mp(), x.mp()); }
  /// compare with \f$x*2^e\f$ for <tt>int</tt>
  int cmp_2exp(int x, exp_t e) const
  { return mpfr_cmp_si_2exp(mp(), x, e); }
  /// compare with \f$x*2^e\f$ for <tt>unsigned int</tt>
  int cmp_2exp(unsigned int x, exp_t e) const
  { return mpfr_cmp_ui_2exp(mp(), x, e); }
  /// compare with \f$x*2^e\f$ for <tt>long</tt>
  int cmp_2exp(long x, exp_t e) const
  { return mpfr_cmp_si_2exp(mp(), x, e); }
  /// compare with \f$x*2^e\f$ for <tt>unsigned long</tt>
  int cmp_2exp(unsigned long x, exp_t e) const
  { return mpfr_cmp_ui_2exp(mp(), x, e); }
  /// compare (in absolute value) with <tt>BigFloat</tt>
  int cmpabs(const BigFloat& x) const
  { return mpfr_cmpabs(mp(), x.mp()); }
  //@}

  /// \name conversion functions
  //@{
  /// return double value
  double get_d(rnd_t rnd = MPFR_RND) const
  { return mpfr_get_d(mp(), rnd); }
  /// find d and exp s.t. \f$d*2^{exp}\f$ with \f$0.5\le|d|<1\f$
  double get_d_2exp(long* exp, rnd_t rnd = MPFR_RND) const
  { return mpfr_get_d_2exp(exp, mp(), rnd); }
  /// return long value
  long get_si(rnd_t rnd = MPFR_RND) const
  { return mpfr_get_si(mp(), rnd); }
  /// return unsigned long value
  unsigned long get_ui(rnd_t rnd = MPFR_RND) const
  { return mpfr_get_ui(mp(), rnd); }
  /// return BigInt value
  BigInt get_z(rnd_t rnd = MPFR_RND) const
  { BigInt r; mpfr_get_z(r.mp(), mp(), rnd); return r; }
  /// return z and exp s.t. it equals \f$x*2^{exp}\f$
  exp_t get_z_exp(BigInt& z) const
  { return mpfr_get_z_exp(z.mp(), mp()); }
  /// return BigRat value
  BigRat get_q() const {
    BigInt x; exp_t e = get_z_exp(x);
    if (e >= 0) { // convert to integer
      x.mul_2exp(x, e); return BigRat(x);
    } else { // convert to rational
      BigRat q; q.div_2exp(x, -e); return q;
    }
  }
  /// return the string representation
  std::string get_str(size_t ndigits=0,int base=10,rnd_t rnd=MPFR_RND) const {
    if (is_integer()) {
      BigInt z = get_z();
      if (ndigits == 0 || z.sizeinbase(base) < ndigits) return z.get_str();
    } 
    return mpfr2str(mp(), ndigits, base, false, rnd);
  }
  /// return the string representation in fixed format
  std::string get_fixed_str(size_t ndigits = 0, int base = 10, rnd_t rnd = MPFR_RND) const
  { return mpfr2str(mp(), ndigits, base, true, rnd); }
  //@}
  
  /// \name helper functions
  //@{
  bool is_nan() const
  { return mpfr_nan_p(mp()) != 0; }
  bool is_inf() const
  { return mpfr_inf_p(mp()) != 0; }
  bool is_number() const
  { return mpfr_number_p(mp()) != 0; }
  bool is_zero() const
  { return mpfr_zero_p(mp()) != 0; }
  bool greater(const BigFloat& x) const
  { return mpfr_greater_p(mp(), x.mp()) != 0; }
  bool greaterequal(const BigFloat& x) const
  { return mpfr_greaterequal_p(mp(), x.mp()) != 0; }
  bool less(const BigFloat& x) const
  { return mpfr_less_p(mp(), x.mp()) != 0; }
  bool lessequal(const BigFloat& x) const
  { return mpfr_lessequal_p(mp(), x.mp()) != 0; }
  bool lessgreater(const BigFloat& x) const
  { return mpfr_lessgreater_p(mp(), x.mp()) != 0; }
  bool equal(const BigFloat& x) const
  { return mpfr_equal_p(mp(), x.mp()) != 0; }
  bool unordered(const BigFloat& x) const
  { return mpfr_unordered_p(mp(), x.mp()) != 0; }
  bool is_integer() const
  { return mpfr_integer_p(mp()) != 0; }
  bool is_ulong(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_ulong_p(mp(), rnd) != 0; }
  bool is_slong(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_slong_p(mp(), rnd) != 0; }
  bool is_uint(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_uint_p(mp(), rnd) != 0; }
  bool is_sint(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_sint_p(mp(), rnd) != 0; }
  bool is_ushort(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_ushort_p(mp(), rnd) != 0; }
  bool is_sshort(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_sshort_p(mp(), rnd) != 0; }
  //@}

  /// miscellaneous functions
  //@{
  /// set to +infty
  void set_pos_inf() 
  { mpfr_set_inf(mp(), 1); }
  /// set to -infty
  void set_neg_inf() 
  { mpfr_set_inf(mp(), -1); }
  /// set to NaN
  void set_nan() 
  { mpfr_set_nan(mp()); }
  /// swap Function
  void swap(BigFloat& other)
  { mpfr_swap(mp(), other.mp()); }
  /// return sign
  int sgn() const
  { return mpfr_sgn(mp()); }
  /// return upper bound of MSB
  long uMSB() const
  { BigInt x; exp_t e = get_z_exp(x); return x.ceillg() + e; }
  /// return lower bound of MSB
  long lMSB() const
  { BigInt x; exp_t e = get_z_exp(x); return x.floorlg() + e; }
  /// remove trailing zeros
  void remove_trailing_zeros()
  { mpfr_remove_trailing_zeros(mp()); }

  void rint(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint(mp(), x.mp(), rnd); }
  void ceil(const BigFloat& x)
  { mpfr_ceil(mp(), x.mp()); }
  void floor(const BigFloat& x)
  { mpfr_floor(mp(), x.mp()); }
  void round(const BigFloat& x)
  { mpfr_round(mp(), x.mp()); }
  void trunc(const BigFloat& x)
  { mpfr_trunc(mp(), x.mp()); }
  void rint_ceil(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_ceil(mp(), x.mp(), rnd); }
  void rint_floor(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_floor(mp(), x.mp(), rnd); }
  void rint_round(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_round(mp(), x.mp(), rnd); }
  void rint_trunc(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_trunc(mp(), x.mp(), rnd); }
  void frac(const BigFloat& x, rnd_t rnd = MPFR_RND)
  { mpfr_frac(mp(), x.mp(), rnd); }
  void nexttoward(const BigFloat& x) 
  { mpfr_nexttoward(mp(), x.mp()); }
  void nextabove() 
  { mpfr_nextabove(mp()); }
  void nextbelow() 
  { mpfr_nextbelow(mp()); }
  void min(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { mpfr_min(mp(), x.mp(), y.mp(), rnd); }
  void max(const BigFloat& x, const BigFloat& y, rnd_t rnd = MPFR_RND)
  { mpfr_max(mp(), x.mp(), y.mp(), rnd); }
  //@}

  // count the precision of a int
  static prec_t count_prec(int)
  { return INT_PREC; }
  // count the precision of a unsigned int
  static prec_t count_prec(unsigned int)
  { return INT_PREC; }
  // count the precision of a long
  static prec_t count_prec(long)
  { return INT_PREC; }
  // count the precision of a unsigned long
  static prec_t count_prec(unsigned long)
  { return INT_PREC; }
  // count the precision of a double
  static prec_t count_prec(double)
  { return DOUBLE_PREC; }
  // count the precision of a BigInt
  static prec_t count_prec(const BigInt& z)
  { return std::max(z.sizeinbase(2), INT_PREC); }
  // count the precision of a BigInt
  static prec_t count_prec(const BigRat&)
  { return DOUBLE_PREC; }
  static prec_t count_prec(const BigFloat& z)
  { return z.get_prec(); }
  // count the precision in a string representation
  //   prec <= len*log_2(base) <= len*(1+ilogb(base))
  static prec_t count_prec(const char* str, int base = 10)
  { return strlen(str)*(1+ilogb(base)); }	
  static prec_t count_prec(const std::string& str, int base = 10)
  { return str.length()*(1+ilogb(base)); }	

  // count how many precision needed for addition/subtraction
  static prec_t add_prec(const BigFloat& x, const BigFloat& y) {
    exp_t diff = x.get_exp() - y.get_exp();
    if (diff >= 0)
      return std::max(x.get_prec() + diff, y.get_prec());
    else
      return std::max(x.get_prec(), y.get_prec() - diff);
  }
  // count how many precision needed for muliplication
  static prec_t mul_prec(const BigFloat& x, const BigFloat& y) 
  { return x.get_prec() + y.get_prec(); }


public: // C++ operators
  /// \name unary, increment, decrement operators
  //@{
  /// unary plus operator
  BigFloat operator+() const
  { return BigFloat(*this); }
  /// unary negation operator
  BigFloat operator-() const
  { BigFloat r; r.neg(*this); return r; }
  /// prefix increment operator
  BigFloat& operator++()  
  { add(*this, 1); return *this; }
  /// postfix increment operator
  BigFloat operator++(int) 
  { BigFloat r(*this); ++(*this); return r; }
  /// prefix decrement operator
  BigFloat& operator--()
  { sub(*this, 1); return *this; }
  /// postfix decrement operator
  BigFloat operator--(int)
  { BigFloat r(*this); --(*this); return r; }
  //@}

  /// \name assignment and compound assignment operators
  //@{
  /// assignment operator for <tt>BigFloat</tt>
  BigFloat& operator=(const BigFloat& rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>int</tt>
  BigFloat& operator=(int rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>unsigned int</tt>
  BigFloat& operator=(unsigned int rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>long</tt>
  BigFloat& operator=(long rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>unsigned long</tt>
  BigFloat& operator=(unsigned long rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>double</tt>
  BigFloat& operator=(double rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>char*</tt>
  BigFloat& operator=(const char* rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>std::string</tt>
  BigFloat& operator=(const std::string& rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>BigInt</tt>
  BigFloat& operator=(const BigInt& rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>BigRat</tt>
  BigFloat& operator=(const BigRat& rhs)
  { set(rhs); return *this; }

  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(const BigFloat& rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(int rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(unsigned int rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(long rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(unsigned long rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(double rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(const BigInt& rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigFloat& operator+=(const BigRat& rhs)
  { add(*this, rhs); return *this; }

  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(const BigFloat& rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(int rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(unsigned int rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(long rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(unsigned long rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(double rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(const BigInt& rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigFloat& operator-=(const BigRat& rhs)
  { sub(*this, rhs); return *this; }

  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(const BigFloat& rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(int rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(unsigned int rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(long rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(unsigned long rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(double rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(const BigInt& rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigFloat& operator*=(const BigRat& rhs)
  { mul(*this, rhs); return *this; }

  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(const BigFloat& rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(int rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(unsigned int rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(long rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(unsigned long rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(double rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(const BigInt& rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigFloat& operator/=(const BigRat& rhs)
  { div(*this, rhs); return *this; }
  //@}

  /// compound assignment operator <tt><<=</tt>
  BigFloat& operator<<=(int i)
  { mul_2exp(*this, i); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigFloat& operator<<=(unsigned int ui)
  { mul_2exp(*this, ui); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigFloat& operator<<=(long l)
  { mul_2exp(*this, l); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigFloat& operator<<=(unsigned long ul)
  { mul_2exp(*this, ul); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigFloat& operator>>=(int i)
  { div_2exp(*this, i); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigFloat& operator>>=(unsigned int ui)
  { div_2exp(*this, ui); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigFloat& operator>>=(long l)
  { div_2exp(*this, l); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigFloat& operator>>=(unsigned long ul)
  { div_2exp(*this, ul); return *this; }
  //@}

public:
#ifndef CORE_DISABLE_OLDNAMES
  /// \name back-compatiable functions
  //@{ 
  /// Has Exact Division
  static bool hasExactDivision() { return false; }
  /// set value from <tt>const char*</tt>
  int set_str(const char* s, int base = 0) { return set(s, base); }
  /// intValue
  int intValue() const { return static_cast<int>(get_si()); }
  /// longValue
  long longValue() const { return get_si(); }
  /// ulongValue 
  unsigned long ulongValue() const { return get_ui(); }
  /// doubleValue
  double doubleValue() const { return get_d(); }
  /// BigIntValue
  BigInt BigIntValue() const { return get_z(); }
  /// BigRatValue
  BigRat BigRatValue() const { return get_q(); }
  //@}
#endif

};

/// \addtogroup BigFloatArithmeticOperators
//@{
/// BigFloat + BigFloat
inline BigFloat operator+(const BigFloat& x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + int
inline BigFloat operator+(const BigFloat& x, int y)
{ BigFloat r; r.add(x, y); return r; }
/// int + BigFloat
inline BigFloat operator+(int x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + unsigned int
inline BigFloat operator+(const BigFloat& x, unsigned int y)
{ BigFloat r; r.add(x, y); return r; }
/// unsigned int + BigFloat
inline BigFloat operator+(unsigned int x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + long
inline BigFloat operator+(const BigFloat& x, long y)
{ BigFloat r; r.add(x, y); return r; }
/// long + BigFloat
inline BigFloat operator+(long x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + unsigned long
inline BigFloat operator+(const BigFloat& x, unsigned long y)
{ BigFloat r; r.add(x, y); return r; }
/// unsigned long + BigFloat
inline BigFloat operator+(unsigned long x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + double
inline BigFloat operator+(const BigFloat& x, double y)
{ BigFloat r; r.add(x, y); return r; }
/// double + BigFloat
inline BigFloat operator+(double x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + BigInt
inline BigFloat operator+(const BigFloat& x, const BigInt& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigInt + BigFloat
inline BigFloat operator+(const BigInt& x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigFloat + BigRat
inline BigFloat operator+(const BigFloat& x, const BigRat& y)
{ BigFloat r; r.add(x, y); return r; }
/// BigRat + BigFloat
inline BigFloat operator+(const BigRat& x, const BigFloat& y)
{ BigFloat r; r.add(x, y); return r; }

/// BigFloat - BigFloat
inline BigFloat operator-(const BigFloat& x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - int
inline BigFloat operator-(const BigFloat& x, int y)
{ BigFloat r; r.sub(x, y); return r; }
/// int - BigFloat
inline BigFloat operator-(int x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - unsigned int
inline BigFloat operator-(const BigFloat& x, unsigned int y)
{ BigFloat r; r.sub(x, y); return r; }
/// unsigned int - BigFloat
inline BigFloat operator-(unsigned int x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - long
inline BigFloat operator-(const BigFloat& x, long y)
{ BigFloat r; r.sub(x, y); return r; }
/// long - BigFloat
inline BigFloat operator-(long x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - unsigned long
inline BigFloat operator-(const BigFloat& x, unsigned long y)
{ BigFloat r; r.sub(x, y); return r; }
/// unsigned long - BigFloat
inline BigFloat operator-(unsigned long x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - double
inline BigFloat operator-(const BigFloat& x, double y)
{ BigFloat r; r.sub(x, y); return r; }
/// double - BigFloat
inline BigFloat operator-(double x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - BigInt
inline BigFloat operator-(const BigFloat& x, const BigInt& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigInt - BigFloat
inline BigFloat operator-(const BigInt& x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigFloat - BigRat
inline BigFloat operator-(const BigFloat& x, const BigRat& y)
{ BigFloat r; r.sub(x, y); return r; }
/// BigRat - BigFloat
inline BigFloat operator-(const BigRat& x, const BigFloat& y)
{ BigFloat r; r.sub(x, y); return r; }

/// BigFloat * BigFloat
inline BigFloat operator*(const BigFloat& x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * int
inline BigFloat operator*(const BigFloat& x, int y)
{ BigFloat r; r.mul(x, y); return r; }
/// int * BigFloat
inline BigFloat operator*(int x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * unsigned int
inline BigFloat operator*(const BigFloat& x, unsigned int y)
{ BigFloat r; r.mul(x, y); return r; }
/// unsigned int * BigFloat
inline BigFloat operator*(unsigned int x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * long
inline BigFloat operator*(const BigFloat& x, long y)
{ BigFloat r; r.mul(x, y); return r; }
/// long * BigFloat
inline BigFloat operator*(long x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * unsigned long
inline BigFloat operator*(const BigFloat& x, unsigned long y)
{ BigFloat r; r.mul(x, y); return r; }
/// unsigned long * BigFloat
inline BigFloat operator*(unsigned long x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * double
inline BigFloat operator*(const BigFloat& x, double y)
{ BigFloat r; r.mul(x, y); return r; }
/// double * BigFloat
inline BigFloat operator*(double x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * BigInt
inline BigFloat operator*(const BigFloat& x, const BigInt& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigInt * BigFloat
inline BigFloat operator*(const BigInt& x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigFloat * BigRat
inline BigFloat operator*(const BigFloat& x, const BigRat& y)
{ BigFloat r; r.mul(x, y); return r; }
/// BigRat * BigFloat
inline BigFloat operator*(const BigRat& x, const BigFloat& y)
{ BigFloat r; r.mul(x, y); return r; }

/// BigFloat / BigFloat
inline BigFloat operator/(const BigFloat& x, const BigFloat& y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / int
inline BigFloat operator/(const BigFloat& x, int y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / unsigned int
inline BigFloat operator/(const BigFloat& x, unsigned int y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / long
inline BigFloat operator/(const BigFloat& x, long y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / unsigned long
inline BigFloat operator/(const BigFloat& x, unsigned long y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / double
inline BigFloat operator/(const BigFloat& x, double y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / BigInt
inline BigFloat operator/(const BigFloat& x, const BigInt& y)
{ BigFloat r; r.div(x, y); return r; }
/// BigFloat / BigRat
inline BigFloat operator/(const BigFloat& x, const BigRat& y)
{ BigFloat r; r.div(x, y); return r; }
//@}

/// BigFloat  << int
inline BigFloat  operator<<(const BigFloat & x, int y)
{ BigFloat  r; r.mul_2exp(x, y); return r; }
/// BigFloat  << unsigned int
inline BigFloat  operator<<(const BigFloat & x, unsigned int y)
{ BigFloat  r; r.mul_2exp(x, y); return r; }
/// BigFloat  << long
inline BigFloat  operator<<(const BigFloat & x, long y)
{ BigFloat  r; r.mul_2exp(x, y); return r; }
/// BigFloat  << unsigned long
inline BigFloat  operator<<(const BigFloat & x, unsigned long y)
{ BigFloat  r; r.mul_2exp(x, y); return r; }
/// BigFloat  >> int
inline BigFloat  operator>>(const BigFloat & x, int y)
{ BigFloat  r; r.div_2exp(x, y); return r; }
/// BigFloat  >> unsigned int
inline BigFloat  operator>>(const BigFloat & x, unsigned int y)
{ BigFloat  r; r.div_2exp(x, y); return r; }
/// BigFloat  >> long
inline BigFloat  operator>>(const BigFloat & x, long y)
{ BigFloat  r; r.div_2exp(x, y); return r; }
/// BigFloat  >> unsigned long
inline BigFloat  operator>>(const BigFloat & x, unsigned long y)
{ BigFloat  r; r.div_2exp(x, y); return r; }
//@}

/// \addtogroup BigFloat ComparisonOperators
//@{
/// BigFloat  == BigFloat 
inline bool operator==(const BigFloat & x, const BigFloat & y)
{ return x.cmp(y) == 0; }
/// BigFloat  == int
inline bool operator==(const BigFloat & x, int y)
{ return x.cmp(y) == 0; }
/// int == BigFloat 
inline bool operator==(int x, const BigFloat & y)
{ return y.cmp(x) == 0; }
/// BigFloat  == unsigned int
inline bool operator==(const BigFloat & x, unsigned int y)
{ return x.cmp(y) == 0; }
/// unsigned int == BigFloat 
inline bool operator==(unsigned int x, const BigFloat & y)
{ return y.cmp(x) == 0; }
/// BigFloat  == long
inline bool operator==(const BigFloat & x, long y)
{ return x.cmp(y) == 0; }
/// long == BigFloat 
inline bool operator==(long x, const BigFloat & y)
{ return y.cmp(x) == 0; }
/// BigFloat  == unsigned long
inline bool operator==(const BigFloat & x, unsigned long y)
{ return x.cmp(y) == 0; }
/// unsigned long == BigFloat 
inline bool operator==(unsigned long x, const BigFloat & y)
{ return y.cmp(x) == 0; }
/// BigFloat  == double
inline bool operator==(const BigFloat & x, double y)
{ return x.cmp(y) == 0; }
/// double == BigFloat 
inline bool operator==(double x, const BigFloat & y)
{ return y.cmp(x) == 0; }
/// BigFloat  == BigInt
inline bool operator==(const BigFloat & x, const BigInt& y)
{ return x.cmp(y) == 0; }
/// BigInt == BigFloat 
inline bool operator==(const BigInt& x, const BigFloat & y)
{ return y.cmp(x) == 0; }
/// BigFloat  == BigRat
inline bool operator==(const BigFloat & x, const BigRat& y)
{ return x.cmp(y) == 0; }
/// BigRat == BigFloat 
inline bool operator==(const BigRat& x, const BigFloat & y)
{ return y.cmp(x) == 0; }

/// BigFloat  != BigFloat 
inline bool operator!=(const BigFloat & x, const BigFloat & y)
{ return x.cmp(y) != 0; }
/// BigFloat  != int
inline bool operator!=(const BigFloat & x, int y)
{ return x.cmp(y) != 0; }
/// int != BigFloat 
inline bool operator!=(int x, const BigFloat & y)
{ return y.cmp(x) != 0; }
/// BigFloat  != unsigned int
inline bool operator!=(const BigFloat & x, unsigned int y)
{ return x.cmp(y) != 0; }
/// unsigned int != BigFloat 
inline bool operator!=(unsigned int x, const BigFloat & y)
{ return y.cmp(x) != 0; }
/// BigFloat  != long
inline bool operator!=(const BigFloat & x, long y)
{ return x.cmp(y) != 0; }
/// long != BigFloat 
inline bool operator!=(long x, const BigFloat & y)
{ return y.cmp(x) != 0; }
/// BigFloat  != unsigned long
inline bool operator!=(const BigFloat & x, unsigned long y)
{ return x.cmp(y) != 0; }
/// unsigned long != BigFloat 
inline bool operator!=(unsigned long x, const BigFloat & y)
{ return y.cmp(x) != 0; }
/// BigFloat  != double
inline bool operator!=(const BigFloat & x, double y)
{ return x.cmp(y) != 0; }
/// double != BigFloat 
inline bool operator!=(double x, const BigFloat & y)
{ return y.cmp(x) != 0; }
/// BigFloat  != BigInt
inline bool operator!=(const BigFloat & x, const BigInt& y)
{ return x.cmp(y) != 0; }
/// BigInt != BigFloat 
inline bool operator!=(const BigInt& x, const BigFloat & y)
{ return y.cmp(x) != 0; }
/// BigFloat  != BigRat
inline bool operator!=(const BigFloat & x, const BigRat& y)
{ return x.cmp(y) != 0; }
/// BigRat != BigFloat 
inline bool operator!=(const BigRat& x, const BigFloat & y)
{ return y.cmp(x) != 0; }

/// BigFloat  >= BigFloat 
inline bool operator>=(const BigFloat & x, const BigFloat & y)
{ return x.cmp(y) >= 0; }
/// BigFloat  >= int
inline bool operator>=(const BigFloat & x, int y)
{ return x.cmp(y) >= 0; }
/// int >= BigFloat 
inline bool operator>=(int x, const BigFloat & y)
{ return y.cmp(x) <= 0; }
/// BigFloat  >= unsigned int
inline bool operator>=(const BigFloat & x, unsigned int y)
{ return x.cmp(y) >= 0; }
/// unsigned int >= BigFloat 
inline bool operator>=(unsigned int x, const BigFloat & y)
{ return y.cmp(x) <= 0; }
/// BigFloat  >= long
inline bool operator>=(const BigFloat & x, long y)
{ return x.cmp(y) >= 0; }
/// long >= BigFloat 
inline bool operator>=(long x, const BigFloat & y)
{ return y.cmp(x) <= 0; }
/// BigFloat  >= unsigned long
inline bool operator>=(const BigFloat & x, unsigned long y)
{ return x.cmp(y) >= 0; }
/// unsigned long >= BigFloat 
inline bool operator>=(unsigned long x, const BigFloat & y)
{ return y.cmp(x) <= 0; }
/// BigFloat  >= double
inline bool operator>=(const BigFloat & x, double y)
{ return x.cmp(y) >= 0; }
/// double >= BigFloat 
inline bool operator>=(double x, const BigFloat & y)
{ return y.cmp(x) <= 0; }
/// BigFloat  >= BigInt
inline bool operator>=(const BigFloat & x, const BigInt& y)
{ return x.cmp(y) >= 0; }
/// BigInt >= BigFloat 
inline bool operator>=(const BigInt& x, const BigFloat & y)
{ return y.cmp(x) <= 0; }
/// BigFloat  >= BigRat
inline bool operator>=(const BigFloat & x, const BigRat& y)
{ return x.cmp(y) >= 0; }
/// BigRat >= BigFloat 
inline bool operator>=(const BigRat& x, const BigFloat & y)
{ return y.cmp(x) <= 0; }

/// BigFloat  <= BigFloat 
inline bool operator<=(const BigFloat & x, const BigFloat & y)
{ return x.cmp(y) <= 0; }
/// BigFloat  <= int
inline bool operator<=(const BigFloat & x, int y)
{ return x.cmp(y) <= 0; }
/// int <= BigFloat 
inline bool operator<=(int x, const BigFloat & y)
{ return y.cmp(x) >= 0; }
/// BigFloat  <= unsigned int
inline bool operator<=(const BigFloat & x, unsigned int y)
{ return x.cmp(y) <= 0; }
/// unsigned int <= BigFloat 
inline bool operator<=(unsigned int x, const BigFloat & y)
{ return y.cmp(x) >= 0; }
/// BigFloat  <= long
inline bool operator<=(const BigFloat & x, long y)
{ return x.cmp(y) <= 0; }
/// long <= BigFloat 
inline bool operator<=(long x, const BigFloat & y)
{ return y.cmp(x) >= 0; }
/// BigFloat  <= unsigned long
inline bool operator<=(const BigFloat & x, unsigned long y)
{ return x.cmp(y) <= 0; }
/// unsigned long <= BigFloat 
inline bool operator<=(unsigned long x, const BigFloat & y)
{ return y.cmp(x) >= 0; }
/// BigFloat  <= double
inline bool operator<=(const BigFloat & x, double y)
{ return x.cmp(y) <= 0; }
/// double <= BigFloat 
inline bool operator<=(double x, const BigFloat & y)
{ return y.cmp(x) >= 0; }
/// BigFloat  <= BigInt
inline bool operator<=(const BigFloat & x, const BigInt& y)
{ return x.cmp(y) <= 0; }
/// BigInt <= BigFloat 
inline bool operator<=(const BigInt& x, const BigFloat & y)
{ return y.cmp(x) >= 0; }
/// BigFloat  <= BigRat
inline bool operator<=(const BigFloat & x, const BigRat& y)
{ return x.cmp(y) <= 0; }
/// BigRat <= BigFloat 
inline bool operator<=(const BigRat& x, const BigFloat & y)
{ return y.cmp(x) >= 0; }

/// BigFloat  > BigFloat 
inline bool operator>(const BigFloat & x, const BigFloat & y)
{ return x.cmp(y) > 0; }
/// BigFloat  > int
inline bool operator>(const BigFloat & x, int y)
{ return x.cmp(y) > 0; }
/// int > BigFloat 
inline bool operator>(int x, const BigFloat & y)
{ return y.cmp(x) < 0; }
/// BigFloat  > unsigned int
inline bool operator>(const BigFloat & x, unsigned int y)
{ return x.cmp(y) > 0; }
/// unsigned int > BigFloat 
inline bool operator>(unsigned int x, const BigFloat & y)
{ return y.cmp(x) < 0; }
/// BigFloat  > long
inline bool operator>(const BigFloat & x, long y)
{ return x.cmp(y) > 0; }
/// long > BigFloat 
inline bool operator>(long x, const BigFloat & y)
{ return y.cmp(x) < 0; }
/// BigFloat  > unsigned long
inline bool operator>(const BigFloat & x, unsigned long y)
{ return x.cmp(y) > 0; }
/// unsigned long > BigFloat 
inline bool operator>(unsigned long x, const BigFloat & y)
{ return y.cmp(x) < 0; }
/// BigFloat  > double
inline bool operator>(const BigFloat & x, double y)
{ return x.cmp(y) > 0; }
/// double > BigFloat 
inline bool operator>(double x, const BigFloat & y)
{ return y.cmp(x) < 0; }
/// BigFloat  > BigInt
inline bool operator>(const BigFloat & x, const BigInt& y)
{ return x.cmp(y) > 0; }
/// BigInt > BigFloat 
inline bool operator>(const BigInt& x, const BigFloat & y)
{ return y.cmp(x) < 0; }
/// BigFloat  > BigRat
inline bool operator>(const BigFloat & x, const BigRat& y)
{ return x.cmp(y) > 0; }
/// BigRat > BigFloat 
inline bool operator>(const BigRat& x, const BigFloat & y)
{ return y.cmp(x) < 0; }

/// BigFloat  < BigFloat 
inline bool operator<(const BigFloat & x, const BigFloat & y)
{ return x.cmp(y) < 0; }
/// BigFloat  < int
inline bool operator<(const BigFloat & x, int y)
{ return x.cmp(y) < 0; }
/// int < BigFloat 
inline bool operator<(int x, const BigFloat & y)
{ return y.cmp(x) > 0; }
/// BigFloat  < unsigned int
inline bool operator<(const BigFloat & x, unsigned int y)
{ return x.cmp(y) < 0; }
/// unsigned int < BigFloat 
inline bool operator<(unsigned int x, const BigFloat & y)
{ return y.cmp(x) > 0; }
/// BigFloat  < long
inline bool operator<(const BigFloat & x, long y)
{ return x.cmp(y) < 0; }
/// long < BigFloat 
inline bool operator<(long x, const BigFloat & y)
{ return y.cmp(x) > 0; }
/// BigFloat  < unsigned long
inline bool operator<(const BigFloat & x, unsigned long y)
{ return x.cmp(y) < 0; }
/// unsigned long < BigFloat 
inline bool operator<(unsigned long x, const BigFloat & y)
{ return y.cmp(x) > 0; }
/// BigFloat  < double
inline bool operator<(const BigFloat & x, double y)
{ return x.cmp(y) < 0; }
/// double < BigFloat 
inline bool operator<(double x, const BigFloat & y)
{ return y.cmp(x) > 0; }
/// BigFloat  < BigInt
inline bool operator<(const BigFloat & x, const BigInt& y)
{ return x.cmp(y) < 0; }
/// BigInt < BigFloat 
inline bool operator<(const BigInt& x, const BigFloat & y)
{ return y.cmp(x) > 0; }
/// BigFloat  < BigRat
inline bool operator<(const BigFloat & x, const BigRat& y)
{ return x.cmp(y) < 0; }
/// BigRat < BigFloat 
inline bool operator<(const BigRat& x, const BigFloat & y)
{ return y.cmp(x) > 0; }
//@}

/// \addtogroup BigFloatIostreamOperators
//@{
/// istream operator for <tt>BigFloat</tt>
inline std::istream& operator>>(std::istream& is, BigFloat& x)
{ return is >> x.mp(); }
/// ostream operator for <tt>BigFloat</tt>
inline std::ostream& operator<<(std::ostream& os, const BigFloat& x)
{ return os << x.get_str(); }
//@}

/// \addtogroup BigFloatGlobalFunctions
//@{
/// square root
inline BigFloat sqrt(const BigFloat& x, prec_t prec = MPFR_DEF_SQRT_PREC)
{ BigFloat r(0, prec); r.sqrt(x); return r; }
/// cubic root
inline BigFloat cbrt(const BigFloat& x, prec_t prec = MPFR_DEF_CBRT_PREC)
{ BigFloat r(0, prec); r.cbrt(x); return r; }
/// k-th root
inline BigFloat root(const BigFloat& x, unsigned long k, prec_t prec = MPFR_DEF_ROOT_PREC)
{ BigFloat r(0, prec); r.root(x, k); return r; }
//@}

#ifndef CORE_DISABLE_OLDNAMES 
/// \addtogroup BigFloatBackCompatiableFunctions
//@{
/// comparison
inline int cmp(const BigFloat& x, const BigFloat& y) { return x.cmp(y); }
/// sign 
inline int sign(const BigFloat& a) { return a.sgn(); }
/// abs
inline BigFloat abs(const BigFloat& a) { BigFloat r; r.abs(a); return r; }
/// neg
inline BigFloat neg(const BigFloat& a) { BigFloat r; r.neg(a); return r; }
/// pow 
inline BigFloat pow(const BigFloat& a, unsigned long p) 
{ BigFloat r; r.pow(a, p); return r; }
//@}
#endif


CORE_END_NAMESPACE

#endif /*__BIGFLOAT_H__*/
