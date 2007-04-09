#include <CORE/poly/Poly.h>
#include <CORE/BigInt.h>
#include <CORE/BigRat.h>
#include <CORE/BigFloat.h>
#include <iostream>

using namespace CORE;

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

int main() {
  NT coeff1[5] = {1, 2, 3, 4, 5};
  NT coeff2[9] = {9, 8, 7, 6, 5, 4, 3, 2, 1};

  PolyNT P1(4, coeff1);
  PolyNT P2(8, coeff2);

  std::cout << P1 << std::endl;
  std::cout << P2 << std::endl;

  std::cout << "P1+P2=" << (P1+P2) << std::endl;
  std::cout << "P1-P2=" << (P1-P2) << std::endl;
  std::cout << "P2-P1=" << (P2-P1) << std::endl;
  std::cout << "P1*P2=" << (P1*P2) << std::endl;
  
  BigInt x(100);
  BigRat y(100);
  BigFloat z(100);
  
  std::cout << "x.rc=" << x.get_rc() << std::endl;
  std::cout << "y.rc=" << y.get_rc() << std::endl;
  std::cout << "z.rc=" << z.get_rc() << std::endl;
  return 0;
}
