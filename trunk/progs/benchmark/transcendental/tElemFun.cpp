
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
  Expr COS = cos(Expr("0.7"));
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

  timer.start();
  Expr EXPO = expo(pi()/3);
  EXPO.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute e^(pi/3) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "e^(pi/3)  = " << EXPO << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  timer.start();
  Expr ARCSIN = arcsin("0.9");
  ARCSIN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute arcsin(0.9) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "arcsin(0.9)  = " << ARCSIN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
 
  timer.start();
  Expr ARCCOS = arccos("0.2");
  ARCCOS.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute arccos(0.2) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "arccos(0.2)  = " << ARCCOS << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  timer.start();
  Expr ARCTAN = arctan("2");
  ARCTAN.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute arctan(2) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "arctan(2)  = " << ARCTAN << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  timer.start();
  Expr LOG = log_2(pi());
  LOG.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute log2(pi) to " << prec << " relative precision" << std::endl;
  if (print > 0)
    std::cout << "log2(pi)  = " << LOG << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;

  return 0;
}
