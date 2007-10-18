/* **************************************
   File: bcir.cpp

   Purpose: This is an implementation of the BCIR algorithm
        described in the #### paper by kielbasinski entitled
        from bit ...  This algorithm solves the linear system
        Ax=b to an arbitrary specified accuracy 'epsilon. 
          

   Usage:
	bcir <input_matrix_file> <input_vector_file> <epsilon>

	where <input_matrix_file> contains the matrix A
	  and <input_vector_file> is the vector b.
	 

   Since CORE Library Version 1.2
   $Id: bcir.cpp,v 1.2 2007-10-18 16:19:43 exact Exp $
************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 1
#endif

#include <fstream>
#include <CORE\linearAlgebraT.h>
#include <CORE.h>

typedef MatrixT<double> MatrixD;
typedef VectorT<double> VectorD;

// This function decomposes the matrix A into the product
// A=LU where L is lower triangular (with unit diagonal and
// U is upper triangular. P is a permutation matrix that 
// records the rearrangment of row due to any pivoting of A.
// P, L, and U are overwriten.
void LU_decompose(MatrixD& A, MatrixD& P, MatrixD& L, MatrixD& U ) {
   int n = A.dimension_1();
   int i, j, k;
   U = A;
   for (i = 0; i < n; i++) {
	// This is the pivoting version of
	// Gaussian elimination:
      	// if exchange rows to make sure that A(i,i) != 0
      if(U(i,i) == 0)
        for (j = i +1; j < n; j++)
          if (U(j, i) != 0) {
            U.rowExchange(i, j);
            P.rowExchange(i, j);
            L.rowExchange(i, j);
            break;
          }

      for (j = i + 1; j < n; j++) {
	 L(j,i)= U(j,i)/U(i,i);
         for (k = i + 1; k < n; k++) {
            U(j,k) -= U(j,i)*U(i,k)/U(i,i);
         }
         U(j,i) = 0;   
      }
      L(i,i)=1;
   }
}

// This function solves an upper triangular system Ux=b.
void solve_Upper_Triangular(MatrixD& m, VectorD& y, VectorD& r ) {
   int n = m.dimension_1();
   int i, j;
   double t=0;
   y[n-1] = r[n-1]/m(n-1,n-1);
   for (i = n - 2; i >= 0; i--) {
     t = 0;
     for (j = n-1; j > i; j--) {
       t = t + y[j]*m(i,j);
     }
     y[i] = (r[i] - t)/m(i,i);
   }
}

// This function solves a lower triangular system Lx=b.
void solve_Lower_Triangular(MatrixD& m, VectorD& y, VectorD& r ) {
   int n = m.dimension_1();
   int i, j;
   double t=0;
   y[0] = r[0]/m(0,0);
   for (i = 1; i < n; i++) {
     t = 0;
     for (j = 0; j < i; j++) {
       t = t + y[j]*m(i,j);
     }
     y[i] = (r[i] - t)/m(i,i);
   }
}

int readMatrix(char *filename, double **A) {
    std::ifstream ifs(filename);
    if (!ifs) { 
       perror("cannot open the file");
       exit(1);
    }
    int n;
    int a, b;
    double la, lb;
    ifs >> n;
    *A = new double[n*n];
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
	ifs >> a;
        ifs >> b;
        la = a;
        lb = b;
        (*A)[j+i*n] =  la/lb;
      }
    }
    ifs.close();
    return n;
}

int readVector(char *filename, double **A) {
    std::ifstream ifs(filename);
    if (!ifs) { 
       perror("cannot open the file");
       exit(1);
    }
    int n;
    int a, b;
    double la, lb;
    ifs >> n;
    *A = new double[n];
    for (int i=0; i<n; i++) {
	ifs >> a;
        ifs >> b;
        la = a;
        lb = b;
        (*A)[i] =  la/lb;
    }
    ifs.close();
    return n;
}
 
// variables that need to be global.
static int tau;
static int p;
static int c;
static int n;
static MatrixD A(0,0);
static MatrixD L(0,0);
static MatrixD U(0,0);
static VectorD b(0);


//The recursive 'solve' function of bcir,
//the result of each call is placed in
//the vector argument 'd'.
// c, tau, p, A, b, L, and U  need to be globally accessible.
void solve(VectorD& d, VectorD& f, int j) {
  int n=d.dimension();
  VectorD z(n),u(n),v(n);
  int new_precision = int(ceil(c+tau*2^(j-p)));
    if (j==0) { 
      //set prec = new_precision  
      solve_Lower_Triangular(L,z,f);
      solve_Upper_Triangular(U,d,z);
      return;  
    } else {
      solve(z,f,j-1);
      //set prec = new_precision
      u = A * z;
      u -= b;
      solve(v,u,j-1);
      //set prec = new_precision
      d = z - v;
      return;
    }
}


int main( int argc, char *argv[] ) {
  double B_cond_num = 128.0;   
  defRelPrec = 100;
  int defPrtDgt = 40;

  if (argc != 4) {
    std::cerr << "Usage: bcir <matrix_input_file> <vector_inout_file> <epsilon>" << std::endl;
    exit(1);
  }
  
  //load matrix 'A'
  double *M;
  n = readMatrix(argv[1], &M);
  MatrixD TA(n, n, M);
  A = TA;
  std::cout << std::endl << std::endl << "A = " << std::setprecision(defPrtDgt) <<  A << std::endl << std::endl;

  //load vector 'b' here
  double *V;
  n = readVector(argv[2], &V);
  VectorD Tb(n, V);

  std::cout << "b = " << std::setprecision(defPrtDgt) <<  Tb << std::endl << std::endl;

  // calculate number of iterative calls.
  int epsilon; //this epsilon is -log2 of the orig. epsilon from the paper.
  epsilon = atoi(argv[3]);
  tau = 2 + epsilon;
  c = int(ceil(log2(B_cond_num)))+5;
  p = int( max( 0, int( floor( log2( min( double(tau/c), double(n/2.0)) ) ) ) ) );

 std::cout << "epsilon = " << std::setprecision(defPrtDgt) << epsilon   << std::endl << std::endl;
 std::cout << "tau = " << std::setprecision(defPrtDgt) << tau   << std::endl << std::endl;
 std::cout << "c = " << std::setprecision(defPrtDgt) << c   << std::endl << std::endl;
 std::cout << "p = " << std::setprecision(defPrtDgt) <<  p   << std::endl << std::endl;

  //set prec to base prcision (t0= ceil(c+tau^(-p)))

  //factor A into L and U
  // first prepare L and P (P is a permutation matrix to undo any pivoting)
  MatrixD P_matrix(n,n);
  L = P_matrix;
  int i;       
  for (i = 0; i < n; i++) {
    P_matrix(i,i) = 1;
  }

  LU_decompose(A,P_matrix,L,U);  
  std::cout << std::endl << "After LU decomposition we have: " << std::endl << std::endl;
  std::cout << " L = " << std::setprecision(defPrtDgt) <<  L << std::endl << std::endl;
  std::cout << " P = " << std::setprecision(defPrtDgt) <<  P_matrix << std::endl << std::endl;
  std::cout << " U = " << std::setprecision(defPrtDgt) <<  U << std::endl << std::endl;
  
  //solve our system
  //first permute Tb to get new b since P_matrix*A=LU
  b = P_matrix * Tb; 
  VectorD x(n); 

  std::cout << " P*b = " << std::setprecision(defPrtDgt) <<  b << std::endl << std::endl;

  std::cout << std::endl << "Computing solution ... " << std::endl << std::endl;
  //p=0; //for debuging
  solve(x,b,p);

  // output vector x
  std::cout << " solution: x = " << std::setprecision(defPrtDgt) <<  x << std::endl << std::endl;
  
  std::cout << std::endl << "Verifying solution ..." << std::endl << std::endl;
  std::cout << "      is A*x = " << std::setprecision(defPrtDgt) <<  A*x << std::endl << std::endl;
  std::cout << " eqalal to b = " << std::setprecision(defPrtDgt) << Tb << std::endl << std::endl;
  

  return 0;

}
