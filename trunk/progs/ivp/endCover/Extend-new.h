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
         result1 = stepA(F, B0, H, veps, degree);
    }
    else {
        result1 = stepA0(F, B0, H, veps, degree);
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
        mu = M1.rightBound();
       
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
        result1 = stepA(F, B0, H, veps, degree);
    }
    else {
        result1 = stepA0(F, B0, H, veps, degree);
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
        mu = M1.rightBound();

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

    double MM = M1.rightBound() / 2;
    double h1;
    if (MM == 0.0 || mu == 0.0) {

    }
    else {
        h1 = eulerstep(h, MM, xform.mu2[0], delta1);
    }
    S.G[m].heuler = h1;

    // cout << "Mu" << result.mu << endl;
}


IVector fullenclosure(
    IMap f,
    IVector B0,
    double h,
    IVector B1,
    int degree
)
{

    int n = B0.dimension();

    ICnOdeSolver solver(f, degree);

    solver.computeCoefficients(B0, degree);
    IVector B0_bar(n);

    IVector coeffs(n);
    for (int i = 0; i < n; ++i) {
        coeffs[i] = B0[i];
        for (int j = 1.0; j < degree; ++j)
            //cout << solver.coefficient(i, j) * interval(0, pow(H, j));
            coeffs[i] += solver.coefficient(i, j) * interval(0, pow(h, j));

    }
    // cout << "stepAC" << endl;
    IVector B0_sum = coeffs;


    B0_bar = B0_sum;

    // Step 2:  M = \|\bff^{[k]}(\overline{B}_0)\|_2

  //  IMap f(bff, degree);

    ICnOdeSolver solver1(f, degree);

    solver1.computeCoefficients(B1, degree);
    // cout << "stepAD" << endl;
    IVector B0bar_derivative_k(n); // Assume bff has k-th deriviative
    for (int i = 0; i < n; ++i) {
        B0bar_derivative_k[i] = solver1.coefficient(i, degree) * interval(0, pow(h, degree));

    }




    B0_bar = B0_sum + B0bar_derivative_k;
    return B0_bar;

}

