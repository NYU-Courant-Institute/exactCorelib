/*
  A collection of essential tools and data structures for performing root 
  isolation using subdivision methods.
 */
#ifndef __ROOTISOLATIONTOOLS_H__
#define __ROOTISOLATIONTOOLS_H__


#define CORE_LEVEL 4
#include <CORE/CORE.h>
#include"lowerBounds.h"
#include<stack>

using namespace CORE;
using namespace std;

typedef std::pair<BigRat, BigRat>  BRInterval;
typedef std::vector<BRInterval> BRVecInterval;
/*******************************************************/
/**********DATA STRUCTURES******************************/
/*******************************************************/

// Stores the interval endpoints and a polynomial (can be in either basis)
template < typename RT, typename T >
struct SubDivData 
{
  RT a, b;
  Polynomial<T> P;
  
  SubDivData(const RT a_, const RT b_, const Polynomial<T>& P_){
    a = a_; b = b_; P= Polynomial<T>(P_);
  }
  //  SubDivData( SubDivData SD){
  //    a = SD.a; b = SD.b; P = SD.P;
  //  }
  
  SubDivData(int sz) {
    P= Polynomial<T>(sz);
  }
  
};

template < typename RT, typename T >
struct CFDivData
{

  RT a, b, c, d;
  int sv;
  Polynomial<T> P;

  CFDivData(const RT a_, const RT b_, const RT c_, const RT d_, int sv_, const
	    Polynomial<T>& P_){
    a = a_; b = b_; c=c_; d=d_; sv = sv_; P= Polynomial<T>(P_);// Assignment operation
  }
  //  SubDivData( SubDivData SD){
  //    a = SD.a; b = SD.b; P = SD.P;
  //  }
  
  CFDivData(int sz) {
    P= Polynomial<T>(sz); // CAUTION: constant coefficient is set to one
  }
};


template<typename RT,typename T, typename Prec>
  struct IntvData{
    RT a, b;
    T s;
    Prec p;

    IntvData(){}
    IntvData(const RT a_, RT b_, T s_, Prec p_){
      a = a_; b= b_; s=s_; p=p_;
    }
  };

template < typename RT, typename T >
  struct SlvSubDivData
  {
    RT a, b;
    std::vector<T> slv;
    unsigned int depth;

    SlvSubDivData(RT a_, RT b_, std::vector<T> &slv_){
      a=a_; b=b_; slv=slv_; depth=0;
    }
    SlvSubDivData(int sz){
      slv.reserve(sz);
    }
    
  };
/*******************************************************/
/**********PROCEDURES***********************************/
/*******************************************************/


// Required for sorting vectors of BFIntervals
template <typename T>
inline bool operator <(const std::pair<T, T>& I, const std::pair<T, T>& J){
  if(I.second <= J.first)
    return true;
  if(I.first >= J.second)
    return false;
}

// Checking if the sign variations are 0, 1, or greater than one.
// Deg should be the true degree, otherwise me may return an incorrect answer.
template <typename T>
int signVar(T* coeff, int deg){
  int num=0;
  int lastsign =0;//The last non-zero sign
  int currsign;//The sign of the current coefficient

  for(int i=0; i <= deg; i++){
    currsign = sign(coeff[i]);
    if(currsign !=0){
      if (lastsign * currsign < 0) num++;
      lastsign = currsign;//lastsign is always non-zero except for the starting.
    }
     if(num > 1) return num;
  }
  return num;
}


//Counts the total number of sign variations in the coefficients of P
//while disregarding any change of sign from zero to non-zero coefficients.
template<typename NT>
int signVariation(Polynomial<NT>& P)
{
  int num=0;
  int lastsign =0;//The last non-zero sign
  int currsign;//The sign of the current coefficient

  //Get the number of sign variations. 
  for(int i=0; i<= P.getTrueDegree();i++){
    currsign = sign(P.coeff()[i]);
    if(currsign !=0){
      if (lastsign * currsign < 0) num++;
      lastsign = currsign;//lastsign is always non-zero except for the starting.
    }
  }
  return num;
}



