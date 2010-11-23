#include <CORE/CORE.h>
#include <iostream>

using namespace std;

static int
cputime ()
{
  struct rusage rus;
  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}

int main(int argc, char* argv[]) {
  //unsigned long prec = 100;
  size_t prec = 100;
  int st, st0;

  if (argc > 1)
    prec = atoi(argv[1]);

  st = cputime();

  cout << digits2bits(100) << endl;

  Expr x = root(Expr(2), 5);
  Expr xx = root(Expr(4), 5);
  Expr xxx = root(Expr(8), 5);
  Expr xxxx = Expr(7) + x - 5*xxx;
  cout << "x=" << x.r_approx(digits2bits(prec)) << endl;
  cout << "xx=" << xx.r_approx(digits2bits(prec))<< endl;
  cout << "xxx=" << xxx.r_approx(digits2bits(prec)) << endl;
  cout << "xxxx=" << xxxx.r_approx(digits2bits(prec)) << endl;
  Expr r = cbrt(xxxx) + xx - x;
  cout << "r=" << r.r_approx(digits2bits(prec)) << endl;
  cout << "r.sign()=" << r.sign() << endl; 
  cout << "r.uMSB()=" << r.uMSB() << endl; 
  cout << "r.lMSB()=" << r.lMSB() << endl; 
  cout << "digits2bits(prec)=" << digits2bits(prec) << endl; 

  Expr::KT value = r.r_approx(digits2bits(prec));
  cout << "r.diam=" << value.abs_diam() << endl;
  
  st0 = cputime();
  cout << "r=" << value << endl;
  st0 = cputime() - st0;

  cout << "Cputime: " << cputime() - st << 
    "ms (output " << st0 << "ms)" << endl;

  Expr e = sqrt(Expr(6)) - sqrt(Expr(2))*sqrt(Expr(3));
  //Expr e = sqrt(Expr(2))*sqrt(Expr(3));
  cout << "e.sign()=" << e.sign() << endl; 
  cout << "e.r_approx()=" << e.r_approx(digits2bits(prec)) << endl; 

  Expr ee = sqrt(Expr(7));
  cout << "ee.sign()=" << ee.sign() << endl; 
  cout << "ee.r_approx()=" << ee.r_approx(digits2bits(prec)) << endl; 

  Expr eee = ee - e;
  cout << "eee.sign()=" << eee.sign() << endl; 
  cout << "eee.r_approx()=" << eee.r_approx(digits2bits(prec)) << endl; 
  cout << "eee=" << eee << endl; 

  return 0;
}
