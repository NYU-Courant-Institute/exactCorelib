//////////////////////////////////////////////////////////////////
//
// interval.h
//		-- simple interval class
//
//////////////////////////////////////////////////////////////////

#ifndef __CORE_INTERVAL_H__
#define __CORE_INTERVAL_H__

#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "base.h"

class interval;
std::ostream& operator<<(std::ostream& os, const interval & in);

//////////////////////////////////////////////////////////////////
//
// very basic interval class
//
class interval {
public:
	real a;				// left endpoint
	real b;				// right endpoint
	int level;		// level

  int a_data = -1;
  int m_data = -1;
  int b_data = -1;
	  
	interval() : a(0), b(0), level(0) {};
	interval(real x) : a(x), b(x), level(0) {};
	interval(real x, real y, int l=0) : level(l) { if (x<y) { a=x; b=y; } else { a=y; b=x; } };

	// midpoint, width and radius are methods of interval:
	real m() {return (a+b)/2;}
	real w() {return (b-a);}
	real r() {return (b-a)/2;}
	  
	// Note: interval + real is OK, but not real + interval.
	interval operator + (real x) {
	  return interval(a+x,b+x);
	}

	interval operator + (interval I) {
	  return interval(a+I.a,b+I.b);
	}

	// Note: interval - real is OK, but not real + interval.
	interval operator - (real x) {
	  return interval(a-x,b-x);
	}

	interval operator - (interval I) {
	  return interval(a-I.b,b-I.a);
	}

	// Note: interval * real is OK, but not real * interval.
	interval operator * (real x) {
	  if (x>0)
	    return interval(x*a,x*b);
	  else
	    return interval(x*b,x*a);
	}

	interval operator / (real x) {
	  if (x>0)
	    return interval(a/x,b/x);
	  else
	    return interval(b/x,a/x);
	}

	interval operator * (interval I) {
	  std::vector<real> x(4);
	  x[0] = a*I.a;
	  x[1] = a*I.b;
	  x[2] = b*I.a;
	  x[3] = b*I.b;
	  return interval(*std::min_element(x.begin(), x.end()),
										*std::max_element(x.begin(), x.end()));
	}

	interval operator / (interval I) {
	  std::vector<real> x(4);
	  x[0] = a/I.a;
	  x[1] = a/I.b;
	  x[2] = b/I.a;
	  x[3] = b/I.b;
	  return interval(*std::min_element(x.begin(), x.end()),
										*std::max_element(x.begin(), x.end()));
	}

	////////////////////////////////////////////////////
	// interval predicates
	bool operator == (interval I) {
	  if ((I.a == a) && (I.b == b)) return true;
	  return false;
	}

	bool zerop () { // is 0 in interval?
	  if ((a <= 0) && (0 <= b)) return true;
	  return false;
	}

  bool contains(real x) { // is x in interval?
    if ((a <= x) && (x <= b)) return true;
    return false;
  }

	// Two ways to convert interval to string:
	// 	(1) show() for end-point representation
	// 	(2) showm() for mid-point representation
	// Both takes 2 optional arguments:
	// 	string (default=""), and int (precision default=5) 
	// 	E.g., I.show("\n", 5) or I.show(", ").
	std::string show(std::string end = "",
		unsigned int prec=5) {
	    std::stringstream ss;
	    ss.precision(prec);
	    //  ios_base::fmtflags ff = ss.flags();
	    //  ff |= ios_base::floatfield;
	    //  ff |= ios_base::fixed;
	    //  ss.flags(ff);
	    ss << "[" << a << ", " << b << "]" << end;
	    return ss.str();
	}
	std::string showm(std::string end = "",
		unsigned int prec=5) {
	    std::stringstream ss;
	    ss.precision(prec);
	    ss << "[" << m() << " +/- " << r() << "]" << end;
      return ss.str();
	}

	// Unit test
	static bool test() {
	  bool fail = false;
	  std::cout << "=====> TESTING interval class"
	      << std::endl;
	  interval I(-1.0,1.0);
	  interval J(1.001234567, 2.001234567);
	  std::cout << "I = " <<  I.show("\n");  
	  std::cout << "J = " <<  J.show() << std::endl;
	  std::cout << "J = (prec=7) " <<  J.show("\n",7);
	  std::cout << "I+J = " <<  (I+J).show("\n");
	  std::cout << "I+J = (mid-form) "
	      <<  (I+J).showm() << std::endl;
	  std::cout << "I*J = " <<  (I*J).show("\n");
	  std::cout << "I+3 = " <<  (I+3).show("\n");
	  std::cout << "I*3 = " <<  (I*3).show(" (prec=7)\n",7);
	  std::cout << "-- NOTE: 3+I and 3*I not allowed!! \n";
	  std::cout << "J.m() = " <<  J.m() << std::endl;
	  std::cout << "J.w() = " <<  J.w() << std::endl;
	  std::cout << "J.r() = " <<  J.r() << std::endl;
	  //////////
	  std::cout << "=====> another interval K" << std::endl;
	  interval K(-3.0,1.0);
	  std::cout << "K = " <<  K.show("\n");  
	  std::cout << "I+K = " <<  (I+K).show("\n");
	  std::cout << "I+K = " <<  (I+K).showm("\n");
	  std::cout << "I*K = " <<  (I*K).show("\n");
	  //////////
	  std::cout << "cout << interval(-4.01234,-1.01234): "
		<< std::setprecision(4)
		<< interval(-4.0123,-1.0123)
		<< " (prec=4)" << std::endl;
	  std::cout << "=====> end TESTING interval class"
	  << std::endl;
	  return true;
	}
}; //interval class


//////////////////////////////////////////////////////////////////
//
// operator << (os, interval)
// 	NOTE: use std::setprecision(n) to control precision of numbers
// 	(see unit test)
//
std::ostream& operator<<(std::ostream& os, const interval & in) {  
	os << "[" << in.a << ", " << in.b << "]";  
	return os;  
}  

#endif
//
//////////////////////////////////////////////////////////////////
