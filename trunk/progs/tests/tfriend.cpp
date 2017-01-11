// file:  tfriend.cpp
//
// Note: On the scope of friend method.
//	The rootOf() method of Core2 failed in
// 	the transition between g++4.9.3 and g++5.4.0.
// 	Michael Burr's posed a stackoverflow question to illustrate this error:
//
// 	http://stackoverflow.com/questions/41462908/scope-difference-between-g4-9-4-and-g5-4-0
//
// 	The following code of Burr
// 	works in g++4.9.3, but not in g++5.4.0:
/*****************************************
	template <typename T>
	class C{
	  template <typename S>
	  friend C fun(S &s){C d; return d;}
	};//C
	
	int main(){
	  int a = 0;
	  C<double> b = fun<int>(a);
	  return 0;
	}//main
*****************************************/
// 	The following modification works in g++5.4.0:

#include <iostream>

// Original definition of friend function "fun" in class C:
template <typename T>
class C{
  template <typename S>
  friend C fun(S &s,C &c) {C d;return d;}

    public:
  void show(){ std::cout << "This is an instance of C" << std::endl;}
};//C

// Externalized declaration of friend function "fun"
template<typename T,typename S>
C<T> fun(S s){
  C<T> d;
  return fun<S>(s,d);
}// external declaration of fun

int main(){
  int a = 0;
  C<double> b = fun<double,int>(a);
  b.show();
  return 0;
}//main