/*
// Taylor shift by one on the polynomial represented by coeff; output is in shifted
// Deg should be the true degree, otherwise me may return an incorrect answer.
template <typename T>
void shift(T* coeff, int deg, T* shifted){
  //This is the ascending coefficient method suggested by
  //Krandick in Isolierung reeller Nullstellen von Polynomen
  //( English version is called Isolation of Polynomial Real Roots)
  //to compute the Taylor shift of a polynomial by one.
  //  if(*shifted != *coeff)
  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];

  for(int i=0; i<= deg-1;i++)
    for(int j=deg-1; j>=i; j--)
      shifted[j]+=shifted[j+1];

}
*/
// Scale the variable by 2 of the polynomial represented by coeff; 
// output is in halved.
// Deg should be the true degree, otherwise me may return an incorrect answer
template <typename T>
void half(const T* coeff, int deg, T* halved){
  for(int i=0; i<= deg; i++)
    halved[i] = coeff[i]*(BigInt(1)<<(deg-i));
}

// Scale by a positive power of two.
// Deg should be the true degree, otherwise me may return an incorrect answer
template <typename T1, typename T2>
void scale(T1* coeff, int deg, T2 k, T2* scaled){
  for(int i=0; i<= deg; i++)
    scaled[i] = coeff[i]*(BigInt(1)<<(k*i));
}


//Given the coefficient sequence coeff of some polynomial P
//this computes the coefficient sequence contracted of the polynomial
//P(\lambda X). Since lambda can be of a different type than the
//coefficients of P we need introduce another typename to resolve this.
//We assume that T2 is more general than T1 so that the conversion from
// the latter to the former can take place without error.
// Deg should be the true degree, otherwise me may return an incorrect answer
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
//for contract above..
// Deg should be the true degree, otherwise me may return an incorrect answer
template <typename T1, typename T2>
void shift(T1* coeff, int deg, T2 lambda, T2* shifted){

  for(int i=0; i<= deg ; i++)
    shifted[i] = coeff[i];
  
  if(lambda != 0){
    for(int i=0; i<= deg-1;i++)
      for(int j=deg-1; j>=i; j--)
	shifted[j]+= lambda * shifted[j+1];
  }
}

/*
//This is the ascending coefficient method suggested by
//Krandick in Isolierung reeller Nullstellen von Polynomen
//( English version is called Isolation of Polynomial Real Roots)
//to compute the Taylor shift of a polynomial by one.
//The advantage of this method is that it computes the coefficient of x^i
//in n-i steps. Thus we can check for sign variation as we compute
//the coefficients.
// Deg should be the true degree, otherwise me may return an incorrect answer
template <typename T>
int shiftAndSigncount(T* coeff, int deg){

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
      lastsign = currsign;//lastsign is always non-zero except for the starting.
    }
     if(num > 1) return num;
  }
  //To account the sign variation between the previous non-zero coefficient
  // and deg coeffecient.
  //This is done if the number of sign variations were less than or
  //equal to one from the above loop.
  if(lastsign*sign(temp[deg]) < 0) num++;
  //  if(sign(temp[deg-1])*sign(temp[deg]) < 0) num++;
  if(num ==2 ) return num;
  return num;
}
*/


// Perform deCasteljau's algorithm on P with the subdivision point
// at num/den, and stores the left coefficients in PL and right 
// coefficients in PR. This is the NON fraction-free variant.
// u is the point of subdivision
// Deg should be the true degree, otherwise me may return an incorrect answer
template <typename Vec, typename T2>
  void deCasteljau (const Vec& P, Vec& PL, Vec& PR, T2 u, int deg){
  
  //  cout<<"den = "<< den << " denLg = "<<denLg << endl;
  PL[0] = P[0]; PR[deg] = P[deg];

  Vec coefft(deg);
  for(int i=0; i < deg; i++)
    coefft[i] = u*P[i]+(1-u)*P[i+1];
  
  PL[1] = coefft[0]; PR[deg-1] = coefft[deg-1];

  for(int i=2; i <= deg; i++){
    for(int j=0; j <= deg-i; j++)
      coefft[j] = u*coefft[j]+ (1-u)*coefft[j+1];    
    PL[i] = coefft[0]; PR[deg-i] = coefft[deg-i];
  }
}


