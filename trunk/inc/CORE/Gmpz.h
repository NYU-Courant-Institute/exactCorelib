/****************************************************************************
 * Gmpz.h -- A C++ wrapper class for GMP mpz
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
 * $Id: Gmpz.h,v 1.1 2006-02-28 18:00:15 exact Exp $
 ***************************************************************************/
#ifndef __GMPZ_H__
#define __GMPZ_H__

#include <gmp.h>
#include <CORE/Config.h>

#ifndef CORE_DISABLE_REFCOUNTING
  #include <CORE/RefCounting.h>
#endif

CORE_BEGIN_NAMESPACE

/// \class Gmpz Gmpz.h
/// \brief Gmpz is a wrapper class of <tt>mpz</tt> in GMP
class Gmpz 
#ifndef CORE_DISABLE_REFCOUNTING
  : public RcRepImpl<Gmpz>
#endif
{
private:
  mpz_t m_mp;
public:
  //internal structure accessors
  const mpz_t& mp() const { return m_mp; }
  mpz_t& mp() { return m_mp; }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  Gmpz()
  { mpz_init(m_mp); }
  /// copy constructor
  Gmpz(const Gmpz& rhs)
  { mpz_init_set(m_mp, rhs.m_mp); }

  /// constructor for <tt>long</tt>
  Gmpz(long i)
  { mpz_init_set_si(m_mp, i); }
  /// constructor for <tt>unsigned long</tt>
  Gmpz(unsigned long i)
  { mpz_init_set_ui(m_mp, i); }

  /// constructor for <tt>double</tt>
  Gmpz(double i)
  { mpz_init_set_d(m_mp, i); }

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit Gmpz(const char* str, int base)
  { mpz_init_set_str(m_mp, str, base); }

  // internal used by Gmpq
  explicit Gmpz(const mpz_t x)
  { mpz_init_set(m_mp, x); }

  /// destructor
  ~Gmpz()
  { mpz_clear(m_mp); }
  //@}
  
  /// \name assignment operator
  //@{
  /// assignment operator for <tt>Gmpz</tt>
  Gmpz& operator=(const Gmpz& rhs) {
    if (this != &rhs)  mpz_set(m_mp, rhs.m_mp); 
    return *this;
  }
  //@}
};

#ifndef CORE_DISABLE_REFCOUNTING
/// \class RcGmpz Gmpz.h
/// \brief RcGmpz is a wrapper class of <tt>Gmpz</tt> with referecen counting
class RcGmpz : public RcImpl<Gmpz> {
  typedef RcImpl<Gmpz> base_cls;
public:
  //internal structure accessors
  const mpz_t& mp() const { return ((const Gmpz*)_rep)->mp(); }
  mpz_t& mp() { return _rep->mp(); }

public:
  /// \name constructors and destructor
  //@{
  /// default constructor
  RcGmpz() : base_cls(new Gmpz()) {}
  /// copy constructor
  RcGmpz(const RcGmpz& rhs) : base_cls(rhs._rep) { _rep->inc_rc(); }

  /// constructor for <tt>long</tt>
  RcGmpz(long i) : base_cls(new Gmpz(i)) {}
  /// constructor for <tt>unsigned long</tt>
  RcGmpz(unsigned long i) : base_cls(new Gmpz(i)) {}

  /// constructor for <tt>double</tt>
  RcGmpz(double i) : base_cls(new Gmpz(i)) {}

  /// constructor for <tt>char*</tt> (no implicit conversion)
  explicit RcGmpz(const char* str, int base) : base_cls(new Gmpz(str, base)) {}

  // internal used by Gmpq
  explicit RcGmpz(const mpz_t x) : base_cls(new Gmpz(x)) {}

  /// destructor
  ~RcGmpz() { _rep->dec_rc(); }
  //@}
  
  /// \name assignment operator
  //@{
  /// assignment operator for <tt>RcGmpz</tt>
  RcGmpz& operator=(const RcGmpz& rhs) {
    if (this != &rhs) { _rep->dec_rc(); _rep = rhs._rep; _rep->inc_rc(); } 
    return *this;
  }
};
#endif

CORE_END_NAMESPACE

#endif /*__GMPZ_H__*/
