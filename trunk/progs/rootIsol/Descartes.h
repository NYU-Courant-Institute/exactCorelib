/*
This is an implementation a root isolation algorithm based
on Descartes Rule of Signs.
Author: Vikram Sharma
*/

#define CORE_LEVEL 4
#include "CORE/CORE.h"

using namespace CORE;
using namespace std;

template <typename T>
void shift(T* coeff, int deg, T* shifted){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.

  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];
  
  for(int i=0; i<= deg-1;i++)
    for(int j=deg-1; j>=i; j--)
      shifted[j]+=shifted[j+1];

}

template <typename T>
void half(T* coeff, int deg, T* halved){
  for(int i=0; i<= deg; i++)
    halved[i] = coeff[i]*(BigInt(1)<<(deg-i));
}

//Given the coefficient sequence coeff of some polynomial P
//this computes the coefficient sequence contracted of the polynomial
//P(\lambda X). Since lambda can be of a different type than the
//coefficients of P we need introduce another typename to resolve this.
//We assume that T2 is more general than T1 so that the conversion from
// the latter to the former can take place without error.
template <typename T1, typename T2>
void contract(T1* coeff, int deg, T2 lambda, T2* contracted){
  T2 pow=1;
  for(int i=0; i <= deg ; i++){
    contracted[i] = coeff[i]*pow;
    pow*=lambda;
  }
}

//void shift(BigFloat* coeff, int deg, BigFloat lambda, BigFloat* shifted){}

//Computes the Taylor shift by a constant lambda. Confer the comments
//for contract above.
template <typename T1, typename T2>
void shift(T1* coeff, int deg, T2 lambda, T2* shifted){

  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];
  
  for(int i=0; i<= deg-1;i++)
    for(int j=deg-1; j>=i; j--)
      shifted[j]+= lambda * shifted[j+1];
}


template <typename T>
int shiftAndSigncount(T* coeff, int deg){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.
  //The advantage of this method is that ir computes the coefficient of x^i
  //in n-i steps. Thus we can check for sign variation as we compute
  //the coefficients.

  //First reverse the polynomial
  T temp[deg + 1];
  for(int i=0; i<= deg ; i++)
    temp[i] = coeff[deg-i];

  
  int num=0, i;
  int lastsign =0;//The last non-zero sign
  int currsign;//The sign of the current coefficient

  //Compute the sequence of coefficients and simultaneously get
  //the number of sign variations. If the sign variations are greater
  //than one then break out of the loop, i.e. return num.
  for(i=0; i<= deg-1;i++){
    for(int j=deg-1; j>=i; j--)
      temp[j]+=temp[j+1];

    currsign = sign(temp[i]);
    if(currsign !=0){
      if (lastsign * currsign < 0) num++;
      lastsign = currsign;//lastsign is always non-zero except in the starting.
    }
     if(num > 1) return -1;
  }
  //To account the sign variation between the previous non-zero coefficient
  // and deg coeffecient.
  //This is done if the number of sign variations were less than or
  //equal to one from the above loop.
  if(lastsign*sign(temp[deg]) < 0) num++;

  if(num > 1 ) return -1;
  return num;
}


//Isolates real roots of P in the interval (0,1).
//The roots of P are in bijective correspondence with the roots of
//the original input polynomial P_{in}. More precisely,
//P has a root in (0,1) iff P_{in} has a root in I
template <typename T>
void isolateRoots(Polynomial<T> P, const BFInterval I, int deg,
                    BFVecInterval &v) {
  
  
  int num = shiftAndSigncount(P.coeff, deg);
  //  cout << "sign variations after shift " << num << endl;
  
  if(num == 0) return;
  else if(num == 1)
    v.push_back(I);
  else{
    BigFloat m = (I.second + I.first).div2();
    T* temp1 = new T[deg +1];
    T* temp2 = new T[deg +1];

    half(P.coeff,deg, temp1);
    Polynomial<T> Q(deg, temp1);
    //    cout<<"Inside isolateRoots "<< Q << endl;
    
    shift(temp1, deg, temp2);
    Polynomial<T> R(deg, temp2);
    //    cout<<"Inside isolateRoots: second polynomial "<< R << endl;
    
    if(R.coeff[0] == 0) 
      v.push_back(std::make_pair(m,m));
    BFInterval J = std::make_pair(I.first, m);
    BFInterval JJ = std::make_pair(m, I.second);
    isolateRoots(Q, J, deg, v);
    isolateRoots(R, JJ, deg, v);
  }
  
}

