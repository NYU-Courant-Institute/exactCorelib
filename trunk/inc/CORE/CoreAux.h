/****************************************************************************
 * CoreAux.h -- Auxilliary functions for the Core Library
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
 * $Id: CoreAux.h,v 1.16 2007-04-02 20:23:44 exact Exp $
 ***************************************************************************/
#ifndef __CORE_COREAUX_H__
#define __CORE_COREAUX_H__

#include <cmath>
#include <iostream>

CORE_BEGIN_NAMESPACE

/// Writes out an error or warning message in the local file CORE_DIAGFILE
/** If last argument (err) is TRUE, then this is considered an error
 *  (not just warning).  In this case, the message is also printed in
 *  std::cerr, using std::perror().
 *  */
void core_error(std::string msg, std::string file, int lineno, bool err);

/// This is for debugging messages
inline void core_debug(std::string msg){
  std::cout << __FILE__ << "::" << __LINE__ << ": " << msg
            << std::endl;
}

// help inline functions for size_t
msb_t ceillg(size_t v);

// help inline functions for long
inline sign_t sgn(long v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline bool isDivisible(long x, long y)
{ return x % y == 0; }
inline long sign(long a)
{ return a==0 ? 0 : a > 0 ? 1 : -1; }
inline long abs(long x)
{ return (x>=0) ? x : (-x); }
inline long div_exact(long x, long y)
{ return x/y; }
msb_t ceillg(long v);
msb_t floorlg(long v);
inline msb_t ceilLg(long v)
{ return ceillg(v); }
inline msb_t floorLg(long v)
{ return floorlg(v); }
long gcd(long x, long y);

// help inline functions for int
inline sign_t sgn(int v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline bool isDivisible(int x, int y)
{ return x % y == 0; }
inline int sign(int v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline int abs(int x)
{ return (x>=0) ? x : (-x); }
inline int div_exact(int x, int y)
{ return x/y; }
inline msb_t ceillg(int v)
{ return ceillg(long(v)); }
inline msb_t floorlg(int v)
{ return floorlg(long(v)); }
inline msb_t ceilLg(int v)
{ return ceillg(v); }
inline msb_t floorLg(int v)
{ return floorlg(v); }
int gcd(int x, int y);
inline int max(int x, int y)
{ return x>=y ? x : y; }

// help inline functions for unsigned long
inline sign_t sgn(unsigned long v)
{ return v==0 ? 0 : 1; }
unsigned long gcd(unsigned long x, unsigned long y);
/* size_t and unsigned long are the same in some OS and different in others.
 * When they are different, the compiler does not complain.
 * When they are the same, the compiler will complain about
 * the following functions (ceillg and floorlg) because of duplicated definitions
 * So we decide to comment this out:
 * */
#if defined (gnu) || (cyg)
msb_t ceillg(unsigned long v);
msb_t floorlg(unsigned long v);
#endif


// help inline functions for double
inline sign_t sgn(double v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline msb_t ceillg(double v)
{ return ilogb(v)+1; }
inline msb_t floorlg(double v)
{ return ilogb(v); }

/// template function returns the absolute value
template <class T>
inline const T core_abs(const T& a) {
  return ((a < T(0)) ? -a : a);
}

template <class T>
inline const T core_max(const T& a, const T& b) {
  return ((a < b) ? b : a);
}

template <class T>
inline  void core_swap(T& a, T& b) {
  T tmp;
  tmp = a;
  a = b;
  b = tmp;
}

////////////////////////////////////////////////////
// Functions to check if two decimal strings are
// compatible.   Here is the definition:
//     For any decimal string SX, it has a nominal value X
//     and also an uncertainty EX.
//     E.g., SX= 1.234 then X=1.234 and EX=0.001
//     E.g., SX= 1200e-2 then X=12.00 and EX=0.01
//     E.g., SX= 12 then X=12 and EX=1
//
// The string SX then defines an interval [X-EX, X+EX].
// 
//     When we say two strings, SX and SY are compatible, we mean
//     that the corresponding intervals overlap.
//
//     To decide compatibility of SX and SY, we first compute
//     X, EX, Y, EY.  Wlog, let X <= Y.  Then
//     SX and SY are compatible iff
//           X + EX >= Y - EY.
//     REMARK: we could try to work with the stricter notion of X+EX > Y-EY
//     but that might cause trouble with some of our other implementations.
//
//
//     E.g., 1.200 is compatible with 1.19999 and 1.201
//     E.g., 1.234 is compatible with 1.235 and 1.233
//
////////////////////////////////////////////////////

/// Returns the uncertainty log_10(EX) in an output decimal string SX
inline int getUncertainty( std::string& strin) {
  int u = 0;  		// eventually we want to return u
  bool dot=false;	// have we seen decimal point yet?
  int j=0;		// position of most significant digit
  if ((strin[0] == '+')|| (strin[0] == '-')) j++;	// Takes care of sign
  while (strin[j] == '0') j++;   // Takes care of initial zeros: 00123.456

  for (size_t i = j; i < strin.size(); i++) {	
    if (strin[i] == 'e' || strin[i] == 'E') {  
      u += atoi(strin.substr(i+1,strin.size()-i).c_str());
      break;
    } 
    if (strin[i] == '.')
     dot = true;   	// found dot
    if (strin[i] >= '0' && strin[i] <= '9') {
      if (dot) u--;
    }// else error!
  }//for
  return u;
}  

// helper function for isCompatible
// this extract mantissa and exponent from decimal string
// i.e., if value is m 10^e, then return (m, e).
inline void getDigits(std::string& strin, std::string& strout, int& exponent) {
	// strin = string to be analyzed,
	// strout = string representing an integer (removing preceding 0's,
	// 	dot and exponents)
  exponent = 0;			// value of exponent
  bool dot = false;		// if decimal point is found
  int j = 0;			// position of most significant digit

  if ((strin[0] == '+')|| (strin[0] == '-')) {
    if (strin[0] == '-') strout += strin[0]; // +sign in strout cause trouble!!
    j++;			// Takes care of sign
  }
  while (strin[j] == '0') j++;    // Takes care of 00012.345 

  for (size_t i = j; i < strin.size(); i++) {	
    if (strin[i] == 'e' || strin[i] == 'E') {  
      exponent += atoi(strin.substr(i+1,strin.size()-i).c_str());
      break;
    } 
    if (strin[i] == '.')
     dot = true;		// found dot
    if (strin[i] >= '0' && strin[i] <= '9') {
      strout += strin[i];
      if (dot) exponent--;	// takes account of decimal point 
    }// else error!
  }//for
  if (strout.size() == 0) strout="0";
}  

/// Function to convert a decimal string into a BigRat
inline BigRat stringToBigRat(const char* pStrIn) {
  std::string strIn(pStrIn);
  std::string digitIn;
  int expIn;
  
  getDigits(strIn, digitIn, expIn);
  BigRat XIn(digitIn);
  BigInt powIn;
  powIn.pow(10, abs(expIn));
  if (expIn > 0)
    XIn *= powIn;
  else
    XIn /= powIn;
  return(XIn);
}

inline BigRat stringToBigRat(std::string & strIn) {
  std::string digitIn;
  int expIn;
  
  getDigits(strIn, digitIn, expIn);
  BigRat XIn(digitIn);
  BigInt powIn;
  powIn.pow(10, abs(expIn));
  if (expIn > 0)
    XIn *= powIn;
  else
    XIn /= powIn;
  return(XIn);
}

/// Function to check if two decimal string values are compatible
/// Every decimal string has an implicit uncertainty, and 2 strings are
/// compatible if they could be equal within this uncertainty.
inline bool isCompatible(std::string & strIn, std::string & strAns) {
  int uIn=getUncertainty(strIn);
  int uAns=getUncertainty(strAns);
  
  BigInt temp;
  temp.pow(10,abs(uIn));
  BigRat uncertaintyIn(temp);
  temp.pow(10,abs(uAns));
  BigRat uncertaintyAns(temp);
  if (uIn < 0)
    uncertaintyIn.inv();  
  if (uAns < 0)
    uncertaintyAns.inv();  

  BigRat XIn=stringToBigRat(strIn);
  BigRat XAns=stringToBigRat(strAns);

  if (XIn >= XAns) 
	  return (XIn - uncertaintyIn <= XAns + uncertaintyAns);
  else
	  return (XIn + uncertaintyIn >= XAns - uncertaintyAns);
}//isCompatible




#ifdef CORE_OLDNAMES 
/// \addtogroup GlobalBackCompatiableFunctions
//@{
inline long ceilLg(long a) { return ceilLg(BigInt(a)); }
inline long ceilLg(int a) { return ceilLg(BigInt(a)); }
//@}
#endif

CORE_END_NAMESPACE

#endif /*__CORE_COREAUX_H__*/
