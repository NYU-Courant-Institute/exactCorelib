#define CORE_NO_AUTOMATIC_NAMESPACE
#ifdef CORE_LEVEL
#undef CORE_LEVEL
#define CORE_LEVEL 4
#endif

#include <CORE/CORE.h>
#include <CORE/poly/Poly.h>
#include <iostream>

using namespace CORE;
using namespace std;

int main()
{
   // Polynomial:-2 + 20t - 50t^2 + t^50
   // Roots : -1.09254, 0.2, 0.2, 1.07565

   BigRat c[51];
   c[0]= BigRat(-2);
   c[1]= BigRat(20);
   c[2]= BigRat(-50);
   c[50]= BigRat(1);
   for (int k=3;k<50;++k) c[k]=0;

   Polynomial<BigRat> poly(50, c);
   poly.contract();

   cout << "Polynomial is    x^{50} - 50 x^2 + 20 x - 2" << endl;
   cout << "Its real roots are: " << endl;
   cout << "       -1.09254,   0.2,    0.2,    1.07565 " << endl;
   cout << "Sturm Sequence produces these intervals: " << endl;

   Sturm<BigRat> my_sturm(poly);

   int nb_roots=my_sturm.numberOfRoots(-1000, 1000);
   for (int k=1;k<=nb_roots;++k){
     BFInterval bfi_sturm = my_sturm.isolateRoot(k, -1000.5, 1000);
     cout << "     [" << bfi_sturm.first << ", "
	 << bfi_sturm.second << "]" << endl;

	// TODO WARNING: rootOf(poly, bfi_sturm) did not work.
	// 	Is it because poly::Polynomial<BigRat>?
	//
     // Expr res= CORE::rootOf(poly, bfi_sturm);
     // cout <<  "Root "<< res << endl;
   }


   // ======================================
   // Repeat the above for Polynomial<BigInt>:
   // ======================================
   BigInt cc[51]; 	// cc cannot be const
   cc[0]= BigInt(-2);
   cc[1]= BigInt(20);
   cc[2]= BigInt(-50);
   cc[50]= BigInt(1);
   for (int k=3;k<50;++k) cc[k]=0;

   // Next, we need a "const" here polynomial:
   //
   Polynomial<BigInt> const new_poly(50, cc);
   // new_poly.contract(); 		// I cannot do this to const poly.

   Polynomial<BigInt> const const_poly(new_poly);

   Sturm<BigFloat> new_sturm(new_poly);

   nb_roots = new_sturm.numberOfRoots(-1000, 1000);
   
   const BFInterval bfi_1 = new_sturm.isolateRoot(1, -1000, 1000);
   Expr res1= CORE::rootOf(const_poly, bfi_1);

   for (int k=1;k<=nb_roots;++k){
     const BFInterval bfi_sturm = new_sturm.isolateRoot(k, -1000, 1000);
     cout << "     [" << bfi_sturm.first << ", "
	 << bfi_sturm.second << "]" << endl;

	// TODO WARNING: rootOf(poly, bfi_sturm) is not yet implemented
	// 		for Polynomial<BigFloat>
	//
      //Expr res= CORE::rootOf(const_poly, bfi_sturm);
      //cout <<  "Root "<< res << endl;
   }

   return 0;
}

