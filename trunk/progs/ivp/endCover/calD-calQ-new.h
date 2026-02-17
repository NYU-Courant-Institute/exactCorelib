#pragma once

#include <iostream>
#include "capd/capdlib.h"
using namespace capd;
using namespace std;
struct ReturnType {
    std::vector<std::string> p2;	//pi2
    std::vector<std::string> p3;	//pi3
    IMap finp3;
    IMap fp3;
    std::vector<double> mu2;
    std::vector<std::string> ggv;	// new function after pi2
    std::vector<std::string> inp2;	// inverse of pi2
    double d;							// uniform radical value
    std::vector<std::string> gg;	//bfg as string
    IMap gg1;						//bfg as IMap
    IVector B3;						//F1 after pi2
    std::vector<std::string> inp3;	//inverse of pi3
    IVector B4;						//F1 after pi
    IMatrix J;						//Jacobian of new ode (bfg)
};
struct ministeps {
    ReturnType Xform;
    std::vector<double> mu1;
    double delta;
    double heuler;
    int ell;
    std::vector<IVector>  bfE;
    std::vector<IVector>  bfF;
};

struct Stage {
    std::vector<double> T;            // Time series T = [t0, t1, ...]
    std::vector<IVector> E;           // The set of boxes E = [B0, B1, ...]
    std::vector<IVector> F; 
    std::vector<ministeps> G;
};

struct StageBound {
            
    std::vector<IVector> E0;           // The set of boxes E = [B0, B1, ...]
    std::vector<IVector> E1;
    IVector E;
};

struct StageE {
    Stage S;
    IVector E;
};

struct ReturnType2
{
    double veps;
    Stage  stage;

};

// Converts a string representing \bff into IMap:
string Convert_to_IMap(
    std::vector<std::string> SVar,
    std::vector<std::string> SFun)
{
    std::string sumF = "var:";
    // Use a for loop to dynamically concatenate the var: part
    for (size_t i = 0; i < SVar.size(); ++i) {
        sumF += SVar[i];
        if (i < SVar.size() - 1) {
            sumF += ", ";
        }
    }

    sumF += "; fun:";

    //Use a for loop to dynamically concatenate the fun: parts
    for (size_t i = 0; i < SFun.size(); ++i) {
        sumF += SFun[i];
        if (i < SFun.size() - 1) {
            sumF += ", ";
        }
    }

    sumF += "; ";
    return sumF;
}//string Convert_to_IMap

/*Example to convert string to IMap:
    std::vector<std::string> SVar = {
       "x",
       "y"
    };
    std::vector<std::string> SFun = {
      "-2*x^2",
      "y"
    };

    string sumF = Convert_to_IMap(SVar, SFun);
    //////cout << sumF << endl;
*/

// Compute the linear map \pi_1(f,F1):

//Box function implementation
// Computes Box(B1, B2) enclosing B1, B2:
IVector Box(const IVector& B1, const IVector& B2) {
    int n = B1.dimension(); //Get Dimensions
    IVector B(n);           // Initialize the result vector
    for (int i = 0; i < n; ++i) {
        //Use std::min and std::max to ensure no naming conflicts
        double lower = std::min(B1[i].leftBound(), B2[i].leftBound());
        double upper = std::max(B1[i].rightBound(), B2[i].rightBound());
        B[i] = interval(lower, upper); // Update interval vector
    }
    return B;
}


// IntersectB function implementation
// Computes intersection of 2 boxes B1 and B2
IVector IntersectB(const IVector& B1, const IVector& B2) {
    int n = B1.dimension(); // Get Dimensions
    IVector B(n);           // Initialize the result vector
    for (int i = 0; i < n; ++i) {
        // Compute lower and upper bounds of intersection
        double lower = std::max(B1[i].leftBound(), B2[i].leftBound());
        double upper = std::min(B1[i].rightBound(), B2[i].rightBound());
        // If the intersection is empty, return an empty interval
        if (lower > upper) {
           cout << "intersecterror" << endl;
           return B2;
        }
        else {
            B[i] = interval(lower, upper); // Update interval vector
        }
    }
    return B;
}

//Function: Calculates the Jacobian matrix of a given function f in interval B
// Compute the Jacobian matrix J_f(B)
IMatrix computeJacobian(IMap f, IVector B) {
    // Get the dimensions of the interval
    int n = B.dimension();

    // Creating a Storage Jet Object
    IJet jet(n, n, 2);
    // cout << "1" << endl;
      //Calculate Jet
    f(B, jet);
    // cout << "2" << endl;
     // Initialize the Jacobian matrix
    IMatrix J(n, n);
    // cout << "3" << endl;
     // Filling the Jacobian matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // cout << "5" << endl;
            J[i][j] = jet(i, j);
            //   cout << "6" << endl;
        }
    }
    // cout << "4" << endl;
     //cout << J << endl;
    return J;
}

