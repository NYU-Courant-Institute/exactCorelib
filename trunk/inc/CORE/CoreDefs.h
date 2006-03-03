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
 * $Id: CoreDefs.h,v 1.3 2006-03-03 16:53:45 exact Exp $
 ***************************************************************************/
#ifndef __CORE_COREDEFS_H__
#define __CORE_COREDEFS_H__

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
#define LOG10_2 0.30102999566398120 /* upper approximation of log2/log10 */
  
inline unsigned long digits2bits(unsigned long digits)
{ return (unsigned long)(digits*LOG2_10); }

inline unsigned long bits2digits(unsigned long bits)
{ return (unsigned long)(bits*LOG10_2); }

#define CORE_INFTY LONG_MAX
#define CORE_posInfty LONG_MAX
#define CORE_negInfty LONG_MIN

extern long defAbsPrec;
extern long defRelPrec;

inline void setDefaultPrecision(long r, long a)
{ defRelPrec = r; defAbsPrec = a; }
inline void setDefaultRelPrecision(long r)
{ defRelPrec = r; }
inline void setDefaultAbsPrecision(long a)
{ defAbsPrec = a; }

CORE_END_NAMESPACE

#endif /*__CORE_COREDEFS_H__*/