//Isolates all real roots of P. Assumes that the polynomial is square-free.
//v will contain the isolating intervals for P. The isolating intervals
//are open intervals and are of two types:
template <typename T>
void isolateRoots(Polynomial<T>& P, BFVecInterval& v)
{
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;
  
  T temp[deg];
  Polynomial<T> Q;
  //Check whether zero is a root of P.
  if(P.coeff[0] == 0){
    v.push_back(std::make_pair(0,0));
    for(int i=1; i <= deg;i++)
      temp[i-1]=P.coeff[i];
    Q = Polynomial<T>(deg-1, temp);
  }else{
    Q=P;
  }

  //  cout<< endl << "Inside isolate " << Q << endl;
  
  //Compute an upper bound on the positive roots of Q.
  T B = Q.CauchyBound();

  //cout<< "Upper bound "<< B << "Error in B "<< B.err() << endl;
  //We now construct a polynomial whose roots in the unit interval
  //correspond with the roots of Q in (0, B).
  //This is obtained by the transformation Q(X) to Q(B*X)
  int n = Q.getTrueDegree();
  T temp1[n + 1];

  contract(Q.coeff, n, B, temp1);
  Polynomial<T> R(n, temp1);
  //cout << "Inside isolate " << R << endl;

  
  BFInterval I(0, B);
  isolateRoots(R, I, n, v);

  //cout <<"Number of positive roots " << v.size();
  
  for(int i=1; i<= n; i++){
    if(i % 2 != 0)
      temp1[i] *=-1;

  }
  
  R = Polynomial<T>(n, temp1);
  //  cout << "The negative polynomial " << R << endl;

  I = std::make_pair(0, B);
  BFVecInterval vNeg;//Stores the isolating intervals for positive roots.
  isolateRoots(R, I, n, vNeg);
  //cout <<" Number of negative  roots " << vNeg.size()<<endl;
  //Flip the sign of the intervals and push them in v
  for (BFVecInterval::const_iterator it = vNeg.begin();
       it != vNeg.end(); ++it) {
    v.push_back(std::make_pair(-1*it->second,-1*it->first));
  }

}

//Isolates roots of P in the closed interval I. Assumes P is square-free
//This is achieved by computing a polynomial Q whose roots in the
//unit interval are in bijective correspondence with the roots of
//P in I; more precisely, Q(X) = P((b-a)X + a) where I=(a,b).
//This version has a problem: The original polynomial may be an integer polynomial,
//i.e. NT=BigInt, but since the interval is a BigFloat interval we need a 
//BigFloat polynomial later on. How to make this shift?
template <typename T>
void isolateRoots(Polynomial<T>& P, BFInterval I, BFVecInterval& v){
  int deg = P.getTrueDegree();
  if(deg == 0)
    cout<< "Polynomial is a constant" << endl;
  
  T temp[deg];
  Polynomial<T> Q;
  //Check whether zero is a root of P.
  if(P.coeff[0] == 0){
    v.push_back(std::make_pair(0,0));
    for(int i=1; i <= deg;i++)
      temp[i-1]=P.coeff[i];
    Q = Polynomial<T>(deg-1, temp);
  }else{
    Q=P;
  }

  
  BigFloat a = I.first, b=I.second;
  if(Q.eval(a) == 0)
    v.push_back(std::make_pair(a, a));
  if(Q.eval(b) == 0)
    v.push_back(std::make_pair(b, b));
  
  int n = Q.getTrueDegree();
  BigFloat temp1[n + 1];

  contract(Q.coeff, n, b-a, temp1);
  shift(temp1, n, a, temp1);
  
  Polynomial<BigFloat> R(n, temp1);
  isolateRoots(R, I, n, v);
  }


template <typename T>
int numberOfRoots(Polynomial<T> &P){

  BFVecInterval v;
  isolateRoots(P, v);
  return v.size();
}

//Given the count n of the number of roots, this function isolates
//the roots of P and checks if they are equal to n.
template <typename T>
CORE_INLINE void testDescartes(Polynomial<T>& P, int n = -1) {

  BFVecInterval v;
  isolateRoots(P,v);
  std::cout << "   Number of roots is " << v.size() <<std::endl;
  if ((n >= 0) & (v.size() == (unsigned)n))
    std::cout << " (CORRECT!)" << std::endl;
  else
    std::cout << " (ERROR!) " << std::endl;
  int i = 0;
  for (BFVecInterval::const_iterator it = v.begin();
       it != v.end(); ++it) {
    std::cout << ++i << "th Root is in ["
    << it->first << " ; " << it->second << "]" << std::endl;
  }
}// testDescartes

