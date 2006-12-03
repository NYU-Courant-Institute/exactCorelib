
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
  Expr PI = pi();
  PI.approx(prec, CORE_INFTY);
  timer.stop();

  std::cout << "compute pi to " << prec << " relative precision" << std::endl;
  std::cout << "it took " << timer.get_mseconds() << " mseconds." << std::endl;
  //Expr E = e();
  //E.approx(prec, CORE_INFTY);
  //std::cout << "e  = " << E << std::endl;
 
  return 0;
}
