/****************************************************************************
 * BigRat.h -- A C++ wrapper class for GMP mpq
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
 * $Id: BigRat.h,v 1.1.1.1 2006-02-09 09:18:04 exact Exp $
 ***************************************************************************/
#ifndef __BIGRAT_H__
#define __BIGRAT_H__

#include <CORE/BigInt.h>

#define mpq_init_set(z, x)            (mpq_init(z), mpq_set(z, x))
#define mpq_init_set_si(z, x)         (mpq_init(z), mpq_set_si(z, x, 1))
#define mpq_init_set_ui(z, x)         (mpq_init(z), mpq_set_ui(z, x, 1))
#define mpq_init_set_d(z, x)          (mpq_init(z), mpq_set_d(z, x))
#define mpq_init_set_z(z, x)          (mpq_init(z), mpq_set_z(z, x))
#define mpq_init_set_str(z, s, base)  (mpq_init(z), mpq_set_str(z, s, base), mpq_canonicalize(z))

#ifdef CORE_BEGIN_NAMESPACE
CORE_BEGIN_NAMESPACE
#endif

/// \class BigRat BigRat.h
/// \brief BigRat is a wrapper class of <tt>mpq</tt> in GMP
class BigRat {
private:
  mpq_t m_mp;
public:
  // internal structure accessors
  const mpq_t& mp() const { return m_mp; }
  mpq_t& mp() { return m_mp; }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  BigRat()
  { mpq_init(m_mp); }
  /// copy constructor
  BigRat(const BigRat& rhs)
  { mpq_init_set(m_mp, rhs.m_mp); }

