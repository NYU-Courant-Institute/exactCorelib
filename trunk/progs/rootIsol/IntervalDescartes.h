/*
  File contains the code for BITSTREAM real root isolation based upon the Bernstein
  basis representation of intermediate polynomials. 
 */
#ifndef __DESCARTESB_H__
#define __DESCARTESB_H__

#define CORE_LEVEL 4
#include<stack>
#include "CORE/CORE.h"
#include"RootIsolationTools.h"

using namespace CORE;
using namespace std;

namespace IntDescartesB {
  
int COUNTER = 0;


/*
  The number of times the sleeve defined by ubp and dbp intersects
  the x-axis. Returns 0, 1, or 2. The last
  value is returned if the sleeve intersects with the x-axis more than
  once. This procedure is useful if we want to know whether the sleeve intersects
  strictly, not strictly, or not at all.
 */
template<class VECT>
int sleeve_var(double* up, double* low, int sz)
{
  int lastsign=0, currsign, num=0;
  int ui, di;
  for(int i=0; i<= sz; i++){
    ui = sign(up[i]); di = sign(low[i]);
    //    cout<<"sgn ubp["<<i<<"]="<< ui << " sgn dbp["<<i<<"]="<< di << std::endl;
    if(ui * di >= 0){
      currsign = sign(ui+di); // currsign is zero only if both ui and di are zero
      if(currsign != 0){
	if(lastsign * currsign < 0) num++;
	if(num > 1) return num;
	lastsign = currsign;
      }
    }
    if(ui * di < 0){
      if(i == 0 || i == sz -1) // Cannot decide if there is an indecisive crossing in starting or end.
	return 2;
      else{
	lastsign *=-1;
	num ++;
	if(num > 1) return num;
      }
    }
  }
  return num; //either zero or one
}


template <typename T>
  void isolateRoots(double* up, double* low, int deg, BFVecInterval &v) {
  


 }
 

//Given the count n of the number of roots, this function isolates
//the roots of P and checks if they are equal to n.
template <typename NT>
CORE_INLINE void testDescartes(Polynomial<NT>& P, int n = -1) {

  BRVecInterval v;
  isolateRoots(P,v);
  std::cout << "   Number of roots is " << v.size() <<std::endl;
  if ((n >= 0) & (v.size() == (unsigned)n))
    std::cout << " (CORRECT!)" << std::endl;
  else
    std::cout << " (ERROR!) " << std::endl;
  int i = 0;
  for (BRVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
}// testDescartes

// Isolate the real roots of P iteratively for n iterations. Then return the
// average tree size, time for n iterations.
template <typename NT>
CORE_INLINE void test(Polynomial<NT>& P, int n) {
  
  BRVecInterval v;
  
  Timer t1;
  t1.start();
  for(int i=1; i <= n ; i++)
    isolateRoots(P, v);
  
  t1.stop();
  std::cout << "Size of recursion tree = " << COUNTER/n;
  std::cout << " #roots = " << v.size()/n;
  std::cout<<" Time " << t1.getSeconds()/n <<endl;
  //  std::cout<<" Time per evaluation " << t1.getSeconds()/(COUNTER) <<std::endl;
}

}// end namespace DescartesB
#endif // __DESCARTESB_H__
