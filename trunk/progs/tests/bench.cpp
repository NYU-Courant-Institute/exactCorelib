/* ************************************************
   File: bench.cpp

   Purpose:  Bench

   Usage:
        % bench [number of iterations]

   Author: Sylvain Pion, August 2002.

   Since Core Library 1.5
   $Id: bench.cpp,v 1.1 2006-03-07 04:51:26 exact Exp $
 ************************************************ */        

#include <iostream>
#include <cmath>

#define CORE_LEVEL 4
#include "CORE.h"

// Verifies that 0 == (sqrt(x)+sqrt(y))-sqrt(x+y-2*sqrt(x*y))
void bench_1(const Expr &x, const Expr &y)
{
  Expr e = (sqrt(x) + sqrt(y)) - sqrt(x + y + 2*sqrt(x*y));
  sign(e); // force to evaluate
  assert(e == 0); 
  // Expr e = (sqrt(x) + sqrt(y)) + sqrt(x + y + 2*sqrt(x*y));
  // assert(e != 0);

  // Print the root bound of e
  std::cerr << "root bound = " << e.Rep()->computeBound() << std::endl;
}

void bench_2(const Expr &x, const Expr &y)
{
  Expr c = (sqrt(x) - sqrt(y)) / (x - y);
  Expr e = c-c;
  sign(e); // force to evaluate
  assert(e == 0); 

  // Print the root bound of e
  std::cerr << "root bound = " << e.Rep()->computeBound() << std::endl;
}

void experiment_1(int loops) {
  for (int i=0; i<loops; i++) {
    bench_1(0.666666789, 0.987654321);
    bench_1(0.524556789, 0.687279321);
    bench_1(0.799856789, 0.724681221);
    bench_1(0.823956789, 0.880004321);
    bench_1(0.666666789, 0.987654321);
    bench_1(0.524556789, 0.687279321);
    bench_1(0.799856789, 0.724681221);
    bench_1(0.823956789, 0.880004321);
    bench_1(0.799856789, 0.724681221);
    bench_1(0.823956789, 0.880004321);
  }
}

void experiment_2(int loops) {
  for (int i=0; i<loops; i++) {
    bench_2(0.666666789, 0.987654321);
    bench_2(0.524556789, 0.687279321);
    bench_2(0.799856789, 0.724681221);
    bench_2(0.823956789, 0.880004321);
    bench_2(0.666666789, 0.987654321);
    bench_2(0.524556789, 0.687279321);
    bench_2(0.799856789, 0.724681221);
    bench_2(0.823956789, 0.880004321);
    bench_2(0.799856789, 0.724681221);
    bench_2(0.823956789, 0.880004321);
  }
}

int main(int argc, char **argv)
{
  int loops = (argc < 2) ? 10 : atoi(argv[1]);

  std::cout << "Doing " << loops << " iterations." << std::endl;

  // experiment_1(loops);

  experiment_2(loops);

  return 0;
}
