/* file: stepAB-new.h
 *
 *      Purpose:
 *              This file provides the core building blocks for computing
 *              enclosures over one time step for an ODE system using CAPD
 *              interval arithmetic.  It implements:
 *
 *                      (A) StepA / StepA0:
 *                              An admissible full-enclosure constructor.
 *                              Given an end-enclosure B0, a time horizon H,
 *                              tolerance eps, and Taylor order k, it returns
 *                              (h, F1) where 0 < h <= H and F1 is an interval
 *                              box intended to enclose the flow from B0 over
 *                              time h (a "full-enclosure candidate").
 *
 *                      (B) StepB variants:
 *                              Methods to compute an end-enclosure E1 at time h
 *                              from an initial end-enclosure E0, using either:
 *                              - CAPD C^n (C^r) Lohner type sets, or
 *                              - CAPD direct (C0HO) methods,
 *                              optionally tightened by a growth bound mu.
 *
 *      Main routines:
 *
 *              stepA(f, B0, H, eps, degree=MAX_TAYLOR_ORDER) -> (h, B0_bar)
 *                      Computes a candidate full-enclosure B0_bar by evaluating
 *                      the degree-(k-1) Taylor polynomial of the flow over [0,H]
 *                      and inflating it by [-eps, eps]^n.
 *
 *                      Then estimates the k-th derivative bound M over B0_bar
 *                      and chooses a step size
 *                              h = (eps / M)^(1/k),
 *                      halving H repeatedly until h >= H/2 (or H becomes small).
 *                      Returns (h, B0_bar) with h <= H1.
 *
 *              stepA0(f, B0, H, eps, degree=MAX_TAYLOR_ORDER) -> (h, B0_bar)
 *                      A non-halving variant: constructs B0_bar using H1 directly,
 *                      computes M over B0_bar, and sets
 *                              h = min(H1, (eps / M)^(1/k)).
 *
 *              stepBcrlohner(f, degree, h, E0) -> E1
 *                      Uses CAPD ICnOdeSolver + ICnTimeMap with CnRect2Set to
 *                      compute an end-enclosure after time h.
 *
 *              stepBcrlohnerwithmu(f, degree, h, E0, mu) -> E1
 *                      Computes a CAPD end-enclosure and then tightens it by:
 *                              - extracting the center P of the result
 *                              - expanding a symmetric box of radius
 *                                      L * exp(mu * h)
 *                                where L = wmax(E0)/2
 *                              - adding a remainder term derived from CAPD
 *                                remainder coefficients
 *                              - intersecting with the original CAPD enclosure.
 *
 *              directmethodwithmu(f, degree, h, E0, mu) -> E1
 *                      Similar to stepBcrlohnerwithmu but uses IOdeSolver +
 *                      ITimeMap + C0HORect2Set (direct method) and then applies
 *                      the same mu-based tightening + remainder + intersection.
 *
 *              puredirectmethod(f, degree, h, E0) -> E1
 *                      Direct method end-enclosure without mu-based tightening.
 *
 *              pureTaylormethod(f, B0, B1, H, degree) -> approx box
 *                      A helper routine that combines Taylor coefficients
 *                      computed at B0 and B1 (used experimentally).
 *
 *      Parameters / conventions:
 *
 *              degree:
 *                      Taylor order k used in CAPD solvers and StepA logic.
 *                      Default is MAX_TAYLOR_ORDER (typically 5).
 *
 *              epsilon (eps):
 *                      Target inflation radius used to build B0_bar and to set h.
 *
 *              mu:
 *                      A scalar growth/log-norm bound used to tighten enclosures
 *                      via radius expansion L * exp(mu*h) around the center.
 *
 *      Output:
 *              These routines return an interval vector (IVector) enclosure or
 *              (h, enclosure) as a std::pair<double, IVector>.  They are used by
 *              higher-level modules (Extend/Refine) to build and tighten Stage S.
 *
 *      Dependencies:
 *              - CAPD library: capd/capdlib.h
 *              - Shared utilities (defined elsewhere):
 *                      wmax(...), center(...), IntersectB(...)
 *              - External global:
 *                      MAX_TAYLOR_ORDER
 *
 *      Notes / cautions:
 *              - stepA uses an adaptive halving strategy on H; stepA0 does not.
 *                They may yield different h for the same inputs.
 *              - The mu-tightening versions rely on remainder coefficients from
 *                CAPD solvers; correctness depends on consistent use of degree.
 *              - The type name "vector P = center(result);" assumes an alias;
 *                in standard C++ it should typically be "std::vector<double>".
 *
 *      Author: <Bingwei Zhang and Chee Yap>  (<Feb 2026>)
 */

