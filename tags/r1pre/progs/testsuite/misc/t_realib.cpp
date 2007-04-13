#define CORE_LEVEL 4

#include <iostream>
#include <CORE/CORE.h>
#include <CORE/Timer.h>

using namespace std;

void bf_example1(int num) {
  Timer2 timer;

  BigFloat2 s(0.0);
  BigFloat2 one(1.0);
  timer.start();
  for (int i=1; i<=num; ++i)
    s += i;
  timer.stop();
  std::cout << "s=" << s << std::endl;
  std::cout << "s.get_prec()=" << s.get_prec() << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << " mseconds." << std::endl;
}

void example1(int num) {
  Timer2 timer;

  Expr s(0.0);
  Expr one(1.0);
  timer.start();  
  for (int i=1; i<=num; ++i)
    s += one / i;
  BigFloat2 r = s.r_approx(60);
  timer.stop();
  std::cout << "r=" << r << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << " mseconds." << std::endl;
}

void example2(int num) {
  Timer2 timer;

  Expr s(0.5);
  Expr coeff(3.75);
  Expr one(1.0);
  timer.start();  
  for (int i=1; i<=num; ++i) {
    s = coeff * (one - s) * s;
    s.sign();
//    std::cout << "s=" << s << std::endl;
  }
  BigFloat2 r = s.r_approx(53);
  timer.stop();
  std::cout << "r=" << r << std::endl;
  std::cout << "it tooks " << timer.get_mseconds() << " mseconds." << std::endl;
}

int main(int argc, char** argv) {
  int num = 1000;
  int method = 0;

  if (argc >= 2)
    num = atoi(argv[1]);
  if (argc >= 3)
    method = atoi(argv[2]);
  
  if (method == 0)
    bf_example1(num);
  else if (method == 1)
    example1(num);
  else
    example2(num);

  return 0;
}
