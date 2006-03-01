/****************************************************************************
 * Mpfr.h -- A C++ wrapper class for MPFR mpfr
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
 * $Id: Mpfr.h,v 1.3 2006-03-01 01:04:05 exact Exp $
 ***************************************************************************/
#ifndef __MPFR_H__
#define __MPFR_H__

#include <mpfr.h>
#include <CORE/Config.h>

// typedefs
typedef mp_rnd_t rnd_t;
typedef mp_exp_t exp_t;
typedef mp_prec_t prec_t;

// default rouning mode
#ifndef MPFR_RND
#define MPFR_RND mpfr_get_default_rounding_mode()
#endif

#ifndef CORE_DISABLE_REFCOUNTING
  #include <CORE/RefCounting.h>
#endif

CORE_BEGIN_NAMESPACE

/// \class Mpfr Mpfr.h
/// \brief Mpfr is a wrapper class of <tt>mpfr</tt> in MPFR
class Mpfr 
#ifndef CORE_DISABLE_REFCOUNTING
  : public RcRepImpl<Mpfr>
#endif
{
private:
  mpfr_t m_mp;
public:
  //internal structure accessors
  const mpfr_t& mp() const { return m_mp; }
  mpfr_t& mp() { return m_mp; }

public:
  /// \name constructors (auto version)
  //@{
  /// default constructor
  Mpfr() 
  { mpfr_init(m_mp); }
  /// copy constructor
  Mpfr(const Mpfr& rhs) {
    mpfr_init2(m_mp, mpfr_get_prec(rhs.m_mp)); 
    mpfr_set(m_mp, rhs.m_mp, MPFR_RND); 
  }
  /// copy constructor with specified precision
  Mpfr(const Mpfr& rhs, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set(m_mp, rhs.m_mp, rnd); }

  /// constructor for <tt>long</tt> with specified precision
  Mpfr(long i, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_si(m_mp, i, rnd); }
  /// constructor for <tt>unsigned long</tt> with specified precision
  Mpfr(unsigned long i, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_ui(m_mp, i, rnd); }
  /// constructor for <tt>double</tt> with specified precision
  Mpfr(double i, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_d(m_mp, i, rnd); }

  /// constructor for <tt>mpz_t</tt> with specified precision
  explicit Mpfr(const mpz_t& x, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_z(m_mp, x, rnd); }
  /// constructor for <tt>mpq_t</tt> with specified precision
  explicit Mpfr(const mpq_t& x, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_q(m_mp, x, rnd); }

  /// constructor for <tt>char*</tt> with specified precision
  explicit Mpfr(const char* s, int base, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_str(m_mp, s, base, rnd); }

  /// constructor with value \f$i*2^e\f$ for <tt>long</tt>
  Mpfr(long i, exp_t e, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_si_2exp(m_mp, i, e, rnd); }
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned long</tt>
  Mpfr(unsigned long i, exp_t e, prec_t prec, rnd_t rnd)
  { mpfr_init2(m_mp, prec); mpfr_set_ui_2exp(m_mp, i, e, rnd); }

  /// destructor
  ~Mpfr()
  { mpfr_clear(m_mp); }
  //@}

  /// \name assignment operator
  //@{
  /// assignment operator for <tt>Mpfr</tt>
  Mpfr& operator=(const Mpfr& rhs) { 
    if (this != &rhs)  { 
      mpfr_set_prec(m_mp, mpfr_get_prec(rhs.m_mp)); 
      mpfr_set(m_mp, rhs.m_mp, MPFR_RND);
    }
    return *this;
  }
  //@}
};

#ifndef CORE_DISABLE_REFCOUNTING
/// \class RcMpfr Mpfr.h
/// \brief RcMpfr is a wrapper class of <tt>Mpfr</tt> with referecen counting
class RcMpfr : public RcImpl<Mpfr> {
  typedef RcImpl<Mpfr> base_cls;
public:
  //internal structure accessors
  const mpfr_t& mp() const { return ((const Mpfr*)_rep)->mp(); }
  mpfr_t& mp() { return _rep->mp(); }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  RcMpfr() : base_cls(new Mpfr()) {}
  /// copy constructor
  RcMpfr(const RcMpfr& rhs) : base_cls(rhs._rep) { _rep->inc_rc(); }
  /// copy constructor with specified precision
  RcMpfr(const RcMpfr& rhs, prec_t prec, rnd_t rnd)
    : base_cls(new Mpfr(*rhs._rep, prec, rnd)) {}

  /// constructor for <tt>long</tt>
  RcMpfr(long i, prec_t prec, rnd_t rnd) 
    : base_cls(new Mpfr(i, prec, rnd)) {}
  /// constructor for <tt>unsigned long</tt>
  RcMpfr(unsigned long i, prec_t prec, rnd_t rnd) 
    : base_cls(new Mpfr(i, prec, rnd)) {}
  /// constructor for <tt>double</tt>
  RcMpfr(double i, prec_t prec, rnd_t rnd)
    : base_cls(new Mpfr(i, prec, rnd)) {}

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit RcMpfr(const char* str, int base, prec_t prec, rnd_t rnd) 
    : base_cls(new Mpfr(str, base, prec, rnd)) {}

  /// constructor for <tt>mpz_t</tt>
  explicit RcMpfr(const mpz_t& z, prec_t prec, rnd_t rnd) 
    : base_cls(new Mpfr(z, prec, rnd)) {}
  /// constructor for <tt>mpq_t</tt>
  explicit RcMpfr(const mpq_t& q, prec_t prec, rnd_t rnd) 
    : base_cls(new Mpfr(q, prec, rnd)) {}

  /// constructor with value \f$i*2^e\f$ for <tt>long</tt>
  RcMpfr(long i, exp_t e, prec_t prec, rnd_t rnd)
    : base_cls(new Mpfr(i, e, prec, rnd)) {}
  /// constructor with value \f$i*2^e\f$ for <tt>unsigned long</tt>
  RcMpfr(unsigned long i, exp_t e, prec_t prec, rnd_t rnd)
    : base_cls(new Mpfr(i, e, prec, rnd)) {}

  /// destructor
  ~RcMpfr() { _rep->dec_rc(); }
  //@}

  /// \name assignment operator
  //@{
  /// assignment operator for <tt>RcMpfr</tt>
  RcMpfr& operator=(const RcMpfr& rhs) {
    if (this != &rhs) { _rep->dec_rc(); _rep = rhs._rep; _rep->inc_rc(); }
    return *this;
  }
};
#endif

CORE_END_NAMESPACE

#endif /*__GMPQ_H__*/
