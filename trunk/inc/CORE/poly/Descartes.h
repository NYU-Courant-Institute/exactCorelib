#ifndef CORE_DESCARTES_H
#define CORE_DESCARTES_H

#include "CORE/poly/Poly.h"

CORE_BEGIN_NAMESPACE

/***************************************************
 * Descartes Class:
 *   this is modeled after the Sturm class.
 *
 * Author: Jihun Yu (Dec 2006)
 ***************************************************/

template <class NT>
class Descartes {
private:
  int len;      // len is 1 less than number of non-zero entries in array seq.
  		//     I.e., len + 1 = length of the Sturm Sequence
                // N.B. When len = -1 or len = 0 are special,
                //     the array seq is not used!
                //     Hence, one must test these special cases
  static int N_STOP_ITER;    // Stop IterE after this many iterations. This
                             // is initialized below, outside the Newton class
  Polynomial<NT> _poly;
  Polynomial<NT> _poly_derivative;
  bool NEWTON_DIV_BY_ZERO;  // this is set to true when divide by 0 in Newton
public:
  Descartes() : NEWTON_DIV_BY_ZERO(false) {}
  Descartes(Polynomial<NT> p) : _poly(p), NEWTON_DIV_BY_ZERO(false){
    len = p.getTrueDegree();
    if (len < 0) return;
    
    _poly.sqFreePart();
    _poly_derivative = differentiate(_poly);  
  }
  Descartes(Descartes& d) : _poly(d._poly), NEWTON_DIV_BY_ZERO(false),
  	len(d.len){}

  void setPoly(Polynomial<NT> p) {
    len = p.getTrueDegree();
    if (len < 0) return;

    _poly = p;
    _poly.sqFreePart();
    _poly_derivative = differentiate(_poly);  
  }


  void isolateRoots(const BigFloat &x, const BigFloat &y,
                    BFVecInterval &v) {
    int n = signVar(x,y);
    if (n==0) return;
    if (n==1)
      v.push_back(std::make_pair(x,y));
    if (n>1) {
      BigFloat mid; mid.div2(x+y);
      if (sign(evalExactSign(_poly,mid)) != 0) {
         isolateRoots(x, mid, v);
         isolateRoots(mid, y, v);
      } else {
        isolateRoots(x, mid, v);
        isolateRoots(mid, y, v);
        v.push_back(std::make_pair(mid, mid));
      }
    }
  }

  // isolateRoots(v)
  ///   isolates all roots and returns them in v
  /**   v is a vector of isolated intervals
   */
  void isolateRoots(BFVecInterval &v) {
    if (len <= 0) {
       v.clear(); return;
    }
    BigFloat bd = CauchyUpperBound(_poly);
    // Note: bd is an exact BigFloat (this is important)
    isolateRoots(-bd, bd, v);
  }

  // isolateRoot(i)
  ///   Isolates the i-th smallest root 
  ///         If i<0, isolate the (-i)-th largest root
  ///   Defaults to i=0 (i.e., the smallest positive root a.k.a. main root)
  BFInterval isolateRoot(int i = 0) {
    if (len <= 0) 
       return BFInterval(1,0);   // ERROR CONDITION
    if (i == 0)
      return mainRoot();
    BigFloat bd = CauchyUpperBound(_poly);
    return isolateRoot(i, -bd, bd);
  }

  // isolateRoot(i, x, y)
  ///   isolates the i-th smallest root in [x,y]
  /**   If i is negative, then we want the i-th largest root in [x,y]
   *    We assume i is not zero.
   */
  BFInterval isolateRoot(int i, BFInterval &I)
  { return isolateRoot(i, I.first, I.second); }

