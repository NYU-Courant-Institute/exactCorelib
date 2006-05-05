#ifndef __CORE_POLYFUN_H__
#define __CORE_POLYFUN_H__

//#include <CORE/poly/Poly.h>
//#include <CORE/BigFloat.h>
//#include <CORE/Expr.h>
#include <CORE/Promote.h>

CORE_BEGIN_NAMESPACE
typedef long extLong;

///Various forms of evaluation. Some specific to the type of the point of evaluation.
//{@

/// Evaluation Function (generic version, always returns the exact value).
///
///  This evaluation function is easy to use, but may not be efficient
///  when you have BigRat or Expr values.
///
/// User must be aware that the return type of eval is Max of Types NT and T.
///
/// E.g., If NT is BigRat, and T is Expr then Max(NT,T)=Expr. 
/// 	
/// REMARK: If NT is BigFloat, it is assumed that the BigFloat is error-free.  
template <typename NT, typename T>
MAX_TYPE(NT, T) eval(Polynomial<NT> &p,const T& f) {	// evaluation
  typedef MAX_TYPE(NT, T) ResultT;
  int deg = p.getTrueDegree();
  if (deg == -1)
    return ResultT(0);
  if (deg == 0)
    return ResultT(p.coeff()[0]);
  ResultT val(0);
  for (int i=deg; i>=0; i--) {
    val *= ResultT(f);
    val += ResultT(p.coeff()[i]);	
  }
  return val;
}//eval


/// Approximate Evaluation of Polynomials
/// 	the coefficients of the polynomial are approximated to some
///	specified composite precision (r,a).
/// @param  f evaluation point 
/// @param  r relative precision to which the coefficients are evaluated
/// @param  a absolute precision to which the coefficients are evaluated
/// @return a BigFloat with error containing value of the polynomial.
///     If zero is in this BigFloat interval, then we don't know the sign.
//
// 	ASSERT: NT = BigRat or Expr
//
template <class NT>
BigFloat2 evalApprox(Polynomial <NT> &p, const BigFloat& f, 
	const extLong& r, const extLong& a) {	// evaluation
  int deg = p.getTrueDegree();
  
  if (deg == -1)
    return BigFloat2(0);
  if (deg == 0)
    return BigFloat(p.coeff()[0], r);

  BigFloat2 val(0), c;
  for (int i=deg; i>=0; i--) {
    c = BigFloat2(p.coeff()[i], r);	
    val *= f; 
    val += c;
  }
  return val;
}//evalApprox

// This BigInt version of evalApprox should never be called...
template <>
BigFloat2 evalApprox( Polynomial<BigInt> &p, const BigFloat& f,
	const extLong& r, const extLong& a) {	// evaluation
  assert(0);
  return BigFloat2(0);
}



/**
 * Evaluation at a BigFloat value
 * using "filter" only when NT is BigRat or Expr.
 * Using filter means we approximate the polynomials
 * coefficients using BigFloats.  If this does not give us
 * the correct sign, we will resort to an "exact" evaluation
 * using Expr.
 *
 * If NT <= BigFloat, we just call eval().
 *
   We use the following heuristic estimates of precision for coefficients:

      r = 1 + lg(|P|_\infty) + lg(d+1)  		if f <= 1
      r = 1 + lg(|P|_\infty) + lg(d+1) + d*lg|f| 	if f > 1
      
   if the filter fails, then we use Expr to do evaluation.

   This function is mainly called by Newton iteration (which
   has some estimate for oldMSB from previous iteration).

   @param p polynomial to be evaluated
   @param val the evaluation point
   @param oldMSB an rough estimate of the lg|p(val)|
   @return bigFloat interval contain p(val), with the correct sign

 ***************************************************/
template <class NT>
BigFloat evalExactSign(Polynomial<NT> &p, const BigFloat& val,
	 const extLong& oldMSB) {
    if (p.getTrueDegree() == -1)
      return BigFloat(0);
  
    extLong r;
    r = 1 + BigFloat(p.height()).uMSB() + ceilLg(long(p.getTrueDegree()+1));
    if (val > 1)
      r += p.getTrueDegree() * val.uMSB();
    r += std::max(extLong(0), -oldMSB);
  
    if (hasExactDivision<NT>::check()) { // currently, only to detect NT=Expr and NT=BigRat
        BigFloat2 rVal = p.evalApprox(val, r);
        if (rVal.isZeroIn()) {
	  Expr eVal = p.eval(Expr(val));	// eval gives exact value
	  eVal.approx(54,CORE_INFTY);  // if value is 0, we get exact 0
	  return eVal.BigFloatValue();
	} else 
          return rVal;
    } else
	return BigFloat(eval(val));

   return 0;
  }//evalExactSign
  
//@}

/// Bounds
//{@

/// Cauchy Upper Bound on Roots.
// -- ASSERTION: NT is an integer type
template < class NT >
BigFloat CauchyUpperBound(Polynomial<NT> &p) {
  if (zeroP(p))
    return BigFloat(0);
  NT mx = 0;
  int deg = p.getTrueDegree();
  for (int i = 0; i < deg; ++i) {
    mx = core_max(mx, abs(p.coeff()[i]));
  }
  Expr e = mx;
  e /= Expr(abs(p.coeff()[deg]));
  e.approx(CORE_INFTY, 2);
  // get an absolute approximate value with error < 1/4
  return (e.BigFloatValue() + 2);
}


