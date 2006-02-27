/****************************************************************************
 * BigInt.h -- A C++ wrapper class for GMP mpz
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
 * $Id: BigInt.h,v 1.2 2006-02-27 04:37:28 exact Exp $
 ***************************************************************************/
#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <gmp.h>
#include <string>

/* _gmp_alloc_cstr */
struct _gmp_alloc_cstr {
  char *str;
  _gmp_alloc_cstr(int len) { str = new char[len]; }
  ~_gmp_alloc_cstr() { delete[] str; }
};

/* addition */
inline void mpz_add_si(mpz_ptr z, mpz_srcptr x, long y)
{ y >= 0 ? mpz_add_ui(z, x, y) : mpz_sub_ui(z, x, -(y)); }
    
/* subtraction */
inline void mpz_sub_si(mpz_ptr z, mpz_srcptr x, long y)
{ y >= 0 ? mpz_sub_ui(z, x, y) : mpz_add_ui(z, x, -(y)); }     
inline void mpz_si_sub(mpz_ptr z, long x, mpz_srcptr y)
{ if (x >= 0) mpz_ui_sub(z, x, y); else { mpz_add_ui(z, y, -(x)); mpz_neg(z, z); } }

/* division */
inline void mpz_div_si(mpz_ptr z, mpz_srcptr x, long y) 
{ if (y >= 0) mpz_div_ui(z, x, y); else { mpz_div_ui(z, x, -(y)); mpz_neg(z, z); } }

/* modular */
inline void mpz_mod_si(mpz_ptr z, mpz_srcptr x, long y) 
{ if (y >= 0) mpz_mod_ui(z, x, y); else { mpz_mod_ui(z, x, -(y)); mpz_neg(z, z); } }

/* exact division (faster) */
inline void mpz_divexact_si(mpz_ptr z, mpz_srcptr x, long y) 
{ if (y >= 0) mpz_divexact_ui(z, x, y); else { mpz_divexact_ui(z, x, -(y)); mpz_neg(z, z); } }

/* division with remainder */
inline void mpz_divrem(mpz_ptr z, mpz_ptr r, mpz_srcptr x, mpz_srcptr y)
{ mpz_fdiv_qr(z, r, x, y); }
inline void mpz_divrem_ui(mpz_ptr z, mpz_ptr r, mpz_srcptr x, unsigned long y) 
{ mpz_fdiv_qr_ui(z, r, x, y); }
inline void mpz_divrem_si(mpz_ptr z, mpz_ptr r, mpz_srcptr x, long y) 
{ if (y >= 0) mpz_divrem_ui(z, r, x, y); else { mpz_divrem_ui(z, r, x, -(y)); mpz_neg(z, z); } }
 
/* comparison */
inline int mpz_cmpabs_si(mpz_srcptr z, long x) 
{ return x >= 0 ? mpz_cmpabs_ui(z, x) : mpz_cmpabs_ui(z, -x); }

/* power function */
inline void mpz_si_pow_ui(mpz_ptr z, long base, unsigned long exp) 
{ if (base >= 0) mpz_ui_pow_ui(z, base, exp); else { mpz_ui_pow_ui(z, -(base), exp); if (exp % 2 == 1) mpz_neg(z, z); } }

#ifdef CORE_BEGIN_NAMESPACE
CORE_BEGIN_NAMESPACE
#endif

/// \class BigInt BigInt.h
/// \brief BigInt is a wrapper class of <tt>mpz</tt> in GMP
class BigInt {
private:
  mpz_t m_mp;
public:
  //internal structure accessors
  const mpz_t& mp() const { return m_mp; }
  mpz_t& mp() { return m_mp; }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  BigInt()
  { mpz_init(m_mp); }
  /// copy constructor
  BigInt(const BigInt& rhs)
  { mpz_init_set(m_mp, rhs.m_mp); }

