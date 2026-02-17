#include <iostream>
#include "capd/capdlib.h"
#include "Refine-new.h"
using namespace capd;
using namespace std;



// Extend:
void Extendnew(
    std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
   Stage& S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype,
    int debuglevel
) {
    int m = S.T.size();
    double t0 = S.T[m - 1];
    IVector B0 = S.E[m - 1];
    S.G.push_back(S.G[0]);
   
    std::pair<double, IVector> result1; // Declare result1 here
    if (stepAtype == 0) {
         result1 = stepA(F, B0, H, veps);
    }
    else {
        result1 = stepA0(F, B0, H, veps);
    }
    
    
    //  cout << "stop9" << endl;

   // Output results
    double h = result1.first;
   
    IVector B1 = result1.second;

    
    if (debuglevel == 1) {
        for (int i = 0; i < B0.dimension(); ++i) {
            if (B1[i].leftBound() > B0[i].leftBound() || B1[i].rightBound() < B0[i].rightBound()) {
                std::cout << "Error: end-enclosure is NOT contained in Full-enclosure at dimension " << i << " when time = "
                    << S.T.back() << std::endl;
                std::cout <<"stepA inputs: H = " << H << ", veps = " << veps
                    << ", degree = " << degree << std::endl;
                std::cout << "  B0[" << i << "] = " << B0[i] << ", B1[" << i << "] = " << B1[i] << std::endl;
                std::cout << "  endencl B0 = " << B0 << std::endl;
                std::cout << "  Fullencl B1 = " << B1 << std::endl;
                cout << "h=" << h << endl;
            }
        }
    }

    S.T.push_back(S.T[m - 1] + h);

    S.F.push_back(B1);
    S.G[m].bfF = { B1,B1 };
   // cout << "B1" << B1 << endl;
    //   cout << h << endl;
    //IVector B2= directmethod(F1, B0, B1,h, degree);
    //  cout << "stop7" << endl;
    
    // cout << "B2" << B2 << std::endl;
    //  cout << "stop8" << endl;
    double mu;
    int nn = B0.dimension();
    IMatrix J = computeJacobian(F, B1);
    if ( nn= 2) {
        mu = computemu(J);
      
    }
    else {
        //cout << "J" << J << endl;
        capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
        interval M1 = euclLogNorm(J); // Calculate the 2-log norm
        //cout << "M1" << M1 << endl;
        mu = sup(M1);
       
    }

    S.G[m].mu1 = { mu,mu };
   
   
    IVector E1;
    switch (stepBtype) {
    case 0: E1 = stepBcrlohner(F, degree, h, B0); break;
    case 1: E1 = stepBcrlohnerwithmu(F, degree, h, B0, mu); break;
    case 2: E1 = directmethodwithmu(F, degree, h, B0, mu); break;
    case 3: E1 = puredirectmethod(F, degree, h, B0); break;
    }

    if (debuglevel == 1) {
        for (int i = 0; i < B0.dimension(); ++i) {
            if (B1[i].leftBound() > E1[i].leftBound() || B1[i].rightBound() < E1[i].rightBound()) {
                std::cout << "Error: end-enclosure is NOT contained in Full-enclosure at dimension " << i << " when time = "
                    << S.T.back() << std::endl;
                std::cout << "stepB inputs: h = " << h << ", degree = " << degree << std::endl;
                cout << "mu=" << mu << endl;
                std::cout << "  endencl B0 = " << B0 << std::endl;
                std::cout << "  Fullencl B1 = " << B1 << std::endl;
               
            }
        }
    }
    S.E.push_back(E1);
    S.G[m].bfE = { B0,E1 };
    

  // ReturnType xform= Transform(SVar, SFun, F, B1,S, mu
 //   ReturnType xform = S.G[0].Xform;
 //  S.G[m].Xform = xform;
  // S.G[m].Xform.mu2 = { S.G[m].Xform.mu2[0], S.G[m].Xform.mu2[0] };
   //IMap p3(Convert_to_IMap(SVar, xform.p3), 2.0);
   //IMap inp3(Convert_to_IMap(SVar, xform.inp3), 2.0);
  // IMap gg = xform.gg1;
  
  // double delta1 = TransformBound(inp3, veps, B1);
   
  // IVector coeffs = xform.J * gg(xform.B4);
  // capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

  // interval M1 = euclNorm(coeffs); // 计算 2-范数
   // cout << "M1" << M1 << endl;

  // double MM = sup(M1) / 2;
  // double h1;
  // if (MM == 0.0 || mu == 0.0) {

  // }
  // else {
   //    h1 = eulerstep(h, MM, xform.mu2[0], delta1);
  // }
  // S.G[m].heuler = h1;

    // cout << "Mu" << result.mu << endl;
}