IVector Extendlohner(IMap F,IVector E0,int n,
    double veps, int order,
    double T, int stepBtype,
    int stepAtype
    ) {
    double t0 = 0;
    double H = T;
    IMatrix Q0 = IMatrix::Identity(n);
    IMatrix R0 = IMatrix::Identity(n);
    IVector r0(n);
    IVector c0(n);
    ICnOdeSolver solver(F, order);
    IVector F1(n);
    IMatrix J = IMatrix::Identity(n);
    IMatrix Q1 = IMatrix::Identity(n);
    IMatrix R1 = IMatrix::Identity(n);
    IVector E1(n);
    IVector r1(n);
    IVector z1(n); // Assume bff has k-th deriviative
    IVector ss1(n);
    IVector hatY(n);
    std::pair<double, IVector> result1; // Declare result1 here
    while (t0 < T)
    {

        // cout << "Iteration: " << t0 << endl;
        for (int i = 0; i < n; ++i) {
            c0[i] = interval(center(E0)[i], center(E0)[i]);
        }
        //  cout << "c0=" << c0 << endl;
        r0 = E0 - c0;  //[-rx,rx]\times[-ry,ry]

        result1 = stepA(F, E0, H, veps, order);


        double h = result1.first;

        F1 = result1.second;

        F1 = fullenclosure(F, E0, h, F1, order); // sum([0,h]^k*F^[k](E0)+Remainder
        ICnTimeMap timeMap(solver);
        CnMultiMatrixRect2Set set(E0, 3);
        timeMap(h, set);


        IMatrix A0 = set.currentSet(); //sum(h^iJ_{f^[i]}(E0)))

        solver.computeCoefficients(F1, order);

        for (int i = 0; i < n; ++i) {
            z1[i] = solver.coefficient(i, order);

        }
        z1 = z1 * pow(h, order);
       // cout << "z1=" << z1 << wmax(z1) << endl;

        for (int i = 0; i < n; ++i) {
            ss1[i] = interval(center(z1)[i], center(z1)[i]);

        }


        solver.computeCoefficients(c0, order);

        for (int i = 0; i < n; ++i) {
            hatY[i] = c0[i];
            for (int j = 1.0; j < order - 1; ++j)
                ////////cout << solver.coefficient(i, j) * interval(0, pow(H, j));
                hatY[i] += solver.coefficient(i, j) * pow(h, j);

        }
       //  cout << "hatY=" << hatY << endl;
        hatY = hatY + ss1;

        //  hatY = stepBcrlohner(F, order -1, h,c0 )+ss1;///error 换成 自己弄
        // cout << "hatY=" << hatY<<endl;



         // 在 QR 分解前添加检查
        IMatrix product = A0 * Q0;
       // cout << "A0*Q0 product: " << product << endl;

        IMatrix midMatrix(product.numberOfRows(), product.numberOfColumns());

        for (int i = 1; i < product.numberOfRows() + 1; ++i) {
            for (int j = 1; j < product.numberOfColumns() + 1; ++j) {
                double mid = (product(i, j).leftBound() + product(i, j).rightBound()) / 2.0;
                midMatrix(i, j) = Interval(mid, mid);
            }
        }

        // cout << "midMatrix=" << midMatrix << endl;

        try {
           //  cout << "here" << endl;
            capd::matrixAlgorithms::QR_decompose(midMatrix, Q1, R1);
        }
        catch (const std::exception& e) {
           // cout << "midMatrix=" << midMatrix << endl;
            cerr << "QR decomposition failed: " << e.what() << endl;
            IMatrix Q1 = IMatrix::Identity(n);
        }


        //  cout << "Q1=" << Q1 << endl;

        E1 = hatY + A0 * Q0 * r0 + z1 - ss1;

        //IVector TMP = stepBcrlohner(F, order, h, E0);
      //  E1=IntersectB(E1,TMP);

        r1 = transpose(Q1) * A0 * Q0 * r0 + transpose(Q1) * (z1 - ss1);

        //  cout << "r1=" << r1 << endl;


        E0 = E1;
       //  cout << "E0=" << E0 << endl;
        t0 = t0 + h;
      //   cout << "t0=" << t0 << endl;
        H = T - t0;

        r0 = r1;

        Q0 = Q1;


    }
    return E0;
}


