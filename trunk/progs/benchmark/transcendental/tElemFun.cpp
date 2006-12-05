
// This program MUST be run at CORE_LEVEL 3:
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

  Timer2 timer;
  timer.start();
  Expr SIN = sin(Expr("0.7"));
  SIN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute sin(0.7) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "sin(0.7)  = " << SIN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  timer.start();
  Expr COS = cos(Expr("0.81"));
  COS.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute cos(0.7) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "cos(0.7)  = " << COS << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  timer.start();
  Expr TAN = tan(Expr("0.7"));
  TAN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute tan(0.7) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "tan(0.7)  = " << TAN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  timer.start();
  Expr COT = cot(Expr("0.7"));
  COT.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute cot(0.7) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "cot(0.7)  = " << COT << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

/* 
  timer.start();
  Expr EXPO = expo(pi()/3);
  EXPO.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute e^(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "e^(pi/3)  = " << EXPO << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  timer.start();
  Expr ARCSIN = arcsin(pi()/6);
  ARCSIN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute arcsin(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "arcsin(pi/3)  = " << ARCSIN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  timer.start();
  Expr ARCCOS = arccos(pi()/6);
  ARCCOS.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute arccos(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "arccos(pi/3)  = " << ARCCOS << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
  timer.start();
  Expr ARCTAN = arctan(pi()/3);
  ARCTAN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute arctan(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "arctan(pi/3)  = " << ARCTAN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
*/ 
  return 0;
}