#include <iostream>
#include "capd/capdlib.h"
#include "calD-calQ-new.h"
using namespace capd;
using namespace std;
extern int MAX_TAYLOR_ORDER;

IVector stepBcrlohner(
    IMap f,
    int degree,
    double timeStep,
    IVector initialCondition
) {
    IVector initialC = initialCondition;
    ICnOdeSolver solver(f, degree);
    ICnTimeMap timeMap(solver);
    CnRect2Set s(initialC, 3.0);
    interval T(timeStep);
   
    IVector result = timeMap(T, s);
    
    return  result;
}


//typedef double Scalar;

// the following lines define new names for vectors and matrices of an arbitrarily dimensions

IVector stepBcrlohnerwithmu(
    IMap f,
    int degree,
    double timeStep,
    IVector initialCondition,
    double mu
) {
    IVector initialC = initialCondition;
    double L = wmax(initialCondition)/2;
    ICnOdeSolver solver(f, degree);
    ICnTimeMap timeMap(solver);
  CnRect2Set s(initialC, 3.0); 
    interval T(timeStep);
    IVector result = timeMap(T, s);   
    const IVector& remaindCoeffs = *(solver.getRemainderCoefficients());

    // 创建与余项系数相同维度的余项向量
    IVector remainder(remaindCoeffs.dimension());

    // 计算每个维度上的完整余项 [0, h^k] × (该维度的余项系数)
    interval timeFactor(0, pow(timeStep, degree));
    for (int i = 0; i < result.dimension(); i++) {
        remainder[i] = timeFactor * remaindCoeffs[i];
    }
   vector P = center(result);
    IVector result1(result.dimension());
    for (int i = 0; i < result.dimension(); ++i) {
        result1[i] = interval( P[i] - sqrt(2)*L * exp(mu * timeStep), P[i] + sqrt(2)* L  * exp(mu * timeStep));
	}
    result1 = result1 + remainder;
    for (int i = 0; i < result.dimension(); i++) {
        double lower = std::max(result1[i].leftBound(), result[i].leftBound());
        double upper = std::min(result1[i].rightBound(), result[i].rightBound());
        if (lower > upper) { cout << "stepBerror" << endl; }
    }
    result=IntersectB(result1, result);
    return  result;
}


// Based on CAPD code(need to be changed future):
IVector directmethodwithmu(
    IMap f,
    int degree,
    double timeStep,
    IVector initialCondition,
    double mu
) {
    IVector initialC = initialCondition;
    double L = wmax(initialCondition) / 2;
    IOdeSolver solver(f, degree);
    solver.setStep(timeStep);
    ITimeMap timeMap(solver);
    C0HORect2Set s(initialC);
    interval T(timeStep);
    IVector result = timeMap(T, s);
    // 正确获取余项系数（解引用指针）
   // cout << "result" << result << endl;

    // 获取余项系数向量（高维盒子）
    const IVector& remaindCoeffs = *(solver.getRemainderCoefficients());

    // 创建与余项系数相同维度的余项向量
    IVector remainder(remaindCoeffs.dimension());

    // 计算每个维度上的完整余项 [0, h^k] × (该维度的余项系数)
    interval timeFactor(0, pow(timeStep, degree));
    for (int i = 0; i < result.dimension(); i++) {
        remainder[i] = timeFactor * remaindCoeffs[i];
    }
    // cout << "remaind" << remainder << endl;

    vector P = center(result);
    IVector result1(result.dimension());
    for (int i = 0; i < result.dimension(); ++i) {
        result1[i] = interval(P[i] - L * exp(mu * timeStep), P[i] + L * exp(mu * timeStep));
    }
    result1 = result1 + remainder;
    result = IntersectB(result1, result);
    // cout << result << endl;
    return  result;
}


