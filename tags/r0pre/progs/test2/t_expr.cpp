#include <CORE/CORE.h>
#include <iostream>

static int
cputime ()
{
  struct rusage rus;
  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

int main(int argc, char* argv[]) {
  unsigned long prec = 100;
  int st, st0;

  if (argc > 1)
    prec = atoi(argv[1]);

  st = cputime();

  std::cout << digits2bits(100) << std::endl;

  Expr x = root(Expr(2), 5);
  Expr xx = root(Expr(4), 5);
  Expr xxx = root(Expr(8), 5);
  Expr xxxx = Expr(7) + x - 5*xxx;
  std::cout << "x=" << x.r_approx(digits2bits(prec)) << std::endl;
  std::cout << "xx=" << xx.r_approx(digits2bits(prec))<< std::endl;
  std::cout << "xxx=" << xxx.r_approx(digits2bits(prec)) << std::endl;
  std::cout << "xxxx=" << xxxx.r_approx(digits2bits(prec)) << std::endl;
  Expr r = cbrt(xxxx) + xx - x;
  std::cout << "r=" << r.r_approx(digits2bits(prec)) << std::endl;
  std::cout << "r.sign()=" << r.sign() << std::endl; 
  std::cout << "r.uMSB()=" << r.uMSB() << std::endl; 
  std::cout << "r.lMSB()=" << r.lMSB() << std::endl; 
  std::cout << "digits2bits(prec)=" << digits2bits(prec) << std::endl; 

  Expr::KT value = r.r_approx(digits2bits(prec));
  std::cout << "r.diam=" << value.abs_diam() << std::endl;
  
  st0 = cputime();
  std::cout << "r=" << value << std::endl;
  st0 = cputime() - st0;

  std::cout << "Cputime: " << cputime() - st << 
    "ms (output " << st0 << "ms)" << std::endl;

  Expr e = sqrt(Expr(6)) - sqrt(Expr(2))*sqrt(Expr(3));
  //Expr e = sqrt(Expr(2))*sqrt(Expr(3));
  std::cout << "e.sign()=" << e.sign() << std::endl; 
  std::cout << "e.r_approx()=" << e.r_approx(digits2bits(prec)) << std::endl; 

  Expr ee = sqrt(Expr(7));
  std::cout << "ee.sign()=" << ee.sign() << std::endl; 
  std::cout << "ee.r_approx()=" << ee.r_approx(digits2bits(prec)) << std::endl; 

  Expr eee = ee - e;
  std::cout << "eee.sign()=" << eee.sign() << std::endl; 
  std::cout << "eee.r_approx()=" << eee.r_approx(digits2bits(prec)) << std::endl; 
  std::cout << "eee=" << eee << std::endl; 

  return 0;
}
