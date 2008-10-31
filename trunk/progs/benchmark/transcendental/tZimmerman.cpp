

// This program MUST be run at CORE_LEVEL 3:
// Many Digits Friendly Competition problems test

#define CORE_LEVEL 3
using namespace std;

#include <CORE.h>

void computeexp(Expr e, prec_t prec) {
  cout << "exp(" << e << ") = " << exp(e).approx(prec, CORE_INFTY) << endl;
}

int main( int argc, char *argv[] ) {

  prec_t prec = 200;	// relative digits
  int digits;
  int print = 0;

  if (argc > 1) {
    digits = atoi(argv[1]);
    prec = digits2bits(digits);
  }
  if (argc > 2)
    print = atoi(argv[2]);
 
  setDefaultRelPrecision(prec); 
  setDefaultOutputDigits(70);

  computeexp(Expr("9.407822313572878" / Expr("100")), prec);
  computeexp(Expr("9.999999999999995" / pow(Expr("10"), 16)), prec);
  computeexp(Expr("0.5091077534282133") , prec);
  computeexp(Expr("0.7906867968553504") , prec);
  computeexp(Expr("0.1548443067391468" / Expr("100")) , prec);
  computeexp(Expr("0.2953379504777270") , prec);
  computeexp(Expr("6.581539478341669" / Expr("1000000000")) , prec);
  computeexp(Expr("2.662858264545929" / Expr("100000000")) , prec);
  computeexp(Expr("3.639588333766983" / Expr("100000000")) , prec);

  return 0;
}
