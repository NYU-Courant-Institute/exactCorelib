#ifndef __CORE_POLY_H__
#define __CORE_POLY_H__

#include <Core/poly/PolyBase.h>
#include <string>
#include <iostream>

CORE_BEGIN_NAMESPACE

/// \class Polynomial Poly.h
/// \brief A template polynomial class
/**
 *      REPRESENTATION:
 *      --Each polynomial has a nominal "degree" (this
 *              is an upper bound on the true degree, which
 *              is determined by the first non-zero coefficient).
 *      --coefficients are parametrized by some number type "NT".
 *      --coefficients are stored in the "coeff" array of
 *              length "degree + 1".  
 *              CONVENTION: coeff[i] is the coefficient of X^i.  So, a
 *                          coefficient list begins with the constant term.
 *      --IMPORTANT CONVENTION:
 *              the zero polynomial has degree -1
 *              while nonzero constant polynomials have degree 0.
 * 
 *      FUNCTIONALITY:
 *      --Polynomial Ring Operations (+,-,*)
 *      --Power
 *      --Evaluation
 *      --Differentiation
 *      --Remainder, Quotient 
 *      --GCD
 *      --Resultant, Discriminant (planned)
 *      --Polynomial Composition (planned)
 *      --file I/O (planned)
 */

template <typename NT>
class Polynomial : public 
#ifndef CORE_DISABLE_REFCOUNTING 
  PolyBase<NT> 
#else
  RcPolyBase<NT> 
#endif
{
#ifndef CORE_DISABLE_REFCOUNTING 
  typedef PolyBase<NT> base_cls;
#else
  typedef RcPolyBase<NT> base_cls;
#endif
public:
  typedef std::vector<NT> VecNT;
  /// \name Constructors and Destructor
  //@{
  /// default constructor (zero polynomial)
  Polynomial() {}
  /// copy constructor
  Polynomial(const Polynomial &rhs) : base_cls(rhs) {}
  /// constructor of the Unit Polynomial of nominal deg n>=0
  Polynomial(int n) : base_cls(n) {}
  /// constructor with coeff array
  Polynomial(int n, NT* coef) : base_cls(n, coef) {}
  /// constructor with coeff vector
  Polynomial(const VecNT & coef) : base_cls(coef) {}
  Polynomial(int n, const char* s[]);
  /// constructor from char*
  Polynomial(const char* s, char myX='x') : base_cls(s, myX) {}
  /// constructor from std::string
  Polynomial(const std::string& s, char myX='x') : base_cls(s.c_str(), myX) {}
  //@}

  /// \name help functions
  //@{
  /// return coeff (const)
  const NT* coeff() const { return base_cls::coeff(); }
  /// return coeff (non-const)
  NT* coeff() { return base_cls::coeff(); }
  /// return the degree
  int degree() const { return base_cls::degree(); }
  /// return the degree (same as degree())
  int getDegree() const { return base_cls::degree(); }
  /// return the true degree
  int getTrueDegree() const {
    int i = getDegree();
    while (i>=0 && sign(coeff()[i]) != 0) --i;
    return i;
  }

  /// return the i-th coeff
  const NT& getCoeff(int i) const
  { assert(i>=0 && i<=getDegree()); return coeff()[i]; }
  /// set the i-th coeff
  bool setCoeff(int i, const NT& cc)
  { if (i<0 || i>getDegree()) return false; coeff()[i] = cc; return true; }
  
  /// return the leading coeff
  const NT& getLeadCoeff() const 
  { return getCoeff(getTrueDegree()); }
  /// return the last non-zero coeff
  const NT& getTailCoeff() const {
    int i = 0;
    while (i<=getTrueDegree() && getCoeff(i) == 0)
      ++i;
    return getCoeff(i); 
  }
  /// Expands the nominal degree to n;
  ///      Returns n if nominal degree is changed to n
  ///      Else returns -2
  int expand(int n) {
    if ((n <= getDegree())||(n < 0))
      return -2;
    int i;
    NT * c = new NT[n+1];
    for (i = 0; i<= getDegree(); i++)
      c[i] = coeff()[i];
    for (i = getDegree()+1; i<=n; i++)
      c[i] = 0;
    this->set(n, c);
    return n;
  }
  //@}

  /// \name assignment and compound assignment operators
  //@{
  /// assignment operator for <tt>Polynomial</tt>
  Polynomial& operator=(const Polynomial& rhs)
  { base_cls::operator=(rhs); return *this; }
  /// compound assignment operator <tt>+=</tt>
  Polynomial& operator+=(const Polynomial& rhs) {
    int d = rhs.getDegree();
    if (d > getDegree()) expand(d);
    for (int i=0; i<=d; ++i)
      coeff()[i] += rhs.coeff()[i];
    return *this;
  }
  /// compound assignment operator <tt>-=</tt>
  Polynomial& operator-=(const Polynomial& rhs) {
    int d = rhs.getDegree();
    if (d > getDegree()) expand(d);
    for (int i=0; i<=d; ++i)
      coeff()[i] -= rhs.coeff()[i];
    return *this;
  }
  /// compound assignment operator <tt>*=</tt>
  Polynomial& operator*=(const Polynomial& rhs);
  //@}
};

/// Polynomial + Polynomial
template <typename NT>
inline 
Polynomial<NT> operator+(const Polynomial<NT>& x, const Polynomial<NT>& y) {
  return Polynomial<NT>(x) += y;
}
/// Polynomial - Polynomial
template <typename NT>
inline 
Polynomial<NT> operator-(const Polynomial<NT>& x, const Polynomial<NT>& y) {
  return Polynomial<NT>(x) -= y;
}
/// Polynomial * Polynomial
template <typename NT>
inline 
Polynomial<NT> operator*(const Polynomial<NT>& x, const Polynomial<NT>& y) {
  int d = x.getDegree() + y.getDegree();
  NT* c = new NT[d+1];
  for (int i=0; i<=x.getDegree(); ++i)
    for (int j=0; j<y.getDegree(); ++j)
      c[i+j] += x.coeff()[i] * y.coeff()[i];
  return Polynomial<NT>(d, c);
}
template <typename NT>
inline
Polynomial<NT>& Polynomial<NT>::operator*=(const Polynomial<NT>& rhs) {
  *this = *this * rhs; return *this;
}

// inline functions
template <typename NT>
inline bool zeroP(const Polynomial<NT>& p)
{ return p.getTrueDegree() == -1; }
template <typename NT>
inline bool unitP(const Polynomial<NT>& p)
{ return p.getTrueDegree() == 0 && p.coeff()[0] == 1; }

// stream i/o
template <typename NT>
std::ostream& operator<<(std::ostream& o, const Polynomial<NT>& p) {
  o <<   "Polynomial<NT> ( deg = " << p.getDegree() ;
  if (p.getDegree() >= 0) {
    o << "," << std::endl;
    o << ">  coeff c0,c1,... = " << p.coeff()[0];
    for (int i=1; i<= p.getDegree(); i++)
      o << ", " <<  p.coeff()[i] ;
  }
  o << ")" << std::endl;
  return o;
}

CORE_END_NAMESPACE

#endif /*__CORE_POLY_H__*/
