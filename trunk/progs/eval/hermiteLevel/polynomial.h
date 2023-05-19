//////////////////////////////////////////////////////////////////
//
// polynomial.h
//		-- simple polynomial class
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_POLYNOMIAL_H__
#define __CORE_POLYNOMIAL_H__

#include "interval.h"

// Friends:
natural binomial(int n, int k);
natural ffact(int n, int k);

class polynomial;
std::ostream& operator<<(std::ostream& os, const polynomial & p);

//////////////////////////////////////////////////////////////////
//
// compute "n choose k"
//
class polynomial {
public:
	int n;		// degree
	std::vector<real> c; // coefficients
	
  //////////////////////////////////////////////////////////////
  // CONSTRUCTORS
  //
  polynomial() : n(-1)  {		// this represents the zero polynomial
		c.push_back(0); 	// but this polynomial seems strange
		// (See bug in unit test)
  };

	polynomial(int nn) { 
		n = nn;
		if (nn >= 0) 
			for (int i=0; i<=nn; i++)
				c.push_back(0);	// users must fill in the coefficients
	};

	polynomial(std::vector<real> v) {
		n = -1;
		for (std::vector<real>::iterator it= v.begin(); it != v.end(); it++) {
		  c.push_back(*it);
		  n++;
		}
		if (c[n] == 0)
			std:: cout<< "WARNING: c[n] should not be zero!\n";
	};

	//////////////////////////////////////////////////////////
	// 
	// equality
	//
	bool operator==(polynomial q) {	
	  if (n != q.n) return false;
	  for (int i=0; i<=n; i++)
      if (c[i] != q.c[i]) return false;
    return true; 
	}

	//////////////////////////////////////////////////////////
	//
	// evaluate f at x, using Horner's method
	// 	(x can be real or interval)
	//
	real eval(real x) {	
	  real fx = c[n];
	  for (int i=n-1; i>=0; i--)	
	    fx = x * fx + c[i];
	  return fx;
	}

	interval eval(interval x) {
	  interval fx(c[n]);
	  for (int i=n-1; i>=0; i--) {
	    fx = x * fx + c[i];
	  }
	  return fx;
	}

	//////////////////////////////////////////////////////////
	//
	// evaluate f^{(k)} at x, using Horner's method
	// 	 (optional argument: k=1)
    //      (x can be real or interval)
	//
	real evalDiff(real x, int k=1) {
	  if (k == 0)
	     return eval(x);
	  natural factor = n;
	  for (int j=1; j<k; j++)
	    factor *= (n-j);
	  real dfx = factor*c[n];
	  for (int i=n-1; i>=k; i--) {
	    factor /= (i+1);
	    factor *= (i-k+1);
	    dfx = dfx * x + factor*c[i];
	  }
	  return dfx;
	}
        
	interval evalDiff(interval x, int k=1) {
		if (k == 0)
			return eval(x);
		natural factor = n;
		for (int j=1; j<k; j++)
		factor *= (n-j);
		interval dfx = interval(c[n]) * factor;
		for (int i=n-1; i>=k; i--) {
		factor /= (i+1);
		factor *= (i-k+1);
		dfx = dfx * x + factor*c[i];
		}
		return dfx;
	}

	///////////////////////////////////////////////////////////
	//
	// evaluate f^{(k)} / k! at x, using Horner's method
	// 	This is called "Taylor coefficients"
	// 	Optional argument: k=1
	//
	real evalTaylor(real x, int k=1) {
		if (k == 0)
			return eval(x);
	  natural factor = binomial(n,k);
	  real dfx = factor*c[n];
	  for (int i=n-1; i>=k; i--) {
	    factor *= (i-k+1);
	    factor /= (i+1);
	    dfx = dfx * x + factor*c[i];
	  }
	  return dfx;
	}

