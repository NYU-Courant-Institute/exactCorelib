/* ********************************************************************
 File: determinant.cpp

   Purpose: 
   	Compares the computation of determinants using 3 methods:
		1) Gaussian elimination
		2) Factorial expansion method
		3) Dynamic programming method

	The results of this comparison is reported in the paper

	"Constructive Root Bound Method for k-Ary Rational Input Numbers"
		by Sylvain Pion and Chee Yap,
        	To appear, Proc. 19th ACM Symp. Comp.Geom., June 2003.

   Usage:
	% determinant <input_file> 

	where 
	
	<input_file> is the name of an ascii file containing <n> 
	matrices, each of dimension <d>.  The first two integers in
	in <input_file> are <n> and <d>.  The remaining entries
	contains the entries of each of the matrices in the usual row
	major order.  In particular, the program "createMatrices"
	produce such input files.

	A sample input file named "inputs/1000x4matrices"
	may be found in this directory.

   Author:
   	Sylvain Pion 

   $Id: determinant.cpp,v 1.1 2006-03-07 04:51:23 exact Exp $
 ******************************************************************** */


#include <fstream>
#include <CORE.h>
#include "CORE/linearAlgebra.h"
#include "determinant.h"

using namespace CORE;

double factorial_determinant(const Matrix & A) {
  int dim = A.dimension_1();
  if (dim == 1)
    return A(0,0);

  double det = 0;
  int parity = 1;
  Matrix B(dim-1,dim-1);
  for (int i=0; i<dim; ++i) {
    for (int j=0; j<dim; j++) {
      if (j==i)
        continue;
      for (int k=0; k<dim-1; k++)
        B(j>i ? j-1 : j, k) = A(j, k+1);
    }
    if (parity>0)
      det += factorial_determinant(B) * A(i, 0);
    else
      det -= factorial_determinant(B) * A(i, 0);
    parity = -parity;
  }
  return det;
}

double gaussian_determinant(Matrix A)
{
   int dim = A.dimension_1();
   int *permutation = new int[dim];
   int swap_counter = 0; // Number of swaps done.
   int i;

   // Initialize the permutation of rows to Identity
   for (i = 0; i < dim; i++)
     permutation[i] = i;

   // This is the pivoting version of Gaussian elimination:
   for (i = 0; i < dim-1; i++) {

      // Look for a non-zero pivot.
      int pivot = i;
      while (pivot < dim && A(i, permutation[pivot]) == 0)
        pivot++;

      // All candidates are zero => det == 0
      if (pivot == dim) {
	delete[] permutation;
	return 0;
      }

      // Do we need to swap two rows ?
      if (pivot != i) {
        std::swap(permutation[i], permutation[pivot]);
	swap_counter++;
      }

      for (int j = i + 1; j < dim; j++)
         for (int k = i + 1; k < dim; k++) {
	    assert(A(i, permutation[i]) != 0);
            A(j, permutation[k]) -= A(j, permutation[i]) * A(i, permutation[k])
		                  / A(i, permutation[i]);
         }
   }

   double det = 1;
   for (i = 0; i < dim; i++)
      det *= A(i, permutation[i]);

   if (swap_counter & 1 != 0) // Odd number of swaps
     det = -det;

   delete[] permutation;
   return det;
}

enum Method {Gaussian, Dynamic, Factorial};

int main(int argc, char **argv)
{
  defRelPrec = 100;
  defAbsPrec = CORE_INFTY;
  int defPrtDgt = 40;
  int base;
  Method method;

  // setProgressiveEvalFlag(false);

  if (argc != 3 && argc != 4) {
    std::cerr << "Usage: determinant <input_file> <num_of_execution> "
	         "<0,default=gaussian, 1=dynamic_programming, other=factorial>"
		 << std::endl;
    exit(1);
  }

  if (argc < 4) {
    method = Gaussian;
  }
  else if (atoi(argv[3]) == 0) {
    method = Gaussian;
  }
  else if (atoi(argv[3]) == 1) {
    method = Dynamic;
  }
  else {
    method = Factorial;
  }

  std::ifstream ifile(argv[1]);
  int number_of_matrices;
  ifile >> number_of_matrices; //throw it away.
  ifile >> base;
  int dim;
  ifile >> dim;
  double *A = new double[dim*dim];
  for (int i=0; i<dim; i++)
    for (int j=0; j<dim; j++) {
	    int L;
	    BigInt bi;
	    ifile >> bi;
	    ifile >> L;
	    BigInt den; power(den, base, L);
	    BigRat br (bi, den);
	    A[i*dim+j] = br;
    }

  Matrix M(dim, dim, A);

  int iterations = atoi(argv[2]);
  double e;

  // std::cout << M << std::endl; // without the print, I get crashes in gaussian.  Why ???

  switch (method) {
  case Gaussian:
	  std::cout << " Using Gaussian elimination" << std::endl; break;
  case Dynamic:
	  std::cout << " Using Dynamic programming" << std::endl; break;
  case Factorial:
	  std::cout << " Using Factorial expansion" << std::endl;
  }

  for (int i=0; i<iterations; i++) {
    switch (method) {
    case Gaussian:
	    e = gaussian_determinant(M); break;
    case Dynamic:
	    e = dynamic_programming_determinant(M); break;
    case Factorial:
	    e = factorial_determinant(M); break;
    }
    (void) sign(e); // the cast is to kill warning
#if (CORE_LEVEL==3)
    // e.approx( defRelPrec, defAbsPrec );
    /* to force the evaluation, in order to get meaningful performance
     * comparison */
#endif
  }

#if (CORE_LEVEL==3)
    e.approx( defRelPrec, defAbsPrec );
    /* to force the evaluation, in order to get meaningful performance
     * comparison */
#endif
  
  // std::cout << "Matrix : " << std::endl << M << std::endl; // takes time
  std::cout << "Determinant root bound = " << e.getRep().nodeInfo->low << std::endl;
  std::cout << "Determinant = " << std::setprecision(defPrtDgt) << e
	    << std::endl << std::endl;

#ifdef COMPARE_ROOT_BOUNDS
  std::cout << " BFMSS   winner count = " << BFMSS_counter << std::endl;
  std::cout << " Measure winner count = " << Measure_counter << std::endl;
  std::cout << " Cauchy  winner count = " << Cauchy_counter << std::endl;
  std::cout << " Li/Yap  winner count = " << LiYap_counter << std::endl;

  std::cout << " Root Bound has been hit " << rootBoundHitCounter << " times" << std::endl;
#endif

  return 0;
}
