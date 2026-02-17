#include <iostream>
#include "capd/capdlib.h"
#include "stepAB-new.h"
#include "SymbolCompute.h"
using namespace capd;
using namespace std;



// Compute the Euler step:
//		step(mu, eps, f, F1, H) -> h
double eulerstep(double H, double M, double mu, double delta) {
    // int n = B1.dimension();
   // IMatrix J = computeJacobian(f, B1);
   // IVector coeffs = J * f(B1);
   // capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

  //  interval M1 = euclNorm(coeffs); // 计算 2-范数
    // cout << "M1" << M1 << endl;
  //  double h2 = 0.0;
   // double MM = sup(M1);
   // if (MM == 0.0|| mu==0.0) {
  //      h2 = h;
//	}
  //  else { 

    if (mu <= 0) {
        return  (2 * mu * delta) / (M * (exp(mu * H) - 1));
    }
    else {
        return  (2 * mu * delta) / (M * (exp(mu * H) - 1) - mu * mu * delta);
    }


}
/* Example to call step(..)
    int main() {
        double H = 1.0;
        double mu = -1.0;
        double eps = 0.1;


        std::string func = "var:x,y;fun:-2*x^2,y;";
        IMap f(func,3.0);


        IVector B1(2);
        B1[0] = interval(1.0, 1.0);
        B1[1] = interval(1.0, 1.0);
        // IMatrix J = computeJacobian(f, B1);
   // IVector coeffs = J * f(B1);
   // capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

  //  interval M1 = euclNorm(coeffs); // 计算 2-范数
    // cout << "M1" << M1 << endl;
  //  double h2 = 0.0;
   // double MM = sup(M1);
   // if (MM == 0.0|| mu==0.0) {
  //      h2 = h;
//	}
  //  else {
        double h = step(mu, eps, M,H);

        cout << h << endl;
}
*/