	///////////////////////////////////////////////////////////
	//
	// Truncated Taylor shift:
	// 
	// 	shift(m,t) computes
	// 		the polynomial g(x) := f(x+m)
	// 		but truncated at degree t (t <= n = deg(f))
	//
	// 	t is optional (default t=-1, viewed as t=n)
	//
	//	Precisely, if the Taylor expansion of f(x) at m is
	//	  f(x) = f_0 + f_1.(x-m) + f_2.(x-m)^2 +...+ f_t.(x-m)^t
	//	and
	// 		f_k :=  f^{(k)}(m) / k! 
	// 	then
	//        g(y) = f_0 + f_1.y   + f_2.y^2 +...+  f_t.y^t
	//
	// 	Using efficient way to compute (f_0, f_1,..., f_t)
	// 	In the X-version, we use an INEFFICIENT WAY!
	//
	polynomial shift(real m, int t=-1) {
		// If t<0, it means have no truncation of polynomial:
		if (t<0) t = n;
	  std::vector<real> seq;	// coeff of shifted polynomial
    std::vector<real> cc = c;	// initialize coefficients

	  for (int k=0; k<=t; k++) {
			// cc[k..n] are the coefficients of f^{(k)}/k!
			real sum = 0; // sum = Horner eval of f^{(k)}(m)/k!
			for (int i = n; i>=k; i--) {
		    sum = sum * m + cc[i];
		    cc[i] = cc[i] * (i-k) / (1+k); // update cc
			}
			seq.push_back(sum);
		}
	  return polynomial(seq);
  }

	// shiftX(m,t) 
	//   uses inefficient method to compute shift(m,t)
	//   	(for comparison):
	polynomial shiftX(real m, int t=-1) {
	  if (t<0) t=n;
	  std::vector<real> seq; // coeff of shifted polynomial
	  for (int k=0; k<=t; k++) {
			seq.push_back(evalTaylor(m,k));
	  }
	  return polynomial(seq);
	}

	///////////////////////////////////////////////////////////
	//
	// compute f^{(k)} = differentiate f for k times
	// 	k is optional (default k=1)
	//
	polynomial diff(int k=1) {
	  if (k>n) return polynomial();	// return the 0 polynomial
	  natural factor = ffact(k,k);  // factor=k!
	  std::vector<real> dc;
	  for (int j=0; j<=n-k; j++) {
	  	dc.push_back(c[j+k] * factor);
		factor *= (j+1+k);
		factor /= (j+1);
	  }
	  return polynomial(dc);
	}

	///////////////////////////////////////////////////////////
	// Two ways to display a polynomial:
	// 	(1) show() for list of coefficients representation
	// 	(2) showm() for monomial representation
	// Both takes an optional string (default to "\n"). 
	// 	E.g., I.show("") or I.show(", ").
	// 
	std::string show(std::string end = "\n",
		unsigned int prec=5) const {
	    std::stringstream ss;
	    ss.precision(prec);
	    ss << "deg=" << n;
	    if (n >= 0) {
				ss << ", coef=[" << c[n];
	      for (int i=n-1; i>=0; i--)
	  	    ss << ", " << c[i];
	      ss << "]" << end;
	    }
			return ss.str();
	}

	// THIS IS A SOPHISTICATED DISPLAY FUNCTION (it could
	// probably be streamlined, but be less comprehensible)
	std::string showm(std::string end = "\n",
		unsigned int prec=5) const {
	    std::stringstream ss;
	    ss.precision(prec);
	    if (n <= 1) { // process n<=1 and n>=2 separately
				if (n < 0)
					ss << "0";	// zero polynomial!
				else if (n==0)
					ss << c[0];
				else { //n==1
					if (c[1] != 0) // no output if c[1]=0
						if (c[1] < 0)
							if ((c[1] == -1) || (c[1] == -1.0))
		    				ss << "x";
		    	    else
		    				ss << "-" << -c[1] << ".x";
		        else
							if ((c[1] == 1) || (c[1] == 1.0))
		    				ss << "x";
		    	    else
		    				ss << c[1] << ".x";
					if (c[0] != 0) // no output if c[0]=0
						if (c[0] < 0)
		    			ss << "- " << -c[0];
		        else
		    			ss << "+ " << c[0];
				}
				ss << "." << end;
				return ss.str();
	    }// end if n<=1

	    // NOW, n>=2
	    if (c[n] == 1)	// buggy if c[n]=0
	    	ss << "x^" << n;
	    else if (c[n] == -1)
	    	ss << "-x^" << n;
	    else
	    	ss << c[n] << ".x^" << n;
	    for (int i=n-1; i>1; i--) {
				if (c[i] != 0) // no output if c[i]=0
					if ((c[i] == 1) || (c[i] == 1.0))
						ss << "+ x^" << i;
					else if ((c[i] == -1) || (c[i] == -1.0))
						ss << "- x^" << i;
					else if (c[i] < 0)
						ss << "- " << -c[i] << ".x^" << i;
	        else
						ss << "+ "<< c[i] << ".x^" << i;
	    }//for

	    if (c[1] != 0) // no output if c[1]=0
				if (c[1] < 0)
					if ((c[1] == -1) || (c[1] == -1.0))
	    			ss << "- x";
	    	  else
	    			ss << "- " << -c[1] << ".x";
	      else
					if ((c[1] == 1) || (c[1] == 1.0))
	    			ss << "+ x";
	    	  else
	    			ss << "+ " << c[1] << ".x";
	    if (c[0] != 0) // no output if c[0]=0
				if (c[0] < 0)
	    		ss << "- " << -c[0];
	      else
	    		ss << "+ " << c[0];
	    ss << "." << end;
	    return ss.str();
	}
	
