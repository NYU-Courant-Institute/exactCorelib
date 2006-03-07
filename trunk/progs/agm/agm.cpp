#include "agm.h"

using namespace CORE;
typedef CORE::BigInt INT;

double u_inits[] = {1.1549, 1.2972, 1.4322, 1.5708, 1.7228, 1.9021, 2.1364};
double t_inits[] = {3.1738, 3.6591, 4.1878, 4.8105, 5.6004, 6.6999, 8.4688};
double m_inits[] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

// compute 2^{-n}
NT neg_power_2(int n) {
  return NT::exp2(-n);
}

//
// common agm procedure for computing pi
// (note: for now we do in full precision)
//
void agm_pi(NT& A, NT& T, int prec) {
  A = 1;
  NT B(2);
  B = B.sqrt(prec).div2().makeExact(); // sqrt(2)/2
  T = 0.25;
  INT X(1);
  NT Y;

  while (-(A-B).uMSB() < prec) {
    Y = A;
    A = (A+B).div2();
    B = (B*Y).sqrt(prec).makeExact();
    T -= X*(A-Y)*(A-Y);
    X <<= 1;
  }
}

//
// Discrete Newton iteration
//
// TODO: need check, if x+h is out of range, then should use
//
//      ff = (f - fun(x-h, p).div(h, p)
//
NT newton(eval_fun fun, const NT& c, const NT& x0, int prec) {
  int p = prec + 1;
  NT x(x0);
  NT del(1);
  NT f(0), ff(0);
  NT h = neg_power_2(p/2); // has to be > p/2
  //std::cout << "h=" << h << ", p=" << p << std::endl;
#ifdef AGM_DEBUG
  int counter = 0;
#endif
  do {
/*    
    std::cout << "x=" << x << std::endl;
    std::cout << "x.err()=" << x.err() << std::endl;
    std::cout << "x+h=" << (x+h) << std::endl;
    std::cout << "(x+h).err()=" << (x+h).err() << std::endl;
    std::cout << "x+h-x=" << (x+h-x) << std::endl;
    std::cout << "(x+h-x).err()=" << (x+h-x).err() << std::endl;
    
    std::cout << "(x+h-x).uMSB()=" << (x+h-x).uMSB() << std::endl;
    std::cout << "h.uMSB()=" << (h).uMSB() << std::endl;
*/
    f = fun(x, p);
    //std::cout << "f=" << f << std::endl;
    ff = (fun(x+h, p) - f).div(h, p).makeExact();
    //std::cout << "ff=" << ff << std::endl;
    //std::cout << "f.err=" << f.err() << std::endl;
    //std::cout << "c.err=" << c.err() << std::endl;
    del = f - c;
    //std::cout << "del=" << del << std::endl;
    //std::cout << "del.err=" << del.err() << std::endl;
    //std::cout << "del.m=" << del.m() << std::endl;
    //std::cout << "del.exp=" << del.exp() << std::endl;
    //std::cout << "del.sign=" << del.sign() << std::endl;
    //std::cout << "del.m.sign=" << sign(del.m()) << std::endl;
    x -= del.div(ff, p).makeExact();
    //std::cout << "xdel=" << del.div(ff, p) << std::endl;
#ifdef AGM_DEBUG
    std::cout << "del.uMSB()=" << del.uMSB() << std::endl;
    counter ++;
#endif
  } while (del.sign() != 0 && -del.uMSB() < p);
#ifdef AGM_DEBUG
  std::cout << "counter=" << counter << std::endl;
#endif

  return x;
}

//
// Discrete Newton iteration
//
NT newton2(eval_fun2 fun, const NT& c, const NT& x0, int prec, const NT& val) {
  int p = prec + 1;
  NT x(x0);
  NT del(1);
  NT delx(1);
  NT f(0), ff(0);
  NT h = neg_power_2(p/2); // has to be > p/2

#ifdef AGM_DEBUG
  int counter = 0;
#endif
  do {
    //std::cout << "x=" << x << std::endl;
    f = fun(val, x, p);
    //std::cout << "f=" << x << std::endl;
    ff = (fun(val, x+h, p) - f).div(h, p).makeExact();
    //std::cout << "ff=" << ff << std::endl;
    //std::cout << "f.err=" << f.err() << std::endl;
    //std::cout << "c.err=" << c.err() << std::endl;
    del = f - c;
    //std::cout << "del.err=" << del.err() << std::endl;
    x -= del.div(ff, p).makeExact();
#ifdef AGM_DEBUG
    //std::cout << "del.uMSB()=" << del.uMSB() << std::endl;
    counter ++;
#endif
  } while (del.sign() != 0 && -del.uMSB() < p);
#ifdef AGM_DEBUG
  std::cout << "counter=" << counter << std::endl;
#endif

  return x;
}