  /// constructor for <tt>int</tt>
  BigInt(int i)
  { mpz_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned int</tt>
  BigInt(unsigned int i)
  { mpz_init_set_ui(m_mp, i); }

  /// constructor for <tt>long</tt>
  BigInt(long i)
  { mpz_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned long</tt>
  BigInt(unsigned long i)
  { mpz_init_set_ui(m_mp, i); }

  /// constructor for <tt>double</tt>
  BigInt(double i)
  { mpz_init_set_d(m_mp, i); }

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit BigInt(const char* str, int base = 0)
  { mpz_init_set_str(m_mp, str, base); }

  /// constructor for <tt>std::string&</tt> (no implicit conversion)
  explicit BigInt(const std::string& str, int base = 0)
  { mpz_init_set_str(m_mp, str.c_str(), base); }

  // internal used by BigRat
  explicit BigInt(const mpz_t x)
  { mpz_init_set(m_mp, x); }

  /// destructor
  ~BigInt()
  { mpz_clear(m_mp); }
  //@}
  
public:
  /// \name assignment functions
  //@{
  /// assignment function for <tt>BigInt</tt>
  void set(const BigInt& rhs)
  { mpz_set(m_mp, rhs.m_mp); }
  /// assignment function for <tt>int</tt>
  void set(int i)
  { mpz_set_si(m_mp, i); }
  /// assignment function for <tt>unsigned int</tt>
  void set(unsigned int i)
  { mpz_set_ui(m_mp, i); }
  /// assignment function for <tt>long</tt>
  void set(long i)
  { mpz_set_si(m_mp, i); }
  /// assignment function for <tt>unsigned long</tt>
  void set(unsigned long i)
  { mpz_set_ui(m_mp, i); }
  /// assignment function for <tt>double</tt>
  void set(double i)
  { mpz_set_d(m_mp, i); }
  /// assignment function for <tt>char*</tt>
  int set(const char* str, int base = 0)
  { return mpz_set_str(m_mp, str, base); }
  /// assignment function for <tt>std::string&</tt>
  int set(const std::string& str, int base = 0)
  { return mpz_set_str(m_mp, str.c_str(), base); }
  //@}

  /// \name arithmetic functions -- addition
  //@{
  /// addition for <tt>BigInt + BigInt</tt>
  void add(const BigInt& x, const BigInt& y)
  { mpz_add(m_mp, x.m_mp, y.m_mp); }
  /// addition for <tt>BigInt + int</tt>
  void add(const BigInt& x, int y)
  { mpz_add_si(m_mp, x.m_mp, y); }
  /// addition for <tt>BigInt + unsigned int</tt>
  void add(const BigInt& x, unsigned int y)
  { mpz_add_ui(m_mp, x.m_mp, y); }
  /// addition for <tt>BigInt + long</tt>
  void add(const BigInt& x, long y)
  { mpz_add_si(m_mp, x.m_mp, y); }
  /// addition for <tt>BigInt + unsigned long</tt>
  void add(const BigInt& x, unsigned long y)
  { mpz_add_ui(m_mp, x.m_mp, y); }
  /// addition for <tt>BigInt + double</tt>
  void add(const BigInt& x, double y)
  { this->add(x, BigInt(y)); }
  /// addition for <tt>int + BigInt</tt>
  void add(int x, const BigInt& y)
  { this->add(y, x); }
  /// addition for <tt>unsigned int + BigInt</tt>
  void add(unsigned int x, const BigInt& y)
  { this->add(y, x); }
  /// addition for <tt>long + BigInt</tt>
  void add(long x, const BigInt& y)
  { this->add(y, x); }
  /// addition for <tt>unsigned long + BigInt</tt>
  void add(unsigned long x, const BigInt& y)
  { this->add(y, x); }
  /// addition for <tt>double + BigInt</tt>
  void add(double x, const BigInt& y)
  { this->add(y, x); }
  //@}

  /// \name arithmetic functions -- subtraction
  //@{
  /// subtraction for <tt>BigInt - BigInt</tt>
  void sub(const BigInt& x, const BigInt& y)
  { mpz_sub(m_mp, x.m_mp, y.m_mp); }
  /// subtraction for <tt>BigInt - int</tt>
  void sub(const BigInt& x, int y)
  { mpz_sub_si(m_mp, x.m_mp, y); }
  /// subtraction for <tt>BigInt - unsigned int</tt>
  void sub(const BigInt& x, unsigned int y)
  { mpz_sub_ui(m_mp, x.m_mp, y); }
  /// subtraction for <tt>BigInt - long</tt>
  void sub(const BigInt& x, long y)
  { mpz_sub_si(m_mp, x.m_mp, y); }
  /// subtraction for <tt>BigInt - unsigned long</tt>
  void sub(const BigInt& x, unsigned long y)
  { mpz_sub_ui(m_mp, x.m_mp, y); }
  /// subtraction for <tt>BigInt - double</tt>
  void sub(const BigInt& x, double y)
  { this->sub(x, BigInt(y)); }
  /// subtraction for <tt>int - BigInt</tt>
  void sub(int x, const BigInt& y)
  { mpz_si_sub(m_mp, x, y.m_mp); }
  /// subtraction for <tt>unsigned int - BigInt</tt>
  void sub(unsigned int x, const BigInt& y)
  { mpz_ui_sub(m_mp, x, y.m_mp); }
  /// subtraction for <tt>long - BigInt</tt>
  void sub(long x, const BigInt& y)
  { mpz_si_sub(m_mp, x, y.m_mp); }
  /// subtraction for <tt>unsigned long - BigInt</tt>
  void sub(unsigned long x, const BigInt& y)
  { mpz_ui_sub(m_mp, x, y.m_mp); }
  /// subtraction for <tt>double - BigInt</tt>
  void sub(double x, const BigInt& y)
  { this->sub(y, x); }
  //@}

  /// \name arithmetic functions -- multiplication
  //@{
  /// multiplication for <tt>BigInt * BigInt</tt>
  void mul(const BigInt& x, const BigInt& y)
  { mpz_mul(m_mp, x.m_mp, y.m_mp); }
  /// multiplication for <tt>BigInt * int</tt>
  void mul(const BigInt& x, int y)
  { mpz_mul_si(m_mp, x.m_mp, y); }
  /// multiplication for <tt>BigInt * unsigned int</tt>
  void mul(const BigInt& x, unsigned int y)
  { mpz_mul_ui(m_mp, x.m_mp, y); }
  /// multiplication for <tt>BigInt * long</tt>
  void mul(const BigInt& x, long y)
  { mpz_mul_si(m_mp, x.m_mp, y); }
  /// multiplication for <tt>BigInt * unsigned long</tt>
  void mul(const BigInt& x, unsigned long y)
  { mpz_mul_ui(m_mp, x.m_mp, y); }
  /// multiplication for <tt>BigInt * double</tt>
  void mul(const BigInt& x, double y)
  { this->mul(x, BigInt(y)); }
  /// multiplication for <tt>int * BigInt</tt>
  void mul(int x, const BigInt& y)
  { this->mul(y, x); }
  /// multiplication for <tt>unsigned int * BigInt</tt>
  void mul(unsigned int x, const BigInt& y)
  { this->mul(y, x); }
  /// multiplication for <tt>long * BigInt</tt>
  void mul(long x, const BigInt& y)
  { this->mul(y, x); }
  /// multiplication for <tt>unsigned long * BigInt</tt>
  void mul(unsigned long x, const BigInt& y)
  { this->mul(y, x); }
  /// multiplication for <tt>double * BigInt</tt>
  void mul(double x, const BigInt& y)
  { this->mul(y, x); }
  //@}

  /// \name arithmetic functions -- division
  //@{
  /// division for <tt>BigInt / BigInt</tt>
  void div(const BigInt& x, const BigInt& y)
  { mpz_div(m_mp, x.m_mp, y.m_mp); }
  /// division for <tt>BigInt / int</tt>
  void div(const BigInt& x, int y)
  { mpz_div_si(m_mp, x.m_mp, y); }
  /// division for <tt>BigInt / unsigned int</tt>
  void div(const BigInt& x, unsigned int y)
  { mpz_div_ui(m_mp, x.m_mp, y); }
  /// division for <tt>BigInt / long</tt>
  void div(const BigInt& x, long y)
  { mpz_div_si(m_mp, x.m_mp, y); }
  /// division for <tt>BigInt / unsigned long</tt>
  void div(const BigInt& x, unsigned long y)
  { mpz_div_ui(m_mp, x.m_mp, y); }
  /// division for <tt>BigInt / double</tt>
  void div(const BigInt& x, double y)
  { this->div(x, BigInt(y)); }
  //@}

  /// \name arithmetic functions -- modular
  //@{
  /// modular for <tt>BigInt % BigInt</tt>
  void mod(const BigInt& x, const BigInt& y)
  { mpz_mod(m_mp, x.m_mp, y.m_mp); }
  /// modular for <tt>BigInt % int</tt>
  void mod(const BigInt& x, int y)
  { mpz_mod_si(m_mp, x.m_mp, y); }
  /// modular for <tt>BigInt % unsigned int</tt>
  void mod(const BigInt& x, unsigned int y)
  { mpz_mod_ui(m_mp, x.m_mp, y); }
  /// modular for <tt>BigInt % long</tt>
  void mod(const BigInt& x, long y)
  { mpz_mod_si(m_mp, x.m_mp, y); }
  /// modular for <tt>BigInt % unsigned long</tt>
  void mod(const BigInt& x, unsigned long y)
  { mpz_mod_ui(m_mp, x.m_mp, y); }
  /// modular for <tt>BigInt % double</tt>
  void mod(const BigInt& x, double y)
  { this->mod(x, BigInt(y)); }
  //@}

  /// \name arithmetic functions -- exact division
  //@{
  /// exact division for <tt>BigInt / BigInt</tt>
  void divexact(const BigInt& x, const BigInt& y)
  { mpz_divexact(m_mp, x.m_mp, y.m_mp); }
  /// exact division for <tt>BigInt / int</tt>
  void divexact(const BigInt& x, int y)
  { mpz_divexact_si(m_mp, x.m_mp, y); }
  /// exact division for <tt>BigInt / unsigned int</tt>
  void divexact(const BigInt& x, unsigned int y)
  { mpz_divexact_ui(m_mp, x.m_mp, y); }
  /// exact division for <tt>BigInt / long</tt>
  void divexact(const BigInt& x, long y)
  { mpz_divexact_si(m_mp, x.m_mp, y); }
  /// exact division for <tt>BigInt / unsigned long</tt>
  void divexact(const BigInt& x, unsigned long y)
  { mpz_divexact_ui(m_mp, x.m_mp, y); }
  /// exact division for <tt>BigInt / double</tt>
  void divexact(const BigInt& x, double y)
  { this->divexact(x, BigInt(y)); }
  //@}

  /// \name arithmetic functions -- division with remainder
  //@{
  /// division with remainder for <tt>BigInt / BigInt</tt>
  void divrem(BigInt& r, const BigInt& x, const BigInt& y)
  { mpz_divrem(m_mp, r.m_mp, x.m_mp, y.m_mp); }
  /// division with remainder for <tt>BigInt / int</tt>
  void divrem(BigInt& r, const BigInt& x, int y)
  { mpz_divrem_si(m_mp, r.m_mp, x.m_mp, y); }
  /// division with remainder for <tt>BigInt / unsigned int</tt>
  void divrem(BigInt& r, const BigInt& x, unsigned int y)
  { mpz_divrem_ui(m_mp, r.m_mp, x.m_mp, y); }
  /// division with remainder for <tt>BigInt / long</tt>
  void divrem(BigInt& r, const BigInt& x, long y)
  { mpz_divrem_si(m_mp, r.m_mp, x.m_mp, y); }
  /// division with remainder for <tt>BigInt / unsigned long</tt>
  void divrem(BigInt& r, const BigInt& x, unsigned long y)
  { mpz_divrem_ui(m_mp, r.m_mp, x.m_mp, y); }
  /// division with remainder for <tt>BigInt / double</tt>
  void divrem(BigInt& r, const BigInt& x, double y)
  { this->divrem(r, x, BigInt(y)); }

  /// \name squart root function
  //@{
  /// square root for <tt>BigInt</tt>
  void sqrt(const BigInt& x)
  { mpz_sqrt(m_mp, x.m_mp); }
  //@}

  /// \name power functions
  //@{
  /// power function for <tt>BigInt</tt>
  void pow(const BigInt& x, unsigned long y)
  { mpz_pow_ui(m_mp, x.m_mp, y); }
  /// power function for <tt>int</tt>
  void pow(int x, unsigned long y)
  { mpz_si_pow_ui(m_mp, x, y); }
  /// power function for <tt>unsigned int</tt>
  void pow(unsigned int x, unsigned long y)
  { mpz_ui_pow_ui(m_mp, x, y); }
  /// power function for <tt>long</tt>
  void pow(long x, unsigned long y)
  { mpz_si_pow_ui(m_mp, x, y); }
  /// power function for <tt>unsigned long</tt>
  void pow(unsigned long x, unsigned long y)
  { mpz_ui_pow_ui(m_mp, x, y); }
  //@}

  /// \name other arithmetic functions
  //@{
  /// negation function
  void neg(const BigInt& x)
  { mpz_neg(m_mp, x.m_mp); }
  /// absolute value function
  void abs(const BigInt& x)
  { mpz_abs(m_mp, x.m_mp); }
  //@}
  
  /// \name shift functions
  //@{
  /// left shift
  void mul_2exp(const BigInt& x, int y)
  { if (y>=0) mpz_mul_2exp(m_mp,x.m_mp,y); else mpz_div_2exp(m_mp,x.m_mp,-y); }
  /// left shift
  void mul_2exp(const BigInt& x, unsigned int y)
  { mpz_mul_2exp(m_mp, x.m_mp, y); }
  /// left shift
  void mul_2exp(const BigInt& x, long y)
  { if (y>=0) mpz_mul_2exp(m_mp,x.m_mp,y); else mpz_div_2exp(m_mp,x.m_mp,-y); }
  /// left shift
  void mul_2exp(const BigInt& x, unsigned long y)
  { mpz_mul_2exp(m_mp, x.m_mp, y); }
  /// right shift
  void div_2exp(const BigInt& x, int y)
  { if (y>=0) mpz_div_2exp(m_mp,x.m_mp,y); else mpz_mul_2exp(m_mp,x.m_mp,-y);}
  /// right shift
  void div_2exp(const BigInt& x, unsigned int y)
  { mpz_div_2exp(m_mp, x.m_mp, y); }
  /// right shift
  void div_2exp(const BigInt& x, long y)
  { if (y>=0) mpz_div_2exp(m_mp,x.m_mp,y); else mpz_mul_2exp(m_mp,x.m_mp,-y);}
  /// right shift
  void div_2exp(const BigInt& x, unsigned long y)
  { mpz_div_2exp(m_mp, x.m_mp, y); }
  //@}

  /// \name comparison functions
  //@{
  /// compare with <tt>BigInt</tt>
  int cmp(const BigInt& x) const
  { return mpz_cmp(m_mp, x.m_mp); }
  /// compare with <tt>int</tt>
  int cmp(int x) const
  { return mpz_cmp_si(m_mp, x); }
  /// compare with <tt>unsigned int</tt>
  int cmp(unsigned int x) const
  { return mpz_cmp_ui(m_mp, x); }
  /// compare with <tt>long</tt>
  int cmp(long x) const
  { return mpz_cmp_si(m_mp, x); }
  /// compare with <tt>unsigned long</tt>
  int cmp(unsigned long x) const
  { return mpz_cmp_ui(m_mp, x); }
  /// compare with <tt>double</tt>
  int cmp(double x) const
  { return mpz_cmp_d(m_mp, x); }
  //@}

  /// \name comparison functions (in absolute value)
  //@{
  /// compare (in absolute value) with <tt>BigInt</tt>
  int cmpabs(const BigInt& x) const
  { return mpz_cmpabs(m_mp, x.m_mp); }
  /// compare (in absolute value) with <tt>int</tt>
  int cmpabs(int x) const
  { return mpz_cmpabs_si(m_mp, x); }
  /// compare (in absolute value) with <tt>unsigned int</tt>
  int cmpabs(unsigned int x) const
  { return mpz_cmpabs_ui(m_mp, x); }
  /// compare (in absolute value) with <tt>long</tt>
  int cmpabs(long x) const
  { return mpz_cmpabs_si(m_mp, x); }
  /// compare (in absolute value) with <tt>unsigned long</tt>
  int cmpabs(unsigned long x) const
  { return mpz_cmpabs_ui(m_mp, x); }
  /// compare (in absolute value) with <tt>double</tt>
  int cmpabs(double x) const
  { return mpz_cmp_d(m_mp, x); }
  //@}

  /// \name logical and bit manipulation functions
  //@{
  /// logical and
  void logical_and(const BigInt& x, const BigInt& y)
  { mpz_and(m_mp, x.m_mp, y.m_mp); }
  /// logical ior
  void logical_ior(const BigInt& x, const BigInt& y)
  { mpz_ior(m_mp, x.m_mp, y.m_mp); }
  /// logical xor
  void logical_xor(const BigInt& x, const BigInt& y)
  { mpz_xor(m_mp, x.m_mp, y.m_mp); }
  /// logical com
  void logical_com(const BigInt& x)
  { mpz_com(m_mp, x.m_mp); }
  //@}
  
  /// \name conversion functions
  //@{
  /// return double value
  double get_d() const
  { return mpz_get_d(m_mp); }
  /// find d and exp s.t. \f$d*2^{exp}\f$ with \f$0.5\le|d|<1\f$
  double get_d_2exp(long* exp) const
  { return mpz_get_d_2exp(exp, m_mp); }
  /// return long value
  long get_si() const
  { return mpz_get_si(m_mp); }
  /// return unsigned long value
  unsigned long get_ui() const
  { return mpz_get_ui(m_mp); }
  /// return the string representation
  std::string get_str(int base = 10) const {
    int len = mpz_sizeinbase(m_mp, base) + 2;
    _gmp_alloc_cstr tmp(len);
    return std::string(mpz_get_str(tmp.str, base, m_mp));
  }
  /// get exponent of power 2
  unsigned long get_2exp() const
  { return mpz_scan1(m_mp, 0); }
  /// get exponent of power k
  unsigned long get_k_exp(BigInt& m, unsigned long k) const
  { return mpz_remove(m.m_mp, m_mp, BigInt(k).m_mp); }
  //@}
  
  /// \name miscellaneous functions
  //@{
  /// swap function 
  void swap(BigInt& other)
  { mpz_swap(m_mp, other.m_mp); }
  /// gcd function
  void gcd(const BigInt& x, const BigInt& y)
  { mpz_gcd(m_mp, x.m_mp, y.m_mp); }
  /// return size in base
  size_t sizeinbase(int base = 2) const
  { return mpz_sizeinbase(m_mp, base); }
  /// return \f$\lceil\log|x|\rceil\f$
  unsigned long ceillg() const
  { unsigned long len=sizeinbase(); return (get_2exp()==len-1)?(len-1):len; }
  /// return \f$\lfloor\log|x|\lfloor\f$
  unsigned long floorlg() const
  { return sizeinbase() - 1; }
  /// return sign
  int sgn() const
  { return mpz_sgn(m_mp); }
  /// return upper bound of MSB
  long uMSB() const
  { return ceillg(); } 
  /// return lower bound of MSB
  long lMSB() const
  { return floorlg(); } 
  //@}

  /// \name helper functions
  //@{
  /// return true if it is divisible
  bool is_divisible(const BigInt& x) const
  { return mpz_divisible_p(m_mp, x.m_mp) != 0; }  
  /// return true if it is odd
  bool is_odd() const
  { return mpz_odd_p(m_mp) != 0; }
  /// return true if it is even
  bool is_even() const
  { return mpz_even_p(m_mp) != 0; }
  /// return true if it fits unsigned long
  bool is_ulong() const
  { return mpz_fits_ulong_p(m_mp) != 0; }
  /// return true if it fits signed long
  bool is_slong() const
  { return mpz_fits_slong_p(m_mp) != 0; }
  /// return true if it fits unsigned int
  bool is_uint() const
  { return mpz_fits_uint_p(m_mp) != 0; }
  /// return true if it fits signed int
  bool is_sint() const
  { return mpz_fits_sint_p(m_mp) != 0; }
  /// return true if it fits unsigned short
  bool is_ushort() const
  { return mpz_fits_ushort_p(m_mp) != 0; }
  /// return true if it fits signed short
  bool is_sshort() const
  { return mpz_fits_sshort_p(m_mp) != 0; }
  //@}

public: // C++ operators
  /// \name unary, increment, decrement operators
  //@{
  /// unary plus operator
  BigInt operator+() const
  { return BigInt(*this); }
  /// unary negation operator
  BigInt operator-() const
  { BigInt r; r.neg(*this); return r; }
  /// prefix increment operator
  BigInt& operator++()
  { add(*this, 1); return *this; }
  /// postfix increment operator
  BigInt operator++(int)
  { BigInt r(*this); ++(*this); return r; }
  /// prefix decrement operator
  BigInt& operator--()
  { sub(*this, 1); return *this; }
  /// postfix decrement operator
  BigInt operator--(int)
  { BigInt r(*this); --(*this); return r; }
  //@}

  /// \name assignment and compound assignment operators
  //@{
  /// assignment operator for <tt>BigInt</tt>
  BigInt& operator=(const BigInt& rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>int</tt>
  BigInt& operator=(int rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>unsigned int</tt>
  BigInt& operator=(unsigned int rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>long</tt>
  BigInt& operator=(long rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>unsigned long</tt>
  BigInt& operator=(unsigned long rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>double</tt>
  BigInt& operator=(double rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>char*</tt>
  BigInt& operator=(const char* rhs)
  { set(rhs); return *this; }
  /// assignment operator for <tt>std::string</tt>
  BigInt& operator=(const std::string& rhs)
  { set(rhs); return *this; }

  /// compound assignment operator <tt>+=</tt>
  BigInt& operator+=(int rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigInt& operator+=(unsigned int rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigInt& operator+=(long rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigInt& operator+=(unsigned long rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigInt& operator+=(double rhs)
  { add(*this, rhs); return *this; }

  /// compound assignment operator <tt>-=</tt>
  BigInt& operator-=(int rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigInt& operator-=(unsigned int rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigInt& operator-=(long rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigInt& operator-=(unsigned long rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigInt& operator-=(double rhs)
  { sub(*this, rhs); return *this; }

  /// compound assignment operator <tt>*=</tt>
  BigInt& operator*=(int rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigInt& operator*=(unsigned int rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigInt& operator*=(long rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigInt& operator*=(unsigned long rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigInt& operator*=(double rhs)
  { mul(*this, rhs); return *this; }

  /// compound assignment operator <tt>/=</tt>
  BigInt& operator/=(int rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigInt& operator/=(unsigned int rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigInt& operator/=(long rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigInt& operator/=(unsigned long rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigInt& operator/=(double rhs)
  { div(*this, rhs); return *this; }

  /// compound assignment operator <tt>%=</tt>
  BigInt& operator%=(int rhs)
  { mod(*this, rhs); return *this; }
  /// compound assignment operator <tt>%=</tt>
  BigInt& operator%=(unsigned int rhs)
  { mod(*this, rhs); return *this; }
  /// compound assignment operator <tt>%=</tt>
  BigInt& operator%=(long rhs)
  { mod(*this, rhs); return *this; }
  /// compound assignment operator <tt>%=</tt>
  BigInt& operator%=(unsigned long rhs)
  { mod(*this, rhs); return *this; }
  /// compound assignment operator <tt>%=</tt>
  BigInt& operator%=(double rhs)
  { mod(*this, rhs); return *this; }

  //@}
  /// compound assignment operator <tt>&=</tt>
  BigInt& operator&=(const BigInt& rhs)
  { logical_and(*this, rhs); return *this; }
  /// compound assignment operator <tt>|=</tt>
  BigInt& operator|=(const BigInt& rhs)
  { logical_ior(*this, rhs); return *this; }
  /// compound assignment operator <tt>^=</tt>
  BigInt& operator^=(const BigInt& rhs)
  { logical_xor(*this, rhs); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigInt& operator<<=(int i)
  { mul_2exp(*this, i); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigInt& operator<<=(unsigned int ui)
  { mul_2exp(*this, ui); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigInt& operator<<=(long l)
  { mul_2exp(*this, l); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigInt& operator<<=(unsigned long ul)
  { mul_2exp(*this, ul); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigInt& operator>>=(int i)
  { div_2exp(*this, i); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigInt& operator>>=(unsigned int ui)
  { div_2exp(*this, ui); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigInt& operator>>=(long l)
  { div_2exp(*this, l); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigInt& operator>>=(unsigned long ul)
  { div_2exp(*this, ul); return *this; }
  //@}

#ifdef CORE_OLDNAMES
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
  //@}
#endif
};

/// \addtogroup BigIntArithmeticOperators
//@{
/// BigInt + BigInt
inline BigInt operator+(const BigInt& x, const BigInt& y)
{ BigInt r; r.add(x, y); return r; }
/// BigInt + int
inline BigInt operator+(const BigInt& x, int y)
{ BigInt r; r.add(x, y); return r; }
/// int + BigInt
inline BigInt operator+(int x, const BigInt& y)
{ BigInt r; r.add(x, y); return r; }
/// BigInt + unsigned int
inline BigInt operator+(const BigInt& x, unsigned int y)
{ BigInt r; r.add(x, y); return r; }
/// unsigned int + BigInt
inline BigInt operator+(unsigned int x, const BigInt& y)
{ BigInt r; r.add(x, y); return r; }
/// BigInt + long
inline BigInt operator+(const BigInt& x, long y)
{ BigInt r; r.add(x, y); return r; }
/// long + BigInt
inline BigInt operator+(long x, const BigInt& y)
{ BigInt r; r.add(x, y); return r; }
/// BigInt + unsigned long
inline BigInt operator+(const BigInt& x, unsigned long y)
{ BigInt r; r.add(x, y); return r; }
/// unsigned long + BigInt
inline BigInt operator+(unsigned long x, const BigInt& y)
{ BigInt r; r.add(x, y); return r; }
/// BigInt + double
inline BigInt operator+(const BigInt& x, double y)
{ BigInt r; r.add(x, y); return r; }
/// double + BigInt
inline BigInt operator+(double x, const BigInt& y)
{ BigInt r; r.add(x, y); return r; }

/// BigInt - BigInt
inline BigInt operator-(const BigInt& x, const BigInt& y)
{ BigInt r; r.sub(x, y); return r; }
/// BigInt - int
inline BigInt operator-(const BigInt& x, int y)
{ BigInt r; r.sub(x, y); return r; }
/// int - BigInt
inline BigInt operator-(int x, const BigInt& y)
{ BigInt r; r.sub(x, y); return r; }
/// BigInt - unsigned int
inline BigInt operator-(const BigInt& x, unsigned int y)
{ BigInt r; r.sub(x, y); return r; }
/// unsigned int - BigInt
inline BigInt operator-(unsigned int x, const BigInt& y)
{ BigInt r; r.sub(x, y); return r; }
/// BigInt - long
inline BigInt operator-(const BigInt& x, long y)
{ BigInt r; r.sub(x, y); return r; }
/// long - BigInt
inline BigInt operator-(long x, const BigInt& y)
{ BigInt r; r.sub(x, y); return r; }
/// BigInt - unsigned long
inline BigInt operator-(const BigInt& x, unsigned long y)
{ BigInt r; r.sub(x, y); return r; }
/// unsigned long - BigInt
inline BigInt operator-(unsigned long x, const BigInt& y)
{ BigInt r; r.sub(x, y); return r; }
/// BigInt - double
inline BigInt operator-(const BigInt& x, double y)
{ BigInt r; r.sub(x, y); return r; }
/// double - BigInt
inline BigInt operator-(double x, const BigInt& y)
{ BigInt r; r.sub(x, y); return r; }

/// BigInt * BigInt
inline BigInt operator*(const BigInt& x, const BigInt& y)
{ BigInt r; r.mul(x, y); return r; }
/// BigInt * int
inline BigInt operator*(const BigInt& x, int y)
{ BigInt r; r.mul(x, y); return r; }
/// int * BigInt
inline BigInt operator*(int x, const BigInt& y)
{ BigInt r; r.mul(x, y); return r; }
/// BigInt * unsigned int
inline BigInt operator*(const BigInt& x, unsigned int y)
{ BigInt r; r.mul(x, y); return r; }
/// unsigned int * BigInt
inline BigInt operator*(unsigned int x, const BigInt& y)
{ BigInt r; r.mul(x, y); return r; }
/// BigInt * long
inline BigInt operator*(const BigInt& x, long y)
{ BigInt r; r.mul(x, y); return r; }
/// long * BigInt
inline BigInt operator*(long x, const BigInt& y)
{ BigInt r; r.mul(x, y); return r; }
/// BigInt * unsigned long
inline BigInt operator*(const BigInt& x, unsigned long y)
{ BigInt r; r.mul(x, y); return r; }
/// unsigned long * BigInt
inline BigInt operator*(unsigned long x, const BigInt& y)
{ BigInt r; r.mul(x, y); return r; }
/// BigInt * double
inline BigInt operator*(const BigInt& x, double y)
{ BigInt r; r.mul(x, y); return r; }
/// double * BigInt
inline BigInt operator*(double x, const BigInt& y)
{ BigInt r; r.mul(x, y); return r; }

/// BigInt / BigInt
inline BigInt operator/(const BigInt& x, const BigInt& y)
{ BigInt r; r.div(x, y); return r; }
/// BigInt / int
inline BigInt operator/(const BigInt& x, int y)
{ BigInt r; r.div(x, y); return r; }
/// BigInt / unsigned int
inline BigInt operator/(const BigInt& x, unsigned int y)
{ BigInt r; r.div(x, y); return r; }
/// BigInt / long
inline BigInt operator/(const BigInt& x, long y)
{ BigInt r; r.div(x, y); return r; }
/// BigInt / unsigned long
inline BigInt operator/(const BigInt& x, unsigned long y)
{ BigInt r; r.div(x, y); return r; }
/// BigInt / double
inline BigInt operator/(const BigInt& x, double y)
{ BigInt r; r.div(x, y); return r; }

/// BigInt % BigInt
inline BigInt operator%(const BigInt& x, const BigInt& y)
{ BigInt r; r.mod(x, y); return r; }
/// BigInt % int
inline BigInt operator%(const BigInt& x, int y)
{ BigInt r; r.mod(x, y); return r; }
/// BigInt % unsigned int
inline BigInt operator%(const BigInt& x, unsigned int y)
{ BigInt r; r.mod(x, y); return r; }
/// BigInt % long
inline BigInt operator%(const BigInt& x, long y)
{ BigInt r; r.mod(x, y); return r; }
/// BigInt % unsigned long
inline BigInt operator%(const BigInt& x, unsigned long y)
{ BigInt r; r.mod(x, y); return r; }
/// BigInt % double
inline BigInt operator%(const BigInt& x, double y)
{ BigInt r; r.mod(x, y); return r; }

/// BigInt & BigInt
inline BigInt operator&(const BigInt& x, const BigInt& y)
{ BigInt r; r.logical_and(x, y); return r; }
/// BigInt | BigInt
inline BigInt operator|(const BigInt& x, const BigInt& y)
{ BigInt r; r.logical_ior(x, y); return r; }
/// BigInt ^ BigInt
inline BigInt operator^(const BigInt& x, const BigInt& y)
{ BigInt r; r.logical_xor(x, y); return r; }

/// BigInt << int
inline BigInt operator<<(const BigInt& x, int y)
{ BigInt r; r.mul_2exp(x, y); return r; }
/// BigInt << unsigned int
inline BigInt operator<<(const BigInt& x, unsigned int y)
{ BigInt r; r.mul_2exp(x, y); return r; }
/// BigInt << long
inline BigInt operator<<(const BigInt& x, long y)
{ BigInt r; r.mul_2exp(x, y); return r; }
/// BigInt << unsigned long
inline BigInt operator<<(const BigInt& x, unsigned long y)
{ BigInt r; r.mul_2exp(x, y); return r; }
/// BigInt >> int
inline BigInt operator>>(const BigInt& x, int y)
{ BigInt r; r.div_2exp(x, y); return r; }
/// BigInt >> unsigned int
inline BigInt operator>>(const BigInt& x, unsigned int y)
{ BigInt r; r.div_2exp(x, y); return r; }
/// BigInt >> long
inline BigInt operator>>(const BigInt& x, long y)
{ BigInt r; r.div_2exp(x, y); return r; }
/// BigInt >> unsigned long
inline BigInt operator>>(const BigInt& x, unsigned long y)
{ BigInt r; r.div_2exp(x, y); return r; }
//@}

/// \addtogroup BigIntComparisonOperators
//@{
/// BigInt == BigInt
inline bool operator==(const BigInt& x, const BigInt& y)
{ return x.cmp(y) == 0; }
/// BigInt == int
inline bool operator==(const BigInt& x, int y)
{ return x.cmp(y) == 0; }
/// int == BigInt
inline bool operator==(int x, const BigInt& y)
{ return y.cmp(x) == 0; }
/// BigInt == unsigned int
inline bool operator==(const BigInt& x, unsigned int y)
{ return x.cmp(y) == 0; }
/// unsigned int == BigInt
inline bool operator==(unsigned int x, const BigInt& y)
{ return y.cmp(x) == 0; }
/// BigInt == long
inline bool operator==(const BigInt& x, long y)
{ return x.cmp(y) == 0; }
/// long == BigInt
inline bool operator==(long x, const BigInt& y)
{ return y.cmp(x) == 0; }
/// BigInt == unsigned long
inline bool operator==(const BigInt& x, unsigned long y)
{ return x.cmp(y) == 0; }
/// unsigned long == BigInt
inline bool operator==(unsigned long x, const BigInt& y)
{ return y.cmp(x) == 0; }
/// BigInt == double
inline bool operator==(const BigInt& x, double y)
{ return x.cmp(y) == 0; }
/// double == BigInt
inline bool operator==(double x, const BigInt& y)
{ return y.cmp(x) == 0; }

/// BigInt != BigInt
inline bool operator!=(const BigInt& x, const BigInt& y)
{ return x.cmp(y) != 0; }
/// BigInt != int
inline bool operator!=(const BigInt& x, int y)
{ return x.cmp(y) != 0; }
/// int != BigInt
inline bool operator!=(int x, const BigInt& y)
{ return y.cmp(x) != 0; }
/// BigInt != unsigned int
inline bool operator!=(const BigInt& x, unsigned int y)
{ return x.cmp(y) != 0; }
/// unsigned int != BigInt
inline bool operator!=(unsigned int x, const BigInt& y)
{ return y.cmp(x) != 0; }
/// BigInt != long
inline bool operator!=(const BigInt& x, long y)
{ return x.cmp(y) != 0; }
/// long != BigInt
inline bool operator!=(long x, const BigInt& y)
{ return y.cmp(x) != 0; }
/// BigInt != unsigned long
inline bool operator!=(const BigInt& x, unsigned long y)
{ return x.cmp(y) != 0; }
/// unsigned long != BigInt
inline bool operator!=(unsigned long x, const BigInt& y)
{ return y.cmp(x) != 0; }
/// BigInt != double
inline bool operator!=(const BigInt& x, double y)
{ return x.cmp(y) != 0; }
/// double != BigInt
inline bool operator!=(double x, const BigInt& y)
{ return y.cmp(x) != 0; }

/// BigInt >= BigInt
inline bool operator>=(const BigInt& x, const BigInt& y)
{ return x.cmp(y) >= 0; }
/// BigInt >= int
inline bool operator>=(const BigInt& x, int y)
{ return x.cmp(y) >= 0; }
/// int >= BigInt
inline bool operator>=(int x, const BigInt& y)
{ return y.cmp(x) <= 0; }
/// BigInt >= unsigned int
inline bool operator>=(const BigInt& x, unsigned int y)
{ return x.cmp(y) >= 0; }
/// unsigned int >= BigInt
inline bool operator>=(unsigned int x, const BigInt& y)
{ return y.cmp(x) <= 0; }
/// BigInt >= long
inline bool operator>=(const BigInt& x, long y)
{ return x.cmp(y) >= 0; }
/// long >= BigInt
inline bool operator>=(long x, const BigInt& y)
{ return y.cmp(x) <= 0; }
/// BigInt >= unsigned long
inline bool operator>=(const BigInt& x, unsigned long y)
{ return x.cmp(y) >= 0; }
/// unsigned long >= BigInt
inline bool operator>=(unsigned long x, const BigInt& y)
{ return y.cmp(x) <= 0; }
/// BigInt >= double
inline bool operator>=(const BigInt& x, double y)
{ return x.cmp(y) >= 0; }
/// double >= BigInt
inline bool operator>=(double x, const BigInt& y)
{ return y.cmp(x) <= 0; }

/// BigInt <= BigInt
inline bool operator<=(const BigInt& x, const BigInt& y)
{ return x.cmp(y) <= 0; }
/// BigInt <= int
inline bool operator<=(const BigInt& x, int y)
{ return x.cmp(y) <= 0; }
/// int <= BigInt
inline bool operator<=(int x, const BigInt& y)
{ return y.cmp(x) >= 0; }
/// BigInt <= unsigned int
inline bool operator<=(const BigInt& x, unsigned int y)
{ return x.cmp(y) <= 0; }
/// unsigned int <= BigInt
inline bool operator<=(unsigned int x, const BigInt& y)
{ return y.cmp(x) >= 0; }
/// BigInt <= long
inline bool operator<=(const BigInt& x, long y)
{ return x.cmp(y) <= 0; }
/// long <= BigInt
inline bool operator<=(long x, const BigInt& y)
{ return y.cmp(x) >= 0; }
/// BigInt <= unsigned long
inline bool operator<=(const BigInt& x, unsigned long y)
{ return x.cmp(y) <= 0; }
/// unsigned long <= BigInt
inline bool operator<=(unsigned long x, const BigInt& y)
{ return y.cmp(x) >= 0; }
/// BigInt <= double
inline bool operator<=(const BigInt& x, double y)
{ return x.cmp(y) <= 0; }
/// double <= BigInt
inline bool operator<=(double x, const BigInt& y)
{ return y.cmp(x) >= 0; }

/// BigInt > BigInt
inline bool operator>(const BigInt& x, const BigInt& y)
{ return x.cmp(y) > 0; }
/// BigInt > int
inline bool operator>(const BigInt& x, int y)
{ return x.cmp(y) > 0; }
/// int > BigInt
inline bool operator>(int x, const BigInt& y)
{ return y.cmp(x) < 0; }
/// BigInt > unsigned int
inline bool operator>(const BigInt& x, unsigned int y)
{ return x.cmp(y) > 0; }
/// unsigned int > BigInt
inline bool operator>(unsigned int x, const BigInt& y)
{ return y.cmp(x) < 0; }
/// BigInt > long
inline bool operator>(const BigInt& x, long y)
{ return x.cmp(y) > 0; }
/// long > BigInt
inline bool operator>(long x, const BigInt& y)
{ return y.cmp(x) < 0; }
/// BigInt > unsigned long
inline bool operator>(const BigInt& x, unsigned long y)
{ return x.cmp(y) > 0; }
/// unsigned long > BigInt
inline bool operator>(unsigned long x, const BigInt& y)
{ return y.cmp(x) < 0; }
/// BigInt > double
inline bool operator>(const BigInt& x, double y)
{ return x.cmp(y) > 0; }
/// double > BigInt
inline bool operator>(double x, const BigInt& y)
{ return y.cmp(x) < 0; }

/// BigInt < BigInt
inline bool operator<(const BigInt& x, const BigInt& y)
{ return x.cmp(y) < 0; }
/// BigInt < int
inline bool operator<(const BigInt& x, int y)
{ return x.cmp(y) < 0; }
/// int < BigInt
inline bool operator<(int x, const BigInt& y)
{ return y.cmp(x) > 0; }
/// BigInt < unsigned int
inline bool operator<(const BigInt& x, unsigned int y)
{ return x.cmp(y) < 0; }
/// unsigned int < BigInt
inline bool operator<(unsigned int x, const BigInt& y)
{ return y.cmp(x) > 0; }
/// BigInt < long
inline bool operator<(const BigInt& x, long y)
{ return x.cmp(y) < 0; }
/// long < BigInt
inline bool operator<(long x, const BigInt& y)
{ return y.cmp(x) > 0; }
/// BigInt < unsigned long
inline bool operator<(const BigInt& x, unsigned long y)
{ return x.cmp(y) < 0; }
/// unsigned long < BigInt
inline bool operator<(unsigned long x, const BigInt& y)
{ return y.cmp(x) > 0; }
/// BigInt < double
inline bool operator<(const BigInt& x, double y)
{ return x.cmp(y) < 0; }
/// double < BigInt
inline bool operator<(double x, const BigInt& y)
{ return y.cmp(x) > 0; }
//@}

/// \addtogroup BigIntIostreamOperators
//@{
/// istream operator for <tt>BigInt</tt>
inline std::istream& operator>>(std::istream& is, BigInt& x)
{ return is >> x.mp(); }
/// ostream operator for <tt>BigInt</tt>
inline std::ostream& operator<<(std::ostream& os, const BigInt& x)
{ return os << x.get_str(); }
//@}

/// \addtogroup BigIntGlobalFunctions
//@{
/// read from file
void readFromFile(BigInt& z, std::istream& in, long maxLength = 0);
/// write to file
void writeToFile(const BigInt& z, std::ostream& in, int base=10, int width=80);
/// return a gmp_randstate_t structure
inline gmp_randstate_t* getRandstate() {
  static gmp_randstate_t rstate;
  static bool initialized = false;
  if (!initialized) {
    gmp_randinit(rstate, GMP_RAND_ALG_DEFAULT, 32L);
    initialized = true;
  }
  return &rstate;
}
/// randomize function
inline BigInt randomize(const BigInt& a)
{ BigInt r; mpz_urandomm(r.mp(), *getRandstate(), a.mp()); return r; }
//@}

#ifdef CORE_OLDNAMES 
/// \addtogroup BigIntBackCompatiableFunctions
//@{
/// comparison
inline int cmp(const BigInt& x, const BigInt& y) { return x.cmp(y); }
/// sign 
inline int sign(const BigInt& a) { return a.sgn(); }
/// abs
inline BigInt abs(const BigInt& a) { BigInt r; r.abs(a); return r; }
/// neg
inline BigInt neg(const BigInt& a) { BigInt r; r.neg(a); return r; }
/// negate
inline void negate(BigInt& a) { a.neg(a); }
/// cmpabs
inline int cmpabs(const BigInt& a, const BigInt& b) { return a.cmpabs(b); }
/// longValue
inline long longValue(const BigInt& a) { return a.longValue(); }
/// ulongValue
inline unsigned long ulongValue(const BigInt& a) { return a.ulongValue(); }
/// doubleValue
inline double doubleValue(const BigInt& a) { return a.doubleValue(); }
// isEven
inline bool isEven(const BigInt& z) { return z.is_even(); }
/// isOdd
inline bool isOdd(const BigInt& z) { return z.is_odd(); }
/// get exponent of power 2
inline unsigned long getBinExpo(const BigInt& z) { return z.get_2exp(); }
/// get exponent of power k
inline void getKaryExpo(const BigInt& z, BigInt& m, int& e, unsigned long k) 
{ e = z.get_k_exp(m, k); }
/// divisible(x,y) = "x | y"
inline bool isDivisible(const BigInt& x, const BigInt& y)
{ return x.is_divisible(y) != 0; }
/// exact div
inline void divexact(BigInt& z, const BigInt& x, const BigInt& y)
{ z.divexact(x, y); }
/// exact div
inline BigInt div_exact(const BigInt& x, const BigInt& y)
{ BigInt z; divexact(z, x, y); return z; }
/// gcd
inline BigInt gcd(const BigInt& a, const BigInt& b)
{ BigInt r; r.gcd(a, b); return r; }
/// div_rem
inline void div_rem(BigInt& q, BigInt& r, const BigInt& a, const BigInt& b)
{ q.divrem(r, a, b); } 
/// power
inline void power(BigInt& c, const BigInt& a, unsigned long ul) 
{ c.pow(a, ul); }
// pow
inline BigInt pow(const BigInt& a, unsigned long ui) 
{ BigInt r; r.pow(a, ui); return r; }
// bit length
inline int bitLength(const BigInt& a) 
{ return a.sizeinbase(2); }
/// floorLg -- floor of log_2(a)
/** Convention: a=0, floorLg(a) returns -1. (!!changed: return 0) 
 *  This makes sense for integer a.
 */
inline long floorLg(const BigInt& a) { return a.floorlg(); }
/// ceilLg -- ceiling of log_2(a) where a=BigInt, int or long
/** Convention: a=0, ceilLg(a) returns -1. (!!changed: return 0 now)
 *  This makes sense for integer a.
 */
inline long ceilLg(const BigInt& a) { return a.ceillg(); }
//@}
#endif

#ifdef CORE_END_NAMESPACE
CORE_END_NAMESPACE
#endif

#endif /*__BIGINT_H__*/
