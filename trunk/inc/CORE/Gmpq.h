/****************************************************************************
 * Gmpq.h -- A C++ wrapper class for GMP mpq
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
 * $Id: Gmpq.h,v 1.1 2006-02-28 18:00:15 exact Exp $
 ***************************************************************************/
#ifndef __GMPQ_H__
#define __GMPQ_H__

#include <gmp.h>
#include <CORE/Config.h>

#ifndef CORE_DISABLE_REFCOUNTING
  #include <CORE/RefCounting.h>
#endif

CORE_BEGIN_NAMESPACE

/// \class Gmpq Gmpq.h
/// \brief Gmpq is a wrapper class of <tt>mpq</tt> in GMP
class Gmpq 
#ifndef CORE_DISABLE_REFCOUNTING
  : public RcRepImpl<Gmpq>
#endif
{
private:
  mpq_t m_mp;
public:
  //internal structure accessors
  const mpq_t& mp() const { return m_mp; }
  mpq_t& mp() { return m_mp; }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  Gmpq()
  { mpq_init(m_mp); }
  /// copy constructor
  Gmpq(const Gmpq& rhs)
  { mpq_init(m_mp); mpq_set(m_mp, rhs.m_mp); }

  /// constructor for <tt>long</tt>
  Gmpq(long i)
  { mpq_init(m_mp); mpq_set_si(m_mp, i, 1UL); }
  /// constructor for <tt>unsigned long</tt>
  Gmpq(unsigned long i)
  { mpq_init(m_mp); mpq_set_ui(m_mp, i, 1UL); }

  /// constructor for <tt>double</tt>
  Gmpq(double i)
  { mpq_init(m_mp); mpq_set_d(m_mp, i); }

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit Gmpq(const char* str)
  { mpq_init(m_mp); mpq_set_str(m_mp, str, 0); mpq_canonicalize(m_mp); }

  /// constructor for <tt>mpz_t</tt>
  explicit Gmpq(const mpz_t& z)
  { mpq_init(m_mp); mpq_set_z(m_mp, z); }

  /// constructor for <tt>mpz_t, mpz_t</tt>
  explicit Gmpq(const mpz_t& num, const mpz_t& den) {
    mpq_init(m_mp); 
    mpz_set(mpq_numref(m_mp), num); mpz_set(mpq_denref(m_mp), den);
    mpq_canonicalize(m_mp);
  }

  /// destructor
  ~Gmpq()
  { mpq_clear(m_mp); }
  //@}
  
  /// \name assignment operator
  //@{
  /// assignment operator for <tt>Gmpq</tt>
  Gmpq& operator=(const Gmpq& rhs) {
    if (this != &rhs)  mpq_set(m_mp, rhs.m_mp); 
    return *this;
  }
  //@}
};

#ifndef CORE_DISABLE_REFCOUNTING
/// \class RcGmpq Gmpq.h
/// \brief RcGmpq is a wrapper class of <tt>Gmpq</tt> with referecen counting
class RcGmpq : public RcImpl<Gmpq> {
  typedef RcImpl<Gmpq> base_cls;
public:
  //internal structure accessors
  const mpq_t& mp() const { return ((const Gmpq*)_rep)->mp(); }
  mpq_t& mp() { return _rep->mp(); }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  RcGmpq() : base_cls(new Gmpq()) {}
  /// copy constructor
  RcGmpq(const RcGmpq& rhs) : base_cls(rhs._rep) { _rep->inc_rc(); }

  /// constructor for <tt>long</tt>
  RcGmpq(long i) : base_cls(new Gmpq(i)) {}
  /// constructor for <tt>unsigned long</tt>
  RcGmpq(unsigned long i) : base_cls(new Gmpq(i)) {}

  /// constructor for <tt>double</tt>
  RcGmpq(double i) : base_cls(new Gmpq(i)) {}

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit RcGmpq(const char* str) : base_cls(new Gmpq(str)) {}

  /// constructor for <tt>mpz_t</tt>
  explicit RcGmpq(const mpz_t& z) : base_cls(new Gmpq(z)) {}

  /// constructor for <tt>mpz_t, mpz_t</tt>
  explicit RcGmpq(const mpz_t& num, const mpz_t& den) 
    : base_cls(new Gmpq(num, den)) {}

  /// destructor
  ~RcGmpq() { _rep->dec_rc(); }
  //@}
  
  /// \name assignment operator
  //@{
  /// assignment operator for <tt>RcGmpq</tt>
  RcGmpq& operator=(const RcGmpq& rhs) {
    if (this != &rhs) { _rep->dec_rc(); _rep = rhs._rep; _rep->inc_rc(); } 
    return *this;
  }
};
#endif

CORE_END_NAMESPACE

#endif /*__GMPQ_H__*/
