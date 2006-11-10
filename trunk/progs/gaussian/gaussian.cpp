/* **************************************
   File: gaussian.cc

   Purpose: This is a simple example using the straightforward
	Gaussian elimination to compute matrix determinants.
	It does not do pivoting -- so the input matrices
	must have non-zero principal minors! 

   Usage:
	gaussian <input_file> <number of iterations>

	where <input_file> contains the matrix
	  and <number_of_iterations> is the number of times
	  to run the program.

   Since CORE Library Version 1.2
   $Id: gaussian.cpp,v 1.3 2006-11-10 21:08:05 exact Exp $
 ************************************** */

#ifndef CORE_LEVEL
#   define CORE_LEVEL 3
#endif

#include <fstream>
#include "CORE.h"

double* SNF;
int s, dim_n, dim_m;

class Matrix {
public:
  int     n, m;
private:
  double* _rep;

public:
   Matrix(int r, int c) : n(r), m(c) { _rep   = new double [n*m]; }
   Matrix(int r, int c, double M[]);
   Matrix(const Matrix&);
   const Matrix& operator=(const Matrix&);
   ~Matrix() { delete [] _rep; }
   const double& operator()(int r, int c) const { return _rep[r * m + c]; }
   double& operator()(int r, int c) { return _rep[r * m + c]; }
   double determinant() const;
   void row_minus(int x);
   void row_switch(int x);
   void col_minus(int x);
   void col_switch(int x);
   bool isZero();
   friend std::ostream& operator<<(std::ostream&, const Matrix&);
};

void solve(Matrix& A); 

bool Matrix::isZero() {
  bool ret = true;
   for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
         if ((*this)(i,j) != 0) {
           ret = false;
           break;
         }
  return ret;
}
  
  

Matrix::Matrix(int r, int c, double M[]) : n(r), m(c) {
   int i, j;
   _rep   = new double [n*m];
   for (i = 0; i < n; i++)
      for (j = 0; j < m; j++)
         _rep[i * m + j] = M[i * m + j];
}

Matrix::Matrix(const Matrix& M) : n(M.n), m(M.m) {
   int i, j;
   _rep   = new double [n*m];
   for (i = 0; i < n; i++)
      for (j = 0; j < m; j++)
         _rep[i * m + j] = M._rep[i * m + j];
}

const Matrix& Matrix::operator=(const Matrix& M) {
   int i, j;
   if (n != M.n || m != M.m) {
      delete [] _rep;
      n = M.n;
      m = M.m;
      _rep = new double [n*m];
   }
   for (i = 0; i < n; i++)
      for (j = 0; j < m; j++)
         _rep[i * m + j] = M._rep[i * m
 + j];
   return *this;
}

void Matrix::row_minus(int x) {
  double temp = (*this)(x, 0);
  for (int i = 0; i < m; ++i) {
    (*this)(x,i) = (*this)(x,i) - (*this)(0,i) * floor(temp/(*this)(0,0));
  }
  std::cout << "row_minus of A = \n" << (*this) << std::endl;
  for (int i = 0; i < m; ++i) {
    if ( abs((*this)(x,i)) != 0 && abs((*this)(x,i)) < abs((*this)(0,0))) {
      row_switch(x);
      col_switch(i);
      solve(*this); return; 
    }
  }
}

void Matrix::col_minus(int y) {
  double temp = (*this)(0, y);
  for (int j = 0; j < n; j++) {
    (*this)(j, y) = (*this)(j,y) - (*this)(j,0) * floor(temp/(*this)(0,0));
  }
  std::cout << "col_minus of A = \n" << (*this) << std::endl;
  for (int j = 0; j < n; j++) {
    if ( abs((*this)(j,y)) != 0 && abs((*this)(j,y)) < abs((*this)(0,0)) ) {
      row_switch(j); 
      col_switch(y);
      solve(*this); return;
    }
  }  
}

void Matrix::row_switch(int x) {
  for (int i = 0; i < m; ++i) {
    double temp = (*this)(0,i);
    (*this)(0,i) = (*this)(x,i);
    (*this)(x,i) = temp;
  }
}

void Matrix::col_switch(int y) {
  for (int j = 0; j < n; ++j) {
    double temp = (*this)(j,0);
    (*this)(j,0) = (*this)(j,y);
    (*this)(j,y) = temp;
  }
}

