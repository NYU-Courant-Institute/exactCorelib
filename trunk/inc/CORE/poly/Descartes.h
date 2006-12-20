#ifndef CORE_DESCARTES_H
#define CORE_DESCARTES_H

#include "CORE/poly/Sturm.h"
#include "CORE/poly/composePoly.h"
#include "CORE/poly/composeBiPoly.h"

CORE_BEGIN_NAMESPACE

template <class NT>
class Descartes {
private:
  Polynomial<NT> _poly;
public:
  template<class T>
  Descartes(Polynomial<T> p) : _poly(p){
    if (p.getTrueDegree() < 0) return;
    _poly.sqFreePart();
  }

  void isolateRoots(const BigFloat &x, const BigFloat &y,
                    BFVecInterval &v) {
    int n = numberOfRoots(x,y);
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

  int numberOfRoots(const BigFloat &x, const BigFloat &y) {
    return signVariationofCoeff(moebiusTransform(_poly, y, x, 1, 1));
  }

  BFInterval refine(const BFInterval& I, int aprec) {
    return refine(I.first, I.second, aprec);
  }

  BFInterval refine(const BigFloat &x, const BigFloat &y, int aprec) {
    assert(x<=y);
    BFInterval retI(std::make_pair(x, y));
    BigFloat eps = BigFloat::exp2(-aprec);
    BigFloat mid;

    while (retI.second - retI.first > eps) {
      mid.div2(retI.second + retI.first);
      sign_t midsign = sign(evalExactSign(_poly,mid));
      if (midsign == 0) {
        retI.first = retI.second = mid;
        return retI;
      }
      if (sign(evalExactSign(_poly, retI.first)) * midsign < 0) {
        retI.second = mid;
      } else {
        retI.first = mid;
      }
    }
    return retI;
  } 

private:    
  Polynomial<NT>
  moebiusTransform (const Polynomial<NT>& _poly,
                    const BigFloat& a, const BigFloat& b,
                    const BigFloat& c, const BigFloat& d) {
    NT coeff[2];

    coeff[0] = a; coeff[1] = b;
    Polynomial<NT> bXplusa(1, coeff);

    coeff[0] = c; coeff[1] = d;
    Polynomial<NT> dXplusc(1, coeff); 

    return composeHornerBinary(_poly, bXplusa, dXplusc);
  }

  int signVariationofCoeff(const Polynomial<NT> p) {
    int deg = p.getTrueDegree();
    int count = 0;
    int last_sign = sign(p.coeff()[deg]);
    for (int i=deg-1; i>=0; i--) {
      if (sign(p.coeff()[i]) * last_sign < 0) {
        count++;
        last_sign *= -1;
      }
    }
    return count;
  }

};

CORE_END_NAMESPACE

#endif
