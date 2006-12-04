/****************************************************************************
 * BigFloat2.inl -- Inline functions for BigFloat2
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
 * $Id: BigFloat2.inl,v 1.8 2006-12-04 21:14:20 exact Exp $
 ***************************************************************************/
#define BF_RNDD GMP_RNDD
#define BF_RNDU GMP_RNDU

////////////////////////////////////////////////////////////////////////////////
/// assignment -- set(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_set_f(const BigFloat2& x, prec_t prec) {
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
inline bool BigFloat2::_set_str(const char* x, int base, prec_t prec) {
  // since MPFR set_str() function cannot tell the exactness of result,
  // we need compare the two results after conversion
  Policy<FT, FT, FT>::set(m_l, x, base, prec, BF_RNDD);
  Policy<FT, FT, FT>::set(m_r, x, base, prec, BF_RNDU);
  set_exact(m_l.cmp(m_r));
  return is_exact();
}
/// assignment -- set(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_set(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::set(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::set(m_r, x, prec, BF_RNDU);
  return is_exact();
}
/// assignment -- set_2exp(long)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_set_2exp_si(long x, exp_t e, prec_t prec) {
  set_exact(Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDU);
  return is_exact();
}
/// assignment -- set_2exp(long)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_set_2exp_ui(unsigned long x, exp_t e, prec_t prec) {
  set_exact(Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, FT, FT>::set_2exp(m_l, x, e, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// negation -- neg(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_neg_f(const BigFloat2& x, prec_t prec) {
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
inline bool BigFloat2::_neg(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::neg(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::neg(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// square root -- sqrt(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_sqrt_f(const BigFloat2& x, prec_t prec) {
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
inline bool BigFloat2::_sqrt(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::sqrt(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::sqrt(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// cubic root -- cbrt(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_cbrt_f(const BigFloat2& x, prec_t prec) {
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
inline bool BigFloat2::_cbrt(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::cbrt(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::cbrt(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// k-th root -- root(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_root_f(const BigFloat2& x, unsigned long k, prec_t prec) {
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
inline bool BigFloat2::_root(const T& x, unsigned long k, prec_t prec) {
  set_exact(Policy<FT, T, FT>::root(m_l, x, k, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::root(m_r, x, k, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// sine -- sin(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_sin_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_sin<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::sin(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::sin(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// sine -- sin(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_sin(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::sin(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::sin(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// cosine -- cos(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_cos_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_cos<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::cos(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::cos(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// cosine -- cos(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_cos(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::cos(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::cos(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// tangent -- tan(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_tan_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_tan<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::tan(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::tan(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// tangent -- tan(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_tan(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::tan(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::tan(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// cotangent -- cot(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_cot_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_cot<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::cot(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::cot(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// cotangent -- cot(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_cot(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::cot(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::cot(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// arcsine -- arcsin(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_arcsin_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_arcsin<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::arcsin(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::arcsin(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// arcsine -- arcsin(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_arcsin(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::arcsin(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::arcsin(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// arccosine -- arccos(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_arccos_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_arccos<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::arccos(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::arccos(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// arccosine -- arccos(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_arccos(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::arccos(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::arccos(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// arctangent -- arctan(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_arctan_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_arctan<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::arctan(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::arctan(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// arctangent -- arctan(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_arctan(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::arctan(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::arctan(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// log base 2 -- log2(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_log_2_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_log_2<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::log_2(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::log_2(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// log base 2 -- log2(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_log_2(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::log_2(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::log_2(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// exponent -- expo(BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_expo_f(const BigFloat2& x, prec_t prec) {
  if (x.is_exact())
    return this->_expo<Policy, FT>(x.m_l, prec);
  else {
    Policy<FT, FT, FT>::expo(m_l, x.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::expo(m_r, x.m_r, prec, BF_RNDU);
    set_exact(false);
  }
  return is_exact();
}
/// exponent -- expo(T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_expo(const T& x, prec_t prec) {
  set_exact(Policy<FT, T, FT>::expo(m_l, x, prec, BF_RNDD));
  if (!is_exact()) Policy<FT, T, FT>::expo(m_r, x, prec, BF_RNDU);
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// addition -- (BigFloat2 + BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_add_f(const BigFloat2& x, const BigFloat2& y, prec_t prec) {
  if (x.is_exact())
    return this->_add<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact())
    return this->_add<Policy, FT>(x, y.m_l, prec);
  else {
    Policy<FT, FT, FT>::add(m_l, x.m_l, y.m_l, prec, BF_RNDD);
    Policy<FT, FT, FT>::add(m_r, x.m_r, y.m_r, prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
    return is_exact();
  }
}
/// addition -- (BigFloat2 + T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_add(const BigFloat2& x, const T& y, prec_t prec) {
  set_exact(Policy<FT, FT, T>::add(m_l, x.m_l, y, prec, BF_RNDD));
  if (!is_exact() || !x.is_exact()) {
    Policy<FT, FT, T>::add(m_r, x.m_r, y, prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
  }
  return is_exact();
}
/// addition -- (T + BigFloat2)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_add(const T& x, const BigFloat2& y, prec_t prec) {
  set_exact(Policy<FT, T, FT>::add(m_l, x, y.m_l, prec, BF_RNDD));
  if (!is_exact() || !y.is_exact()) {
    Policy<FT, T, FT>::add(m_r, x, y.getRight(), prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
  } 
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// subtraction -- (BigFloat2 - BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_sub_f(const BigFloat2& x, const BigFloat2& y, prec_t prec) {
  if (x.is_exact())
    return this->_sub<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact())
    return this->_sub<Policy, FT>(x, y.m_l, prec);
  else {
    Policy<FT, FT, FT>::sub(m_l, x.m_l, y.m_r, prec, BF_RNDD);
    Policy<FT, FT, FT>::sub(m_r, x.m_r, y.m_l, prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
    return is_exact();
  }
}
/// subtraction -- (BigFloat2 - T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_sub(const BigFloat2& x, const T& y, prec_t prec) {
  set_exact(Policy<FT, FT, T>::sub(m_l, x.m_l, y, prec, BF_RNDD));
  if (!is_exact() || !x.is_exact()) {
    Policy<FT, FT, T>::sub(m_r, x.getRight(), y, prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
  }
  return is_exact();
}
/// subtraction -- (T - BigFloat2)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_sub(const T& x, const BigFloat2& y, prec_t prec) {
  set_exact(Policy<FT, T, FT>::sub(m_l, x, y.getRight(), prec, BF_RNDD));
  if (!is_exact() || !y.is_exact()) {
    Policy<FT, T, FT>::sub(m_r, x, y.m_l, prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
  } 
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// multiplication -- (BigFloat2 * BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_mul_f(const BigFloat2& x, const BigFloat2& y, prec_t prec) {
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
    assert(m_r!=m_l);
    set_exact(false);
    return is_exact();
  }
}
/// multiplication -- (BigFloat2 * T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_mul(const BigFloat2& x, const T& y, prec_t prec) {
  typedef Policy<FT, FT, T> P;
  if (x.is_exact()) {
    set_exact(P::mul(m_l, x.m_l, y, prec, BF_RNDD));
    if (!is_exact()) P::mul(m_r, x.m_l, y, prec, BF_RNDU);
  } else {
    P::mul(m_l, (y>0?x.m_l:x.m_r), y, prec, BF_RNDD);
    P::mul(m_r, (y>0?x.m_r:x.m_l), y, prec, BF_RNDU);
    set_exact(m_l==m_r);
  } 
  return is_exact();
}
/// multiplication -- (T * BigFloat2)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_mul(const T& x, const BigFloat2& y, prec_t prec) {
  typedef Policy<FT, T, FT> P;
  if (y.is_exact()) {
    set_exact(P::mul(m_l, x, y.m_l, prec, BF_RNDD));
    if (!is_exact()) P::mul(m_r, x, y.m_l, prec, BF_RNDU);
  } else {
    P::mul(m_l, x, (x>0?y.m_l:y.m_r), prec, BF_RNDD);
    P::mul(m_r, x, (x>0?y.m_r:y.m_l), prec, BF_RNDU);
    set_exact(m_l==m_r);
  } 
  return is_exact();
}

////////////////////////////////////////////////////////////////////////////////
/// division -- (BigFloat2 / BigFloat2)
template <template <typename, typename, typename> class Policy>
inline bool BigFloat2::_div_f(const BigFloat2& x, const BigFloat2& y, prec_t prec) {
  if (x.is_exact())
    return this->_div<Policy, FT>(x.m_l, y, prec);
  else if (y.is_exact()) {
	  return this->_div<Policy, FT>(x, y.m_l, prec);; }
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
    assert(m_r!=m_l);
    set_exact(false);
  }
  return is_exact();
}
/// division -- (BigFloat2 / T)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_div(const BigFloat2& x, const T& y, prec_t prec) {
  typedef Policy<FT, FT, T> P;
  if (x.is_exact()) {
    set_exact(P::div(m_l, x.m_l, y, prec, BF_RNDD));
    if (!is_exact()) P::div(m_r, x.m_l, y, prec, BF_RNDU);
  } else {
    P::div(m_l, (y>0?x.m_l:x.m_r), y, prec, BF_RNDD);
    P::div(m_r, (y>0?x.m_r:x.m_l), y, prec, BF_RNDU);
    assert(m_r!=m_l);
    set_exact(false);
  }
  return is_exact();
}
/// division -- (T / BigFloat2)
template <template <typename, typename, typename> class Policy, typename T>
inline bool BigFloat2::_div(const T& x, const BigFloat2& y, prec_t prec) {
  typedef Policy<FT, T, FT> P;
  if (y.is_exact()) {
    set_exact(P::div(m_l, x, y.m_l, prec, BF_RNDD));
    if (!is_exact()) P::div(m_r, x, y.m_l, prec, BF_RNDU);
  } else {
    P::div(m_l, x, (x>0?y.m_r:y.m_l), prec, BF_RNDD);
    P::div(m_r, x, (x>0?y.m_l:y.m_r), prec, BF_RNDU);
    set_exact(m_l==m_r); // for the case of x=0;
  } 
  return is_exact();
}