	// Unit Test
	static void test(){
      	    std::cout << "=====> TESTING polynomial class <======"
		<< std::endl;
	    testConstruct();
	    testEval();
	    testMisc();
	    testDiff();
	    testShift();
      	    std::cout << "=====> END TESTING polynomial class <======"
		<< std::endl;
	}
	static void testConstruct(){
      	    std::cout << "=====> TESTING construction/display"
		<< std::endl;
	    std::cout << "The zero polynomials: \n";
	    polynomial z;   		// CAREFUL: DO NOT say "poly z();"
	    polynomial zz(0);  	
	    std::cout << "-- polynomial z : " << z 
			<< ", polynomial zz(0) : " << zz << std::endl;
	    std::cout << z << ", " << zz << std::endl;
	    std::cout << "vector v = {-1, -1, 0}" << std::endl;
	    std::cout << "-- polynomial(v) has leading coeff warning:\n";
	    std::vector<real> v = { -1.0, -1.0, 0};
	    polynomial p(v); // should get warning of zero leading coeff
	    std::cout << "vector v = {-1, -1, 0, 1}" << std::endl;
	    v = std::vector<real>{ -1.0, -1.0,  0, 1.0};
	    p=polynomial(v);	// set the leading coeff to 1.0
	    std::cout << "polynomial(v).show() = " <<
	    	p.show(" (Note reversed listing of coeff)\n");
	    std::cout << "polynomial(v).showm() = " << p.showm();
	    std::cout << "cout << polynomial(w) : " << p << std::endl;
	}//
	    ///////////////
	static void testEval(){
      	    std::cout <<
		"=====> TESTING Evals (on reals or intervals)"
		<< std::endl;
	    std::vector<real> v = { -1, -1, 0, 1};
	    polynomial p(v); 
	    std::cout << "polynomial p = " << p << std::endl;
	    std::cout << "p.eval(-2) = "
	     		<< p.eval(-2) << std::endl;
	    std::cout << "p.eval(-2.0123) = "
	     		<< p.eval(-2.0123) << std::endl;
	    std::cout << "p.eval(interval(1,2)) = "
	     		<< p.eval(interval(1,2))
			<< "(answer is [-1,1])" << std::endl;
	    std::cout << "p.eval(interval(0,3.123)) = "
	     		<< p.eval(interval(0,3.123)) << std::endl;
	}//
	    ///////////////
	static void testDiff(){
      	    std::cout << "=====> TESTING differentiations" << std::endl;
	    std::vector<real> v = { -1, -1, 0, 1};
	    polynomial p(v); 
	    std::cout << "polynomial p = " << p << std::endl;
	    std::cout << "p.diff(1) : " << p.diff(1) << std::endl;
	    std::cout << "p.diff(2) : " << p.diff(2) << std::endl;
	    std::cout << "p.diff(3) : " << p.diff(3) << std::endl;
	    std::cout << "p.diff(4) : " << p.diff(4) << std::endl;
	    ///////////////
      	    std::cout << "=====> Eval of differentiations "
		<< std::endl;
	    std::cout << "p.evalDiff(1.23, 1) = " 
			<< p.evalDiff(1.23, 1) << std::endl;
	    std::cout << "p.evalDiff(1.23, 2) = " 
			<< p.evalDiff(1.23, 2) << std::endl;
	    std::cout << "p.eval(1.23, 3) = " 
			<< p.evalDiff(1.23, 3) << std::endl;
	    std::cout << "p.eval(1.23, 4) = " 
			<< p.evalDiff(1.23, 4) << std::endl;
	    ///////////////
      	    std::cout << "=====> Taylor coeffs" << std::endl;
	    std::cout << "p.evalTaylor(1.23, 1) = " 
			<< p.evalTaylor(1.23, 1) << std::endl;
	    std::cout << "p.evalTaylor(1.23, 2) = " 
			<< p.evalTaylor(1.23, 2) << std::endl;
	    std::cout << "p.evalTaylor(1.23, 3) = " 
			<< p.evalTaylor(1.23, 3) << std::endl;
	    std::cout << "p.evalTaylor(1.23, 4) = " 
			<< p.evalTaylor(1.23, 4) << std::endl;
	}
	    ///////////////
	static void testMisc(){
      	    std::cout << "=====> Miscellaneous" << std::endl;
	    std::cout << "ffact(5,2) = " << ffact(5,2) << "\n";
	    std::cout << "binomial(5,2) = "
		<< binomial(5,2) << std::endl;
	}
	    ///////////////
	static int testShift(){
	    int err=0; // number of errors
      	    std::cout << "=====> Shifts" << std::endl;
	    std::vector<real> v = { -1, -1, 0, 1};
	    polynomial p(v); 
	    std::cout << "polynomial p = " << p << std::endl;
	    std::cout << "p.showm() : "
			<<  p.showm(" (monomial form)\n");
	    std::cout << "p.shift(2) = " << p.shift(2) << "\n";
	    std::cout << "-- the next result should be the same\n";
	    std::cout << "p.shiftX(2) = " << p.shiftX(2) << "\n";
	    std::vector<real> vv = {5, 11, 6, 1};
	    if (p.shift(2) == polynomial(vv))
		std::cout << "OK! p.shift(2) is correct\n";
	    else {
		std::cout << "ERROR! p.shift(2) is wrong\n";
		err++;
	    }
	    ///////////////
	    std::cout << "p.shift(-1) = " << p.shift(-1) << "\n";
	    std::cout << "-- the next result should be the same\n";
	    std::cout << "p.shiftX(-1) = " << p.shiftX(-1) << "\n";
	    if (p.shift(-1.23) == p.shiftX(1.23)) {
		err++;
	    	std::cout
		  << "ERROR! p.shiftX(-1.23) == p.shiftX(1.23)\n";
	    }
	    else
	    	std::cout
		  << "OK! p.shiftX(-1.23) != p.shiftX(1.23)\n";
	    if (p.shift(-1.23) == p.shiftX(-1.23))
	    	std::cout
		  << "OK! p.shiftX(-1.23) == p.shiftX(-1.23)\n";
	    else {
		err++;
	    	std::cout
		  << "ERROR! p.shiftX(-1.23) != p.shiftX(-1.23)\n";
	    }
	    if (err == 0)
	    	std::cout << "No ERRORS found!\n";
	    return err;
	}
}; // polynomial


/////////////////////////////////////////////////////////////////
//
// compute "falling factorial or pochhammer symbol (n)_k=n(n-1)..(n-k+1)"
// 	So (n)_n is n!
//  	friend of polynomial
//
natural ffact(int n, int k) {
	if ( n < k ) return 0;
	natural f = n;
	for (int i=1; i<k; i++)
	    f *= n-i;
	return f;
}

//////////////////////////////////////////////////////////////////
//
// compute "n choose k"
//  	friend of polynomial
//
natural binomial(int n, int k) {
	if ( (k < 0) || (k > n)) 
	  return 0;
	if (n-k < k)
	  k = n-k;
	natural b = 1;
	for (int d=1; d<=k; d++) {
	  b *= n--;
	  b /= d;
	}
	return b;
}

//////////////////////////////////////////////////////////////////
//
// operator << (os, polynomial)
//
//  REMARK: if v is a vector<real>, and we call
// 		 cout << v;
// 	then the compiler constructs polynomial(v) and calls
// 	this operator<<.
//
std::ostream& operator<<(std::ostream& os, const polynomial & p) {  
	os << p.showm();
  return os;  
}  

#endif
//
//////////////////////////////////////////////////////////////////