/// An iterative version of computing Cauchy Bound from Erich Kaltofen.
// See the writeup under collab/sep/.
template < class NT >
BigInt CauchyBound(Polynomial<NT> &p) {
  int deg = p.getTrueDegree();
  BigInt B(1);
  BigFloat lhs(0), rhs(1);
  while (true) {
    /* compute \sum_{i=0}{deg-1}{|a_i|B^i} */
    lhs = 0;
    for (int i=deg-1; i>=0; i--) {
      lhs *= B;
      lhs += abs(p.coeff()[i]);
    }
    //lhs /= abs(p.coeff()[deg]);
    //lhs.makeFloorExact();
    /* compute B^{deg} */
    if (rhs * abs(p.coeff()[deg]) <= lhs) {
      B <<= 1;
      rhs *= (BigInt(1)<<deg);
    } else
      break;
  }
  return B;
}


///Another iterative bound which is at least as good as the above bound
///by Erich Kaltofen.
template < class NT >
BigInt UpperBound(Polynomial<NT> &p) {
  int deg = p.getTrueDegree();

  BigInt B(1);
  BigFloat lhsPos(0), lhsNeg(0), rhs(1);
  while (true) {
    /* compute \sum_{i=0}{deg-1}{|a_i|B^i} */
    lhsPos = lhsNeg = 0;
    for (int i=deg-1; i>=0; i--) {
      if (p.coeff()[i]>0) {
      	lhsPos = lhsPos * B + p.coeff()[i];
      	lhsNeg = lhsNeg * B;
      } else {
      	lhsNeg = lhsNeg * B - p.coeff()[i];
      	lhsPos = lhsPos * B;
      } 
    }

    /*lhsNeg /= abs(p.coeff()[deg]);
    lhsPos /= abs(p.coeff()[deg]);
    lhsPos.makeCeilExact();
    lhsNeg.makeCeilExact();*/
    //We can avoid the above steps by multiplying rhs by the leading coefficient
    //and then compare the result.
    /* compute B^{deg} */
    if (rhs * abs(p.coeff()[deg]) <= max(lhsPos,lhsNeg)) {
      B <<= 1;
      rhs *= (BigInt(1)<<deg);
    } else
      break;
  }
  return B;
}

// Cauchy Lower Bound on Roots
// -- ASSERTION: NT is an integer type
template < class NT >
BigFloat CauchyLowerBound(Polynomial<NT> &p) {
  if ((zeroP(p)) || p.coeff()[0] == 0)
    return BigFloat(0);
  NT mx = 0;
  int deg = p.getTrueDegree();
  for (int i = 1; i <= deg; ++i) {
    mx = core_max(mx, abs(p.coeff()[i]));
  }
  Expr e = Expr(abs(p.coeff()[0]))/ Expr(abs(p.coeff()[0]) + mx);
  e.approx(2, CORE_INFTY);
  // get an relative approximate value with error < 1/4
  return (e.BigFloatValue().div2());
}

// Separation bound for polynomials that may have multiple roots.
// We use the Rump-Schwartz bound.
//
//    ASSERT(the return value is an exact BigFloat and a Lower Bound)
//
template < class NT >
BigFloat sepBound(Polynomial<NT> &p) {
  BigInt d;
  BigFloat2 e;
  int deg = p.getTrueDegree();

  CORE::power(d, BigInt(deg), ((deg)+4)/2);
  e = CORE::power(p.height()+1, deg);
  e.makeCeilExact(); // see NOTE below
  return BigFloat((BigFloat2(1)/(e*2*d)).makeFloorExact());
        // BUG fix: ``return 1/e*2*d'' was wrong
        // NOTE: the relative error in this division (1/(e*2*d))
        //   is defBFdivRelPrec (a global variable), but
        //   since this is always positive, we are OK.
        //   But to ensure that defBFdivRelPrec is used,
        //   we must make sure that e and d are exact.
        // Finally, using "makeFloorExact()" is OK because
        //   the mantissa minus error (i.e., m-err) will remain positive
        //   as long as the relative error (defBFdivRelPrec) is >1.
}

//@}

///Norms on Polynomials
//{@
/// height function
/// @return a BigFloat with error
template < class NT >
BigFloat2 height(Polynomial<NT> &p) {
  if (zeroP(p))
    return BigFloat(0);
  int deg = p.getTrueDegree();
  NT ht = 0;
  for (int i = 0; i< deg; i++)
    if (ht < abs(p.coeff()[i]))
      ht = abs(p.coeff()[i]);
  return BigFloat2(ht);
}

/// length function
/// @return a BigFloat with error
template < class NT >
BigFloat2 length(Polynomial<NT> &p) {
  if (zeroP(p))
    return BigFloat(0);
  int deg = p.getTrueDegree();
  NT length = 0;
  for (int i = 0; i< deg; i++)
    length += abs(p.coeff()[i]*p.coeff()[i]);
  return sqrt(BigFloat2(length));
}

//@}

CORE_END_NAMESPACE

#endif /*__CORE_POLYFUN_H__*/