// 
IVector puredirectmethod(
    IMap f,
    int degree,
    double timeStep,
    IVector initialCondition
) {
    IVector initialC = initialCondition;
    double L = wmax(initialCondition) / 2;
    IOdeSolver solver(f, degree);
    solver.setStep(timeStep);
    ITimeMap timeMap(solver);
    C0HORect2Set s(initialC);
    interval T(timeStep);
    IVector result = timeMap(T, s);
    
    return  result;
}




IVector pureTaylormethod(
    IMap f,
    IVector B0,
    IVector B1,
    double H,
    int degree
) {
    int n = B0.dimension();
    // Step 1:  \overline{B}_0
   // IMap f(bff, degree);
    ICnOdeSolver solver(f, degree);

    solver.computeCoefficients(B0, degree);
    IVector coeffs(n);
    for (int i = 0; i < n; ++i) {
        coeffs[i] = B0[i];
        for (int j = 1.0; j < degree - 1; ++j)
            ////////cout << solver.coefficient(i, j) * interval(0, pow(H, j));
            coeffs[i] += solver.coefficient(i, j) * pow(H, j);

    }
    ICnOdeSolver solver1(f, degree);

    solver1.computeCoefficients(B1, degree);
    for (int i = 0; i < n; ++i) {


        coeffs[i] += solver1.coefficient(i, degree - 1) * pow(H, degree - 1);

    }


    return coeffs;
}

// AdaptiveStepA(f, E0, H, eps, order) -> (h,F1) admissible for E0
// current set degree be 5. One can revise it.
std::pair<double, IVector> stepA(
    IMap f,
    IVector B0,
    double H1,
    double epsilon,
    int degree1 = MAX_TAYLOR_ORDER
) {
    int degree = 5.0;
    int n = B0.dimension();
    // Step 1:  \overline{B}_0
   // IMap f(bff, degree);
    ICnOdeSolver solver(f, degree);

    solver.computeCoefficients(B0, degree);
  //  cout<< solver.coefficient(1,2)<< endl;
    //  cout << "stepAB" << endl;
    double H = H1;
  //  cout << "H=" << H << endl;
    double h = 0.0;
  //  cout << "stepA1" << endl;
    IVector B0_bar(n);
    while (h < H/2) {
      // cout << "stepA" << endl;
        IVector coeffs(n);
        for (int i = 0; i < n; ++i) {
            coeffs[i] = solver.coefficient(i, degree - 1);  //
          // cout << "coeffs[i]=" << coeffs[i] << endl;
            for (int j = degree - 2; j >= 1; --j) {
                coeffs[i] = coeffs[i] * interval(0, H) + solver.coefficient(i, j);
              // cout << "coeffs[i]=" << coeffs[i] << endl;
            }
            coeffs[i] = coeffs[i] * interval(0, H) + B0[i];  // 
        }
      // cout << "stepAC" << endl;
        IVector B0_sum = coeffs;
    //  cout << "B0_sum=" << B0_sum << endl;
        IVector epsilonVector(n);// [-epsilon, epsilon]^n
        for (int i = 0; i < n; ++i) {
            epsilonVector[i] = interval(-epsilon, epsilon);
        }
     //   cout << "epsilonVector=" << epsilonVector << endl;
         B0_bar = B0_sum + epsilonVector;
      // cout << "B0_bar=" << B0_bar << endl;
        // Step 2:  M = \|\bff^{[k]}(\overline{B}_0)\|_2

      //  IMap f(bff, degree);

        ICnOdeSolver solver1(f, degree);
      //  solver.computeCoefficients(B0, degree);
        solver1.computeCoefficients(B0_bar,degree);
      //  cout<< "solver1=" << solver1.coefficient(0, 2) << endl;
        // cout << "stepAD" << endl;
        IVector B0bar_derivative_k(n); // Assume bff has k-th deriviative
        for (int i = 0; i < n; ++i) {
            B0bar_derivative_k[i] = solver1.coefficient(i, degree);

        }
     //   cout << "B0bar_derivative_k=" << B0bar_derivative_k << endl;
       // cout << "stepAE" << endl;
        capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm; // Interval type
        interval M1 = euclNorm(B0bar_derivative_k); // compute 2-norm
        double M = M1.rightBound();
     //  cout << "M=" << M << endl;
        h = pow((epsilon / M), (1.0 / degree));
      //  cout<< "h=" << h << endl;
        if (h > H) { h = H; break; }
        // cout << "H="<< H <<"h="<< h << endl;
        H = H / 2;
    }


    return std::make_pair(h, B0_bar);

}

