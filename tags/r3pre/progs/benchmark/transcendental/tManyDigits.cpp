
// This program MUST be run at CORE_LEVEL 3:
// Many Digits Friendly Competition problems test

#define CORE_LEVEL 3
using namespace std;

#include "CORE.h"

int main( int argc, char *argv[] ) {

  prec_t prec = 100;	// relative digits
  int digits;
  int print = 0;

  if (argc > 1)
    digits = atoi(argv[1]);
  if (argc > 2)
    print = atoi(argv[2]);
 
  prec = digits2bits(digits);
  setDefaultRelPrecision(prec); 
  setDefaultOutputDigits(70);
  //setDefaultDigits(digits); 

  // C01
  Timer2 timer;
  timer.start();
  Expr SIN = sin(tan(cos("1")));
  SIN.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "compute sin(tan(cos(1))) to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "sin(tan(cos(1)))  = " << SIN << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C02 
  timer.start();
  Expr COS = sqrt(e() / pi());
  COS.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "compute sqrt(e() / pi()) to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "sqrt(e() / pi())  = " << COS << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C03 
  timer.start();
  Expr TAN = sin(pow((e()+1), 3));
  TAN.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "compute sin((e+1)^3) to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "sin((e+1)^3) = " << TAN << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
 
  // C04 
  timer.start();
  Expr COT = exp(pi() * sqrt(2011));
  COT.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "compute exp(pi * sqrt(2011)) to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "exp(pi * sqrt(2011))  = " << COT << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C05 
  timer.start();
  Expr EXPO = exp(exp(exp("1/2")));
  EXPO.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "compute exp(exp(exp(1/2))) to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "exp(exp(exp(1/2)))  = " << EXPO << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  // C07
  timer.start();
  Expr ARCSIN = pow(pi(), 1000);
  ARCSIN.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "compute pi^1000 to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "pi^1000  = " << ARCSIN << endl;

  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
 
  // C08
  /*
  timer.start();
  Expr ARCCOS = sin(pow(6,pow(6,6)));
  ARCCOS.approx(prec, CORE_INFTY);
  timer.stop();

  cout << "sin(6^(6^6)) to " << prec << " relative precision" << endl;
  if (print > 0)
    cout << "sin(6^(6^6))  = " << ARCCOS << endl;
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;
  */

  // basic test for trigonometric formulas
  // disable filter feature : trigonometric filers functions are not implemented yet.
  set_escape_bound(1000);
  
  cout << "test for sinx^2 + cosx^2 = 1" << endl;
  timer.start();
  Expr x = pi() / 2;
  Expr expr1 = sin(x);// * sin(x);// + cos(x) * cos(x);
  //expr1.approx(prec, CORE_INFTY);
  if (expr1 == Expr("1"))
    cout << "CORRECT!!! sinx^2 + cosx^2 = 1" << endl;
  else {
    cout << "ERROR!!! sinx^2 + cosx^2 != 1" << endl;
    cout << "sinx^2 + cosx^2 - 1 = " << expr1 - 1<< endl;
  }
  timer.stop();
  cout << "it took " << timer.get_mseconds() << " mseconds." << endl;

  return 0;
}
