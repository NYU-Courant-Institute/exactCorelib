/****************************************************************************
 * Mpfr.h -- A C++ wrapper class for MPFR mpfr
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
 * $Id: Mpfr.h,v 1.1.1.1 2006-02-09 09:18:05 exact Exp $
 ***************************************************************************/
#ifndef __MPFR_H__
#define __MPFR_H__

#include <CORE/BigRat.h>
#include <mpfr.h>
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
// typedefs
typedef mp_rnd_t rnd_t;
typedef mp_exp_t exp_t;
typedef mp_prec_t prec_t;

/* addition */
inline int mpfr_si_add(mpfr_ptr z, long x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_add_si(z, y, x, rnd); } 
inline int mpfr_ui_add(mpfr_ptr z, unsigned long x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_add_ui(z, y, x, rnd); } 
inline int mpfr_z_add(mpfr_ptr z, mpz_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_add_z(z, y, x, rnd); } 
inline int mpfr_q_add(mpfr_ptr z, mpq_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_add_q(z, y, x, rnd); } 
    
/* subtraction */
inline int mpfr_z_sub(mpfr_ptr z, mpz_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ int r = mpfr_sub_z(z, y, x, rnd); mpfr_neg(z, z, rnd); return -r; }
inline int mpfr_q_sub(mpfr_ptr z, mpq_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ int r = mpfr_sub_q(z, y, x, rnd); mpfr_neg(z, z, rnd); return -r; }

/* multiplication */
inline int mpfr_si_mul(mpfr_ptr z, long x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_mul_si(z, y, x, rnd); } 
inline int mpfr_ui_mul(mpfr_ptr z, unsigned long x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_mul_ui(z, y, x, rnd); } 
inline int mpfr_z_mul(mpfr_ptr z, mpz_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_mul_z(z, y, x, rnd); }
inline int mpfr_q_mul(mpfr_ptr z, mpq_srcptr x, mpfr_srcptr y, rnd_t rnd)
{ return mpfr_mul_q(z, y, x, rnd); }

/* sqrt function */
inline int mpfr_sqrt_si(mpfr_ptr z, long x, rnd_t rnd)
{ assert((x) >= 0); return mpfr_sqrt_ui(z, x, rnd); }

// default rouning mode
#ifndef MPFR_RND
#define MPFR_RND mpfr_get_default_rounding_mode()
#endif

/* remove trailing zeros (by limbs) */
void mpfr_remove_trailing_zeroes(mpfr_t x);
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

/// \class Mpfr Mpfr.h
/// \brief Mpfr is a wrapper class of <tt>mpfr</tt> in MPFR
class Mpfr {
private:
  mpfr_t m_mp;
public:
  //internal structure accessors
  const mpfr_t& mp() const { return m_mp; }
  mpfr_t& mp() { return m_mp; }

public: // public typedefs
  typedef BigInt ZT;
  typedef BigRat QT;

public:

