/****************************************************************************
 * CoreDefs.h -- Global parameters for the Core Library
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
 * $Id: CoreDefs.h,v 1.12 2006-12-03 18:52:06 exact Exp $
 ***************************************************************************/
#ifndef __CORE_COREDEFS_H__
#define __CORE_COREDEFS_H__
#include <climits>
#include <string>

CORE_BEGIN_NAMESPACE

#ifndef sign_t
typedef int sign_t;
#endif

#ifndef msb_t
typedef long msb_t;
#endif
  
#define MSB_MIN LONG_MIN
#define MSB_MAX LONG_MAX
  
#define LOG2_10 3.3219280948873626 /* upper approximation of log(10)/log(2) */
#define LOG10_2 0.30102999566398120 /* upper approximation of log(2)/log(10) */
  
inline unsigned long digits2bits(unsigned long digits)
{ return (unsigned long)((digits)*LOG2_10); }

inline unsigned long bits2digits(unsigned long bits)
{ return (unsigned long)(bits*LOG10_2); }

#define CORE_INFTY LONG_MAX
#define CORE_posInfty LONG_MAX
#define CORE_negInfty LONG_MIN

extern long defAbsPrec;
extern long defRelPrec;
extern long defInputDigits;
extern long defOutputDigits;
extern long defBFdivRelPrec;
extern long defBFradicalRelPrec;   // this replaces defBFsqrtAbsPrec in Core1
                                   // Should also be used for any rootOf()
extern unsigned long cutOffBound;  // arbitary cutoff for ABSOLUTE precision
extern unsigned long escapeBound;  // this is to "escape" in transcendental
                                   //  evaluation when we have no root bounds

extern void core_error(std::string msg, std::string file, int lineno, bool err);

/// This sets the global variable defRelPrec and defAbsPrec.
//  PROBLEM IS this is "composite precision" which we don't really
//  support in Core2.  So, one of these two must be CORE_INFTY.
//  But in Expr.h, we see that approx() is implemented so that
//     If defAbsPrec<CORE_INFTY, then we compute to defAbsPrec (absolute prec)
//     Else defAbsPrec=CORE_INFTY, we compute to defRelPrec (relative prec).
inline void setDefaultPrecision(long r, long a)
{ defRelPrec = r; defAbsPrec = a; }
inline void setDefaultRelPrecision(long r) {
  if (defRelPrec == CORE_INFTY)
    core_error("Relative Prec and Absolute Prec are both CORE_INFTY", __FILE__, __LINE__, false);
  defRelPrec = r; 
  defAbsPrec = CORE_INFTY;
}
inline void setDefaultAbsPrecision(long a) {
  if (defAbsPrec == CORE_INFTY)
    core_error("Relative Prec and Absolute Prec are both CORE_INFTY", __FILE__, __LINE__, false);
  defAbsPrec = a; 
  defRelPrec = CORE_INFTY;
}
inline void setDefaultComPrecision(long r, long a) {
  defAbsPrec = a;
  defRelPrec = r;
}

inline long getDefaultInputDigits()
{ return defInputDigits; }
inline void setDefaultInputDigits(long digits)
{ defInputDigits = digits; }

inline long get_def_input_digits()
{ return defInputDigits; }
inline bool is_infty(long l)
{ return l == CORE_INFTY; }

inline long getDefaultBFdivPrec()
{ return defBFdivRelPrec; }
inline void setDefaultBFdivPrec(long p)
{ defBFdivRelPrec= p; }

inline long getDefaultBFradicalPrec()
{ return defBFradicalRelPrec; }
inline void setDefaultBFradicalPrec(long p)
{ defBFradicalRelPrec= p; }

inline unsigned long get_cut_off_bound()
{ return cutOffBound; }
inline unsigned long set_cut_off_bound(unsigned long p) {
  unsigned long ret = cutOffBound;
  cutOffBound = p; 
  return ret;
}

inline unsigned long get_escape_bound()
{ return escapeBound; }
inline unsigned long set_escape_bound(unsigned long p) {
  unsigned long ret = escapeBound;
  escapeBound = p; 
  return ret;
}

CORE_END_NAMESPACE

#endif /*__CORE_COREDEFS_H__*/
