//////////////////////////////////////////////////////////////////
//
// eval.h
//		-- general root search class, based on EVAL
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_EVAL_H__
#define __CORE_EVAL_H__

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

#include "base.h"
#include "interval.h"
#include "polynomial.h"

//
// different output options
//
bool verbose = false;
bool laconic = false;
bool runtime = false;
bool treesize = false;

class eval{
public:
  polynomial f;         // the input polynomial 
  interval I0;          // the interval to search
  int m;								// number of roots (if known)
  vector<real> z;				// the roots of f (if known)
  queue<interval> Q;		// intervals to be investigated
  vector<interval> Z;   // intervals that contain a zero
  int numInt;           // total number of intervals (tree size)
	string name;          // name of the method

  chrono::duration<double> time;    // running time
  int N;                // counter for the number of runs of EVAL
  
  //////////////////////////////////////////////////////////////
  //
  // CONSTRUCTORS
  //

  //////////////////////////////////////////////////////////////////////////
  //
  // given vector of polynomial coefficients and the intial interval
  //
  eval(vector<real> v, interval I) : time(0), N(0) {
		f = polynomial(v);
		f.n = v.size() -1;
		m = 0;
		I0 = I;
	};

  //////////////////////////////////////////////////////////////////////////
  //
  // read polynomial in "Frisco Format" or simple text format and 
  // also the initial interval from cin
  //
  eval() : time (0), N(0) {	
    string s;
    getline(cin,s);

    if (s[0] == '!') {
      // 
      // if the input starts with "!", we expect the polynomial to be 
      // given in the Frisco format; otherwise we expect a simple text format
      //
      if (!laconic)
        cout << "reading Frisco format";
        
      //
      // skip possible comments at the beginning of the file
      //
      do {
        getline(cin,s);
      } while (s[0] == '!');

      if ( (s[1] != 'r') || (s[2] != 'i') ) {
        cout << "\nWARNING: can read only non-complex integer coefficients" << endl;
        exit(0);
      }
    
      switch (s[0]) {
      case 'd':
        //
        // read dense polynomial
        //
        if (!laconic)
          cout<<" (dense)"<<endl;
        cin >> f.n;           // read (and ignore) precision (should be '0' anyway)

        //
    	  // read degree and coefficients of f
        //
        cin >> f.n;
    	  f.c.resize(f.n+1);
    	  for (int i=0; i<=f.n; i++)
    	    cin >> f.c[i];

        //
    		// read endpoints of initial interval
        //
    	  cin >> I0.a >> I0.b;

      break;

      case 's':
        //
        // read sparse polynomial
        //
        if (!laconic)
          cout<<" (sparse)"<<endl;
        cin >> f.n;           // read (and ignore) precision (should be '0' anyway)

        //
    	  // read degree and coefficients of f
        //
        cin >> f.n;
    	  f.c.resize(f.n+1);
        int m;
        cin >> m;
        int j;
    	  for (int i=0; i<m; i++) {
          cin >> j;
    	    cin >> f.c[j];
        }

        //
    		// read endpoints of initial interval
        //
    	  cin >> I0.a >> I0.b;

      break;
      
      default:
        cout << "\nWARNING: can read only dense and sparse polynomials"<< endl;
      }
    }
    else {
      //
      // if the input does not start with "!", we expect a simple text format:
      //
      //   n          // degree of the polynomial
      //   c_n
      //   c_{n-1}    // (monomial) coefficients of the polynomial
      //   ...        // in decreasing order, that is
      //   c_1        // p(x) = \sum_{i=0}^n c_i x^i
      //   c_0
      //   m          // number of known zeros
      //   z_1
      //   ...        // list of m known zeros
      //   z_m
      //
      if (!laconic)
        cout << "reading simple txt format" << endl;
        
      //
  	  // read degree and coefficients of f
      //
      istringstream is(s);
  	  is >> f.n;
  	  f.c.resize(f.n+1);
  	  for (int i=f.n; i>=0; i--)
  	    cin >> f.c[i];
  
      //
  	  // read number and values of zero (if any)
      //
  		cin >> m;
  	  z.resize(m);
  	  for (int i=0; i<m; i++)
  	    cin >> z[i];
  	  sort(z.begin(),z.end());
  
      //
  		// read endpoints of initial interval
      //
  	  cin >> I0.a >> I0.b;
    }
	};