  BFInterval isolateRoot(int i, const BigFloat& x, const BigFloat& y) {
    BFVecInterval v;

    isolateRoots(x, y, v);
    int n = v.size();

    if (i < 0) {
      i += n + 1;
      if (i <= 0)
        return BFInterval(1,0);
    }
 
    if (i > n)
      return BFInterval(1,0);

    //Now 0 < i <= n
    if (n==1)
      return *(v.begin());

    if (i == n)
      return (*(v.end()-1));

    int count = 1;
    for (BFVecInterval::const_iterator it = v.begin(); ; ++it) {
      if (count == i)
        return (*it);
      else
        count++;
    }
  }

  // same as isolateRoot(i).
  BFInterval diamond(int i) {
    return isolateRoot(i);
  }

  // First root above
  BFInterval firstRootAbove(const BigFloat &e) {
    if (len <= 0)
       return BFInterval(1,0);   // ERROR CONDITION
    return isolateRoot(1, e, CauchyUpperBound(_poly));
  }

  // Main root (i.e., first root above 0)
  BFInterval mainRoot() {
    if (len <= 0)
       return BFInterval(1,0);   // ERROR CONDITION
    return isolateRoot(1, 0, CauchyUpperBound(_poly));
  }

  // First root below
  BFInterval firstRootBelow(const BigFloat &e) {
    if (len <= 0)
       return BFInterval(1,0);   // ERROR CONDITION
    BigFloat bd = CauchyUpperBound(_poly); // bd is exact
    int n = numberOfRoots(-bd, e);
    if (n <= 0)
      return BFInterval(1,0);
    //BigFloat bdBF = BigFloat(ceil(bd));
    BigFloat bdBF;
    bdBF.ceil(bd);
    if (n == 1)
      return BFInterval(-bdBF, e);
    return isolateRoot(n, -bdBF, e);
  }
  
  int signVar(BFInterval& I)
  { return signVar(I.first, I.second); }
  
  int signVar(const BigFloat &x, const BigFloat &y) {
    return signVariationofCoeff(
           moebiusTransform(_poly, x, y, BigFloat(1), BigFloat(1))
	   );
  }

  int numberOfRoots(BFInterval& I)
  { return numberOfRoots(I.first, I.second); }

  int numberOfRoots(const BigFloat &x, const BigFloat &y) {
    BFVecInterval v;
    isolateRoots(x, y, v);
    return v.size();
  }

  BFInterval refine(const BFInterval& I, int aprec) {
    return refine(I.first, I.second, aprec);
  }

  BFInterval refine(const BigFloat &x, const BigFloat &y, int aprec) {
    assert(x<=y);
    BFInterval retI(std::make_pair(x, y));
    BigFloat eps = BigFloat::exp2(-aprec);
    BigFloat mid;

    sign_t x_sign = sign(evalExactSign(_poly, retI.first));

    while (retI.second - retI.first > eps) {
      mid.div2(retI.second + retI.first);
      sign_t mid_sign = sign(evalExactSign(_poly,mid));
      if (mid_sign == 0) {
        retI.first = retI.second = mid;
        return retI;
      }
      if (x_sign * mid_sign < 0) {
        retI.second = mid;
      } else {
        retI.first = mid;
        x_sign = mid_sign;
      }
    }
    return retI;
  }

  void refineAllRoots(BFVecInterval &v, int aprec) {

    BFVecInterval v1;
    BFInterval J;

    if (v.empty())
      isolateRoots(v);

    for (BFVecInterval::iterator it = v.begin();
         it != v.end(); ++it) {        // Iterate through all the intervals
      //refine them to the given precision aprec
      J = refine(*it, aprec);
      if (NEWTON_DIV_BY_ZERO) {
        J.first = 1;
        J.second = 0;   // indicating divide by zero
      }
      v1.push_back(std::make_pair(J.first, J.second));
    }
    v.swap(v1);
  }//refineAllRoots

