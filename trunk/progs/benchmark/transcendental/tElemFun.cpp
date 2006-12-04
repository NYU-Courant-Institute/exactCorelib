
// This program MUST be run at CORE_LEVEL 3:
#define CORE_LEVEL 3

#include "CORE.h"

int main( int argc, char *argv[] ) {

  prec_t prec = 100;

  if (argc > 1)
    prec = atoi(argv[1]);
 
  setDefaultOutputDigits(bits2digits(prec)); 

  Timer2 timer;
  timer.start();
  Expr SIN = sin(pi()/3);
  SIN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute sin(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "sin(pi/3)  = " << SIN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  timer.start();
  Expr COS = cos(pi()/3);
  COS.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute cos(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "cos(pi/3)  = " << COS << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  timer.start();
  Expr EXPO = expo(pi()/3);
  EXPO.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute e^(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "e^(pi/3)  = " << EXPO << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
/* 
  timer.start();
  Expr TAN = tan(pi()/6);
  TAN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute tan(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "tan(pi/3)  = " << TAN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  timer.start();
  Expr COT = cot(pi()/3);
  COT.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute cot(pi/3) to " << prec << " relative precision" << std::endl;
  std::cout << "cot(pi/3)  = " << COT << std::endl;
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
