
// This program MUST be run at CORE_LEVEL 3:
// Many Digits Friendly Competition problems test

#define CORE_LEVEL 3

#include "CORE.h"

int main( int argc, char *argv[] ) {

  prec_t prec = 100;
  int print = 0;

  if (argc > 1)
    prec = atoi(argv[1]);
  if (argc > 2)
    print = atoi(argv[2]);
 
  setDefaultOutputDigits(bits2digits(prec)); 

  // C01
  Timer2 timer;
  timer.start();
  Expr SIN = sin(tan(cos("1")));
  SIN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute sin(tan(cos(1))) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "sin(tan(cos(1)))  = " << SIN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  // C02 
  timer.start();
  Expr COS = sqrt(e() / pi());
  COS.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute sqrt(e() / pi()) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "sqrt(e() / pi())  = " << COS << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  // C03 
  timer.start();
  Expr TAN = sin(pow((e()+1), 3));
  TAN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute sin((e+1)^3) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "sin((e+1)^3) = " << TAN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  // C04 
  timer.start();
  Expr COT = exp(pi() * sqrt(2011));
  COT.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute exp(pi * sqrt(2011)) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "exp(pi * sqrt(2011))  = " << COT << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  // C05 
  timer.start();
  Expr EXPO = exp(exp(exp("1/2")));
  EXPO.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute exp(exp(exp(1/2))) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "exp(exp(exp(1/2)))  = " << EXPO << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  // C07
  timer.start();
  Expr ARCSIN = pow(pi(), 1000);
  ARCSIN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute pi^1000 to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "pi^1000  = " << ARCSIN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  // C08
  /*
  timer.start();
  Expr ARCCOS = sin(pow(6,pow(6,6)));
  ARCCOS.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "sin(6^(6^6)) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "sin(6^(6^6))  = " << ARCCOS << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
  */

  // basic test for trigonometric formulas
  // disable filter feature : trigonometric filers functions are not implemented yet.
  set_escape_bound(1000);
  
  std::cout << "test for sinx^2 + cosx^2 = 1" << std::endl;
  Expr x = pi() / 2;
  Expr expr1 = sin(x);// * sin(x);// + cos(x) * cos(x);
  expr1.approx(prec, CORE_INFTY);
  if (expr1 == Expr("1"))
    std::cout << "CORRECT!!! sinx^2 + cosx^2 = 1" << std::endl;
  else {
    std::cout << "ERROR!!! sinx^2 + cosx^2 != 1" << std::endl;
    std::cout << "sinx^2 + cosx^2 - 1 = " << expr1 - 1<< std::endl;
  }

  return 0;
}