// Perform deCasteljau's algorithm on P with the subdivision point
// at num/den, and stores the left coefficients in PL and right 
// coefficients in PR. This is the fraction-free variant, i.e., 
// we compute the Bernstein coefficients of (den)^n P(X), n = deg,
// and hence avoid the appearance of fractions in our algorithm.
// Deg should be the true degree, otherwise me may return an incorrect answer
template <typename T1>
void deCasteljau (const T1* P, T1* PL, 
		  T1* PR, int deg,  BigInt num, BigInt den){
  
  unsigned long denLg = floorLg(den);
  //  cout<<"den = "<< den << " denLg = "<<denLg << endl;
  PL[0] = P[0]<<(deg*denLg); PR[deg] = P[deg]<<(deg*denLg);
  BigInt den_num = den - num;

  T1 coefft[deg];
  for(int i=0; i < deg; i++)
    coefft[i] = P[i]*(den_num)+num*P[i+1];
  
  PL[1] = coefft[0]<<((deg-1)*denLg); PR[deg-1] = coefft[deg-1]<<((deg-1)*denLg);

  for(int i=2; i <= deg; i++){
    for(int j=0; j <= deg-i; j++)
      coefft[j] = coefft[j]*(den_num)+ num*coefft[j+1];    
    PL[i] = coefft[0]; PR[deg-i] = coefft[deg-i];
    PL[i] <<= ((deg-i)*denLg); PR[deg-i] <<=((deg-i)*denLg);
  }
}


// Performs subdivision at lambda on SD and stores the output in SDL and SDR.
// Deg should be the true degree of SD.P, otherwise me may return an incorrect answer
template <typename RT, typename T>
void subdivide (const SubDivData<RT, T>& SD, SubDivData<RT, T>& SDL, 
		SubDivData<RT, T>& SDR, int deg, BigInt num =1, BigInt den=2){

  deCasteljau(SD.P.coeff(), SDL.P.coeff(), SDR.P.coeff(), deg, num, den);
  SDL.a = SD.a;  SDL.b = (SD.a + SD.b).div2();
  SDR.a = SDL.b; SDR.b = SD.b;
}


// This method is used for the power basis variant of the Descartes method.
// Performs subdivision on SD and stores the output in SDL and SDR.
// The inputs represent polynomials in power basis.
// Deg should be the true degree of SD.P, otherwise me may return an incorrect answer
template <typename RT, typename T>
void subdividePow(const SubDivData<RT, T>& SD, SubDivData<RT, T>& SDL, 
		SubDivData<RT, T>& SDR, int deg){
  SDL.a = SD.a; SDR.b = SD.b;
  SDL.b = (SD.a + SD.b).div2();
  SDR.a = SDL.b;

  half(SD.P.coeff(),deg, SDL.P.coeff());
    //    cout <<"After halving polynomial is "; Q.dump(); cout<<endl;
    
  shift(SDL.P.coeff(), deg, SDR.P.coeff());

}