//
// compute U_k(m)
//
NT eval_U_k(const NT& bfPi, const NT& m, int k, int prec) {
  // compute F(\alpha_0) using AGM
  // precision needed for approximate \lim a_n
  int p = prec + 7;
  // iteration needed for approximate \lim a_n
  int kk = clLg(ulong(p + 3));
  // precision needed for approximate b_0
  int pp = p + kk;
  NT A(1);
  NT B = (1-m).sqrt(pp+4).makeExact();
  NT C;

  for (int i=0; i<kk; i++) {
    C = (A+B).div2();
    B = (A*B).sqrt(pp+5).makeExact();
    A = C;
  }
  // precisions needed for approximate \pi and division are p
  A = bfPi.div(A+B, p).makeExact();

  // compute U_k(m)
  // precision needed for approximate s_0
  p = prec + 5 + clLg(ulong(k));
  NT S = m.sqrt(p).makeExact();
  for (int i=0; i<k; i++) {
    C = (1+S).div2();
    A *= C;
    S = S.sqrt(p+8).div(C, p+4).makeExact();
  }
  
  return A*(1+S).div2();
}

//
// compute U(m)
//
NT eval_U(const NT& bfPi, const NT& m, int prec) {
  int k = clLg(ulong(prec+7));
  return eval_U_k(bfPi, m, k, prec+1);
}

//
// compute T_k(m)
//
NT eval_T_k(const NT& m, int k, int prec) {
  // need compute v_k to relative (prec+18) bits
  int p = prec + 18 + 18*k;
  NT V(1);
  NT S = m.sqrt(p).makeExact();
  NT W;

  for (int i=0; i<k; i++) {
//std::cout << "line 152, S=" << S << ",V=" << V << std::endl;
    W = 2*S*V.div(1+V*V, p-2).makeExact();
//std::cout << "line 154, w=" << W << std::endl;
    W = W.div(1 + (1-W*W).sqrt(p-6).makeExact(), p-7).makeExact();
//std::cout << "line 156" << std::endl;
    W = (V+W).div(1-V*W, p-11).makeExact();
//std::cout << "line 158" << std::endl;
    V = W.div(1 + (1+W*W).sqrt(p-15).makeExact(), p-16).makeExact();
//std::cout << "line 160" << std::endl;
    S = 2*S.sqrt(p+4).makeExact().div(1+S, p+4).makeExact();
    p -= 18;
  }

  return (1+V).div(1-V, prec+18).makeExact();
}

//
// compute T(m)
//
NT eval_T(const NT& m, int prec) {
  int k = clLg(ulong(prec+23));
  return eval_T_k(m, k, prec+1);
}

//
// compute arctan(x)
//
NT eval_A_k(const NT& log4, const NT& x, int k, int prec) {
  // need compute v_k to relative (prec+18) bits
  int p = prec + 18 + 18*k;
  NT S = neg_power_2((prec-1)/2);
  NT V = x.div(1+(1+x*x).sqrt(p).makeExact(), p).makeExact();
  NT Q(1);
  NT W;

  for (int i=0; i<k; i++) {
    Q = 2*Q.div(1+S, p).makeExact();
//    std::cout << "V=" << V << std::endl;
    W = 2*S*V.div(1+V*V, p-2).makeExact();
//    std::cout << "W=" << W << std::endl;
    W = W.div(1 + (1-W*W).sqrt(p-6).makeExact(), p-7).makeExact();
//    std::cout << "W=" << W << std::endl;
    W = (V+W).div(1-V*W, p-11).makeExact();
//    std::cout << "W=" << W << std::endl;
    V = W.div(1 + (1+W*W).sqrt(p-15).makeExact(), p-16).makeExact();
//    std::cout << "S=" << S << std::endl;
//    std::cout << "sqrt(S)=" << S.sqrt(p+4) << std::endl;
    S = 2*S.sqrt(p+4).makeExact().div(1+S, p+4).makeExact();
//    std::cout << "S=" << S << std::endl;
    p -= 18;
  }
  //std::cout << "(1-s).uMSB()=" << (1-S).uMSB() << ",prec=" << prec << std::endl;
  //std::cout << "V=" << V << std::endl;

  V = (1+V).div(1-V, prec+18).makeExact();
  //std::cout << "V=" << V << std::endl;

  return Q*(prim_log(4.0*V, prec) - log4);
}

//
// compute arctan(x)
//
NT eval_A(const NT& log4, const NT& m, int prec) {
  int k = 2*clLg(ulong(prec+23));
  return eval_A_k(log4, m, k, prec+1);
}

