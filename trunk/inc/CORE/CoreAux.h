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
 * $Id: CoreAux.h,v 1.1.1.1 2006-02-09 09:18:04 exact Exp $
 ***************************************************************************/
#ifndef __COREAUX_H__
#define __COREAUX_H__

#ifdef CORE_BEGIN_NAMESPACE
CORE_BEGIN_NAMESPACE
#endif

// help inline functions for long
inline sign_t sgn(long v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline msb_t ceillg(long v)
{ return BigInt(v).ceillg(); }
inline msb_t floorlg(long v)
{ return BigInt(v).floorlg(); }

// help inline functions for unsigned long
inline sign_t sgn(unsigned long v)
{ return v==0 ? 0 : 1; }
inline msb_t ceillg(unsigned long v)
{ return BigInt(v).ceillg(); }
inline msb_t floorlg(unsigned long v)
{ return BigInt(v).floorlg(); }

// help inline functions for double
inline sign_t sgn(double v)
{ return v==0 ? 0 : (v>0 ? 1 : -1); }
inline msb_t ceillg(double v)
{ return ilogb(v)+1; }
inline msb_t floorlg(double v)
{ return ilogb(v); }

#ifdef CORE_OLDNAMES 
/// \addtogroup GlobalBackCompatiableFunctions
//@{
inline bool isDivisible(int x, int y) { return x % y == 0; }
inline bool isDivisible(long x, long y) { return x % y == 0; }
inline int div_exact(int x, int y) { return x/y; }
inline long div_exact(long x, long y) { return x/y; }
inline long ceilLg(long a) { return ceilLg(BigInt(a)); }
inline long ceilLg(int a) { return ceilLg(BigInt(a)); }
//@}
#endif

#ifdef CORE_END_NAMESPACE
CORE_END_NAMESPACE
#endif

#endif /*__COREAUX_H__*/
