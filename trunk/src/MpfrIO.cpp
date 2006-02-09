/****************************************************************************
 * MpfrIO.cpp -- I/O functions for MPFR
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
 * $Id: MpfrIO.cpp,v 1.1.1.1 2006-02-09 09:18:13 exact Exp $
 ***************************************************************************/
#include <mpfr.h>
#include <string>

/* Define BITS_PER_MP_LIMB
   Can't use sizeof(mp_limb_t) since it should be a preprocessor constant */
#if defined(GMP_NUMB_BITS) /* GMP 4.1.2 or above */
# define BITS_PER_MP_LIMB  (GMP_NUMB_BITS+GMP_NAIL_BITS)
#elif defined (__GMP_BITS_PER_MP_LIMB) /* Older versions 4.x.x */
# define BITS_PER_MP_LIMB  __GMP_BITS_PER_MP_LIMB
# define GMP_NUMB_BITS BITS_PER_MP_LIMB
# ifndef GMP_NAIL_BITS
#  define GMP_NAIL_BITS 0
# endif
#else
# error "Could not detect BITS_PER_MP_LIMB. Try with gmp internal files."
#endif
  
#define MPFR_MANT(x)      ((x)->_mpfr_d)
#define MPFR_PREC(x)      ((x)->_mpfr_prec)
#define MPFR_LIMB_SIZE(x) ((MPFR_PREC((x))-1)/BITS_PER_MP_LIMB+1)

// remove trailing zeros (only by limbs)
void mpfr_remove_trailing_zeros(mpfr_t x) {
  unsigned int xn = MPFR_LIMB_SIZE(x); 
  mp_limb_t* xp = MPFR_MANT(x);
  unsigned int i = 0;
  while (i < xn && xp[i] == 0) i++;
  if (i > 0 && i < xn) mpfr_round_prec(x, GMP_RNDN, (xn-i)*BITS_PER_MP_LIMB); 
}

// use mpf to read mpfr from istream
std::istream& operator>>(std::istream& is, mpfr_ptr f) {
  mpf_t tmp; 
  mpf_init2(tmp, mpfr_get_prec(f));
  is >> tmp; 
  mpfr_set_f(f, tmp, __gmp_default_rounding_mode);
  mpf_clear(tmp); 
  return is;
}

// convert integer to string
static char* itoa(int val, int base) {
  static char buf[32] = {0};
  int i = 30;
  for (; val && i; --i, val /= base)
    buf[i] = "0123456789abcdef"[val % base];
  return &buf[i+1];
}

// format floating point string
static void formatFloat(std::string& str, mp_exp_t exp, int base, bool fixed) {
  // find the start position implicit radix point
  std::string::size_type first = (str[0] == '-') ? 1 : 0;
  
  if (fixed) { // fixed format
    if (exp > 0) {
      if (first + exp > str.length()) // integer need padding 0
        str.append(first + exp - str.length(), '0');
      else if (first + exp < str.length()) // float point value
        str.insert(first + exp, ".");
    } else if (exp < 0) {
      str.insert(first, -exp, '0');
      str.insert(first, ".");
    } else
      str.insert(first, "0.");
  } else { // scientific format
    str.insert(first, "0.");
    // add exponent
    if (exp != 0) {
      if (base <= 10) 
        str.append("e");
      else
        str.append("@");
      if (exp > 0)
        str.append(itoa(exp, base));
      else {
        str.append("-"); str.append(itoa(-exp, base));
      }
    }
  }
}

// convert mpfr to string
std::string mpfr2str(mpfr_srcptr mp, size_t ndigits, int base, bool fixed, mp_rnd_t rnd) {
  mp_exp_t exp;
  char* s = mpfr_get_str(0, &exp, base, ndigits, mp, rnd);
  if (s != NULL) {
    std::string str(s);
    formatFloat(str, exp, base, fixed);
    mpfr_free_str(s);
    return str;
  } else
    return std::string();
}