  /// constructor for <tt>char</tt>
  BigRat(char i)
  { mpq_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned char</tt>
  BigRat(unsigned char i)
  { mpq_init_set_ui(m_mp, i); }

  /// constructor for <tt>short</tt>
  BigRat(short i)
  { mpq_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned short</tt>
  BigRat(unsigned short i)
  { mpq_init_set_ui(m_mp, i); }

  /// constructor for <tt>int</tt>
  BigRat(int i)
  { mpq_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned int</tt>
  BigRat(unsigned int i)
  { mpq_init_set_ui(m_mp, i); }

  /// constructor for <tt>long</tt>
  BigRat(long i)
  { mpq_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned long</tt>
  BigRat(unsigned long i)
  { mpq_init_set_ui(m_mp, i); }

  /// constructor for <tt>float</tt>
  BigRat(float i)
  { mpq_init_set_d(m_mp, i); }
  /// constructor for <tt>double</tt>
  BigRat(double i)
  { mpq_init_set_d(m_mp, i); }

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit BigRat(const char* str)
  { mpq_init_set_str(m_mp, str, 0); }
  /// constructor for <tt>std::string</tt> (no implicit conversion)
  explicit BigRat(const std::string& str)
  { mpq_init_set_str(m_mp, str.c_str(), 0); }

  /// constructor for <tt>BigInt</tt>
  BigRat(const BigInt& z)
  { mpq_init_set_z(m_mp, z.mp()); } 

  /// constructor for <tt>BigInt, BigInt</tt>
  BigRat(const BigInt& num, const BigInt& den) {
    mpq_init(m_mp); 
    mpz_set(mpq_numref(m_mp), num.mp());
    mpz_set(mpq_denref(m_mp), den.mp());
    mpq_canonicalize(m_mp);
  }
  /// destructor
  ~BigRat()
  { mpq_clear(m_mp); }
  //@}
  
public:
  /// \name assignment functions
  //@{
  /// assignment function for <tt>BigRat</tt>
  void set(const BigRat& rhs)
  { mpq_set(m_mp, rhs.m_mp); }
  /// assignment function for <tt>char</tt>
  void set(char i, unsigned long den = 1)
  { mpq_set_si(m_mp, i, den); }
  /// assignment function for <tt>unsigned char</tt>
  void set(unsigned char i, unsigned long den = 1)
  { mpq_set_ui(m_mp, i, den); }
  /// assignment function for <tt>short</tt>
  void set(short i, unsigned long den = 1)
  { mpq_set_si(m_mp, i, den); }
  /// assignment function for <tt>unsigned short</tt>
  void set(unsigned short i, unsigned long den = 1)
  { mpq_set_ui(m_mp, i, den); }
  /// assignment function for <tt>int</tt>
  void set(int i, unsigned long den = 1)
  { mpq_set_si(m_mp, i, den); }
  /// assignment function for <tt>unsigned int</tt>
  void set(unsigned int i, unsigned long den = 1)
  { mpq_set_ui(m_mp, i, den); }
  /// assignment function for <tt>long</tt>
  void set(long i, unsigned long den = 1)
  { mpq_set_si(m_mp, i, den); }
  /// assignment function for <tt>unsigned long</tt>
  void set(unsigned long i, unsigned long den = 1)
  { mpq_set_ui(m_mp, i, den); }
  /// assignment function for <tt>float</tt>
  void set(float i)
  { mpq_set_d(m_mp, i); }
  /// assignment function for <tt>double</tt>
  void set(double i)
  { mpq_set_d(m_mp, i); }
  /// assignment function for <tt>char*</tt>
  int set(const char* str, int base = 0)
  { return mpq_set_str(m_mp, str, base); }
  /// assignment function for <tt>std::string</tt>
  int set(const std::string& str, int base = 0)
  { return mpq_set_str(m_mp, str.c_str(), base); }
  /// assignment function for <tt>BigInt</tt>
  void set(const BigInt& x)
  { mpq_set_z(m_mp, x.mp()); }
  /// assignment function for <tt>BigInt, BigInt</tt>
  void set(const BigInt& num, const BigInt& den) {
    mpz_set(mpq_numref(m_mp), num.mp());
    mpz_set(mpq_denref(m_mp), den.mp());
    mpq_canonicalize(m_mp);
  }
  //@}
  
  /// \name arithmetic functions
  //@{
  /// addition for <tt>BigRat+BigRat</tt>
  void add(const BigRat& x, const BigRat& y)
  { mpq_add(m_mp, x.m_mp, y.m_mp); }
  /// subtraction for <tt>BigRat-BigRat</tt>
  void sub(const BigRat& x, const BigRat& y)
  { mpq_sub(m_mp, x.m_mp, y.m_mp); }
  /// multiplication for <tt>BigRat*BigRat</tt>
  void mul(const BigRat& x, const BigRat& y)
  { mpq_mul(m_mp, x.m_mp, y.m_mp); }
  /// division for <tt>BigRat/BigRat</tt>
  void div(const BigRat& x, const BigRat& y)
  { mpq_div(m_mp, x.m_mp, y.m_mp); }
  /// negation
  void neg(const BigRat& x)
  { mpq_neg(m_mp, x.m_mp); }
  /// absolute value
  void abs(const BigRat& x)
  { mpq_abs(m_mp, x.m_mp); }
  //@}
  
  /// \name shift functions
  //@{
  /// left shift
  void mul_2exp(const BigRat& x, unsigned long y)
  { mpq_mul_2exp(m_mp, x.m_mp, y); }
  /// right shift
  void div_2exp(const BigRat& x, unsigned long y)
  { mpq_div_2exp(m_mp, x.m_mp, y); }
  //@}
  
  /// \name comparison functions
  //@{ 
  /// compare with <tt>BigInt</tt>
  int cmp(const BigRat& x) const
  { return mpq_cmp(m_mp, x.m_mp); }
  /// compare with <tt>char</tt>
  int cmp(char x, unsigned long den) const
  { return mpq_cmp_si(m_mp, x, den); }
  /// compare with <tt>unsigned char</tt>
  int cmp(unsigned char x, unsigned long den) const
  { return mpq_cmp_ui(m_mp, x, den); }
  /// compare with <tt>short</tt>
  int cmp(short x, unsigned long den) const
  { return mpq_cmp_si(m_mp, x, den); }
  /// compare with <tt>unsigned short</tt>
  int cmp(unsigned short x, unsigned long den) const
  { return mpq_cmp_ui(m_mp, x, den); }
  /// compare with <tt>int</tt>
  int cmp(int x, unsigned long den) const
  { return mpq_cmp_si(m_mp, x, den); }
  /// compare with <tt>unsigned int</tt>
  int cmp(unsigned int x, unsigned long den) const
  { return mpq_cmp_ui(m_mp, x, den); }
  /// compare with <tt>long</tt>
  int cmp(long x, unsigned long den) const
  { return mpq_cmp_si(m_mp, x, den); }
  /// compare with <tt>unsigned long</tt>
  int cmp(unsigned long x, unsigned den) const
  { return mpq_cmp_ui(m_mp, x, den); }
  /// return non-zero if equals (faster than cmp)
  int equal(const BigRat& x) const
  { return mpq_equal(m_mp, x.m_mp); }
  //@}
  
  /// \name conversion functions
  //@{
  /// return double value
  double get_d() const
  { return mpq_get_d(m_mp); }
  /// return signed long value
  long get_si() const
  { return static_cast<long>(get_d()); }
  /// return unsigned long value
  unsigned long get_ui() const
  { return static_cast<unsigned long>(get_d()); }
  /// return the string representation
  std::string get_str(int base = 10) const {
    int len = mpz_sizeinbase(mpq_numref(m_mp), base)
            + mpz_sizeinbase(mpq_denref(m_mp), base) + 3;
    _gmp_alloc_cstr tmp(len);
    return std::string(mpq_get_str(tmp.str, base, m_mp));
  }
  /// return BigInt value
  BigInt get_z() const
  { BigInt r; mpz_div(r.mp(), mpq_numref(m_mp), mpq_denref(m_mp)); return r; }
  //@}

  /// \name miscellaneous functions
  //@{
  /// swap function
  void swap(BigRat& other)
  { mpq_swap(m_mp, other.m_mp); }
  /// canonicalization
  void canonicalize() 
  { mpq_canonicalize(m_mp); }
  /// return numerator
  BigInt numerator() const
  { return BigInt(mpq_numref(m_mp)); }
  /// return numerator
  BigInt num() const
  { return BigInt(mpq_numref(m_mp)); }
  /// return denominator
  BigInt denominator() const
  { return BigInt(mpq_denref(m_mp)); }
  /// return denominator
  BigInt den() const
  { return BigInt(mpq_denref(m_mp)); }
  /// return sign
  int sgn() const
  { return mpq_sgn(m_mp); }
  /// return upper bound of MSB
  long uMSB() const
  { return num().ceillg() - den().floorlg(); }
  /// return lower bound of MSB
  long lMSB() const
  { return num().floorlg() - den().ceillg(); }
  //@}

public: // C++ operators
  /// \name unary, increment, decrement operators
  //@{
  /// unary plus operator
  BigRat operator+() const
  { return BigRat(*this); }
  /// unary negation operator
  BigRat operator-() const
  { BigRat r; r.neg(*this); return r; }
  /// prefix increment operator
  BigRat& operator++()
  { add(*this, 1); return *this; }
  /// postfix increment operator
  BigRat operator++(int)
  { BigRat r(*this); ++(*this); return r; }
  /// prefix decrement operator
  BigRat& operator--()
  { sub(*this, 1); return *this; }
  /// postfix decrement operator
  BigRat operator--(int)
  { BigRat r(*this); --(*this); return r; }
  //@}

  /// \name assignment and compound assignment operators
  //@{
  /// assignment operator for <tt>BigInt</tt>
  BigRat& operator=(const BigRat& rhs)
  { set(rhs); return *this; }
  /// generic assignment operator for <tt>T</tt>
  template <typename T>
  BigRat& operator=(const T& rhs)
  { set(rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  BigRat& operator+=(const BigRat& rhs)
  { add(*this, rhs); return *this; }
  /// compound assignment operator <tt>-=</tt>
  BigRat& operator-=(const BigRat& rhs)
  { sub(*this, rhs); return *this; }
  /// compound assignment operator <tt>*=</tt>
  BigRat& operator*=(const BigRat& rhs)
  { mul(*this, rhs); return *this; }
  /// compound assignment operator <tt>/=</tt>
  BigRat& operator/=(const BigRat& rhs)
  { div(*this, rhs); return *this; }
  /// compound assignment operator <tt><<=</tt>
  BigRat& operator<<=(unsigned long ul)
  { mul_2exp(*this, ul); return *this; }
  /// compound assignment operator <tt>>>=</tt>
  BigRat& operator>>=(unsigned long ul) 
  { div_2exp(*this, ul); return *this; }
  //@}

#ifdef CORE_OLDNAMES
  /// \name back-compatiable functions
  //@{
  /// Has Exact Division
  static bool hasExactDivision() { return true; }
  /// set value from <tt>const char*</tt>
  int set_str(const char* s, int base = 0) { return set(s, base); }
  /// div2
  BigRat div2() const { BigRat r; r.div_2exp(*this, 1U); return r; }
  /// intValue
  int intValue() const { return static_cast<int>(doubleValue()); }
  /// longValue
  long longValue() const { return static_cast<long>(doubleValue()); }
  /// doubleValue
  double doubleValue() const { return get_d(); }
  /// BigIntValue
  BigInt BigIntValue() const { return get_z(); }
  //@}
#endif
};

/// \addtogroup BigRatArithmeticOperators
//@{
/// BigRat + BigRat
inline BigRat operator+(const BigRat& x, const BigRat& y)
{ BigRat r; r.add(x, y); return r; }
/// BigRat - BigRat
inline BigRat operator-(const BigRat& x, const BigRat& y)
{ BigRat r; r.sub(x, y); return r; }
/// BigRat * BigRat
inline BigRat operator*(const BigRat& x, const BigRat& y)
{ BigRat r; r.mul(x, y); return r; }
/// BigRat / BigRat
inline BigRat operator/(const BigRat& x, const BigRat& y)
{ BigRat r; r.div(x, y); return r; }
/// BigRat << unsigned long
inline BigRat operator<<(const BigRat& x, unsigned long y)
{ BigRat r; r.mul_2exp(x, y); return r; }
/// BigRat >> unsigned long
inline BigRat operator>>(const BigRat& x, unsigned long y)
{ BigRat r; r.div_2exp(x, y); return r; }
//@}

/// \addtogroup BigRatComparisonOperators
//@{
/// BigRat == BigRat (use equal instead of cmp will be fast)
inline bool operator==(const BigRat& x, const BigRat& y)
{ return x.equal(y) != 0; }
/// BigRat == T
template <typename T>
inline bool operator==(const BigRat& x, const T& y)
{ return x.cmp(y, 1UL) == 0; }
/// T == BigRat
template <typename T>
inline bool operator==(const T& x, const BigRat& y)
{ return y.cmp(x, 1UL) == 0; }

/// BigRat != BigRat (use equal instead of cmp will be fast)
inline bool operator!=(const BigRat& x, const BigRat& y)
{ return x.equal(y) == 0; }
/// BigRat != T
template <typename T>
inline bool operator!=(const BigRat& x, const T& y)
{ return x.cmp(y, 1UL) != 0; }
/// T != BigRat
template <typename T>
inline bool operator!=(const T& x, const BigRat& y)
{ return y.cmp(x, 1UL) != 0; }

/// BigRat >= BigRat
inline bool operator>=(const BigRat& x, const BigRat& y)
{ return x.cmp(y) >= 0; }
/// BigRat >= T
template <typename T>
inline bool operator>=(const BigRat& x, const T& y)
{ return x.cmp(y, 1UL) >= 0; }
/// T >= BigRat
template <typename T>
inline bool operator>=(const T& x, const BigRat& y)
{ return y.cmp(x, 1UL) <= 0; }

/// BigRat <= BigRat
inline bool operator<=(const BigRat& x, const BigRat& y)
{ return x.cmp(y) <= 0; }
/// BigRat <= T
template <typename T>
inline bool operator<=(const BigRat& x, const T& y)
{ return x.cmp(y, 1UL) <= 0; }
/// T <= BigRat
template <typename T>
inline bool operator<=(const T& x, const BigRat& y)
{ return y.cmp(x, 1UL) >= 0; }

/// BigRat > BigRat
inline bool operator>(const BigRat& x, const BigRat& y)
{ return x.cmp(y) > 0; }
/// BigRat > T
template <typename T>
inline bool operator>(const BigRat& x, const T& y)
{ return x.cmp(y, 1UL) > 0; }
/// T > BigRat
template <typename T>
inline bool operator>(const T& x, const BigRat& y)
{ return y.cmp(x, 1UL) < 0; }

/// BigRat < BigRat
inline bool operator<(const BigRat& x, const BigRat& y)
{ return x.cmp(y) < 0; }
/// BigRat < T
template <typename T>
inline bool operator<(const BigRat& x, const T& y)
{ return x.cmp(y, 1UL) < 0; }
/// T < BigRat
template <typename T>
inline bool operator<(const T& x, const BigRat& y)
{ return y.cmp(x, 1UL) > 0; }
//@}

/// \addtogroup BigRatIostreamOperators 
//@{
/// istream operator for <tt>BigRat</tt>
inline std::istream& operator>>(std::istream& is, BigRat& x)
{ return is >> x.mp(); }
/// ostream operator for <tt>BigRat</tt>
inline std::ostream& operator<<(std::ostream& os, const BigRat& x)
{ return os << x.get_str(); }
//@}

#ifdef CORE_OLDNAMES 
/// \addtogroup BigRatBackCompatiableFunctions
//@{
/// comparison
inline int cmp(const BigRat& x, const BigRat& y) { return x.cmp(y); }
/// sign
inline int sign(const BigRat& a) { return a.sgn(); }
/// abs
inline BigRat abs(const BigRat& a) { BigRat r; r.abs(a); return r; }
/// neg
inline BigRat neg(const BigRat& a) { BigRat r; r.neg(a); return r; }
/// div2
inline BigRat div2(const BigRat& a) { BigRat r(a); return r.div2(); }
/// numerator
inline BigInt numerator(const BigRat& a) { return a.numerator(); }
/// denominator
inline BigInt denominator(const BigRat& a) { return a.denominator(); }
/// longValue
inline long longValue(const BigRat& a) { return a.longValue(); }
/// doubleValue
inline double doubleValue(const BigRat& a) { return a.doubleValue(); }
/// return BigInt value
inline BigInt BigIntValue(const BigRat& a) { return a.BigIntValue(); }
/// exact division
inline BigRat div_exact(const BigRat& x, const BigRat& y) 
{ BigRat z; z.div(x, y); return z; }
/// gcd
/// \brief we may want replace this by the definition of gcd of a quotient 
///        field of a UFD [Yap's book, Chap.3]
inline BigRat gcd(const BigRat&, const BigRat&)
{ return BigRat(1);  }
/// is integer ?
inline bool isInteger(const BigRat& x) { return x.den() == 1; }
// Chee: 8/8/2004: need isDivisible to compile Polynomial<BigRat>
// A trivial implementation is to return true always. But this
// caused tPolyRat to fail.
// So we follow the definition of
// Expr::isDivisible(e1, e2) which checks if e1/e2 is an integer.  
/// is divisible ?
inline bool isDivisible(const BigRat& x, const BigRat& y) 
{ BigRat r; r.div(x, y); return isInteger(r); }
//@}
#endif

#ifdef CORE_END_NAMESPACE
CORE_END_NAMESPACE
#endif

#endif /*__BIGRAT_H__*/
