/****************************************************************************
 * BigFloat.inl -- Inline functions for BigFloat
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
 * $Id: BigFloat.inl,v 1.4 2006-03-03 16:53:45 exact Exp $
 ***************************************************************************/

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

/// \addtogroup BigFloatComparisonOperators
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
inline std::ostream& operator<<(std::ostream& os, const BigFloat& x) {
  return os << mpfr2str(x.mp(), get_output_precision(), get_output_base(),
         get_output_fmt(), get_output_rounding_mode(), get_output_showpoint(),
         get_output_showpos(), get_output_uppercase());
}
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
//inline BigFloat pow(const BigFloat& a, unsigned long p) 
//{ BigFloat r; r.pow(a, p); return r; }
//@}
#endif