  //////////////////////////////////////////////////////////////
  //
  // Statistics
  //
  void stats(int prec = 5) {
    if (!laconic) {
    	cout.precision(prec);
      cout << "STATISTICS for " << name << "\n\n";
    }
    else 
      cout.precision(4);
    
    #ifdef RATIONAL
    if (!laconic)
      cout << "-- using multi-precision rational arithmetics" << endl;
    #else
    if (!laconic)
      cout << "-- using floating point arithmetics with " << mpf_get_default_prec() << " bits precision" << endl;
    #endif
    
    if (verbose)
  		cout << "-- INPUT polynomial f = " << f << "\n";

    if (!laconic)
  		cout << "-- INPUT interval I = " << I0 << "\n";
    if (!laconic)
   		cout << "-- tree size = " << numInt << endl; 
    if (verbose) {
  		cout << "-- These " << Z.size() 
  				 << " intervals are found with zeros:\n";
  	  for (interval I : Z)
  	    cout << "    " << I 
  	         << "  in level " << I.level << endl;
  	  cout << endl;
    }
    else 
      if (!laconic)
        cout << "-- " << Z.size() << " roots were found.\n";
      
		// print zeros found (if any)
	  if (m > 0) {
      if (verbose)
        cout << "-- Known zeros that were found: \n";
      bool allfound = true;
	    for (int i=0; i<m; i++) {
	      bool found = false;
	      for (interval I : Z)
	        if ( (I.a <= z[i]) && (z[i] <= I.b) ) {
	          found = true;
            if (verbose) {
  	          cout << "    " << z[i] 
  								 << " in " << I << endl;
            }
	          break;
	        }
	      if (!found) {
          if (verbose)
	         cout << z[i] << " not found" << endl;
          allfound = false;
        }
	    }
      if (!verbose) {
        if (allfound)
          cout << "-- ALL stated roots were found\n";
        else
          cout << "-- NOT all stated roots were found\n";
      }
	  }
    
    // execution time
    if (!laconic)
      cout << "\nTime to find roots (averaged over " << N << " runs): " 
           << time.count()*1000/(double)N << " ms\n";
           
    if (laconic) {
      if (runtime)
        cout << time.count()/(double)N;
      else if (treesize)
        cout << numInt;
      else {
        cout << name << ", " << I0.a << ", " << I0.b << ", ";
        cout << numInt << ", ";
        cout << Z.size() << ", ";
        cout << time.count()/(double)N << endl;
      }
    }
    
  } // stats

  //////////////////////////////////////////////////////////////
  //
  // split interval I into left and right half
  //
  virtual interval split_left(interval& I) = 0;
  virtual interval split_right(interval& I) = 0;
  
  //////////////////////////////////////////////////////////////
  //
  // range functions for f and f'
  //
	virtual interval get_f (interval& I) = 0;
	virtual interval get_df(interval& I) = 0;
  
  //////////////////////////////////////////////////////////////
  //
  // EVAL algorithm
  //
  void EVAL(int prec = 0) {
    numInt = 0;
    Z.clear();

    auto start = chrono::high_resolution_clock::now();
    //
  	// check if the endpoints of the initial interval I0 = [a,b] are zeros of f
    //
    if (f.eval(I0.a) == 0) {
      interval I(I0.a);
      Z.push_back(I);
    }
    if (f.eval(I0.b) == 0) {
      interval I(I0.b);
      Z.push_back(I);
    }
    
    //
    // analyze and subdivide I0
    //
    Q.push(I0);
    while (!Q.empty()) {
      numInt++;
      interval I = Q.front(); Q.pop();
  
      if (get_f(I).contains(0))               // is 0 in []f(I) ?
        if (get_df(I).contains(0)) {          // is 0 in []f'(I) ?
          Q.push(split_left(I));
          Q.push(split_right(I));
  
          // check if midpoint of I is zero of f        
          if (f.eval(I.m()) == 0)
            Z.push_back(I.m());
        } 
        else
          if (f.eval(I.a) * f.eval(I.b) < 0) {
            Z.push_back(I);
          }
  	} // while
  
    auto end = chrono::high_resolution_clock::now();
    time += end-start;
    N++;
  } // EVAL

}; // roots class
//
//////////////////////////////////////////////////////////////////
#endif
