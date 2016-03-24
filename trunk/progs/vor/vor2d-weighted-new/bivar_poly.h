#ifndef bivar_poly_h
#define bivar_poly_h

#include <assert.h>
#include <iostream>
#include <math.h>

using namespace std;

class BiPoly {
public:
  BiPoly() {

  }
  
  void print() {
    int i, j, k;
    j = 0;
    for (i = 0; i <= deg; i++) {
      j += i;
      for (k = 0; k < i + 1; k++) {
	cout << c[j + k];
	if (k > 0) {
	  cout << "x^" << k;
	}
	if (i - k > 0) {
	  cout << "y^" << (i - k)
	}
	cout << " ";
      }
    }
    cout << "\n";
  }

private:
  c[]; // coefficients
  int degree;
}

#endif // bivar_poly_h
