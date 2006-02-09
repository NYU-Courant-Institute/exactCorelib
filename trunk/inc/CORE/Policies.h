/****************************************************************************
 * Policies.h -- Computation policies for BigFloat
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
 * $Id: Policies.h,v 1.1.1.1 2006-02-09 09:18:05 exact Exp $
 ***************************************************************************/
#ifndef __POLICIES_H__
#define __POLICIES_H__

#ifdef CORE_BEGIN_NAMESPACE
CORE_BEGIN_NAMESPACE
#endif

// raw version (user need to set precision for result before call)
template <typename T, typename TL = T, typename TR = T>
struct RawArithmeticPolicy {
  static bool set(T& z, const TL& x, prec_t, rnd_t rnd = MPFR_RND)
  { return z.set(x, rnd) == 0; }
  static bool set(T& z, const char* x, int b, prec_t, rnd_t rnd = MPFR_RND)
  { return z.set(x, b, rnd) == 0; }
  static bool set_2exp(T& z, long x, exp_t e, prec_t, rnd_t rnd)
  { return z.set_2exp(x, e, rnd) == 0; }
  static bool set_2exp(T& z, unsigned long x, exp_t e, prec_t, rnd_t rnd)
  { return z.set_2exp(x, e, rnd) == 0; }

  static bool neg(T& z, const TL& x, prec_t, rnd_t rnd)
  { return z.neg(x, rnd) == 0; }
  static bool sqrt(T& z, const TL& x, prec_t, rnd_t rnd)
  { return z.sqrt(x, rnd) == 0; }
  static bool cbrt(T& z, const TL& x, prec_t, rnd_t rnd)
  { return z.cbrt(x, rnd) == 0; }
  static bool root(T& z, const TL& x, unsigned long k, prec_t, rnd_t rnd)
  { return z.root(x, k, rnd) == 0; }

  static bool add(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { return z.add(x, y, rnd) == 0; }
  static bool sub(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { return z.sub(x, y, rnd) == 0; }
  static bool mul(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { return z.mul(x, y, rnd) == 0; }
  static bool div(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { return z.div(x, y, rnd) == 0; }
};

// fix version (call with specified precision)
template <typename T, typename TL = T, typename TR = T>
struct FixedArithmeticPolicy {
  static bool set(T& z, const TL& x, prec_t prec, rnd_t rnd = MPFR_RND)
  { z.set_prec(prec); return z.set(x, rnd) == 0; }
  static bool set(T& z, const char* x, int b, prec_t prec, rnd_t rnd = MPFR_RND)
  { z.set_prec(prec); return z.set(x, b, rnd) == 0; }
  static bool set_2exp(T& z, long x, exp_t e, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.set_2exp(x, e, rnd) == 0; }
  static bool set_2exp(T& z, unsigned long x, exp_t e, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.set_2exp(x, e, rnd) == 0; }

  static bool neg(T& z, const TL& x, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.neg(x, rnd) == 0; }
  static bool sqrt(T& z, const TL& x, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.sqrt(x, rnd) == 0; }
  static bool cbrt(T& z, const TL& x, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.cbrt(x, rnd) == 0; }
  static bool root(T& z, const TL& x, unsigned long k, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.root(x, k, rnd) == 0; }

  static bool add(T& z, const TL& x, const TR& y, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.add(x, y, rnd) == 0; }
  static bool sub(T& z, const TL& x, const TR& y, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.sub(x, y, rnd) == 0; }
  static bool mul(T& z, const TL& x, const TR& y, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.mul(x, y, rnd) == 0; }
  static bool div(T& z, const TL& x, const TR& y, prec_t prec, rnd_t rnd)
  { z.set_prec(prec); return z.div(x, y, rnd) == 0; }
};

// auto version (precision will be determinated automatically)
template <typename T, typename TL = T, typename TR = T>
struct AutoArithmeticPolicy {
  static bool set(T& z, const TL& x, prec_t, rnd_t rnd = MPFR_RND)
  { z.set_prec(T::count_prec(x)); return z.set(x, rnd) == 0; }
  static bool set(T& z, const char* x, int b, prec_t, rnd_t rnd = MPFR_RND)
  { z.set_prec(T::count_prec(x, b)); return z.set(x, b, rnd) == 0; }
  static bool set_2exp(T& z, long x, exp_t e, prec_t, rnd_t rnd)
  { z.set_prec(INT_PREC); return z.set_2exp(x, e, rnd) == 0; }
  static bool set_2exp(T& z, unsigned long x, exp_t e, prec_t, rnd_t rnd)
  { z.set_prec(INT_PREC); return z.set_2exp(x, e, rnd) == 0; }

  static bool neg(T& z, const TL& x, prec_t, rnd_t rnd)
  { z.set_prec(T::count_prec(x)); return z.neg(x, rnd) == 0; }

  static bool add(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { z.set_prec(T::add_prec(x, y)); return z.add(x, y, rnd) == 0; }
  static bool sub(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { z.set_prec(T::add_prec(x, y)); return z.sub(x, y, rnd) == 0; }
  static bool mul(T& z, const TL& x, const TR& y, prec_t, rnd_t rnd)
  { z.set_prec(T::mul_prec(x, y)); return z.mul(x, y, rnd) == 0; }
};

#ifdef CORE_END_NAMESPACE
CORE_END_NAMESPACE
#endif

#endif /*__POLICIES_H__*/