// This method is used for the continued fraction based approach for isolation.
// Performs subdivision on SD and stores the output in SDL and SDR.
// The inputs represent polynomials in power basis.
// We also pass the pointer to the function we want to use for computing
// lower bounds. The bool value tells us whether we constructed the left child
// or not.
// Deg should be the true degree of SD.P, otherwise me may return an incorrect answer
template <typename RT, typename T>
bool subdivide(const CFDivData<RT, T>& SD, CFDivData<RT, T>& SDL, 
	       CFDivData<RT, T>& SDR, int deg, 
	       BigInt (*Bound)(const Polynomial<T>&) = HongLowerBound){


  //  cout<<"Input polynomial"<<endl;
  //  SD.P.dump(); cout<<endl;  

  BigInt B = Bound(SD.P);
  //  cout<<"Bound = "<< B<<endl;
  Polynomial<T> temp(deg);

  RT a=SD.a, b = SD.b, c = SD.c, d = SD.d;
  for(int i=0; i <= deg; i++)
    temp.coeff()[i] = SD.P.coeff()[i];
  
  if(B >= 1){
    unsigned long Bmsb = floorLg(B);
    if(B > 1)// for B == 1 nothing to do
      for(int i=0; i <= deg; i++) 
	temp.coeff()[i] *= (T(1)<<(i*Bmsb)); // scale the variable by B
    // Shift by one
    for(int i=0; i<= deg-1;i++)
      for(int j=deg-1; j>=i; j--)
	temp.coeff()[j]+=temp.coeff()[j+1];
    a *= B; b +=a;
    c *= B; d +=c;
  }

  shift(temp.coeff(), deg, SDR.P.coeff());
  //  cout<<"Right polynomial"<<endl;
  //  SDR.P.dump(); cout<<endl;
  SDR.sv = signVariation(SDR.P);
  SDR.a = a; SDR.b = b+a;
  SDR.c = c; SDR.d = c+d;

  bool leftCreated = false;//Flag to signal whether left node was created
  int r=0;
  if(SDR.P.coeff()[0] == T(0)) // There is a root at the endpoint
    r=1;

  if(SDR.sv + r < SD.sv){
    leftCreated = true;
    temp.reverse();
    shift(temp.coeff(), deg, SDL.P.coeff());
    //    cout<<"Left polynomial"<<endl;
    //    SDL.P.dump(); cout<<endl;
    SDL.a = b; SDL.b = SDR.b;
    SDL.c = d; SDL.d = SDR.d;
    SDL.sv = signVariation(SDL.P);
  }
  return leftCreated;
}

// Construct the interval with endpoints M(0), M(infty), where M(X) is
// the Mobius transformation defined by SD. Note SD.d cannot be zero, and both
// SD.a and SD.c cannot be zero simultaneously.
template <typename RT, typename T, typename FT>
 void getInterval(const CFDivData<RT,T>& SD, std::vector<std::pair<FT, FT> >& v, 
		  BigInt CB){


  //  cout <<"a= "<< SD.a << "b= "<< SD.b<< "c= "<<SD.c <<"d= " <<SD.d << endl;
    if(SD.c != 0){
      if(SD.b*SD.c < SD.a*SD.d) // Correctly order the end-points of the interval 
	v.push_back(std::make_pair(FT(SD.b,SD.d), FT(SD.a,SD.c)));
      //cout << "Root in ["<< b/d << " : " << a/c << "]" << endl;
      else 
	v.push_back(std::make_pair(FT(SD.a, SD.c), FT(SD.b, SD.d)));
    }else{
      if(sign(SD.a) > 0)
	v.push_back(std::make_pair(FT(SD.b, SD.d), CB));
      else // This is needed when isolating negative roots
	v.push_back(std::make_pair(-1*CB, FT(SD.b, SD.d)));
      // cout << "Root in ["<< b/d << " : " << CB << "]" << endl;
    }

}

/* FUNCTIONS FOR THE INTERVAL DESCARTES ALGORITHM */

// The vector B contains the binomial coefficients for deg.
// Two ways -- first, \Theta(d) computations involving d multiplications and d exact
// divisions, but the intermediate results are sometimes larger than the output;
// and second, dynmaic programming approach based upon recursion.
template <class C>
void computeMatrix(unsigned deg, std::vector<C>& B)
{
  B.push_back(C(1));
  C temp;
  for(unsigned j=1; j<= deg/2; j++)
    {
      temp = (B[j-1]*(deg - j+1))/j;
      B.push_back(temp);
      //      cout << B[j] << std::endl;
    }
}

/*
  The number of times the sleeve defined by ubp and dbp intersects
  the x-axis. Returns 0, 1, or 2. The last
  value is returned if the sleeve intersects with the x-axis more than
  once. This procedure is useful if we want to know whether the sleeve intersects
  strictly, not strictly, or not at all.
 */

