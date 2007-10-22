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
   $Id: bcir.cpp,v 1.3 2007-10-22 20:29:47 exact Exp $
************************************** */


#include <fstream>
#include <CORE/linearAlgebraT.h>
#include <CORE.h>

using namespace std;

typedef MatrixT<double> MatrixD;
typedef VectorT<double> VectorD;

// This function decomposes the matrix A into the product
// PA=LU where L is lower triangular (with unit diagonal and
// U is upper triangular. P is a permutation matrix that 
// records the rearrangment of row due to any pivoting of A.
// P, L, and U are overwriten.
// We assume A is invertible.
void LU_decompose(MatrixD& A, MatrixD& P, MatrixD& L, MatrixD& U ) {
   int n = A.dimension_1();
   int i, j, k;
   U = A;
   
   for (i = 0; i < n; i++) {
	
        // This is the pivoting version of
	// Gaussian elimination:
      	// if exchange rows to make sure that A(i,i) != 0
        // invariant P*A = L*U
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
    ifstream ifs(filename);
    if (!ifs) { 
       perror("cannot open the file");
       exit(1);
    }
    int n;
    int a;
    double la;
    ifs >> n;
    *A = new double[n*n];
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
	ifs >> a;
        la = a;
        (*A)[j+i*n] =  la;
      }
    }
    ifs.close();
    return n;
}

int readVector(char *filename, double **A) {
    ifstream ifs(filename);
    if (!ifs) { 
       perror("cannot open the file");
       exit(1);
    }
    int n;
    int a;
    double la;
    ifs >> n;
    *A = new double[n];
    for (int i=0; i<n; i++) {
	ifs >> a;
        la = a;
        (*A)[i] =  la;
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
      u -= f;
      solve(v,u,j-1);
      //set prec = new_precision
      d = z - v;
      return;
    }
}


int main( int argc, char *argv[] ) {
  double B_cond_num = 128.0;   
  defRelPrec = 200;
  int defPrtDgt = 50;

  

  if (argc != 4) {
    cerr << "Usage: bcir <matrix_input_file> <vector_inout_file> <epsilon>" << endl;
    exit(1);
  }
 
  cout << setprecision(defPrtDgt);

  //load matrix 'A'
  double *M;
  n = readMatrix(argv[1], &M);
  MatrixD TA(n, n, M);
  A = TA;
  cout << endl << endl << "A = " <<  A << endl << endl;

  //load vector 'b' here
  double *V;
  n = readVector(argv[2], &V);
  VectorD Tb(n, V);

  cout << "b = " <<  Tb << endl << endl;

  // calculate number of iterative calls.
  int epsilon; //this epsilon is -log2 of the orig. epsilon from the paper.
  epsilon = atoi(argv[3]);
  tau = 2 + epsilon;
  c = int(ceil(log2(B_cond_num)))+5;
  p = int( max( 0, int( floor( log2( min( double(tau/c), double(n/2.0)) ) ) ) ) );

 cout << "epsilon = " << epsilon   << endl << endl;
 cout << "tau = " << tau   << endl << endl;
 cout << "c = " << c   << endl << endl;
 cout << "p = " <<  p   << endl << endl;

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
  cout << endl << "After LU decomposition we have: " << endl << endl;
  cout << " L = " <<  L << endl << endl;
  cout << " P = " <<  P_matrix << endl << endl;
  cout << " U = " <<  U << endl << endl;

  A = P_matrix * TA ;  
  cout << " self check should be zero ..." <<  A-L*U << endl << endl;
  cout << " P*A-L*U = " <<  A-L*U << endl << endl;

  //solve our system
  //first permute Tb to get new b since P_matrix*A=LU
  b = P_matrix * Tb; 
  VectorD x(n); 

  cout << " P*b = " <<  b << endl << endl;

  cout << endl << "Computing solution ... " << endl << endl;
  solve(x,b,p);

  // output vector x
  cout << " solution: x = " <<  x << endl << endl;
  
  cout << endl << "Verifying solution ..." << endl << endl;
  cout << "      is A*x = " <<  TA*x << endl << endl;
  cout << " eqalal to b = " << Tb << endl << endl;
  

  return 0;

}
