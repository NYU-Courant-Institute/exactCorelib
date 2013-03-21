/*
 *  test.cpp
 *  
 *
 *  Created by Vikram Sharma on 1/19/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>
#include <string>
#include <sys/time.h>

#define CORE_LEVEL 4
#include "CORE/CORE.h"
#include "CORE/poly/Curves.h"


using namespace std;

typedef BigFloat NT;

// Here we want to test the eval3 routine for interval evaluation
// of bivariate polynomials. Is it really converging quadratically?

int main(int argc, char **argv){
	typedef IntervalT<NT> IntervalNT;

	// "(-1 + x^5) + (-10*x^3)*y^2 + (5x)* y^4"
	// "(5*x^4)*y^1 + (-10*x^2)*y^3 + y^5"
	// "25y^8 + (100x^2)y^6 + (150x^4)y^4 + (100x^6)y^2 + (25x^8)"
	BiPoly<NT> F("25y^8 + (100x^2)y^6 + (150x^4)y^4 + (100x^6)y^2 + (25x^8)"), G("x^2+y^2-1"),
	H("-4xy-2x");
	IntervalNT I(NT(0), NT(2)), J(NT(2), NT(4)), tempI, tempJ;
	Polynomial<NT> P("x^20+2x+1");
	G = BiPoly<NT>("9.54688x^2y + -18.3984x^3 + 18.3984x - 66.8281");
	I=IntervalNT(NT(0.625), NT(3.125));
	J= IntervalNT(NT(3.84375), NT(5.96875));
	

	cout << "F.eval3 " << F.eval3(I, J) << endl;
	cout << "F.eval  "<<F.eval(I, J) << endl;
	cout << "F.eval  "<<F.eval(J, I) << endl;
	cout << "F(Imid) "<<F.eval<NT>(I.mid(), J.mid()) << endl;
//	cout << "F.eval2 "<<F.eval2(I, I) << endl;

//  cout << G.eval3(I, J) << endl;
	cout << G.eval3(I, IntervalNT(J.getL())) << endl;
	cout << G.eval3(I, IntervalNT(J.getR())) << endl;
//	cout << G.eval2(I, I) << endl;

	cout << "H.eval3 " << H.eval3(I, J) << endl;
	cout << "H.eval  "<<H.eval(I, J) << endl;
//	cout << "H.eval2 "<<H.eval2(I, I) << endl;

	NT x_l=I.getL(), y_l=J.getR();
	NT xmid = I.mid();
	NT ymid = J.mid();
	//cout << "F eval at xl, yl " << F.eval<NT>(x_l, y_l)<<endl;
	cout << "P eval at xl " << P.eval<NT>(x_l)<<endl;
	for (int i=0; i <= 1; i++) {
		tempI = IntervalNT(x_l, xmid); tempJ = IntervalNT(y_l, ymid);
		cout << "Interval : "<< tempI.width() << endl;
/*		cout << "Slope    : " << (F.eval3(tempI, tempJ)-IntervalNT(F.eval<NT>(x_l, y_l))).width() << endl;
		cout << "Mean     : " << (F.eval2(tempI, tempJ)-IntervalNT(F.eval<NT>(x_l, y_l))).width() << endl;
		cout << "Horner's : " << F.eval(tempI, tempJ).width() << endl;
		cout << "Horner's : " << F.eval(tempI, tempJ) << endl;
*/
		cout << "SlopeForm: " << (P.evalSlopeForm(tempI)).width()- (P.eval(xmid)- P.eval(x_l)) << endl;
		cout << "Centered : " << (P.evalCenteredForm(tempI)).width() - (P.eval(xmid)- P.eval(x_l)) << endl;
		
		
		xmid = tempI.mid(); ymid = tempJ.mid();
		cout << endl;
	}
	
	/*
	 NT x_l=I.getL(), y_l=J.getR();
	 NT xmid = I.mid();
	 NT ymid = J.mid();
	 
	 for (int i=0; i <= 100; i++) {
	 tempI = IntervalNT(x_l, xmid); tempJ = IntervalNT(y_l, ymid);
	 cout << F.eval3(tempI, tempJ) << endl;
	 cout << F.eval(tempI, tempJ) << endl;
	 xmid = tempI.mid(); ymid = tempJ.mid();
	 
	 }
	 tempI = IntervalNT(x_l, xmid); tempJ = IntervalNT(y_l, ymid);
	 cout << F.eval3(tempI, tempJ) << endl;
	 */	
	
	/*	Il = [ -1.338958740234375, -1.338958740234375 ]
	 Ir = [ 96.302032470703125, 96.302032470703125 ]
	 Iu = [ -556.4744644165039062, 326.5653915405273438 ]
	 Id = [ -548.5498123168945312, 524.6816940307617188 ]
	 */	 

}