template <typename VECT>
int sleeveVar(VECT& slv, int sz)
{
  int lastsign=0, currsign, num=0;
  int ui, di;
  for(int i=0; i<= sz; i++){
    ui = sign(slv[i].getRight()); di = sign(slv[i].getLeft());
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


/*
  Computes the sleeve for the polynomial P (in poewr basis) on the
  interval [a,b], where the scaling to be done is s and the starting precision
  is p. Sleeve is returned in slv and the precision, if increased, in p.
  B contains the binomial coefficients.
 */
template <typename T, typename RT, typename Vec1, typename Vec2, typename FT>
void initializeSleeve(Polynomial<T> &P, int deg, RT a, RT b, unsigned int s, 
		      prec_t &p, Vec1 &slv, Vec2 &B, FT EPS){

  Polynomial<T> Q(deg);
  for(int i=0; i <= deg; i++)
    Q.coeff()[i] = P.coeff()[i]<< s;

  bool suffprec = false;
  prec_t oldp = mpfr_get_default_prec();
  if(p != oldp)
    mpfr_set_default_prec(p);

  std::vector<BigFloat2> SLV(deg+1);
  
  do{
    suffprec = true;
    //monomialToBezier(Q, deg, a, b, SLV, B);
    bezierToBezier(Q, deg, a, b, SLV);

    for(int i=0; i<= deg; i++)
      slv[i].set(SLV[i], DOUBLE_PREC);
    
    for ( int i=0; i <= deg; i++ ){
      if( log_2(slv[i].abs_diam()) < -EPS){
	suffprec = false;
	break;
      }
    }
    
    //    std::cout<<"Suff prec =" << suffprec << std::endl;
    if(!suffprec){
      //      std::cout<<"Precision increased"<<std::endl;
      p <<= 1;     mpfr_set_default_prec(p);;
    }

  }while(!suffprec);

  mpfr_set_default_prec(oldp);
}

// All arithmetic is BigFloat2 arithmetic
template <typename T, typename RT, typename T2, typename Vec>
void monomialToBezier(Polynomial<T> &P, int deg, RT a, RT b, int s, 
		      T2& bz, Vec &Bin){

  Polynomial<BigFloat2> PP(P);
  BigFloat2 A(a), B(b);

  if(A !=0) 
    shift(PP.coeff(), deg, A);

  scale(PP.coeff(), deg, B-A, PP.coeff());

  bz[0] = PP.coeff()[0]; bz[deg] = PP.coeff()[deg];
  for(unsigned i = 1; i < deg; i++){
      if(i <= deg/2)
	bz[i] = PP.coeff()[i]/BigFloat2(Bin[i]);
      else
	bz[i] = PP.coeff()[i]/BigFloat2(Bin[deg-i]);
  }

  for(unsigned j=1; j <= deg; j++)
    for(unsigned i = deg; i >=j; i--)
      bz[i] += bz[i-1];
}


// All arithmetic is BigFloat2 arithmetic. We know the Bernstein coefficients
// of P on the unit interval, and we want to compute its Berstein coefficients on
// [a,b]
template <typename T, typename RT, typename T2>
void bezierToBezier(Polynomial<T> &P, int deg, RT a, RT b, T2& bz){

  BigFloat2 A(a), B(b);
  
  std::vector<BigFloat2> tempL(deg+1), tempR(deg+1), PP(deg+1);
  for(int i=0; i<=deg; i++)
    PP[i] = BigFloat2(P.coeff()[i]);

  // First subdivide at A if it's not zero
  if(A != BigFloat2(0) )
    deCasteljau(PP, tempL, tempR, A, deg);

  // Then subdivide the right half on B, if it's not one
  if(B != BigFloat2(1))
    deCasteljau(tempR, bz, tempL, B, deg);
}

/*
  FUNCTIONS FOR THE EVAL ALGORITHM
 */
// Function for evaluating all the Taylor coefficients of a polynomial 
// around a point x. That is, outputs P^(j)(X)/j! for any point X.
// Involves only linear number of multiplications and divisions; 
// the additions are still quadratic.
template <typename T, typename RT>
void evalAllLin(Polynomial<T>& P, int deg, RT x, MAX_TYPE(RT, T) *seq)
{
  //std::cout << "X = " << x << std::endl;
  if(x != 0){
    MAX_TYPE(RT, T) powx=1;
    for(int i=0; i <= deg; i++){
      seq[i] = P.coeff()[i]*powx;
      powx *= x;
      /*      
      std::cout << "coeff[" << i <<"] = "<< P.coeff()[i] << std::endl;
      std::cout << "powx = "<< powx << std::endl;
      std::cout << "seq[" << i <<"] = "<< seq[i] << std::endl;
      */
    }
    powx =1;
    for(int i=0; i < deg; i++){
      for(int j=deg-1; j >= i; j--){
	seq[j] += seq[j+1];
      }
      std::cout << "seq[" << i <<"] = "<< seq[i] << " powx = " << powx << std::endl;
      seq[i] = div_exact(seq[i], powx);// Divide out the suitable power of x.
      powx *=x;
    }
    seq[deg] = P.coeff()[deg];
  }else{
    for(int i=0; i <= deg; i++)
      seq[i] = P.coeff()[i];
  }
}


template <typename T, typename RT>
  std::pair<MAX_TYPE(RT, T), MAX_TYPE(RT, T)> 
  box(const std::pair<RT, RT>& seg, Polynomial<T> &P, int deg)
{
  typedef MAX_TYPE(RT, T) MT;

  RT midpoint = (seg.first + seg.second).div2();
  //Polynomial <T> temp(P);
  std::cout <<"Input interval " << seg.first <<": " << seg.second << std::endl;
  MT evals[deg+1];
  evalAllLin(P, deg, midpoint, evals);
  std::cout <<"After evaluating all derivatives " << std::endl;
  MT val=0;
  RT halfwidth = (seg.second - seg.first).div2();
  MT t;
  for(int i=deg; i > 0; i--){
    std::cout << "Val before " << val <<std::endl;
    val *=halfwidth;
    val += core_abs(evals[i]);
    std::cout << "Val after " << val <<std::endl;
  }
  val *= halfwidth;
  std::pair<MT, MT> J = std::make_pair(-val,val);
  t = evals[0];
  J.first += t; J.second +=t;
  std::cout << "Range returned " << J.first << ":" << J.second <<std::endl;
  return J;
}

// Do two closed intervals I, J overlap
template <typename RT>
bool ifIntersect(const std::pair<RT, RT>& I, const std::pair<RT, RT>& J){
  if(I.second < J.first || J.second < I.first)
    return false;
  else
    return true;
}


// Interval Newton operator. Computes the interval Newton operator of P, I.
// In our implementation zero is always contained in PDIFF(I)=PdiffI, and hence
// the interval Newton operator will return intervals that are unbounded
// on at least one side. Thus we internally check whether these 
// intervals intersect with the left half and right half of I; if left half
// intersects then we set leftInter to true, and similarly for right half.
// We assume that the midpoint of I is not a root, and by default leftInter
// and rightInter are false.
template <typename T, typename RT>
  void newton(const std::pair<RT, RT>& I, const Polynomial<T> &P, 
	      const std::pair<MAX_TYPE(RT, T), MAX_TYPE(RT, T)> &PdiffI, 
	      unsigned int deg, bool& leftInter, bool& rightInter){
  typedef MAX_TYPE(RT, T) MT; // We assume MAX_TYPE is BigFloat2

  RT m =  (I.first + I.second).div2();
  MT Pm = P.eval(m);
  BigFloat2 M(m), PM(Pm), PDIFFI(PdiffI.first, PdiffI.second), J;
  J = M - PM/PDIFFI;
  BigFloat2 L(I.first, m), R(m, I.second);

  if((J - L).isZeroIn())
    leftInter=true;
  if((J - R).isZeroIn())
    rightInter=true;
}



#endif
