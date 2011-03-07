/* Reciprocal
 *
 * To refine x ~ 1/a, use Newton
 * 	N(x)= x(2-ax) =2x-ax^2
 *
 * To call:
 * 	> recip [a=3] [it=1]
 *
 * where it is the number of times to do the iteration.
 *
 * */
#include <CORE/CORE.h>
#define CORE_LEVEL 4

#include <sstream>
#include <iostream>

using namespace std;

int errPos(CORE::BigFloat e){
	Expr le = e;
	return  (int)(-log(abs(le))/log(Expr(10))).doubleValue();
}

int main(int argc, char * argv[]) {

 machine_double a(3.0);
 if (argc>1) a=atof(argv[1]);
 int it=2;
 if (argc>2) it=atoi(argv[2]);

 CORE::BigFloat c[3];
 c[0]= CORE::BigFloat(0);
 c[1]= CORE::BigFloat(2.0);
 c[2]= -a;

 CORE::Polynomial<CORE::BigFloat> bf(2, c);

 //CORE::BFInterval bfi(CORE::BigFloat(-.5), CORE::BigFloat(-.2));
 cout << "INPUT: a = " << a << endl;
 //cout << "poly=" << bf << endl;

 CORE::BigFloat x(1/a);
 CORE::BigFloat err = a*x-1;
 cout << "initial approximation of 1/a = " << x << endl;;
 cout << "initial err = " << err << endl;
 int logerr = errPos(err);
 cout << "initial logerr = " << logerr << endl;

 for (int i=0; i<it; i++) {
	x = bf.eval(x);
	err = a*x -1;
 	cout << "err = " << err << endl;
	logerr = errPos(err);
 	cout << "logerr = " << logerr << endl;
 	cout << "x = " << setprecision(logerr) << x << endl;;
 }
}