/* EXAMPLE of Extend
    int degree = 3;
    double veps = 0.01;
    double delta = 0.01;
    std::vector<std::string> SVar = {
     "x",
     "y"
    };

    std::vector<std::string> SFun = {
       "-2*x^2",
       "y"
    };

    IVector B(2);
    B[0] = interval(0.891698, 1.012385);
    B[1] = interval(0.9989999, 1.055485);


    std::string sumF = "var:";
    // Use a for loop to dynamically concatenate the var: part
    for (size_t i = 0; i < SVar.size(); ++i) {
        sumF += SVar[i];
        if (i < SVar.size() - 1) {
            sumF += ", ";
        }
    }

    sumF += "; fun:";

    // Use a for loop to dynamically concatenate the fun: parts
    for (size_t i = 0; i < SFun.size(); ++i) {
        sumF += SFun[i];
        if (i < SFun.size() - 1) {
            sumF += ", ";
        }
    }

    sumF += "; ";

    IMap F(sumF, 3.0);
    IMatrix JJ;
    // Initialize ReturnType
    ReturnType ret = {
        {"p2_val1", "p2_val2"},   // p2
        {"p2_val1", "p2_val2"},
        0.0,                      // mu
        {"ggv_val1", "ggv_val2"}, // ggv
        {"inp2_val1", "inp2_val2"}, // inp2
        1,                       // d
         {"inp2_val1", "inp2_val2"},
         F,   // gg
        B,          // B3 (IVector example)
        {"inp3_val1", "inp3_val2"}, // inp3
        B,           // B4 (IVector example)
         JJ
    };

    // Initialize CalD
    CalD calD = {
        {0.0},           // T
        {B},  // E (IVector example)
        {B}   // F (IVector example)
    };

    // Initialize CalQ
    CalQ calQ = {
        {0.0},           // T
        {0.0},              // mu
        {ret},                       // S (ReturnType)
        {0.001},            // delta
        {0.0}                 // h1
    };
    ReturnType2 result = Extend(SVar, SFun, F, calD, calQ, veps, delta, degree);
    ReturnType S = result.calQ.S[1];
    IMatrix J1 = S.J;
    //////cout <<"J1"<< J1 << endl;
    return 0;
*/

void Extendnewnoaffine(
    std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    Stage& S,
    double veps,
    double delta,
    int degree,
    double H,
    int stepBtype,
    int stepAtype
) {
    int m = S.T.size();
    double t0 = S.T[m - 1];
    IVector B0 = S.E[m - 1];
    S.G.push_back(S.G[0]);

    std::pair<double, IVector> result1; // Declare result1 here
    if (stepAtype == 0) {
        result1 = stepA(F, B0, H, veps);
    }
    else {
        result1 = stepA0(F, B0, H, veps);
    }

    //  cout << "stop9" << endl;

   // Output results
    double h = result1.first;
    S.T.push_back(S.T[m - 1] + h);
    IVector B1 = result1.second;
    S.F.push_back(B1);
    S.G[m].bfF = { B1 };
    // cout << "B1" << B1 << endl;
     //   cout << h << endl;
     //IVector B2= directmethod(F1, B0, B1,h, degree);
     //  cout << "stop7" << endl;

     // cout << "B2" << B2 << std::endl;
     //  cout << "stop8" << endl;
    double mu;
    int nn = B0.dimension();
    IMatrix J = computeJacobian(F, B1);
    if (nn = 2) {
        mu = computemu(J);

    }
    else {
        //cout << "J" << J << endl;
        capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
        interval M1 = euclLogNorm(J); // Calculate the 2-log norm
        //cout << "M1" << M1 << endl;
        mu = sup(M1);

    }

    S.G[m].mu1 = { mu };


    IVector E1;
    switch (stepBtype) {
    case 0: E1 = stepBcrlohner(F, degree, h, B0); break;
    case 1: E1 = stepBcrlohnerwithmu(F, degree, h, B0, mu); break;
    case 2: E1 = directmethodwithmu(F, degree, h, B0, mu); break;
    case 3: E1 = puredirectmethod(F, degree, h, B0); break;
    }
    S.E.push_back(E1);
    S.G[m].bfE = { E1 };


    ReturnType xform = Transformnoaffine(SVar, SFun, F, B1,S, mu);
    S.G[m].Xform = xform;
    IMap p3(Convert_to_IMap(SVar, xform.p3), 2.0);
    IMap inp3(Convert_to_IMap(SVar, xform.inp3), 2.0);
    IMap gg = xform.gg1;

    double delta1 = TransformBound(inp3, veps, B1);

    IVector coeffs = xform.J * gg(xform.B4);
    capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

    interval M1 = euclNorm(coeffs); // 计算 2-范数
    // cout << "M1" << M1 << endl;

    double MM = sup(M1) / 2;
    double h1;
    if (MM == 0.0 || mu == 0.0) {

    }
    else {
        h1 = eulerstep(h, MM, xform.mu2[0], delta1);
    }
    S.G[m].heuler = h1;

    // cout << "Mu" << result.mu << endl;
}