IVector ExtendlohnerT(vector<string>& SFun, const vector<string>& SVar, IMap F, IVector E0, int n,
    double veps, int order,
    double T, int stepBtype,
    int stepAtype
) {
    double t0 = 0;
    double H = T;
    IMatrix Q0 = IMatrix::Identity(n);
    IMatrix R0 = IMatrix::Identity(n);
    IVector r0(n);
    IVector c0(n);
    ICnOdeSolver solver(F, order);
    IVector F1(n);
    IMatrix J = IMatrix::Identity(n);
    IMatrix Q1 = IMatrix::Identity(n);
    IMatrix R1 = IMatrix::Identity(n);
    IVector E1(n);
    IVector r1(n);
    IVector z1(n); // Assume bff has k-th deriviative
    IVector ss1(n);
    IVector hatY(n);
    std::pair<double, IVector> result1; // Declare result1 here
    while (t0 < T)
    {

        // cout << "Iteration: " << idex << endl;
        for (int i = 0; i < n; ++i) {
            c0[i] = interval(center(E0)[i], center(E0)[i]);
        }
        //  cout << "c0=" << c0 << endl;
        r0 = E0 - c0;

        result1 = stepA(F, E0, H, veps, order);


        double h = result1.first;

        F1 = result1.second;

        F1 = fullenclosure(F, E0, h, F1, order);
        ICnTimeMap timeMap(solver);
        CnMultiMatrixRect2Set set(E0, 3);
        timeMap(h, set);


        IMatrix A0 = set.currentSet();

        solver.computeCoefficients(F1, order);

        for (int i = 0; i < n; ++i) {
            z1[i] = solver.coefficient(i, order);

        }
        z1 = z1 * pow(h, order);
        //cout << "z1=" << z1 << wmax(z1) << endl;

        for (int i = 0; i < n; ++i) {
            ss1[i] = interval(center(z1)[i], center(z1)[i]);

        }


        solver.computeCoefficients(c0, order);

        for (int i = 0; i < n; ++i) {
            hatY[i] = c0[i];
            for (int j = 1.0; j < order - 1; ++j)
                ////////cout << solver.coefficient(i, j) * interval(0, pow(H, j));
                hatY[i] += solver.coefficient(i, j) * pow(h, j);

        }
        // cout << "hatY=" << hatY << endl;
        hatY = hatY + ss1;

        //  hatY = stepBcrlohner(F, order -1, h,c0 )+ss1;///error 换成 自己弄
       //  cout << "hatY=" << hatY<<endl;



         // 在 QR 分解前添加检查
        IMatrix product = A0 * Q0;
        //cout << "A0*Q0 product: " << product << endl;

        IMatrix midMatrix(product.numberOfRows(), product.numberOfColumns());

        for (int i = 1; i < product.numberOfRows() + 1; ++i) {
            for (int j = 1; j < product.numberOfColumns() + 1; ++j) {
                double mid = (product(i, j).leftBound() + product(i, j).rightBound()) / 2.0;
                midMatrix(i, j) = Interval(mid, mid);
            }
        }

        // cout << "midMatrix=" << midMatrix << endl;

        try {
            // cout << "here"<<idex << endl;
            capd::matrixAlgorithms::QR_decompose(midMatrix, Q1, R1);
        }
        catch (const std::exception& e) {
            cerr << "QR decomposition failed: " << e.what() << endl;
            IMatrix Q1 = IMatrix::Identity(n);
        }
      //  cout << "Q1=" << Q1 << endl;
        IMatrix QQ = transpose(Q1);
       // cout << "QQ=" << QQ << endl;
        IMatrix Q00 = transpose(Q0);
        vector<string> SFun1 = transformSFun(SFun, SVar, Q00, n);
       // cout << SFun1 << endl;
        std::string sumF = Convert_to_IMap(SVar, SFun1);
        IMap FF1(sumF, 3);
        IVector ET1 = Q00 * E0;

        double mu;
        J = computeJacobian(FF1, Q00 * F1);
        if (n == 2) {
            
            mu = computemu(J);

        }
        else {
            //cout << "J" << J << endl;
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval M1 = euclLogNorm(J); // Calculate the 2-log norm
            //cout << "M1" << M1 << endl;
            mu = M1.rightBound();

        }

       
       IVector TMPE = stepBcrlohnerwithmu(FF1, order, h, ET1,mu);
        //  cout << "Q1=" << Q1 << endl;
       TMPE = Q0 * TMPE;
      // cout <<"TMPE="<< TMPE << endl;
        E1 = hatY + A0 * Q0 * r0 + z1 - ss1;
       // cout << "E1=" << E1 << endl;
        IVector TMP = stepBcrlohner(F, order, h, E0);
       // cout << "TMP=" << TMP << endl;
        E1 = IntersectB(E1, TMP);
       /// cout << "E1=" << E1 << endl;
        E1 = IntersectB(TMPE, E1);


        r1 = QQ * A0 * Q0 * r0 + QQ * (z1 - ss1);

        //  cout << "r1=" << r1 << endl;


        E0 = E1;
        // cout << "E0=" << E0 << endl;
        t0 = t0 + h;
        // cout << "t0=" << t0 << endl;
        H = T - t0;

        r0 = r1;

        Q0 = Q1;


    }
    return E0;
}