  // This is the new version of "refineAllRoots"
  //    	based on Newton iteration
  // It should be used instead of refineAllRoots!
  void newtonRefineAllRoots( BFVecInterval &v, int aprec) {

    BFVecInterval v1;
    BFInterval  J;

    if (v.empty())
      isolateRoots(v);

    for (BFVecInterval::iterator it = v.begin();
         it != v.end(); ++it) {        // Iterate through all the intervals
      //refine them to the given precision aprec
      J = newtonRefine(*it, aprec);
      if (NEWTON_DIV_BY_ZERO) {
        J.first = 1;
        J.second = 0;   // indicating divide by zero
      }
      v1.push_back(std::make_pair(J.first, J.second));
    }
    v.swap(v1);
  }//End of newtonRefineAllRoots

  /** val = newtonIterN(n, bf, del, err, fuMSB, ffuMSB)
   * 
   *    val is the root after n iterations of Newton
   *       starting from initial value of bf and is exact.
   *    fuMSB and ffuMSB are precision parameters for the approximating
   *		the coefficients of the underlyinbg polynomial, f(x).
   *    	THEY are used ONLY if the coefficients of the polynomial
   *		comes from a field (in particular, Expr or BigRat).
   *		We initially approximate the coefficients of f(x) to fuMSB 
   *		relative bits, and f'(x) to ffuMSB relative bits.
   *		The returned values of fuMSB and ffuMSB are the final
   *		precision used by the polynomial evaluation algorithm.
   *    Return by reference, "del" (difference between returned val and value
   *       in the previous Newton iteration)
   *
   *    Also, "err" is returned by reference and bounds the error in "del".
   *
   *    IMPORTANT: we assume that when x is an exact BigFloat,
   *    then Polynomial<NT>::eval(x) will be exact!
   *    But current implementation of eval() requires NT <= BigFloat.
   * ****************************************************/    

  BigFloat newtonIterN(long n, const BigFloat& bf, BigFloat& del,
	BigFloat& err, extLong& fuMSB, extLong& ffuMSB) {
    if (len <= 0) return bf;   // Nothing to do!  User must
                               // check this possibility!
    BigFloat val = bf;  

    // newton iteration
    for (int i=0; i<n; i++) {
      ////////////////////////////////////////////////////
      // Filtered Eval
      ////////////////////////////////////////////////////
      BigFloat2 ff = evalExactSign(_poly_derivative,val, 3*ffuMSB); //3 is a slight hack
      ffuMSB = ff.uMSB();
      //ff is guaranteed to have the correct sign as the exact evaluation.
      ////////////////////////////////////////////////////

      if (ff.sgn() == 0) {
        NEWTON_DIV_BY_ZERO = true;
        del = 0;
        core_error("Zero divisor in Newton Iteration",
                __FILE__, __LINE__, false);
        return 0;
      }

      ////////////////////////////////////////////////////
      // Filtered Eval
      ////////////////////////////////////////////////////
      BigFloat2 f= evalExactSign(_poly, val, 3*fuMSB); //3 is a slight hack
      fuMSB = f.uMSB();
      ////////////////////////////////////////////////////

      if (f.sgn() == 0) {
        NEWTON_DIV_BY_ZERO = false;
        del = 0;    // Indicates that we have reached the exact root
		    //    This is because eval(val) is exact!!!
        return val; // val is the exact root, before the last iteration
      }
      del = (f/ff).getLeft(); // But the accuracy of "f/ff" must be controllable
		    // by the caller...
      err = BigFloat(del,getDefaultBFdivPrec());
      val -= del;
    }
    return val;
  }//newtonIterN

  //Another version of newtonIterN which does not return the error 
  //and passing the uMSB as arguments; it is easier for the user to call
  //this.
  BigFloat newtonIterN(long n, const BigFloat& bf, BigFloat& del){
    BigFloat err;
    extLong fuMSB=0, ffuMSB=0;
    return newtonIterN(n, bf, del, err, fuMSB, ffuMSB);
  }