/*  Example of computing Jacobian
    //Compute the Taylor function:
   int maxDerivativeOrder = 2;
    int d = 3;
    IMap f("par: d; var:x,y,z;fun:-2*x^2+y+z,y+z,1/z^d;", maxDerivativeOrder);
    f.setParameter("d", d);


    IVector B(3);
    B[0] = interval(1.0, 1.0);
    B[1] = interval(1.0, 1.0);
    B[2] = interval(1.0, 1.0);
    int n = B.dimension();

    IMatrix J = computeJacobian(f, B);
    //////cout << J << endl;
*/


//IMap f("var:x,y; fun:x^2, y^2;"); 
//IVector initialCondition(2);
//initialCondition[0] = interval(0.1, 0.1);  // x0 = 0.1
//initialCondition[1] = interval(1.0, 1.0);  // y0 = 1.0
//IVector result = f(initialCondition);

double wmax(IVector B) {
    double max_length = 0.0;

    for (size_t i = 0; i < B.dimension(); ++i) {
        max_length = std::max(max_length,
            B[i].rightBound() - B[i].leftBound());
    }
    return max_length;
}

///////////////////////////////

//Calculate the Taylor function：
/*int dimIn = 1, dimOut = 1, noParam = 5;

// this is the maximal order of derivative we request
// default value is set to 1 if the argument is skipped
int maxDerivativeOrder = 3;
DMap f("var:x;fun:x^4;", maxDerivativeOrder);



double v[] = { 2 };
DVector x(dimIn, v);

// declare an object for storing jets
DJet jet(dimOut, dimIn, maxDerivativeOrder);
f(x, jet);

// print the result
// NOTE: indices of variables must form a nondecreasing sequence!
    //////cout << "df0_dx0 = " << jet(0, 0) << endl;
     //////cout << "df0_dx0_dx0 = " << jet(0, 0, 0) << endl;

    //////cout << jet.toString();
*/

std::vector<double> center(IVector B) {

    std::vector<double> value(B.dimension());
    for (size_t i = 0; i < B.dimension(); ++i) {
        value[i] = (B[i].rightBound() + B[i].leftBound()) / 2.0;

    }
    return value;
}




////////////////////////////////
/*
int dimIn = 1, dimOut = 1, noParam = 5;

// this is the maximal order of derivative we request
// default value is set to 1 if the argument is skipped
int maxDerivativeOrder = 6;
IMap f("var:x;fun:2*x^2;", maxDerivativeOrder);

ICnTaylor solver(f, maxDerivativeOrder);
solver.setStep(0.1);
IVector u(1);
u[0] = interval(1.0, 1.0);
// u[1] = interval(1.0,1.0);

 
CnMultiMatrixRect2Set set(u, maxDerivativeOrder);


for (int i = 0; i < 1; ++i) {
    set.move(solver);
}


// IVector x(dimIn, u);
 // declare an object for storing jets
//  DJet jet(dimOut, dimIn, maxDerivativeOrder);
//  f(x, jet);

  // print the result
  // NOTE: indices of variables must form a nondecreasing sequence!
//////cout << "df0_dx0_dx0_dx2 = " << set(0, 0) << endl;


//////cout << set.currentSet().toString();
/*
value :
{0}  : {[1.1111111, 1.111111273116974]}  1/0.9=1/(1-t)
Taylor coefficients of order 1 :
   {1}  : {[1.234567572413863, 1.234569417560361]} 1/0.9^2
Taylor coefficients of order 2 :
   {2}  : {[0.1371722541002819, 0.137180702763908]} 1/0.9^3) * h
Taylor coefficients of order 3 :
   {3}  : {[0.01523578288126368, 0.01525845084855456]}  1/0.9^4) * h^2
Taylor coefficients of order 4 :
   {4}  : {[0.001682071179780092, 0.001723369458573746]}
Taylor coefficients of order 5 :
   {5}  : {[0.0001699570020067538, 0.0002265818803192654]}
Taylor coefficients of order 6 :
   {6}  : {[-2.412917814302444e-06, 5.862646239257471e-05]}

    */

    ////////////////////////////



// This computes an end-enclosure E_1
//		stepB(f, degree, H=1, E_0) -> E_1 
//

double computemu(IMatrix J) {
    Interval a = J[1][1];
    Interval b = (J[1][2]+J[2][1])/2;
    Interval c = b;
    Interval d = J[2][2];
    Interval  B1 = a-d;
    
    Interval Del = B1 * B1 + 4 * c*b;
    double D = Del.rightBound();
    double mu;
    if(D > 0.0)
    {
        mu = ((a+d).rightBound() + sqrt(D))/2;
    }
    else
    {
        mu = ((a+d).rightBound())/2;
    }

    return mu;
}