/* Example for StepA:
    IMap bff ("var:x,y;fun:-2*x^2,y;");
    IVector B0(2);
    B0[0] = interval(1.0, 1.0);
    B0[1] = interval(1.0, 1.0);
    double H = 0.4;
    double epsilon = 0.1;
    int degree = 5.0;
    // Call stepA
    auto result = stepA(bff, B0, H, epsilon, degree);

    // output
    double h = result.first;
    IVector B1 = result.second;

    //////cout << "h = " << h << endl;
    //////cout << "B1 = [" << B1[0] << ", " << B1[1] << "]" << endl;*/

std::pair<double, IVector> stepA0(
    IMap f,
    IVector B0,
    double H1,
    double epsilon,
    int degree1 = MAX_TAYLOR_ORDER
) {
    int degree = 5.0;
    int n = B0.dimension();
    // Step 1:  \overline{B}_0
   // IMap f(bff, degree);
    ICnOdeSolver solver(f, degree);

    solver.computeCoefficients(B0, degree);
    //  cout << "stepAB" << endl;
   // double H = 2 * H1;
    double h = 0.0;
    IVector B0_bar(n);
 
    IVector coeffs(n);
    for (int i = 0; i < n; ++i) {
        coeffs[i] = solver.coefficient(i, degree - 1);  // 最高次项系数
        for (int j = degree - 2; j >= 1; --j) {
            coeffs[i] = coeffs[i] * interval(0, H1) + solver.coefficient(i, j);
        }
        coeffs[i] = coeffs[i] * interval(0, H1) + B0[i];  // 最后加上常数项
    }
        // cout << "stepAC" << endl;
        IVector B0_sum = coeffs;
        IVector epsilonVector(n);// [-epsilon, epsilon]^n
        for (int i = 0; i < n; ++i) {
            epsilonVector[i] = interval(-epsilon, epsilon);
        }

        B0_bar = B0_sum + epsilonVector;

        // Step 2:  M = \|\bff^{[k]}(\overline{B}_0)\|_2

      //  IMap f(bff, degree);

        ICnOdeSolver solver1(f, degree);

        solver1.computeCoefficients(B0_bar, degree);
        // cout << "stepAD" << endl;
        IVector B0bar_derivative_k(n); // Assume bff has k-th deriviative
        for (int i = 0; i < n; ++i) {
            B0bar_derivative_k[i] = solver1.coefficient(i, degree);

        }
        // cout << "stepAE" << endl;
        capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm; // Interval type
        interval M1 = euclNorm(B0bar_derivative_k); // compute 2-norm
        double M = M1.rightBound();
        h = pow((epsilon / M), (1.0 / degree));
        if (h > H1) { h = H1; }
        // cout << "H="<< H <<"h="<< h << endl;
    
    return std::make_pair(h, B0_bar);

}