  // v = newtonIterE(prec, bf, del, fuMSB, ffuMSB)
  //
  //    return the value v which is obtained by Newton iteration
  //    until del.uMSB < -prec, starting from initial value of bf.
  //    Returned value is an exact BigFloat.
  //    We guarantee at least one Newton step (so del is defined).
  //
  //	   The parameters fuMSB and ffuMSB are precision parameters for
  //	   evaluating coefficients of f(x) and f'(x), used similarly
  //	   as described above for newtonIterN(....)
  //
  //    Return by reference "del" (difference between returned val and value
  //       in the previous Newton iteration).  This "del" is an upper bound
  //       on the last (f/f')-value in Newton iteration.
  //
  //    IN particular, if v is in the Newton zone of a root z^*, then z^* is
  //       guaranteed to lie inside [v-del, v+del].
  //
  //    Note that this is dangerous unless you know that bf is already
  //       in the Newton zone.  So we use the global N_STOP_ITER to
  //       prevent infinite loop.

  BigFloat newtonIterE(int prec, const BigFloat& bf, BigFloat& del, 
	extLong& fuMSB, extLong& ffuMSB) {
    // usually, prec is positive
    int count = N_STOP_ITER; // upper bound on number of iterations
    int stepsize = 1;
    BigFloat val = bf;
    BigFloat err = 0;

    do {
      val = newtonIterN(stepsize, val, del, err, fuMSB, ffuMSB);
      count -= stepsize;
      stepsize++; // heuristic
    } while ((del != 0) && ((del.uMSB() >= -prec) && (count >0))) ;

    if (count == 0) core_error("newtonIterE: reached count=0",
		    	__FILE__, __LINE__, true);
    //del = BigFloat(core_abs(del.m()), err, del.exp() );
    //del.makeCeilExact();
    del += err;
    return val;
  }

  //Another version of newtonIterE which avoids passing the uMSB's.
  BigFloat newtonIterE(int prec, const BigFloat& bf, BigFloat& del){
    extLong fuMSB=0, ffuMSB=0;
    return newtonIterE(prec, bf, del, fuMSB, ffuMSB);
  }


