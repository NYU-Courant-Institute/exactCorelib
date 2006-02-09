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
 * $Id: BigFloat.inl,v 1.1.1.1 2006-02-09 09:18:04 exact Exp $
 ***************************************************************************/
#define BF_RNDD GMP_RNDD
#define BF_RNDU GMP_RNDU

////////////////////////////////////////////////////////////////////////////////
/// assignment -- set(BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_set(const BigFloat& x, prec_t prec) {
  if (x.is_exact())
    return this->_set<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::set(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::set(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// assignment -- set(const char*, int base)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_set(const char* x, int base, prec_t prec) {
  // since MPFR set_str() function cannot tell the exactness of result,
  // we need compare the two results after conversion
  Policy<FT, FT, FT>::set(m_l, x, base, prec, BF_RNDD);
  Policy<FT, FT, FT>::set(m_r, x, base, prec, BF_RNDU);
  set_exact(m_l.cmp(m_r));
  return is_exact();
}
/// assignment -- set(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_set(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::set(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::set(m_r, x, prec, BF_RNDU);
  return is_exact();
}
/// assignment -- set_2exp(long)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_set_2exp(long x, exp_t e, prec_t prec) {
  set_exact(Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDU);
  return is_exact();
}
/// assignment -- set_2exp(long)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_set_2exp(unsigned long x, exp_t e, prec_t prec) {
  set_exact(Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// negation -- neg(BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_neg(const BigFloat& x, prec_t prec) {
  if (x.is_exact())
    return this->_neg<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::neg(m_l, x.m_r, prec, BF_RNDD);
    Policy<FT, FT, FT>::neg(m_r, x.m_l, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// negation -- neg(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_neg(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::neg(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::neg(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// square root -- sqrt(BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_sqrt(const BigFloat& x, prec_t prec) {
  if (x.is_exact())
    return this->_sqrt<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::sqrt(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::sqrt(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// square root -- sqrt(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_sqrt(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::sqrt(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::sqrt(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// cubic root -- cbrt(BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_cbrt(const BigFloat& x, prec_t prec) {
  if (x.is_exact())
    return this->_cbrt<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::cbrt(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::cbrt(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// cubic root -- cbrt(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_cbrt(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::cbrt(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::cbrt(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// k-th root -- root(BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_root(const BigFloat& x, unsigned long k, prec_t prec) {
  if (x.is_exact())
    return this->_root<Policy, FT>(x.m_l, k, prec);
  else {
    Policy<FT, FT, FT>::root(m_l, x.m_l, k, prec, BF_RNDD);
    Policy<FT, FT, FT>::root(m_r, x.m_r, k, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// k-th root -- root(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_root(const T& x, unsigned long k, prec_t prec) {
  set_exact(Policy<FT, T, FT>::root(m_l, x, k, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::root(m_r, x, k, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// addition -- (BigFloat + BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_add(const BigFloat& x, const BigFloat& y, prec_t prec) {
  if (x.is_exact())
    return this->_add<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact())
    return this->_add<Policy, FT>(x, y.m_l, prec);
  else {
    Policy<FT, FT, FT>::add(m_l, x.m_l, y.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::add(m_r, x.m_r, y.m_r, prec, BF_RNDU);
    set_exact(false);
    return is_exact();
  }
}
/// addition -- (BigFloat + T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_add(const BigFloat& x, const T& y, prec_t prec) {
  set_exact(Policy<FT, FT, T>::add(m_l, x.m_l, y, prec, BF_RNDD));
  if (!is_exact() || !x.is_exact()) {
    Policy<FT, FT, T>::add(m_r, x.m_r, y, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// addition -- (T + BigFloat)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_add(const T& x, const BigFloat& y, prec_t prec) {
  set_exact(Policy<FT, T, FT>::add(m_l, x, y.m_l, prec, BF_RNDD));
  if (!is_exact() || !y.is_exact()) {
    Policy<FT, T, FT>::add(m_r, x, y.m_r, prec, BF_RNDU);
    set_exact(false);
  } 
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// subtraction -- (BigFloat - BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_sub(const BigFloat& x, const BigFloat& y, prec_t prec) {
  if (x.is_exact())
    return this->_sub<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact())
    return this->_sub<Policy, FT>(x, y.m_l, prec);
  else {
    Policy<FT, FT, FT>::sub(m_l, x.m_l, y.m_r, prec, BF_RNDD);
    Policy<FT, FT, FT>::sub(m_r, x.m_r, y.m_l, prec, BF_RNDU);
    set_exact(false);
    return is_exact();
  }
}
/// subtraction -- (BigFloat - T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_sub(const BigFloat& x, const T& y, prec_t prec) {
  set_exact(Policy<FT, FT, T>::sub(m_l, x.m_l, y, prec, BF_RNDD));
  if (!is_exact() || !x.is_exact()) {
    Policy<FT, FT, T>::sub(m_r, x.m_r, y, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// subtraction -- (T - BigFloat)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_sub(const T& x, const BigFloat& y, prec_t prec) {
  set_exact(Policy<FT, T, FT>::sub(m_l, x, y.m_l, prec, BF_RNDD));
  if (!is_exact() || !y.is_exact()) {
    Policy<FT, T, FT>::sub(m_r, x, y.m_r, prec, BF_RNDU);
    set_exact(false);
  } 
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// multiplication -- (BigFloat * BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_mul(const BigFloat& x, const BigFloat& y, prec_t prec) {
  if (x.is_exact())
    return this->_mul<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact())
    return this->_mul<Policy, FT>(x, y.m_l, prec);
  else {
    typedef Policy<FT, FT, FT> P;
    if (x.m_l.sgn() >= 0) {
      if (y.m_l.sgn() >= 0) {
        P::mul(m_l, x.m_l, y.m_l, prec, BF_RNDD);
        P::mul(m_r, x.m_r, y.m_r, prec, BF_RNDU);
      } else if (y.m_r.sgn() <= 0) {
        P::mul(m_l, x.m_r, y.m_l, prec, BF_RNDD);
        P::mul(m_r, x.m_l, y.m_r, prec, BF_RNDU);
      } else {
        P::mul(m_l, x.m_r, y.m_l, prec, BF_RNDD);
        P::mul(m_r, x.m_r, y.m_r, prec, BF_RNDU);
      }
    } else if (x.m_r.sgn() <= 0) {
      if (y.m_l.sgn() >= 0) {
        P::mul(m_l, x.m_l, y.m_r, prec, BF_RNDD);
        P::mul(m_r, x.m_r, y.m_l, prec, BF_RNDU);
      } else if (y.m_r.sgn() <= 0) {
        P::mul(m_l, x.m_r, y.m_r, prec, BF_RNDD);
        P::mul(m_r, x.m_l, y.m_l, prec, BF_RNDU);
      } else {
        P::mul(m_l, x.m_l, y.m_r, prec, BF_RNDD);
        P::mul(m_r, x.m_l, y.m_l, prec, BF_RNDU);
      }
    } else {
      if (y.m_l.sgn() >= 0) {
        P::mul(m_l, x.m_l, y.m_r, prec, BF_RNDD);
        P::mul(m_r, x.m_r, y.m_r, prec, BF_RNDU);
      } else if (y.m_r.sgn() <= 0) {
        P::mul(m_l, x.m_r, y.m_l, prec, BF_RNDD);
        P::mul(m_r, x.m_l, y.m_l, prec, BF_RNDU);
      } else {
        FT tmp;
        // compute min{x.m_l*y.m_r, x.m_r*y.m_l}
        P::mul(m_l, x.m_l, y.m_r, prec, BF_RNDD);
        tmp.set_prec(m_l.get_prec());
        P::mul(tmp, x.m_r, y.m_l, prec, BF_RNDD);
        if (m_l.cmp(tmp) > 0) m_l.swap(tmp);
        // compute max{x.m_r*y.m_r, x.m_l*y.m_l}
        P::mul(m_r, x.m_r, y.m_r, prec, BF_RNDU);
        tmp.set_prec(m_r.get_prec());
        P::mul(tmp, x.m_l, y.m_l, prec, BF_RNDU);
        if (m_r.cmp(tmp) < 0) m_r.swap(tmp);
      }
    }
    set_exact(false);
    return is_exact();
  }
}
/// multiplication -- (BigFloat * T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_mul(const BigFloat& x, const T& y, prec_t prec) {
  typedef Policy<FT, FT, T> P;
  if (x.is_exact()) {
    set_exact(P::mul(m_l, x.m_l, y, prec, BF_RNDD));
    if (!is_exact()) P::mul(m_r, x.m_l, y, prec, BF_RNDU);
  } else {
    set_exact(P::mul(m_l, (y>0?x.m_l:x.m_r), y, prec, BF_RNDD));
    if (!is_exact()) P::mul(m_r, (y>0?x.m_r:x.m_l), y, prec, BF_RNDU);
  } 
  return is_exact();
}
/// multiplication -- (T * BigFloat)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_mul(const T& x, const BigFloat& y, prec_t prec) {
  typedef Policy<FT, T, FT> P;
  if (y.is_exact()) {
    set_exact(P::mul(m_l, x, y.m_l, prec, BF_RNDD));
    if (!is_exact()) P::mul(m_r, x, y.m_l, prec, BF_RNDU);
  } else {
    set_exact(P::mul(m_l, x, (x>0?y.m_l:y.m_r), prec, BF_RNDD));
    if (!is_exact()) P::mul(m_r, x, (x>0?y.m_r:y.m_l), prec, BF_RNDU);
  } 
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// division -- (BigFloat / BigFloat)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat::_div(const BigFloat& x, const BigFloat& y, prec_t prec) {
  if (x.is_exact())
    return this->_div<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact())
    return this->_div<Policy, FT>(x, y.m_l, prec);
  else if (y.has_zero()) {
    set_inf();
  } else {
    typedef Policy<FT, FT, FT> P;
    if (x.m_l.sgn() >= 0) {
      if (y.m_l.sgn() >= 0) {
        P::div(m_l, x.m_l, y.m_r, prec, BF_RNDD);
        P::div(m_r, x.m_r, y.m_l, prec, BF_RNDU);
      } else if (y.m_r.sgn() <= 0) {
        P::div(m_l, x.m_r, y.m_r, prec, BF_RNDD);
        P::div(m_r, x.m_l, y.m_l, prec, BF_RNDU);
      }
    } else if (x.m_r.sgn() <= 0) {
      if (y.m_l.sgn() >= 0) {
        P::div(m_l, x.m_l, y.m_l, prec, BF_RNDD);
        P::div(m_r, x.m_r, y.m_r, prec, BF_RNDU);
      } else if (y.m_r.sgn() <= 0) {
        P::div(m_l, x.m_r, y.m_l, prec, BF_RNDD);
        P::div(m_r, x.m_l, y.m_r, prec, BF_RNDU);
      }
    } else {
      if (y.m_l.sgn() > 0) {
        P::div(m_l, x.m_l, y.m_l, prec, BF_RNDD);
        P::div(m_r, x.m_r, y.m_l, prec, BF_RNDU);
      } else if (y.m_r.sgn() < 0) {
        P::div(m_l, x.m_r, y.m_r, prec, BF_RNDD);
        P::div(m_r, x.m_l, y.m_r, prec, BF_RNDU);
      }
    }
    set_exact(false);
  }
  return is_exact();
}
/// division -- (BigFloat / T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_div(const BigFloat& x, const T& y, prec_t prec) {
  typedef Policy<FT, FT, T> P;
  if (x.is_exact()) {
    set_exact(P::div(m_l, x.m_l, y, prec, BF_RNDD));
    if (!is_exact()) P::div(m_r, x.m_l, y, prec, BF_RNDU);
  } else {
    set_exact(P::div(m_l, (y>0?x.m_l:x.m_r), y, prec, BF_RNDD));
    if (!is_exact()) P::div(m_r, (y>0?x.m_r:x.m_l), y, prec, BF_RNDU);
  }
  return is_exact();
}
/// division -- (T / BigFloat)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat::_div(const T& x, const BigFloat& y, prec_t prec) {
  typedef Policy<FT, T, FT> P;
  if (y.is_exact()) {
    set_exact(P::div(m_l, x, y.m_l, prec, BF_RNDD));
    if (!is_exact()) P::div(m_r, x, y.m_l, prec, BF_RNDU);
  } else {
    set_exact(P::div(m_l, x, (x>0?y.m_r:y.m_l), prec, BF_RNDD));
    if (!is_exact()) P::div(m_r, x, (x>0?y.m_l:y.m_r), prec, BF_RNDU);
  } 
  return is_exact();
}
