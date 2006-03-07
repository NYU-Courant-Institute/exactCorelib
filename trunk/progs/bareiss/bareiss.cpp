/* **************************************
   File: bareiss.cpp

   Purpose: Implementation of the Bareiss algorithm for
	computing determinants of matrices.  
	This version performs pivoting
	(cf. Gaussian.cc in ${CORE}/progs/gaussian)
	The book "Fundamental Problems of Algorithmic Algebra",
	by C.Yap (Oxford U.Press, 2000) treats this algorithm.

	The input matrix is hard-coded.
   Usage:
	% bareiss

   CORE Library, $Id: bareiss.cpp,v 1.1 2006-03-07 04:51:23 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include "CORE.h"

class Matrix {
private:
  int     n;
  double* _rep;

public:
   Matrix(int d) : n(d) { _rep   = new double [n*n]; }
   Matrix(int d, double M[]);
   Matrix(int d, int M[]);
   Matrix(const Matrix&);
   const Matrix& operator=(const Matrix&);
   ~Matrix() { delete [] _rep; }
   const double& operator()(int r, int c) const { return _rep[r * n + c]; }
   double& operator()(int r, int c) { return _rep[r * n + c]; }
   double determinant() const;
   friend std::ostream& operator<<(std::ostream&, const Matrix&);
};

Matrix::Matrix(int d, double M[]) : n(d) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M[i * n + j];
}
Matrix::Matrix(int d, int M[]) : n(d) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M[i * n + j];
}

Matrix::Matrix(const Matrix& M) : n(M.n) {
   int i, j;
   _rep   = new double [n*n];
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M._rep[i * n + j];
}

const Matrix& Matrix::operator=(const Matrix& M) {
   int i, j;
   if (n != M.n) {
      delete [] _rep;
      n = M.n;
      _rep = new double [n*n];
   }
   for (i = 0; i < n; i++)
      for (j = 0; j < n; j++)
         _rep[i * n + j] = M._rep[i * n + j];
   return *this;
}

double Matrix::determinant() const {
   Matrix A = *this;
   double det;
   int i, j, k;

   for (i = 0; i < n-1; i++) {
      // assert(a(i, i) == 0);
      for (j = i + 1; j < n; j++)
         for (k = i + 1; k < n; k++) {
            A(j,k) = (A(j,k)*A(i,i)-A(j,i)*A(i,k));
	    if (i) A(j, k) /= A(i-1,i-1);
         }
   }

   return A(n-1,n-1);
}

std::ostream& operator<<(std::ostream& o, const Matrix& M) {
   int i, j;
   for (i = 0; i < M.n; i++) {
     for (j = 0; j < M.n; j++) {
       double d = M(i,j);
       o << M(i, j) << " ";
       o << d << std::endl;
     }
      o << std::endl;
   }
   return o;
}

int main( int argc, char *argv[] ) {
  double e, f;

  double A[] = { 512.0,  512.0,  512.0, 1.0,
                512.0, -512.0, -512.0, 1.0,
               -512.0,  512.0, -512.0, 1.0,
               -512.0, -512.0,  512.0, 1.0 };
  Matrix m(4, A);
  e = m.determinant();
  std::cout << "Determinant of A = " << e << std::endl ;
  std::cout << "  Note: Determinant of A will overflow in CORE_LEVEL 1 \n\n";

  double B[] = { 3.0, 0.0, 0.0, 1.0,
               0.0,   3.0, 0.0, 1.0,
               0.0,   0.0, 3.0, 1.0,
               1.0,   1.0, 1.0, 1.0 };
  Matrix n(4, B);
  f = n.determinant();
  std::cout << "Determinant of B = " << f << std::endl ;
  std::cout << "  Note: Determinant of B should be 0, but shows non-zero in CORE_LEVEL 1\n\n";
 
  return 0;

}