  //newtonRefine(J, a) 
  //
  //    ASSERT(J is an isolating interval for some root x^*)
  //
  //    ASSERT(J.first and J.second are exact BigFloats)
  //
  //    Otherwise, the boundaries of the interval are not well defined.
  //    We will return a refined interval with exact endpoints,
  //    still called J, containing x^* and
  //
  // 			|J| < 2^{-a}.
  //
  // 	TO DO: write a version of newtonRefine(J, a, sign) where
  // 	sign=J.first.sign(), since you may already know the sign
  // 	of J.first.  This will skip the preliminary stuff in the
  // 	current version.
  //
  BFInterval newtonRefine(BFInterval &J, int aprec) {

#ifdef CORE_DEBUG_NEWTON
    std::cout << "In newtonRefine, input J=" << J.first
	<< ", " << J.second << " precision = " << aprec << std::endl;
#endif

    if (len <= 0) return J;   // Nothing to do!  User must
                               // check this possibility!
      

    if((J.second - J.first).uMSB() < -aprec){
      return (J);
    }
    int xSign, leftSign, rightSign;

    leftSign = sign(evalExactSign(_poly, J.first));
    if (leftSign == 0) {
      J.second = J.first;
      return J;
    }

    rightSign = sign(evalExactSign(_poly, J.second));
    if (rightSign == 0) {
      J.first = J.second;
      return J;
    }

    assert( leftSign * rightSign < 0 );

    //N is number of times Newton is called without checking
    // whether the result is still in the interval or not
    #define NO_STEPS 2
    // REMARK: NO_STEPS=1 is incorrect, as it may lead to
    //      linear convergence (it is somewhat similar to Dekker-Brent's
    //      idea of guaranteeing that bisection does not
    //	    destroy the superlinear convergence of Newton.
    int N = NO_STEPS;

    BigFloat x, del, olddel, temp;
    BigFloat err;
    BigFloat yap = yapsBound(_poly);

    BigFloat old_width = J.second - J.first;
    x.div2(J.second + J.first);

    // initial estimate for the evaluation of filter to floating point precision
    extLong fuMSB=54, ffuMSB=54;

    //MAIN WHILE LOOP. We ensure that J always contains the root

    while ( !smaleBoundTest(_poly, _poly_derivative, x) && 
	    (J.second - J.first) > yap &&
	   (J.second - J.first).uMSB() >= -aprec) {

     x = newtonIterN(N, x, del, err, fuMSB, ffuMSB);

      if ((del == 0)&&(NEWTON_DIV_BY_ZERO == false)) {  // reached exact root!
        J.first = J.second = x;
        return J;
      }

      BigFloat left(x), right(x);
      if (del>0) {
      	left -= del; right += del;
      } else {
      	left += del; right -= del;
      }

      // update interval
      if ((left > J.first)&&(left <J.second)) {
	  int lSign = sign(evalExactSign(_poly, left));
          if (lSign == leftSign)  // leftSign=sign of J.first
            J.first = left;
	  else if (lSign == 0) {
            J.first = J.second = left;
            return J;
          } else {
	    J.second = left;
          }	
      }
      if ((right < J.second)&&(right >J.first)) {
	  int rSign = sign(evalExactSign(_poly, right));
          if (rSign == rightSign)
            J.second = right;
	  else if (rSign == 0) {
            J.first = J.second = right;
            return J;
          } else {
            J.first = right;
          }
      }
      BigFloat width = J.second - J.first;

      //left and right are exact, since x is exact.
      if (width*2 <= old_width && !NEWTON_DIV_BY_ZERO) {
                                  // we can get a better root:

	// No, it is not necessary to update x to
	// the midpoint of the new interval J.
	// REASON: basically, it is hard to be smarter than Newton's method!
	// Newton might bring x very close to one endpoint, but it can be
	// because the root is near there!  In any case,
	// by setting x to the center of J, you only gain at most
	// one bit of accuracy, but you stand to loose an
	// arbitrary amount of bits of accuracy if you are unlucky!
	// So I will comment out the next line.  --Chee (Aug 9, 2004).
	// 
	// x = (J.second + J.first).div2();
	if (J.first > x || J.second < x)
	  x.div2(J.second + J.first);

	old_width = width; // update width

        N ++;      // be more confident or aggressive
	           //  (perhaps we should double N)
		   //
      } else {// Either NEWTON_DIV_BY_ZERO=true
	      // Or width has not decreased sufficiently
	x.div2(J.second + J.first);//Reset x to midpoint since it was the
	                                //value from a failed Newton step
	xSign = sign(evalExactSign(_poly, x));
	if (xSign == rightSign) {
	  J.second = x;
	} else if (xSign == leftSign) {
	  J.first = x;
	} else { // xSign must be 0
	  J.first = J.second = x; return J;
	}
	x.div2(J.second + J.first);

	old_width.div2(old_width); // update width
	
	// reduce value of N:
	N = std::max(N-1, NO_STEPS);   // N must be at least NO_STEPS
      }
    }//MAIN WHILE LOOP

    if((J.second - J.first).uMSB() >= -aprec){ // The interval J
                    //still hasn't reached the required precision.
                    //But we know the current value of x (call it x_0)
		    //is in the strong Newton basin of the
		    //root x^* (because it passes Smale's bound)
      //////////////////////////////////////////////////////////////////
      //Both x_0 and the root x^* are in the interval J.
      //Let NB(x^*) be the strong Newton basin of x^*.  By definition,
      //x_0 is in NB(x^*) means that:
      //
      //    x_0 is in NB(x^*) iff |x_n-x^*| \le 2^{-2^{n}+1} |x_0-x^*|
      //    
      // where x_n is the n-th iterate of Newton.  
      //    
      //  LEMMA 1: if x_0  \in NB(x^*) then 
      //               |x_0 - x^*| <= 2|del|      (*)
      //  and
      //               |x_1 - x^*| <= |del|       (**)
      //
      //  where del = -f(x_0)/f'(x_0) and x_1 = x_0 + del
      //Proof:
      //Since x_0 is in the strong Newton basin, we have
      //         |x_1-x^*| <= |x_0-x^*|/2.        (***)
      //The bound (*) is equivalent to
      //         |x_0-x^*|/2 <= |del|.
      //This is equivalent to
      //         |x_0-x^*| - |del| <= |x_0-x^*|/2,
      //which follows from
      //         |x_0-x^* + del| <= |x_0-x^*|/2,
      //which is equivalent to (***).  
      //The bound (**) follows from (*) and (***).
      //QED
      //
      //  COMMENT: the above derivation refers to the exact values,
      //  but what we compute is X_1 where X_1 is an approximation to
      //  x_1.  However, let us write X_1 = x_0 - DEL, where DEL is
      //  an approximation to del.  
      //
      //  LEMMA 2:  If |DEL| >= |del|,
      //  then (**) holds with X_1 and DEL in place of x_1 and del.
      //  
      //  NOTE: We implemented this DEL in newtonIterE.   

#ifdef CORE_DEBUG
      std::cout << "Inside Newton Refine: Refining Part " << std::endl;

      if((J.second - J.first) > yap)
	std::cout << "Smales Bound satisfied " << std::endl;
      else
	std::cout << "Chees Bound satisfied " << std::endl;
#endif
      xSign = sign(evalExactSign(_poly, x));
      if(xSign == 0){
	J.first = J.second = x; 
	return J; // missing before!
      }

      //int k = clLg((-(J.second - J.first).lMSB() + aprec).asLong());
      x = newtonIterE(aprec, x, del, fuMSB, ffuMSB);
      xSign = sign(evalExactSign(_poly, x));

      if(xSign == leftSign){//Root is greater than x
	J.first = x;
	J.second = x + abs(del);  // justified by Lemma 2 above
      }else if(xSign == rightSign){//Root is less than x
	J.first = x - abs(del);   // justified by Lemma 2 above
	J.second = x ;
      }else{//x is the root
	J.first = J.second = x;
      }
    }



    if (evalExactSign(_poly,J.first) * evalExactSign(_poly,J.second) > 0)
      std::cout <<" ERROR! Root is not in the Interval " << std::endl;
    if(J.second - J.first >  BigFloat::exp2(-aprec))
      std::cout << "ERROR! Newton Refine failed to achieve desired precision" << std::endl;

      return(J);
 }//End of newton refine
}; //class Descartes