IMatrix lPi1(IMap f, IVector B) {
    int n = B.dimension();
    IMatrix P(n, n); //Initial P=I
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            P[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Step 1: search k satisfies 0 < T[1] * T[2] 
    int k = -1;
    IVector T;
    T = f(B);

    for (int i = 0; i < n; ++i) {

        if (T[i].leftBound() * T[i].rightBound() > 0) {
            k = i; // find k
            break;
        }
    }
    if (k == -1) {
        throw std::runtime_error("No valid index k found where 0 < T[1] * T[2].");
    }
    // Step 2: if  0 < T[1]，update  P
    if (T[k].leftBound() > 0) {
        for (int i = 0; i < n; ++i) {
            interval TT = T[i]; //  f[i] 
            if (TT.leftBound() <= 0) {
                P[i][k] = 1 + std::abs(TT.leftBound() / T[k].leftBound());
            }
        }
    }



    // Step 3: if T[2] < 0，update P
    if (T[k].rightBound() < 0) {
        for (int i = 0; i < n; ++i) {
            interval TT = T[i];
            if (TT.leftBound() <= 0) {
                P[i][k] = -1 - std::abs(TT.leftBound() / T[k].rightBound());
            }
        }
    }
    return P;
}
/*  Example of calling \pi_1:

    std::string func = "var:x,y;fun:-2*x^2, y;";
    IMap f(func);


    IVector B(2);
    B[0] = interval(1.0, 2.0);
    B[1] = interval(-1.0, 1.0);

    IMatrix P = lPi1(f, B);
    //////cout << P;*/


    // Compute the pure translation map (lPi2)
IVector lPi2(const IVector& B) {
    int n = B.dimension(); // Get the dimension of the interval vector
    IVector C(n); // Initialize the return vector C

    for (int i = 0; i < n; ++i) {
        C[i] = B[i].leftBound() - 1; // Take the left bound of the interval and subtract 1
    }

    return C;
}
/*  Example for computing lPi2:
    // Define the input interval vector B
    IVector B(3);
    B[0] = interval(1.5, 2.5);
    B[1] = interval(0.0, 1.0);
    B[2] = interval(-2.0, -1.0);

    // Call the lPi2 function
    IVector C = lPi2(B);

    // Output the result vector C
    std:://////cout << "Result vector C:" << std::endl;
    std:://////cout << C << std::endl;*/



    // Computing the radical map \whpi (lPi3):
double lPi3(IMap f, IVector B) {
    IMatrix J = computeJacobian(f, B);


#include "capd/vectalg/Norm.hpp"

    int n = B.dimension();
    capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;
    interval M1 = euclNorm(J); // Compute the 2-norm
    int M = sup(M1);
    // IVector T;
    //  T = f(B); // Compute f[i] over the interval B
    double d = 0.5;
    d = std::max(1, M * 2 - 1);
    // d = std::min(d, 50.0);

    return d;
}
/* Example of calling \whpi
    int main() {
    int maxDerivativeOrder = 2;
    IMap f("var:x,y;fun:-2*x^2+y,y;", maxDerivativeOrder);

    IVector B(2);
    B[0] = interval(1.0, 1.0);
    B[1] = interval(1.0, 1.0);
    int d = lPi3(f, B);
    cout << d << endl;
    }
*/


//  Given \xi as the desired error bound in the primal space,
//  compute the error bound \xi2 that is needed in the transformed space:
//  computexi(inversePi, xi, F1) --> \xi2
double TransformBound(IMap invpi, double xi, IVector B) {
    IMatrix J = computeJacobian(invpi, B);
    int n = B.dimension();
    capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;
    interval M1 = euclNorm(J); // Compute the 2-norm
    double M = sup(M1);
    // cout << M;
    double xi2 = xi / M;
    /*
     int n = B.dimension();
     // Initialize Cartesian product
     std::vector<IVector> Q; // Store interval combinations
     IVector P(n); // A single interval combination
     for (int i = 0; i < (1 << n); ++i) { // All 2^n combinations
         for (int j = 0; j < n; ++j) {
             if (i & (1 << j)) {
                 P[j] = B[j].rightBound();
             }
             else {
                 P[j] = B[j].leftBound();
             }
         }
         Q.push_back(P);
     }
     double xi2 = 10*xi;
     double length = 0.0;
     IVector delta(n);
     IVector B1(n);
     for (size_t j = 0; j < Q.size(); ++j) {
         for (int k = 0; k < n; ++k) {
             delta[k] = interval(-xi2, xi2);
         }

         for (int k = 0; k < n; ++k) {
             B1[k] = Q[j][k];
         }
         IVector T = invpi(delta + B1);

         for (int k = 0; k < n; ++k) {
             length = std::max(length, T[k].rightBound() - T[k].leftBound());
         }
     }
     for (size_t j = 0; j < Q.size(); ++j) {

         while (length > xi) {
             length = 0.0;

             for (int k = 0; k < n; ++k) {
                 delta[k] = interval(-xi2, xi2);
             }

             for (int k = 0; k < n; ++k) {
                 B1[k] = Q[j][k];
             }
             IVector T = invpi(delta + B1);

             for (int k = 0; k < n; ++k) {
                 length = std::max(length, T[k].rightBound() - T[k].leftBound());
             }

             xi2 = xi2 / 2;
         }

     }
     */
    return  xi2;
}

/* Example of calling computexi:
    int main() {
    double xi = 0.0001;
    IMap invpi("var: x, y;fun:1/x^(1/4)+0.9,1/y^(1/4)+0.9;", 2.0);
    IVector B(2);
    B[0] = interval(0.6380134, 1.0);
    B[1] = interval(0.6380134, 1.0);

    double xi2 = computexi(invpi, xi, B);
    cout << xi2;
    }
*/

// Determines if (\bff, F1) contains a node of \bff:

bool AvoidsZero(IMap F, IVector B) {
    IVector T = F(B);
    for (int i = 0; i < B.dimension(); ++i) {
        if (T[i].leftBound() * T[i].rightBound() > 0) {
            return true;
        }
    }
    return false;
}

//  Transform(bff, F1) computes the transformation
//		of (bff, F1) into (bfg, F2).  It returns a
//		ReturnType structure with lots of data...


bool AvoidsZeroplus(IMap F, IVector B) {
    IVector T = F(B);
    for (int i = 0; i < B.dimension(); ++i) {
        if (T[i].leftBound() * T[i].rightBound() <= 0 || B[i].leftBound() * B[i].rightBound() <= 0) {
            return false;
        }
    }
    return true;
}

ReturnType Transformp3(std::vector<std::string> p2,
    int n, double d, std::vector<std::string> SVar, std::vector<std::string> ggv, IVector B3,
    IMatrix invP, IVector C, std::vector<std::string> inp2


)
{
    std::vector<std::string> p3;
    for (const auto& expr : p2) {
        std::ostringstream oss;
        oss << "1/(" << expr << ")^" << d; // Construct 1/p2[i]^d format
        p3.push_back(oss.str());
    }
    //   cout << p3;
    for (int i = 0; i < n; ++i) {
        p3[i] = expand_expression(p3[i]);

    }
    std::vector<std::string> inigg(n, ""); // Assume that the initial value of inig is an empty string


    for (size_t i = 0; i < n; ++i) {
        std::stringstream result2;

        for (char ch : ggv[i]) {
            bool replaced = false;


            for (size_t j = 0; j < n; ++j) {

                if (ch == SVar[j][0]) {

                    result2 << "(" << SVar[j] << " ^" << "(" << (-1.0 / d) << ")" << ")";
                    replaced = true;

                }
            }


            if (!replaced) {
                result2 << ch;
            }
        }


        inigg[i] = result2.str();
    }

    // cout << inigg;
    std::vector<std::string> gg;


    for (size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "(" << inigg[i] << ")" << " * " << "(" << -d << ")" << " * " << "(" << SVar[i] << " ^ " << "(" << (1.0 + 1.0 / d) << ")" << ")";
        gg.push_back(oss.str());
    }
    for (int i = 0; i < n; ++i) {
        gg[i] = expand_expression(gg[i]);

    }
    // cout << "gg" << gg << endl;;
    IVector B4(n);
    for (int i = 0; i < n; ++i) {
        //cout << "1" << endl;
        B4[i] = interval(1.0 / pow(B3[i].rightBound(), d), 1.0 / pow(B3[i].leftBound(), d));
        // cout << "2" << endl;
    }
    // cout << "B4:" << B4 << endl;;

    std::vector<std::string> inp3;

    // 计算 P.Transpose(Matrix(var)); 

    for (size_t i = 0; i < n; ++i) {
        std::string expr = "";
        for (size_t j = 0; j < n; ++j) {
            if (invP[i][j] != 0) {

                if (!expr.empty()) {
                    expr += " + ";
                }
                if (C[j].leftBound() > 0) {
                    expr += std::to_string(invP[i][j].leftBound()) + "*(" + SVar[j] + "^" + "(" +
                        std::to_string(-1.0 / d) + ")" + " + " + std::to_string(C[j].leftBound()) + ")";
                }
                else
                {
                    double cc = -C[j].leftBound();
                    expr += std::to_string(invP[i][j].leftBound()) + "*(" + SVar[j] + "^" + "(" +
                        std::to_string(-1.0 / d) + ")" + " - " + std::to_string(cc) + ")";
                }

            }
        }
        inp3.push_back(expr);
    }
    for (int i = 0; i < n; ++i) {
        inp3[i] = expand_expression(inp3[i]);

    }
    // cout << "inp3" << inp3 << endl;;

    std::string sumD = Convert_to_IMap(SVar, gg);
    //  cout << sumD << endl;
    double mu = 0.0; IMatrix J;
    // cout << "stop" << endl;
    IMap gg1(sumD, 2);

    J = computeJacobian(gg1, B4);
    if (n == 2) {
        mu = computemu(J);
    }
    else {
        // cout << "J:" << J << endl;
        capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
        // cout << "EuclLogNorm(J)=" << euclLogNorm(J) << std::endl;
        interval Imu = euclLogNorm(J);
        mu = Imu.rightBound();
    }
    //  cout << mu;

    //  cout << "stop1" << endl;
    IMap finp3(Convert_to_IMap(SVar, inp3), 2.0);
    IMap fp3(Convert_to_IMap(SVar, p3), 2.0);
    ReturnType result = { p2,p3,finp3,fp3, {mu}, ggv, inp2, d,gg, gg1, B3, inp3, B4, J };
    return result;
}

ReturnType Transform(
    std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    IVector B,
    Stage S,
    double mu)
{
    bool tmp = AvoidsZeroplus(F, B);
    if (tmp == false || mu <0 ) {
        return S.G[0].Xform;
    }
    int n = B.dimension();

    IMatrix P = lPi1(F, B);
    // cout << "P:" << P << endl;
    std::vector<std::string> p1;

    //  P.Transpose(Matrix(var)); 
    for (size_t i = 0; i < n; ++i) {
        std::string expr = ""; // Linear combination of the current row
        for (size_t j = 0; j < n; ++j) {
            if (P[i][j] != 0) {

                // Combine strings by matrix formula
                if (!expr.empty()) {
                    expr += " + ";
                }
                expr += "(" + std::to_string(P[i][j].leftBound())
                    + ")" + "*(" + SVar[j] + ")";
            }
        }
        p1.push_back(expr);
    }
    // cout << "p1:" << p1 << endl;
    IMatrix invP = capd::matrixAlgorithms::inverseMatrix(P);
    std::vector<std::string> inp1;

    for (size_t i = 0; i < n; ++i) {
        std::string expr = ""; // Linear combination of the current row
        for (size_t j = 0; j < n; ++j) {
            if (invP[i][j] != 0) {
                // Combine strings by matrix formula
                if (!expr.empty()) {
                    expr += " + ";
                }
                expr += "(" + std::to_string(invP[i][j].leftBound()) + ")" + "*(" + SVar[j] + ")";
            }
        }
        inp1.push_back(expr);
    }
    // cout << "inp1:" << inp1 << endl;
    std::vector<std::string> inig(SFun.size(), ""); // Assume that the initial value of inig is an empty string


    // Iterate over each SFun string
    for (size_t i = 0; i < SFun.size(); ++i) {
        std::stringstream result;  // Used to store the result after replacement

        for (char ch : SFun[i]) {
            bool replaced = false;

            // Checks if a character matches any variable in SVar
            for (size_t j = 0; j < SVar.size(); ++j) {
                //If the character matches the first character of the variable
                if (ch == SVar[j][0]) {
                    result << "(" + inp1[j] + ")";  // Splice the corresponding replacement expression into the result
                    replaced = true;

                }
            }

            // If no matching variable is found, the original character is retained.
            if (!replaced) {
                result << ch;
            }
        }

        // Save the processed results to inig
        inig[i] = result.str();
    }
    //  cout << inig << endl;
    std::vector<std::string> g;

    //  P.Transpose(Matrix(var)); 

    for (size_t i = 0; i < n; ++i) {
        std::string expr = ""; // 
        for (size_t j = 0; j < n; ++j) {
            if (P[i][j] != 0) {


                if (!expr.empty()) {
                    expr += " + ";
                }
                expr += "(" + std::to_string(P[i][j].leftBound()) + ")" + "*(" + inig[j] + ")";
            }
        }
        g.push_back(expr);
    }
    // cout << "g:" << g << endl;
     // Dynamically generate var: and fun: parts
    std::string sumB = Convert_to_IMap(SVar, p1);
    //  cout << sumB << endl;
    IMap FB(sumB, 1);
    IVector B2 = FB(B);
    //  cout << B2;
    IVector C = lPi2(B2);
    //   cout << C;
    IVector B3 = B2 - C;
    //  cout << B3 << endl;
    std::vector<std::string> ggv(n, "");



    for (size_t i = 0; i < n; ++i) {
        std::stringstream result1;

        for (char ch : g[i]) {
            bool replaced = false;


            for (size_t j = 0; j < n; ++j) {

                if (ch == SVar[j][0]) {
                    double val = C[j].leftBound();
                    if (val > 0) {
                        result1 << "(" << SVar[j] << "+" << std::to_string(val) << ")";
                    }
                    else
                    {
                        val = -val;
                        result1 << "(" << SVar[j] << "-" << std::to_string(val) << ")";
                    }
                    replaced = true;

                }
            }


            if (!replaced) {
                result1 << ch;
            }
        }


        ggv[i] = result1.str();
    }
    for (int i = 0; i < n; ++i) {
        ggv[i] = expand_expression(ggv[i]);

    }
    // cout << "ggv" << ggv << endl;
    std::vector<std::string> p2 = p1;


    for (size_t i = 0; i < n; ++i) {

        if (C[i].leftBound() > 0) {


            p2[i] = p1[i] + " - " + std::to_string(C[i].leftBound());

        }
        else {
            double cc = -C[i].leftBound();
            p2[i] = p1[i] + " + " + std::to_string(cc);
        }

    }

    for (int i = 0; i < n; ++i) {
        p2[i] = expand_expression(p2[i]);

    }
    // cout << "p2" << p2 << endl;

    std::vector<std::string> inp2(n, "");

    //  P.Transpose(Matrix(var)); 
    for (size_t i = 0; i < n; ++i) {

        std::stringstream result3;

        for (char ch : inp1[i]) {
            bool replaced = false;


            for (size_t j = 0; j < n; ++j) {



                if (ch == SVar[j][0]) {
                    double val = C[j].leftBound();
                    if (val > 0) {
                        result3 << "(" << SVar[j] << "+" << std::to_string(val) << ")";
                    }
                    else
                    {
                        val = -val;
                        result3 << "(" << SVar[j] << "-" << std::to_string(val) << ")";
                    }

                    replaced = true;

                }
            }


            if (!replaced) {
                result3 << ch;
            }
        }


        inp2[i] = result3.str();
    }
    for (int i = 0; i < n; ++i) {
        inp2[i] = expand_expression(inp2[i]);

    }
    // cout << "inp2" << inp2 << endl;

    std::string sumC = Convert_to_IMap(SVar, ggv);
    // cout << "sumC" << sumC << endl;

    IMap Fgv(sumC, 2);
    double d;
    if (mu == 0) {
       
        return S.G[0].Xform;
    }
    // cout << "B3" << B3 << endl;
    if (mu > 0) {
        d = lPi3(Fgv, B3);
        if(d==1){
            return Transformp3(p2, n, d, SVar, ggv, B3, invP, C, inp2);
        }
        ReturnType result = Transformp3(p2, n, 1, SVar, ggv, B3, invP, C, inp2);
        if (result.mu2[0] < 0){
            return result; 
        }

         double mutmp = 1000;
         double d1 = 2;
            while (d1<d) {
                
                result = Transformp3(p2, n, d1, SVar, ggv, B3, invP, C, inp2);
                mutmp = result.mu2[0];
                if (mutmp < 0) {
                    return result;
                }
                d1 = d1 + 1;
            }
            if (abs(result.mu2[0]) < mu) { return result; }
        return S.G[0].Xform;
    }

    if (mu < 0) {
        d = -lPi3(Fgv, B3);
        if (d == -1) {
            return S.G[0].Xform;
        }
            ReturnType result = Transformp3(p2, n, -2, SVar, ggv, B3, invP, C, inp2);
            if (result.mu2[0] > 0) { return result; }

            double mutmp = 1000;
            double d1 = -3;
            while (d1 > d) {

                result = Transformp3(p2, n, d1, SVar, ggv, B3, invP, C, inp2);
                mutmp = result.mu2[0];
                if (mutmp > 0) {
                    return result;
                }
                d1 = d1 - 1;
            }
            if (abs(result.mu2[0]) < abs(mu)) { return result; }
            return S.G[0].Xform;
        
    }
    // cout << "d" << d << endl;;
    return S.G[0].Xform;
}//Transform}

/* Example of calling Transform:
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


    for (size_t i = 0; i < SVar.size(); ++i) {
        sumF += SVar[i];
        if (i < SVar.size() - 1) {
            sumF += ", ";
        }
    }

    sumF += "; fun:";


    for (size_t i = 0; i < SFun.size(); ++i) {
        sumF += SFun[i];
        if (i < SFun.size() - 1) {
            sumF += ", ";
        }
    }

    sumF += "; ";

    IMap F(sumF, 1.0);

    bool verif = Transform(F, B);
    //////cout << verif << endl;

    ReturnType result = Transform(SVar, SFun,F, B);

    //////cout << result.mu << endl;
*/



ReturnType Transformnoaffinetmp(
    std::vector<std::string> p2,
    int n, 
    double d, 
    std::vector<std::string> SVar, 
    std::vector<std::string> ggv, 
    IVector B3,
    IMatrix invP, 
    std::vector<std::string> inp2
)
{
    std::vector<std::string> p3;
        for (const auto& expr : p2) {
            std::ostringstream oss;
            oss << "1/(" << expr << ")^" << d; // Construct 1/p2[i]^d format
            p3.push_back(oss.str());
        }
    for (int i = 0; i < n; ++i) {
        p3[i] = expand_expression(p3[i]);

    }
    // cout << p3;

    std::vector<std::string> inigg(n, ""); // Assume that the initial value of inig is an empty string


    for (size_t i = 0; i < n; ++i) {
        std::stringstream result2;

        for (char ch : ggv[i]) {
            bool replaced = false;


            for (size_t j = 0; j < n; ++j) {

                if (ch == SVar[j][0]) {

                    result2 << "(" << SVar[j] << " ^" << "(" << (-1.0 / d) << ")" << ")";
                    replaced = true;

                }
            }


            if (!replaced) {
                result2 << ch;
            }
        }


        inigg[i] = result2.str();
    }
    // cout << inigg;
    std::vector<std::string> gg;


    for (size_t i = 0; i < n; ++i) {
        std::ostringstream oss;
        oss << "(" << inigg[i] << ")" << " * " << "(" << -d << ")" << " * " << "(" << SVar[i] << " ^ " << "(" << (1.0 + 1.0 / d) << ")" << ")";
        gg.push_back(oss.str());
    }
    for (int i = 0; i < n; ++i) {
        gg[i] = expand_expression(gg[i]);

    }
    // cout << "gg" << gg << endl;;
    IVector B4(n);
    for (size_t i = 0; i < n; ++i) {
        //   cout << "1" << endl;
        B4[i] = interval(1.0 / pow(B3[i].rightBound(), d), 1.0 / pow(B3[i].leftBound(), d));
        // cout << "2" << endl;
    }
    // cout << "B4:" << B4 << endl;;

    std::vector<std::string> inp3 = p3;

    // 计算 P.Transpose(Matrix(var)); 




    std::string sumD = Convert_to_IMap(SVar, gg);
    //  cout << sumD << endl;
    double mu = 1000; IMatrix J;
    // cout << "stop" << endl;
    IMap gg1(sumD, 2);

    J = computeJacobian(gg1, B4);

    int n_rows = J.numberOfRows();
    int n_cols = J.numberOfColumns();

    bool has_zero_row = false;
    for (int i = 1; i <= n_rows; ++i) {
        bool all_zero = true;
        for (int j = 1; j <= n_cols; ++j) {
            if (J(i, j).leftBound() != 0 || J(i, j).rightBound() != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) {
            has_zero_row = true;
            break;
        }
    }

    if (!has_zero_row) {
        if (n == 2) {
            mu = computemu(J);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J);
            mu = Imu.rightBound();
        }
    }
    else {
        mu = 0;
    }
    //    cout<<mu<<endl;
    //  cout << "stop1" << endl;
    IMap finp3(Convert_to_IMap(SVar, inp3), 2.0);
    IMap fp3(Convert_to_IMap(SVar, p3), 2.0);
    ReturnType result = { p2,p3,finp3,fp3, {mu}, ggv, inp2, d,gg, gg1, B3, inp3, B4, J };
    return result;
}



ReturnType Transformnoaffine(
    std::vector<std::string> SVar,
    std::vector<std::string> SFun,
    IMap F,
    IVector B,
    Stage S,
    double mu
)
{
    int n = B.dimension();
    bool tmp = AvoidsZeroplus(F, B);
    if (tmp == false || mu < 0) {
        return S.G[0].Xform;
    }

    IMatrix P(n, n); //Initial P=I
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            P[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    // cout << "P:" << P << endl;


    std::vector<std::string> p1 = SVar;


    // cout << "p1:" << p1 << endl;
    IMatrix invP = P;
    std::vector<std::string> inp1 = p1;


    // cout << "inp1:" << inp1 << endl;
    std::vector<std::string> inig = SFun; // Assume that the initial value of inig is an empty string



    //  cout << inig << endl;
    std::vector<std::string> g = SFun;

    //  P.Transpose(Matrix(var)); 


    // cout << "g:" << g << endl;
     // Dynamically generate var: and fun: parts
    std::string sumB = Convert_to_IMap(SVar, p1);
    //  cout << sumB << endl;
    IMap FB(sumB, 1);
    IVector B2 = B;
    //  cout << B2;


    //   cout << C;
    IVector B3 = B;
    //  cout << B3 << endl;
    std::vector<std::string> ggv = SFun;




    // cout << "ggv" << ggv << endl;
    std::vector<std::string> p2 = p1;


    // cout << "p2" << p2 << endl;

    std::vector<std::string> inp2 = p2;



    // cout << "inp2" << inp2 << endl;

    std::string sumC = Convert_to_IMap(SVar, ggv);
    // cout << "sumC" << sumC << endl;

    IMap Fgv(sumC, 2);
    double d;
    if (mu == 0) {

        return S.G[0].Xform;
    }
    // cout << "B3" << B3 << endl;
    if (mu > 0) {
        d = lPi3(Fgv, B3);
        if (d == 1) {
            return Transformnoaffinetmp(p2, n, d, SVar, ggv, B3, invP,  inp2);
        }
        ReturnType result = Transformnoaffinetmp(p2, n, 1, SVar, ggv, B3, invP,  inp2);
        if (result.mu2[0] < 0) {
            return result;
        }

        double mutmp = 1000;
        double d1 = 2;
        while (d1 < d) {

            result = Transformnoaffinetmp(p2, n, d1, SVar, ggv, B3, invP, inp2);
            mutmp = result.mu2[0];
            if (mutmp < 0) {
                return result;
            }
            d1 = d1 + 1;
        }
        if (abs(result.mu2[0]) < mu) { return result; }
        return S.G[0].Xform;
    }

    if (mu < 0) {
        d = -lPi3(Fgv, B3);
        if (d == -1) {
            return S.G[0].Xform;
        }
        ReturnType result = Transformnoaffinetmp(p2, n, -2, SVar, ggv, B3, invP,  inp2);
        if (result.mu2[0] > 0) { return result; }

        double mutmp = 1000;
        double d1 = -3;
        while (d1 > d) {

            result = Transformnoaffinetmp(p2, n, d1, SVar, ggv, B3, invP,  inp2);
            mutmp = result.mu2[0];
            if (mutmp > 0) {
                return result;
            }
            d1 = d1 - 1;
        }
        if (abs(result.mu2[0]) < abs(mu)) { return result; }
        return S.G[0].Xform;

    }
    // cout << "d" << d << endl;;
    return S.G[0].Xform;
}