  /// \name constructors (auto version)
  //@{
  /// default constructor
  Mpfr() 
  { mpfr_init(m_mp); }
  /// copy constructor
  Mpfr(const Mpfr& rhs, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, rhs.get_prec()); mpfr_set(m_mp, rhs.m_mp, rnd); }
  /// constructor for <tt>char</tt> (use INT_PREC by default)
  Mpfr(char i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned char</tt> (use INT_PREC by default)
  Mpfr(unsigned char i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>short</tt> (use INT_PREC by default)
  Mpfr(short i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned short</tt> (use INT_PREC by default)
  Mpfr(unsigned short i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>int</tt> (use INT_PREC by default)
  Mpfr(int i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned int</tt> (use INT_PREC by default)
  Mpfr(unsigned int i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>long</tt> (use INT_PREC by default)
  Mpfr(long i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned long</tt> (use INT_PREC by default)
  Mpfr(unsigned long i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, INT_PREC); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>float</tt> (use SINGLE_PREC by default)
  Mpfr(float i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, SINGLE_PREC); mpfr_set_d(m_mp, i, rnd); }
  /// constructor for <tt>double</tt> (use DOUBLE_PREC by default)
  Mpfr(double i, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, DOUBLE_PREC); mpfr_set_d(m_mp, i, rnd); }
  /// constructor for <tt>BigInt</tt> 
  Mpfr(const BigInt& x, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, count_prec(x)); mpfr_set_z(m_mp, x.mp(), rnd); }
  /// constructor for <tt>BigRat</tt> (use DOUBLE_PREC by default)
  Mpfr(const BigRat& x, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, DOUBLE_PREC); mpfr_init_set_q(m_mp, x.mp(), rnd); }
  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit Mpfr(const char* s, int base = 10, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, count_prec(s, base)); mpfr_set_str(m_mp, s, base, rnd); }
  /// constructor for <tt>char</tt> (no implicit conversion)
  explicit Mpfr(const std::string& s, int b = 10, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, count_prec(s, b)); mpfr_set_str(m_mp, s.c_str(), b, rnd); }
  /// destructor
  ~Mpfr()
  { mpfr_clear(m_mp); }
  //@}

  /// \name constructors (fixed version)
  //@{
  /// constructor for <tt>Mpfr</tt> with specified precision
  Mpfr(const Mpfr& rhs, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set(m_mp, rhs.m_mp, rnd); }
  /// constructor for <tt>char</tt> with specified precision
  Mpfr(char i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned char</tt> with specified precision
  Mpfr(unsigned char i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>short</tt> with specified precision
  Mpfr(short i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned short</tt> with specified precision
  Mpfr(unsigned short i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>short</tt> with specified precision
  Mpfr(int i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned short</tt> with specified precision
  Mpfr(unsigned int i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>long</tt> with specified precision
  Mpfr(long i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned long</tt> with specified precision
  Mpfr(unsigned long i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>float</tt> with specified precision
  Mpfr(float i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_d(m_mp, i, rnd); }
  /// constructor for <tt>double</tt> with specified precision
  Mpfr(double i, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_d(m_mp, i, rnd); }
  /// constructor for <tt>BigInt</tt> with specified precision
  Mpfr(const BigInt& x, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_z(m_mp, x.mp(), rnd); }
  /// constructor for <tt>BigRat</tt> with specified precision
  Mpfr(const BigRat& x, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_q(m_mp, x.mp(), rnd); }
  /// constructor for <tt>char*</tt> with specified precision
  explicit Mpfr(const char* s, int base, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_str(m_mp, s, base, rnd); }
  /// constructor for <tt>std::string</tt> with specified precision
  explicit Mpfr(const std::string& s,int base,prec_t prec,rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_str(m_mp, s.c_str(), base, rnd); }

  /// constructor with value \f$i*2^e\f$ for <tt>char</tt>
  Mpfr(char i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned char</tt>
  Mpfr(unsigned char i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>short</tt>
  Mpfr(short i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned short</tt>
  Mpfr(unsigned short i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>int</tt>
  Mpfr(int i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned int</tt>
  Mpfr(unsigned int i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>long</tt>
  Mpfr(long i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned long</tt>
  Mpfr(unsigned long i, exp_t e, prec_t prec, rnd_t rnd = MPFR_RND)
  { mpfr_init2(m_mp, prec); mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  //@}

public:
  /// \name precision accessors
  //@{
  /// return current precision
  prec_t get_prec() const
  { return mpfr_get_prec(m_mp); }
  /// set current precision
  void set_prec(prec_t prec)
  { mpfr_set_prec(m_mp, prec); }
  //@}

  /// \name exponent accessors
  //@{
  /// return exponent
  exp_t get_exp() const
  { return sgn() ? mpfr_get_exp(m_mp) : 0; }
  /// set exponent (never need?)
  void set_exp(exp_t e)
  { mpfr_set_exp(m_mp, e); }
  //@}

public:
  /// \name assignment functions (raw version)
  //@{
  /// assignment functions for <tt>Mpfr</tt>
  int set(const Mpfr& rhs, rnd_t rnd = MPFR_RND)
  { return mpfr_set(m_mp, rhs.m_mp, rnd); }
  /// assignment functions for <tt>char</tt>
  int set(char i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si(m_mp, i, rnd); }
  /// assignment functions for <tt>unsigned char</tt>
  int set(unsigned char i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui(m_mp, i, rnd); }
  /// assignment functions for <tt>short</tt>
  int set(short i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si(m_mp, i, rnd); }
  /// assignment functions for <tt>unsigned short</tt>
  int set(unsigned short i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui(m_mp, i, rnd); }
  /// assignment functions for <tt>int</tt>
  int set(int i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si(m_mp, i, rnd); }
  /// assignment functions for <tt>unsigned int</tt>
  int set(unsigned int i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui(m_mp, i, rnd); }
  /// assignment functions for <tt>long</tt>
  int set(long i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si(m_mp, i, rnd); }
  /// assignment functions for <tt>unsigned long</tt>
  int set(unsigned long i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui(m_mp, i, rnd); }
  /// assignment functions for <tt>float</tt>
  int set(float i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_d(m_mp, i, rnd); }
  /// assignment functions for <tt>double</tt>
  int set(double i, rnd_t rnd = MPFR_RND)
  { return mpfr_set_d(m_mp, i, rnd); }
  /// assignment functions for <tt>BigInt</tt>
  int set(const BigInt& x, rnd_t rnd = MPFR_RND)
  { return mpfr_set_z(m_mp, x.mp(), rnd); }
  /// assignment functions for <tt>BigRat</tt>
  int set(const BigRat& x, rnd_t rnd = MPFR_RND) // use DOUBLE_PREC
  { return mpfr_set_q(m_mp, x.mp(), rnd); }
  /// assignment functions for <tt>char*</tt>
  int set(const char* str, int base = 10, rnd_t rnd = MPFR_RND)
  { return mpfr_set_str(m_mp, str, base, rnd); }
  /// assignment functions for <tt>std::string</tt>
  int set(const std::string& str, int base = 10, rnd_t rnd = MPFR_RND)
  { return mpfr_set_str(m_mp, str.c_str(), base, rnd); }

  /// set value to be \f$i*2^e\f$ for <tt>char</tt>
  int set_2exp(char i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned char</tt>
  int set_2exp(unsigned char i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>short</tt>
  int set_2exp(short i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned short</tt>
  int set_2exp(unsigned short i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>int</tt>
  int set_2exp(int i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned int</tt>
  int set_2exp(unsigned int i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>long</tt>
  int set_2exp(long i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// set value to be \f$i*2^e\f$ for <tt>unsigned long</tt>
  int set_2exp(unsigned long i, exp_t e, rnd_t rnd = MPFR_RND)
  { return mpfr_set_ui_2exp(m_mp, i, e, rnd); }
  //@}

public:
  /// \name arithmetic functions -- addition/subtraction (raw version)
  //@{
  /// addition/subtraction for <tt>Mpfr</tt>
  int addsub(const Mpfr& x, const Mpfr& y, bool isadd, rnd_t rnd = MPFR_RND)
  { return isadd ? add(x, y, rnd) : sub(x, y, rnd); }
  /// addition/subtraction for <tt>Mpfr, T</tt>
  template <typename T>
  int addsub(const Mpfr& x, const T& y, bool isadd, rnd_t rnd = MPFR_RND)
  { return isadd ? add(x, y, rnd) : sub(x, y, rnd); }
  /// addition/subtraction for <tt>T, Mpfr</tt>
  template <typename T>
  int addsub(const T& x, const Mpfr& y, bool isadd, rnd_t rnd = MPFR_RND)
  { return isadd ? add(x, y, rnd) : sub(x, y, rnd); }
  //@}

  /// \name arithmetic functions -- addition (raw version)
  //@{
  /// addition for <tt>Mpfr+Mpfr</tt>
  int add(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_add(m_mp, x.m_mp, y.m_mp, rnd); }
  /// addition for <tt>Mpfr+char</tt>
  int add(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_si(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+unsigned char</tt>
  int add(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_ui(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+short</tt>
  int add(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_si(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+unsigned short</tt>
  int add(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_ui(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+int</tt>
  int add(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_si(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+unsigned int</tt>
  int add(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_ui(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+long</tt>
  int add(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_si(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+unsigned long</tt>
  int add(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_ui(m_mp, x.m_mp, y, rnd); }
  /// addition for <tt>Mpfr+float</tt>
  int add(const Mpfr& x, float y, rnd_t rnd = MPFR_RND)
  { return add(x, Mpfr(y), rnd); }
  /// addition for <tt>Mpfr+double</tt>
  int add(const Mpfr& x, double y, rnd_t rnd = MPFR_RND)
  { return add(x, Mpfr(y), rnd); }
  /// addition for <tt>Mpfr+BigInt</tt>
  int add(const Mpfr& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_z(m_mp, x.m_mp, y.mp(), rnd); }
  /// addition for <tt>Mpfr+BigRat</tt>
  int add(const Mpfr& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_add_q(m_mp, x.m_mp, y.mp(), rnd); }
  /// addition for <tt>T+Mpfr</tt>
  template<typename T> int add(const T& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return add(y, x, rnd); }
  //@}

  /// \name arithmetic functions -- subtraction (raw version)
  //@{
  /// subtraction for <tt>Mpfr-Mpfr</tt>
  int sub(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub(m_mp, x.m_mp, y.m_mp, rnd); }
  /// subtraction for <tt>Mpfr-char</tt>
  int sub(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_si(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-unsigned char</tt>
  int sub(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_ui(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-short</tt>
  int sub(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_si(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-unsigned short</tt>
  int sub(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_ui(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-int</tt>
  int sub(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_si(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-unsigned int</tt>
  int sub(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_ui(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-long</tt>
  int sub(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_si(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-unsigned long</tt>
  int sub(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_ui(m_mp, x.m_mp, y, rnd); }
  /// subtraction for <tt>Mpfr-float</tt>
  int sub(const Mpfr& x, float y, rnd_t rnd = MPFR_RND)
  { return sub(x, Mpfr(y), rnd); }
  /// subtraction for <tt>Mpfr-double</tt>
  int sub(const Mpfr& x, double y, rnd_t rnd = MPFR_RND)
  { return sub(x, Mpfr(y), rnd); }
  /// subtraction for <tt>Mpfr-BigInt</tt>
  int sub(const Mpfr& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_z(m_mp, x.m_mp, y.mp(), rnd); }
  /// subtraction for <tt>Mpfr-BigRat</tt>
  int sub(const Mpfr& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_sub_q(m_mp, x.m_mp, y.mp(), rnd); }
  /// subtraction for <tt>char-Mpfr</tt>
  int sub(char x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>unsigned char-Mpfr</tt>
  int sub(unsigned char x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>short-Mpfr</tt>
  int sub(short x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>unsigned short-Mpfr</tt>
  int sub(unsigned short x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>int-Mpfr</tt>
  int sub(int x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>unsigned int-Mpfr</tt>
  int sub(unsigned int x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>long-Mpfr</tt>
  int sub(long x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>unsigned long-Mpfr</tt>
  int sub(unsigned long x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_sub(m_mp, x, y.m_mp, rnd); }
  /// subtraction for <tt>float-Mpfr</tt>
  int sub(float x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return sub(Mpfr(x), y, rnd); }
  /// subtraction for <tt>double-Mpfr</tt>
  int sub(double x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return sub(Mpfr(x), y, rnd); }
  /// subtraction for <tt>BigInt-Mpfr</tt>
  int sub(const BigInt& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_z_sub(m_mp, x.mp(), y.m_mp, rnd); }
  /// subtraction for <tt>BigRat-Mpfr</tt>
  int sub(const BigRat& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_q_sub(m_mp, x.mp(), y.m_mp, rnd); }
  //@}
  
  /// \name arithmetic functions -- multiplication (raw version)
  //@{
  /// multiplication for <tt>Mpfr*Mpfr</tt>
  int mul(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul(m_mp, x.m_mp, y.m_mp, rnd); }
  /// multiplication for <tt>Mpfr*char</tt>
  int mul(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_si(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*unsigned char</tt>
  int mul(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_ui(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*short</tt>
  int mul(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_si(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*unsigned short</tt>
  int mul(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_ui(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*int</tt>
  int mul(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_si(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*unsigned int</tt>
  int mul(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_ui(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*long</tt>
  int mul(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_si(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*unsigned long</tt>
  int mul(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_ui(m_mp, x.m_mp, y, rnd); }
  /// multiplication for <tt>Mpfr*float</tt>
  int mul(const Mpfr& x, float y, rnd_t rnd = MPFR_RND)
  { return mul(x, Mpfr(y), rnd); }
  /// multiplication for <tt>Mpfr*double</tt>
  int mul(const Mpfr& x, double y, rnd_t rnd = MPFR_RND)
  { return mul(x, Mpfr(y), rnd); }
  /// multiplication for <tt>Mpfr*BigInt</tt>
  int mul(const Mpfr& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_z(m_mp, x.m_mp, y.mp(), rnd); }
  /// multiplication for <tt>Mpfr*BigRat</tt>
  int mul(const Mpfr& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_q(m_mp, x.m_mp, y.mp(), rnd); }
  /// multiplication for <tt>T*Mpfr</tt>
  template <typename T> int mul(const T& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mul(y, x, rnd); }
  //@}

  /// \name arithmetic functions -- division (raw version)
  //@{
  /// division for <tt>Mpfr/Mpfr</tt>
  int div(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_div(m_mp, x.m_mp, y.m_mp, rnd); }
  /// division for <tt>Mpfr/char</tt>
  int div(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_si(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/unsigned char</tt>
  int div(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_ui(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/short</tt>
  int div(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_si(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/unsigned short</tt>
  int div(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_ui(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/int</tt>
  int div(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_si(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/unsigned int</tt>
  int div(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_ui(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/long</tt>
  int div(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_si(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/unsigned long</tt>
  int div(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_ui(m_mp, x.m_mp, y, rnd); }
  /// division for <tt>Mpfr/float</tt>
  int div(const Mpfr& x, float y, rnd_t rnd = MPFR_RND)
  { return div(x, Mpfr(y), rnd); }
  /// division for <tt>Mpfr/double</tt>
  int div(const Mpfr& x, double y, rnd_t rnd = MPFR_RND)
  { return div(x, Mpfr(y), rnd); }
  /// division for <tt>Mpfr/BigInt</tt>
  int div(const Mpfr& x, const BigInt& y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_z(m_mp, x.m_mp, y.mp(), rnd); }
  /// division for <tt>Mpfr/BigRat</tt>
  int div(const Mpfr& x, const BigRat& y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_q(m_mp, x.m_mp, y.mp(), rnd); }
  /// division for <tt>char/Mpfr</tt>
  int div(char x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>unsigned char/Mpfr</tt>
  int div(unsigned char x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>short/Mpfr</tt>
  int div(short x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>unsigned short/Mpfr</tt>
  int div(unsigned short x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>int/Mpfr</tt>
  int div(int x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>unsigned int/Mpfr</tt>
  int div(unsigned int x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>long/Mpfr</tt>
  int div(long x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_si_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>unsigned long/Mpfr</tt>
  int div(unsigned long x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_ui_div(m_mp, x, y.m_mp, rnd); }
  /// division for <tt>float/Mpfr</tt>
  int div(float x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return div(Mpfr(x), y, rnd); }
  /// division for <tt>double/Mpfr</tt>
  int div(double x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return div(Mpfr(x), y, rnd); }
  /// division for <tt>BigInt/Mpfr</tt>
  int div(const BigInt& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return div(Mpfr(x), y, rnd); }
  /// division for <tt>BigRat/Mpfr</tt>
  int div(const BigRat& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return div(Mpfr(x), y, rnd); }
  //@}

  /// \name square root functions (raw version)
  //@{
  /// square root function for <tt>Mpfr</tt>
  int sqrt(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt(m_mp, x.m_mp, rnd); }
  /// square root function for <tt>char</tt>
  int sqrt(char x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_si(m_mp, x, rnd); }
  /// square root function for <tt>unsigned char</tt>
  int sqrt(unsigned char x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_ui(m_mp, x, rnd); }
  /// square root function for <tt>short</tt>
  int sqrt(short x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_si(m_mp, x, rnd); }
  /// square root function for <tt>unsigned short</tt>
  int sqrt(unsigned short x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_ui(m_mp, x, rnd); }
  /// square root function for <tt>int</tt>
  int sqrt(int x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_si(m_mp, x, rnd); }
  /// square root function for <tt>unsigned int</tt>
  int sqrt(unsigned int x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_ui(m_mp, x, rnd); }
  /// square root function for <tt>long</tt>
  int sqrt(long x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_si(m_mp, x, rnd); }
  /// square root function for <tt>unsigned long</tt>
  int sqrt(unsigned long x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqrt_ui(m_mp, x, rnd); }
  /// square root function for <tt>float</tt>
  int sqrt(float x, rnd_t rnd = MPFR_RND)
  { return sqrt(Mpfr(x), rnd); }
  /// square root function for <tt>double</tt>
  int sqrt(double x, rnd_t rnd = MPFR_RND)
  { return sqrt(Mpfr(x), rnd); }
  //@}

  /// \name power functions (raw version)
  //@{
  /// power function for <tt>Mpfr</tt>
  int pow(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow(m_mp, x.m_mp, y.m_mp, rnd); }
  /// power function for <tt>char</tt>
  int pow(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_si(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>unsigned char</tt>
  int pow(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_ui(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>short</tt>
  int pow(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_si(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>unsigned short</tt>
  int pow(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_ui(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>int</tt>
  int pow(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_si(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>unsigned int</tt>
  int pow(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_ui(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>long</tt>
  int pow(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_si(m_mp, x.m_mp, y, rnd); }
  /// power function for <tt>unsigned long</tt>
  int pow(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_pow_ui(m_mp, x.m_mp, y, rnd); }
  //@}

  /// \name other arithmetic functions (raw version)
  //@{
  /// square
  int sqr(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { return mpfr_sqr(m_mp, x.m_mp, rnd); }
  /// cubic root
  int cbrt(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { return mpfr_cbrt(m_mp, x.m_mp, rnd); }
  /// kth root
  int root(const Mpfr& x, unsigned long k, rnd_t rnd = MPFR_RND)
  { return mpfr_root(m_mp, x.m_mp, k, rnd); }
  /// negation
  int neg(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { return mpfr_neg(m_mp, x.m_mp, rnd); }
  /// absolute value
  int abs(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { return mpfr_abs(m_mp, x.m_mp, rnd); }
  //@}
  
  /// \name shift functions
  //@{
  /// left shift for <tt>char</tt>
  int mul_2exp(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2si(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>unsigned char</tt>
  int mul_2exp(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2ui(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>short</tt>
  int mul_2exp(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2si(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>unsigned short</tt>
  int mul_2exp(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2ui(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>int</tt>
  int mul_2exp(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2si(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>unsigned int</tt>
  int mul_2exp(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2ui(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>long</tt>
  int mul_2exp(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2si(m_mp, x.m_mp, y, rnd); }
  /// left shift for <tt>unsigned long</tt>
  int mul_2exp(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_mul_2ui(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>char</tt>
  int div_2exp(const Mpfr& x, char y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2si(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>unsigned char</tt>
  int div_2exp(const Mpfr& x, unsigned char y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2ui(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>short</tt>
  int div_2exp(const Mpfr& x, short y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2si(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>unsigned short</tt>
  int div_2exp(const Mpfr& x, unsigned short y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2ui(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>int</tt>
  int div_2exp(const Mpfr& x, int y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2si(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>unsigned int</tt>
  int div_2exp(const Mpfr& x, unsigned int y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2ui(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>long</tt>
  int div_2exp(const Mpfr& x, long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2si(m_mp, x.m_mp, y, rnd); }
  /// right shift for <tt>unsigned long</tt>
  int div_2exp(const Mpfr& x, unsigned long y, rnd_t rnd = MPFR_RND)
  { return mpfr_div_2ui(m_mp, x.m_mp, y, rnd); }
  //@}

  /// \name comparison functions
  //@{
  /// compare with <tt>Mpfr</tt>
  int cmp(const Mpfr& x) const
  { return mpfr_cmp(m_mp, x.m_mp); }
  /// compare with <tt>char</tt>
  int cmp(char x) const
  { return mpfr_cmp_si(m_mp, x); }
  /// compare with <tt>unsigned char</tt>
  int cmp(unsigned char x) const
  { return mpfr_cmp_ui(m_mp, x); }
  /// compare with <tt>short</tt>
  int cmp(short x) const
  { return mpfr_cmp_si(m_mp, x); }
  /// compare with <tt>unsigned short</tt>
  int cmp(unsigned short x) const
  { return mpfr_cmp_ui(m_mp, x); }
  /// compare with <tt>int</tt>
  int cmp(int x) const
  { return mpfr_cmp_si(m_mp, x); }
  /// compare with <tt>unsigned int</tt>
  int cmp(unsigned int x) const
  { return mpfr_cmp_ui(m_mp, x); }
  /// compare with <tt>long</tt>
  int cmp(long x) const
  { return mpfr_cmp_si(m_mp, x); }
  /// compare with <tt>unsigned long</tt>
  int cmp(unsigned long x) const
  { return mpfr_cmp_ui(m_mp, x); }
  /// compare with <tt>float</tt>
  int cmp(float x) const
  { return mpfr_cmp_d(m_mp, x); }
  /// compare with <tt>double</tt>
  int cmp(double x) const
  { return mpfr_cmp_d(m_mp, x); }
  /// compare with <tt>BigInt</tt>
  int cmp(const BigInt& x) const
  { return mpfr_cmp_z(m_mp, x.mp()); }
  /// compare with <tt>BigRat</tt>
  int cmp(const BigRat& x) const
  { return mpfr_cmp_q(m_mp, x.mp()); }
  /// compare with \f$x*2^e\f$ for <tt>char</tt>
  int cmp_2exp(char x, exp_t e) const
  { return mpfr_cmp_si_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>char</tt>
  int cmp_2exp(unsigned char x, exp_t e) const
  { return mpfr_cmp_ui_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>short</tt>
  int cmp_2exp(short x, exp_t e) const
  { return mpfr_cmp_si_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>unsigned short</tt>
  int cmp_2exp(unsigned short x, exp_t e) const
  { return mpfr_cmp_ui_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>int</tt>
  int cmp_2exp(int x, exp_t e) const
  { return mpfr_cmp_si_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>unsigned int</tt>
  int cmp_2exp(unsigned int x, exp_t e) const
  { return mpfr_cmp_ui_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>long</tt>
  int cmp_2exp(long x, exp_t e) const
  { return mpfr_cmp_si_2exp(m_mp, x, e); }
  /// compare with \f$x*2^e\f$ for <tt>unsigned long</tt>
  int cmp_2exp(unsigned long x, exp_t e) const
  { return mpfr_cmp_ui_2exp(m_mp, x, e); }
  /// compare (in absolute value) with <tt>Mpfr</tt>
  int cmpabs(const Mpfr& x) const
  { return mpfr_cmpabs(m_mp, x.m_mp); }
  //@}

  /// \name conversion functions
  //@{
  /// return double value
  double get_d(rnd_t rnd = MPFR_RND) const
  { return mpfr_get_d(m_mp, rnd); }
  /// find d and exp s.t. \f$d*2^{exp}\f$ with \f$0.5\le|d|<1\f$
  double get_d_2exp(long* exp, rnd_t rnd = MPFR_RND) const
  { return mpfr_get_d_2exp(exp, m_mp, rnd); }
  /// return long value
  long get_si(rnd_t rnd = MPFR_RND) const
  { return mpfr_get_si(m_mp, rnd); }
  /// return unsigned long value
  unsigned long get_ui(rnd_t rnd = MPFR_RND) const
  { return mpfr_get_ui(m_mp, rnd); }
  /// return BigInt value
  BigInt get_z(rnd_t rnd = MPFR_RND) const
  { BigInt r; mpfr_get_z(r.mp(), m_mp, rnd); return r; }
  /// return z and exp s.t. it equals \f$x*2^{exp}\f$
  exp_t get_z_exp(BigInt& z) const
  { return mpfr_get_z_exp(z.mp(), m_mp); }
  /// return the string representation
  std::string get_str(size_t ndigits=0,int base=10,rnd_t rnd=MPFR_RND) const {
    if (is_integer()) {
      BigInt z = get_z();
      if (ndigits == 0 || z.sizeinbase(base) < ndigits) return z.get_str();
    } 
    return mpfr2str(m_mp, ndigits, base, false, rnd);
  }
  /// return the string representation in fixed format
  std::string get_fixed_str(size_t ndigits = 0, int base = 10, rnd_t rnd = MPFR_RND) const
  { return mpfr2str(m_mp, ndigits, base, true, rnd); }
  //@}
  
  /// \name helper functions
  //@{
  bool is_nan() const
  { return mpfr_nan_p(m_mp) != 0; }
  bool is_inf() const
  { return mpfr_inf_p(m_mp) != 0; }
  bool is_number() const
  { return mpfr_number_p(m_mp) != 0; }
  bool is_zero() const
  { return mpfr_zero_p(m_mp) != 0; }
  bool greater(const Mpfr& x) const
  { return mpfr_greater_p(m_mp, x.m_mp) != 0; }
  bool greaterequal(const Mpfr& x) const
  { return mpfr_greaterequal_p(m_mp, x.m_mp) != 0; }
  bool less(const Mpfr& x) const
  { return mpfr_less_p(m_mp, x.m_mp) != 0; }
  bool lessequal(const Mpfr& x) const
  { return mpfr_lessequal_p(m_mp, x.m_mp) != 0; }
  bool lessgreater(const Mpfr& x) const
  { return mpfr_lessgreater_p(m_mp, x.m_mp) != 0; }
  bool equal(const Mpfr& x) const
  { return mpfr_equal_p(m_mp, x.m_mp) != 0; }
  bool unordered(const Mpfr& x) const
  { return mpfr_unordered_p(m_mp, x.m_mp) != 0; }
  bool is_integer() const
  { return mpfr_integer_p(m_mp) != 0; }
  bool is_ulong(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_ulong_p(m_mp, rnd) != 0; }
  bool is_slong(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_slong_p(m_mp, rnd) != 0; }
  bool is_uint(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_uint_p(m_mp, rnd) != 0; }
  bool is_sint(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_sint_p(m_mp, rnd) != 0; }
  bool is_ushort(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_ushort_p(m_mp, rnd) != 0; }
  bool is_sshort(rnd_t rnd = MPFR_RND) const
  { return mpfr_fits_sshort_p(m_mp, rnd) != 0; }
  //@}

  /// miscellaneous functions
  //@{
  /// set to +infty
  void set_pos_inf() 
  { mpfr_set_inf(m_mp, 1); }
  /// set to -infty
  void set_neg_inf() 
  { mpfr_set_inf(m_mp, -1); }
  /// set to NaN
  void set_nan() 
  { mpfr_set_nan(m_mp); }
  /// swap Function
  void swap(Mpfr& other)
  { mpfr_swap(m_mp, other.m_mp); }
  /// return sign
  int sgn() const
  { return mpfr_sgn(m_mp); }
  /// return upper bound of MSB
  long uMSB() const
  { BigInt x; exp_t e = get_z_exp(x); return x.ceillg() + e; }
  /// return lower bound of MSB
  long lMSB() const
  { BigInt x; exp_t e = get_z_exp(x); return x.floorlg() + e; }

  void rint(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint(m_mp, x.m_mp, rnd); }
  void ceil(const Mpfr& x)
  { mpfr_ceil(m_mp, x.m_mp); }
  void floor(const Mpfr& x)
  { mpfr_floor(m_mp, x.m_mp); }
  void round(const Mpfr& x)
  { mpfr_round(m_mp, x.m_mp); }
  void trunc(const Mpfr& x)
  { mpfr_trunc(m_mp, x.m_mp); }
  void rint_ceil(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_ceil(m_mp, x.m_mp, rnd); }
  void rint_floor(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_floor(m_mp, x.m_mp, rnd); }
  void rint_round(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_round(m_mp, x.m_mp, rnd); }
  void rint_trunc(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { mpfr_rint_trunc(m_mp, x.m_mp, rnd); }
  void frac(const Mpfr& x, rnd_t rnd = MPFR_RND)
  { mpfr_frac(m_mp, x.m_mp, rnd); }
  void nexttoward(const Mpfr& x) 
  { mpfr_nexttoward(m_mp, x.m_mp); }
  void nextabove() 
  { mpfr_nextabove(m_mp); }
  void nextbelow() 
  { mpfr_nextbelow(m_mp); }
  void min(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { mpfr_min(m_mp, x.m_mp, y.m_mp, rnd); }
  void max(const Mpfr& x, const Mpfr& y, rnd_t rnd = MPFR_RND)
  { mpfr_max(m_mp, x.m_mp, y.m_mp, rnd); }
  //@}

public:
  /// \name global functions (static)
  //@{
  static void set_default_prec(prec_t prec)
  { mpfr_set_default_prec(prec); }
  static prec_t get_default_prec(void)
  { return mpfr_get_default_prec(); }
  static void set_default_rounding_mode(rnd_t rnd)
  { mpfr_set_default_rounding_mode(rnd); }
  static rnd_t get_default_rounding_mode(void)
  { return mpfr_get_default_rounding_mode(); }
  static const char* get_version() 
  { return mpfr_get_version(); }
  static const char* print_rnd_mode(rnd_t rnd)
  { return mpfr_print_rnd_mode(rnd); }
  static exp_t get_emin()
  { return mpfr_get_emin(); }
  static exp_t get_emax()
  { return mpfr_get_emax(); }
  static int set_emin(exp_t e)
  { return mpfr_set_emin(e); }
  static int set_emax(exp_t e)
  { return mpfr_set_emax(e); }
  static exp_t get_emin_min()
  { return mpfr_get_emin_min(); }
  static exp_t get_emin_max()
  { return mpfr_get_emin_max(); }
  static exp_t get_emax_min()
  { return mpfr_get_emax_min(); }
  static exp_t get_emax_max()
  { return mpfr_get_emax_max(); }

  static void clear_underflow()
  { mpfr_clear_underflow(); }
  static void clear_overflow()
  { mpfr_clear_overflow(); }
  static void clear_nanflag()
  { mpfr_clear_nanflag(); }
  static void clear_inexflag()
  { mpfr_clear_inexflag(); }
  static void clear_erangeflag()
  { mpfr_clear_erangeflag(); }
  static void clear_flags()
  { mpfr_clear_flags(); }

  static void set_underflow()
  { mpfr_set_underflow(); }
  static void set_overflow()
  { mpfr_set_overflow(); }
  static void set_nanflag()
  { mpfr_set_nanflag(); }
  static void set_inexflag()
  { mpfr_set_inexflag(); }
  static void set_erangeflag()
  { mpfr_set_erangeflag(); }

  static bool is_underflow()
  { return mpfr_underflow_p() != 0; }
  static bool is_overflow()
  { return mpfr_overflow_p() != 0; }
  static bool is_nanflag()
  { return mpfr_nanflag_p() != 0; }
  static bool is_inexflag()
  { return mpfr_inexflag_p() != 0; }
  static bool is_erangeflag()
  { return mpfr_erangeflag_p() != 0; }
  //@}

  // count the precision of a char
  static prec_t count_prec(char)
  { return INT_PREC; }
  // count the precision of a unsigned char
  static prec_t count_prec(unsigned char)
  { return INT_PREC; }
  // count the precision of a short
  static prec_t count_prec(short)
  { return INT_PREC; }
  // count the precision of a unsigned short
  static prec_t count_prec(unsigned short)
  { return INT_PREC; }
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
  // count the precision of a float
  static prec_t count_prec(float)
  { return SINGLE_PREC; }
  // count the precision of a double
  static prec_t count_prec(double)
  { return DOUBLE_PREC; }
  // count the precision of a BigInt
  static prec_t count_prec(const BigInt& z)
  { return std::max(z.sizeinbase(2), INT_PREC); }
  // count the precision of a BigInt
  static prec_t count_prec(const BigRat&)
  { return DOUBLE_PREC; }
  static prec_t count_prec(const Mpfr& z)
  { return z.get_prec(); }
  // count the precision in a string representation
  //   prec <= len*log_2(base) <= len*(1+ilogb(base))
  static prec_t count_prec(const char* str, int base = 10)
  { return strlen(str)*(1+ilogb(base)); }	
  static prec_t count_prec(const std::string& str, int base = 10)
  { return str.length()*(1+ilogb(base)); }	

  // count how many precision needed for addition/subtraction
  static prec_t add_prec(const Mpfr& x, const Mpfr& y) {
    exp_t diff = x.get_exp() - y.get_exp();
    if (diff >= 0)
      return std::max(x.get_prec() + diff, y.get_prec());
    else
      return std::max(x.get_prec(), y.get_prec() - diff);
  }
  // count how many precision needed for muliplication
  static prec_t mul_prec(const Mpfr& x, const Mpfr& y) {
    return x.get_prec() + y.get_prec();
  }


public: // C++ operators
  /// \name unary, increment, decrement operators
  //@{
  /// unary plus operator
  Mpfr operator+() const
  { return Mpfr(*this); }
  /// unary negation operator
  Mpfr operator-() const
  { Mpfr r; r.neg(*this); return r; }
  /// prefix increment operator
  Mpfr& operator++()  
  { add(*this, 1); return *this; }
  /// postfix increment operator
  Mpfr operator++(int) 
  { Mpfr r(*this); ++(*this); return r; }
  /// prefix decrement operator
  Mpfr& operator--()
  { sub(*this, 1); return *this; }
  /// postfix decrement operator
  Mpfr operator--(int)
  { Mpfr r(*this); --(*this); return r; }
  //@}

  /// \name assignment and compound assignment operators
  //@{
  /// assignment operator for <tt>Mpfr</tt>
  Mpfr& operator=(const Mpfr& rhs)
  { if (&rhs != this) set(rhs); return *this; }
  /// generic assignment operator for <tt>T</tt>
  template <typename T> Mpfr& operator=(const T& rhs)
  { set(rhs); return *this; }
  /// generic compound assignment operator <tt>+=</tt>
  template <typename T> Mpfr& operator+=(const T& rhs)
  { add(*this, rhs); return *this; }
  /// generic compound assignment operator <tt>-=</tt>
  template <typename T> Mpfr& operator-=(const T& rhs)
  { sub(*this, rhs); return *this; }
  /// generic compound assignment operator <tt>*=</tt>
  template <typename T> Mpfr& operator*=(const T& rhs)
  { mul(*this, rhs); return *this; }
  /// generic compound assignment operator <tt>/=</tt>
  template <typename T> Mpfr& operator/=(const T& rhs)
  { div(*this, rhs); return *this; }
  /// generic compound assignment operator <tt><<=</tt>
  template <typename T> Mpfr& operator<<=(const T& rhs)
  { mul_2exp(*this, rhs); return *this; }
  /// generic compound assignment operator <tt>>>=</tt>
  template <typename T> Mpfr& operator>>=(const T& rhs)
  { div_2exp(*this, rhs); return *this; }
  //@}


};

/// \addtogroup MpfrArithmeticOperators
//@{
/// Mpfr + Mpfr
inline Mpfr operator+(const Mpfr& x, const Mpfr& y)
{ Mpfr r; r.add(x, y); return r; }
/// Mpfr + T
template <typename T> inline Mpfr operator+(const Mpfr& x, const T& y)
{ Mpfr r; r.add(x, y); return r; }
/// T + Mpfr
template <typename T> inline Mpfr operator+(const T& x, const Mpfr& y)
{ Mpfr r; r.add(x, y); return r; }

/// Mpfr - Mpfr
inline Mpfr operator-(const Mpfr& x, const Mpfr& y)
{ Mpfr r; r.sub(x, y); return r; }
/// Mpfr - T
template <typename T> inline Mpfr operator-(const Mpfr& x, const T& y)
{ Mpfr r; r.sub(x, y); return r; }
/// T - Mpfr
template <typename T> inline Mpfr operator-(const T& x, const Mpfr& y)
{ Mpfr r; r.sub(x, y); return r; }

/// Mpfr * Mpfr
inline Mpfr operator*(const Mpfr& x, const Mpfr& y)
{ Mpfr r; r.mul(x, y); return r; }
/// Mpfr * T
template <typename T> inline Mpfr operator*(const Mpfr& x, const T& y)
{ Mpfr r; r.mul(x, y); return r; }
/// T * Mpfr
template <typename T> inline Mpfr operator*(const T& x, const Mpfr& y)
{ Mpfr r; r.mul(x, y); return r; }

/// Mpfr / Mpfr (w/ default precision MPFR_DEF_DIV_PREC)
inline Mpfr operator/(const Mpfr& x, const Mpfr& y)
{ Mpfr r; r.div(x, y); return r; }
/// Mpfr / T (w/ default precision MPFR_DEF_DIV_PREC)
template <typename T> inline Mpfr operator/(const Mpfr& x, const T& y)
{ Mpfr r; r.div(x, y); return r; }
/// T / Mpfr (w/ default precision MPFR_DEF_DIV_PREC)
template <typename T> inline Mpfr operator/(const T& x, const Mpfr& y)
{ Mpfr r; r.div(x, y); return r; }

/// left shift
template <typename T> inline Mpfr operator<<(const Mpfr& x, const T& y)
{ Mpfr r; r.mul_2exp(x, y); return r; } 
/// right shift
template <typename T> inline Mpfr operator>>(const Mpfr& x, const T& y)
{ Mpfr r; r.div_2exp(x, y); return r; }
//@}

/// \addtogroup MpfrComparisonOperators
//@{
/// Mpfr == Mpfr
inline bool operator==(const Mpfr& x, const Mpfr& y)
{ return x.cmp(y) == 0; }
/// Mpfr == T
template <typename T>
inline bool operator==(const Mpfr& x, const T& y)
{ return x.cmp(y) == 0; }
/// T == Mpfr
template <typename T>
inline bool operator==(const T& x, const Mpfr& y)
{ return y.cmp(x) == 0; }

/// Mpfr != Mpfr
inline bool operator!=(const Mpfr& x, const Mpfr& y)
{ return x.cmp(y) != 0; }
/// Mpfr != T
template <typename T>
inline bool operator!=(const Mpfr& x, const T& y)
{ return x.cmp(y) != 0; }
/// T != Mpfr
template <typename T>
inline bool operator!=(const T& x, const Mpfr& y)
{ return y.cmp(x) != 0; }

/// Mpfr >= Mpfr
inline bool operator>=(const Mpfr& x, const Mpfr& y)
{ return x.cmp(y) >= 0; }
/// Mpfr >= T
template <typename T>
inline bool operator>=(const Mpfr& x, const T& y)
{ return x.cmp(y) >= 0; }
/// T >= Mpfr
template <typename T>
inline bool operator>=(const T& x, const Mpfr& y)
{ return y.cmp(x) <= 0; }

/// Mpfr <= Mpfr
inline bool operator<=(const Mpfr& x, const Mpfr& y)
{ return x.cmp(y) <= 0; }
/// Mpfr <= T
template <typename T>
inline bool operator<=(const Mpfr& x, const T& y)
{ return x.cmp(y) <= 0; }
/// T <= Mpfr
template <typename T>
inline bool operator<=(const T& x, const Mpfr& y)
{ return y.cmp(x) >= 0; }

/// Mpfr > Mpfr
inline bool operator>(const Mpfr& x, const Mpfr& y)
{ return x.cmp(y) > 0; }
/// Mpfr > T
template <typename T>
inline bool operator>(const Mpfr& x, const T& y)
{ return x.cmp(y) > 0; }
/// T > Mpfr
template <typename T>
inline bool operator>(const T& x, const Mpfr& y)
{ return y.cmp(x) < 0; }

/// Mpfr < Mpfr
inline bool operator<(const Mpfr& x, const Mpfr& y)
{ return x.cmp(y) < 0; }
/// Mpfr < T
template <typename T>
inline bool operator<(const Mpfr& x, const T& y)
{ return x.cmp(y) < 0; }
/// T < Mpfr
template <typename T>
inline bool operator<(const T& x, const Mpfr& y)
{ return y.cmp(x) > 0; }
//@}

/// \addtogroup MpfrIostreamOperators
//@{
/// istream operator for <tt>Mpfr</tt>
inline std::istream& operator>>(std::istream& is, Mpfr& x)
{ return is >> x.mp(); }
/// ostream operator for <tt>Mpfr</tt>
inline std::ostream& operator<<(std::ostream& os, const Mpfr& x)
{ return os << x.get_str(); }
//@}

/*
/// \addtogroup MpfrGlobalFunctions
//@{
/// square root
inline Mpfr sqrt(const Mpfr& x, prec_t prec = MPFR_DEF_SQRT_PREC)
{ Mpfr r; r.sqrt(x, prec); return r; }
/// cubic root
inline Mpfr cbrt(const Mpfr& x, prec_t prec = MPFR_DEF_CBRT_PREC)
{ Mpfr r; r.cbrt(x, prec); return r; }
/// k-th root
inline Mpfr root(const Mpfr& x, unsigned long k, prec_t prec = MPFR_DEF_ROOT_PREC)
{ Mpfr r; r.root(x, k, prec); return r; }
//@}
*/

#ifdef CORE_END_NAMESPACE
CORE_END_NAMESPACE
#endif

#endif /*__MPFR_H__*/