template <class NT>
int Descartes<NT>::N_STOP_ITER = 10000;


// testNewtonDescartes( Poly, aprec, n)
//   will run the Newton-Descartes refinement to isolate the roots of Poly
//         until absolute precision aprec.
//   n is the predicated number of roots
//      (will print an error message if n is wrong)
template<class NT>
inline void testNewtonDescartes(const Polynomial<NT>&P, int prec, int n = -1) {
  Descartes<NT> D (P);
  BFVecInterval v;
  D.newtonRefineAllRoots(v, prec);
//  D.refineAllRoots(v, prec);
  std::cout << "   Number of roots is " << v.size();
  if ((n >= 0) & (v.size() == (unsigned)n))
    std::cout << " (CORRECT!)" << std::endl;
  else
    std::cout << " (ERROR!) " << std::endl;

  int i = 0;
  for (BFVecInterval::iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
    if(it->second - it->first <= (BigFloat(1)/power(BigFloat(2), prec)))
      std::cout << " (CORRECT!) Precision attained" << std::endl;
    else
      std::cout << " (ERROR!) Precision not attained" << std::endl;
  }
}// testNewtonDescartes

template<class NT>
inline int signVar(Polynomial<NT> poly, BFInterval I) {
  return signVariationofCoeff(
         moebiusTransform(poly, I.first, I.second, BigFloat(1), BigFloat(1))
  );
}

CORE_END_NAMESPACE

#endif