void min_matrix(Matrix& A) {
  if (A(0,0) == 0)  {
    for (int i = 0; i < A.n; i++) {
      for (int j = 0; j < A.m; j++) {
        if (A(i, j) != 0) {
          A.row_switch(i);
          A.col_switch(j);
          break;
        }
      }
    }    
  }
  double minA = A(0,0);

  int minr = 0, minc = 0;
  for (int i = 0; i < A.n; i++) {
    for (int j = 0; j < A.m; j++) {
      std::cout << "abs(A(" << i << "," << j << ")) = " << abs(A(i,j)) << std::endl;
      if ( A(i,j) != 0 && abs(A(i,j)) < abs(minA)) {
        std::cout << "minA = " << abs(minA) << " r:c=" << i << ":" << j << std::endl;
        minA = A(i, j);
        minr = i; minc = j;
      }
    }
  }
  std::cout << "min element, row:col" << minr << ":" << minc << std::endl;
  A.row_switch(minr);
  A.col_switch(minc);
}
 
Matrix decrease_matrix(Matrix& A) {
  Matrix B(A.n - 1,A.m - 1);
   for (int i = 0; i < A.n - 1; i++)
     for (int j = 0; j < A.m - 1; j++)
       B(i,j)=A(i+1,j+1);
  return B;
}

bool checkDivisible(Matrix &A) {
  for (int i = 1; i < A.n; i++)
    for (int j = 1; j < A.m; j++) {
        std::cout << "checkDivisible A=\n" << A << std::endl;
      if ( abs(gcd( ToBigInt( A(i, j) ), ToBigInt( A(0,0) ) ) ) != abs(A(0,0)) && A(i, j) != 0) {
        double temp = A(i,j);
        for (int k = 0; k < A.m; k++) {
          A(0, k) += A(i, k);
        }
        std::cout << "checkDivisible A=\n" << A << std::endl;
      
        for (int k = 0; k < A.n; k++) 
          A(k, j) = A(k, j)-(floor(temp / A(0, 0))) * A(k, 0);
        std::cout << "checkDivisible A=\n" << A << std::endl;

        return false;
      }
    }

  return true;
}

void solve(Matrix& A) { 
  if (A.isZero() || A.n == 1 || A.m == 1) {
    SNF[s] = A(0,0);
    s++;
    Matrix result(dim_n,dim_m);
    for (int i = 0; i < s; i++)
      result(i,i) = SNF[i];

    std::cout << "output matrix = \n" << result << std::endl;
    exit(0);
  }
    
  std::cout << "solve " << std::endl;

  do {
    min_matrix(A);
    for (int i = 1; i < A.m; i++)
      A.col_minus(i);
    for (int j = 1; j < A.n; j++)
      A.row_minus(j);
  } while (checkDivisible(A) == false);

  std::cout << "solve A=\n" << A << std::endl;

  SNF[s] = A(0,0);
  s++;
  std::cout << "solve A=\n" << A << std::endl;

  A = decrease_matrix(A);
  std::cout << "solve A=\n" << A << std::endl;
  solve(A);
}

double Matrix::determinant() const {
   Matrix A = *this;
   double det;
   int i, j, k;

   for (i = 0; i < n; i++) {
	// This is the non-pivoting version of
	// Gaussian elimination:
      	// assert(A(i,i) != 0);
      for (j = i + 1; j < n; j++)
         for (k = i + 1; k < n; k++) {
            A(j,k) -= A(j,i)*A(i,k)/A(i,i);
         }
   }

   det = 1.0;
   for (i = 0; i < n; i++)
      det *= A(i,i);

   return det;
}

std::ostream& operator<<(std::ostream& o, const Matrix& M) {
   int i, j;
   for (i = 0; i < M.n; i++) {
     for (j = 0; j < M.m; j++) {

       //       double d = M(i,j);
       o << M(i, j) << " ";
       //       o << d << std::endl;
     }
      o << std::endl;
   }
   return o;
}

std::vector<int> readMatrix(char *filename, double **A) {
    std::ifstream ifs(filename);
    if (!ifs) { 
       perror("cannot open the file");
       exit(1);
    }
    int n, m;
    int a;
    double la;

    ifs >> n >> m;

    *A = new double[n*m];

    for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
	ifs >> a;
        la = a;

        (*A)[j+i*m] =  la;
        std::cout << "A(" << i << "," << j << ")= " << la << " ";
      }
      std::cout << std::endl;
    }
    ifs.close();

    std::vector<int> vec;
    vec.push_back(n);
    vec.push_back(m);

    return vec;
}
 
int main( int argc, char *argv[] ) {

  double* input;
  std::vector<int> dim;
  dim = readMatrix(argv[1], &input);

  int n, m; n = dim[0], m = dim[1];
  std::cout << "m:n=" << m << ":" << n << std::endl;
  Matrix A(n, m, input);

  dim_n = n; dim_m = m;

  std::cout << "Input matrix = \n" << A << std::endl;
  min_matrix(A);
  std::cout << "min_matrix of A = \n" << A << std::endl;

  SNF = new double[std::min(n,m)];
  s = 0;

  std::cout << "begin solve(A) " << std::endl;
  solve(A);
  std::cout << "intermediate matrix = " << A << std::endl;
    


  return 0;
}
