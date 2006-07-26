#define CORE_LEVEL 4
#include "CORE/CORE.h"
#include "readpoly.h"
#include "Descartes.h"

typedef BigInt NT;
typedef Polynomial<NT> PolyNT;

int main(int argc, char* argv[]) {
  VecString v;
  if(argc > 1)
    readString(argv[1], v); //Reads in a file of polynomials
  else
    readString("../data/polynomials/deg20", v);
  //Other options are:
  //../data/polynomials/deg10[20,40,80,100,100_slow,squarefree,nonsqrfree]

  PolyNT P, Q;
  Sturm<NT> S;
  BFVecInterval v1, v2;
  int m,n;
  Timer t1, t2;
  /*for (VecString::const_iterator it = v.begin();
	 it != v.end(); ++it) {
    read_poly((*it).c_str(), P);
    cout << "Polynomial is " << *it << endl;
    P.dump();
    
    S = Sturm<NT>(P);
    t1.start();
    S.isolateRoots(v1);
    t1.stop();
    cout << "Root isolation using Sturm takes time " << t1.getSeconds()<<endl; 

    t2.start();
    isolateRoots(P, v2);
    t2.stop();
    cout<<"Root isolation using Descartes takes time " <<t2.getSeconds()<<endl;

    cout <<" Sturm: Number of real roots = "<< (m=v1.size()) << endl;
    cout <<" Descartes: Number of real roots = "<< (n=v2.size()) << endl;
    if(m != n)
      cout <<"ERROR!!! Root Isolation wrong."<< endl;
    cout << endl;
  }
  */

  //This loop only tests Descartes method and gives average timing
  BigInt B; 
  for (VecString::const_iterator it = v.begin();
	 it != v.end(); ++it) {
    read_poly((*it).c_str(), P);
    cout << *it << " ";
    B = CauchyBound(P);
    testDescartes2(P, BFInterval(-B, B), 10);
  }  
  return 0;
}


