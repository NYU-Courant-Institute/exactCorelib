//////////////////////////////////////////////////////////////////////////
//
// compile:
//
// gcc mignotte.cpp -lstdc++ -o mignotte.exe
//
//////////////////////////////////////////////////////////////////////////
//
// run:
//
// ./mignotte 15 4 
// ./mignotte 15 4 0.0 1.5
//
//////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////
//
// main program
//
int main (int argc, char *argv[]) {
  if (argc < 3)
    return 0;
    
  //
  // read degree and coefficient a from command line
  //
  int n = atoi(argv[1]);
  int a = atoi(argv[2]);
  
  //
  // write degree and coefficients of Mignotte's polyomial
  //
  cout << n << endl;
  cout << "1" << endl;
  for (int i=n-1; i>=3; i--)
    cout << "0" << endl;
  cout << -2*a*a << endl;
  cout << 4*a << endl;
  cout << "-2" << endl;
  
  cout << "0" << endl;
  
  //
  // read and write interval bounds from command line (if given)
  //
  if (argc >= 5) {
    double l = atof(argv[3]);
    double r = atof(argv[4]);
    cout << l << endl;
    cout << r << endl;
  }
  else
    //
    // use interval [0,1] by default
    //
    cout << "0.0\n1.0\n";
}
//
//////////////////////////////////////////////////////////////////////////

