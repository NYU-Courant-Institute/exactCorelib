#include <iostream>
#include <stdexcept>
#include "capd/capdlib.h"
#include "Transform-new.h"
using namespace capd;
using namespace std;



void Bisect(Stage& S, int idx, int dim, IMap f, int stepBtype, int degree, int debuglevel) {
    if (idx > 0) {
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }
    std::vector<IVector> bfE = { S.G[idx].bfE[0] };
    std::vector<IVector> bfF = { S.G[idx].bfE[0] };
    std::vector<double> mu1 = { S.G[idx].mu1[0] };
    std::vector<double> mu2 = { S.G[idx].Xform.mu2[0] };
    int N = std::pow(2, S.G[idx].ell);
    // cout << "N" << N << endl;
    double h = (S.T[idx] - S.T[idx - 1]) / (2 * N);
    // cout << "h" << h << endl;
     //cout << "r=" << wmax(bfE.back());
    IVector IF = S.G[idx].bfE[0];
    for (int j = 1; j < N + 1; ++j) {
        // --- compute tmpF1 ---
        ICnOdeSolver solver1(f, 5);

        solver1.computeCoefficients(bfE.back(), 5);

        IVector coeffs(dim);
        for (int k = 0; k < dim; ++k) {
            coeffs[k] = solver1.coefficient(k, 4);
            for (int d = 3; d >= 1; --d) {
                coeffs[k] = coeffs[k] * interval(0, h) + solver1.coefficient(k, d);
            }
            coeffs[k] = coeffs[k] * interval(0, h) + bfE.back()[k];
        }

        // --- compute derivative of original box ---
        ICnOdeSolver solverB0(f, 5);
        solverB0.computeCoefficients(S.G[idx].bfF[j], 5);

        IVector B0bar_derivative_k(dim);
        for (int k = 0; k < dim; ++k) {
            B0bar_derivative_k[k] = interval(0, std::pow(h, 5)) * solverB0.coefficient(k, 5);
        }

        IVector tmpF1 = coeffs + B0bar_derivative_k;
        if (debuglevel == 1) {
            for (int i = 0; i < S.G[idx].bfF[j].dimension(); ++i) {

                double lower = std::max(tmpF1[i].leftBound(), S.G[idx].bfF[j][i].leftBound());
                double upper = std::min(tmpF1[i].rightBound(), S.G[idx].bfF[j][i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {
                    std::cout << "Error: Full-enclsoure in bisect first segment is NOT match with Full-enclosure when stage= "
                        << idx << std::endl;
                    std::cout << "mini-step: j = " << j << "--E = " << bfE.back() << std::endl;
                    std::cout << " h = " << h << ", Full-encl for stage " << S.G[idx].bfF[j] << std::endl;
                    std::cout << "  full-encl obtained " << tmpF1 << std::endl;
                }


            }
        }
        tmpF1 = IntersectB(tmpF1, S.G[idx].bfF[j]);
        IF = Box(IF, tmpF1);
        // cout << tmpF1;
        IMatrix J1 = computeJacobian(f, tmpF1);
        double mu;
        if (dim == 2) {
            mu = computemu(J1);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J1);
            mu = Imu.rightBound();
        }
        mu1.push_back(mu);
        mu2.push_back(S.G[idx].Xform.mu2[j]);
        IVector nextBox;
        switch (stepBtype) {
        case 0: nextBox = stepBcrlohner(f, degree, h, bfE.back()); break;
        case 1: nextBox = stepBcrlohnerwithmu(f, degree, h, bfE.back(), mu); break;
        case 2: nextBox = directmethodwithmu(f, degree, h, bfE.back(), mu); break;
        case 3: nextBox = puredirectmethod(f, degree, h, bfE.back()); break;
        }

        if (debuglevel == 1) {


            for (int i = 0; i < S.G[idx].bfF[j].dimension(); ++i) {

                double lower = std::max(nextBox[i].leftBound(), S.G[idx].bfF[j][i].leftBound());
                double upper = std::min(nextBox[i].rightBound(), S.G[idx].bfF[j][i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {

                    std::cout << "Error: end-enclsoure in bisect first segment is NOT contained in Full-enclosure when stage= "
                        << idx << std::endl;
                    std::cout << "mini-step: j = " << j << "--E = " << bfE.back() << std::endl;
                    std::cout << " h = " << h << ", Full-encl for stage " << S.G[idx].bfF[j] << std::endl;
                    std::cout << "  end-encl obtained " << nextBox << std::endl;
                    cout << "mu=" << mu << endl;
                }
            }
        }
        nextBox = IntersectB(nextBox, S.G[idx].bfF[j]);
        bfE.push_back(nextBox);
        bfF.push_back(tmpF1);

        // --- repeat same logic for next step ---
        ICnOdeSolver solver2(f, 5);
        solver2.computeCoefficients(nextBox, 5);

        IVector coeffs2(dim);
        for (int k = 0; k < dim; ++k) {
            coeffs2[k] = solver2.coefficient(k, 4);
            for (int d = 3; d >= 1; --d) {
                coeffs2[k] = coeffs2[k] * interval(0, h) + solver2.coefficient(k, d);
            }
            coeffs2[k] = coeffs2[k] * interval(0, h) + nextBox[k];
        }
        IVector tmpF2 = coeffs2 + B0bar_derivative_k;

        if (debuglevel == 1) {
            for (int i = 0; i < S.G[idx].bfF[j].dimension(); ++i) {

                double lower = std::max(tmpF2[i].leftBound(), S.G[idx].bfF[j][i].leftBound());
                double upper = std::min(tmpF2[i].rightBound(), S.G[idx].bfF[j][i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {
                    std::cout << "Error: Full-enclsoure in bisect second segment is NOT mathch with Full-enclosure when stage= "
                        << idx << std::endl;
                    std::cout << "mini-step: j = " << j << "--E = " << nextBox << std::endl;
                    std::cout << " h = " << h << ", Full-encl for stage " << S.G[idx].bfF[j] << std::endl;
                    std::cout << "  full-encl obtained " << tmpF2 << std::endl;
                }

            }
        }
        tmpF2 = IntersectB(tmpF2, S.G[idx].bfF[j]);
        IF = Box(IF, tmpF2);
        IMatrix J2 = computeJacobian(f, tmpF2);
        if (dim == 2) {
            mu = computemu(J2);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J2);
            mu = Imu.rightBound();
        }
        mu1.push_back(mu);
        mu2.push_back(S.G[idx].Xform.mu2[j]);
        IVector nextBox2;
        switch (stepBtype) {
        case 0: nextBox2 = stepBcrlohner(f, degree, h, nextBox); break;
        case 1: nextBox2 = stepBcrlohnerwithmu(f, degree, h, nextBox, mu); break;
        case 2: nextBox2 = directmethodwithmu(f, degree, h, nextBox, mu); break;
        case 3: nextBox2 = puredirectmethod(f, degree, h, nextBox); break;
        }


        if (debuglevel == 1) {
            for (int i = 0; i < S.G[idx].bfE[j].dimension(); ++i) {

                double lower = std::max(nextBox2[i].leftBound(), S.G[idx].bfE[j][i].leftBound());
                double upper = std::min(nextBox2[i].rightBound(), S.G[idx].bfE[j][i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {
                    std::cout << "Error: end-enclsoure in bisect second segment is NOT match with end-enclosure when stage= "
                        << idx << std::endl;
                    std::cout << "mini-step: j = " << j << "--E = " << nextBox << std::endl;
                    std::cout << " h = " << h << ", end-encl " << S.G[idx].bfE[j] << std::endl;
                    std::cout << "  end-encl obtained " << nextBox2 << std::endl;
                    cout << "mu=" << mu << endl;

                }
            }
        }
        nextBox2 = IntersectB(nextBox2, S.G[idx].bfE[j]);
        bfE.push_back(nextBox2);
        bfF.push_back(tmpF2);
    }

    // update Stage object
    S.G[idx].mu1 = mu1;
    S.G[idx].ell = S.G[idx].ell + 1;
    S.G[idx].bfE = bfE;
    S.G[idx].bfF = bfF;
    S.G[idx].Xform.mu2 = mu2;

    if (debuglevel == 1) {
        for (int i = 0; i < S.E[idx].dimension(); ++i) {
            double lower = std::max(S.E[idx][i].leftBound(), bfE.back()[i].leftBound());
            double upper = std::min(S.E[idx][i].rightBound(), bfE.back()[i].rightBound());
            // If the intersection is empty, return an empty interval
            if (lower > upper) {
                std::cout << "Error: end-enclsoure in bisect end is NOT match end-enclsoure when stage= "
                    << idx << std::endl;
            }
        }
    }
    S.E[idx] = IntersectB(bfE.back(), S.E[idx]);

    if (debuglevel == 1) {
        for (int i = 0; i < S.E[idx].dimension(); ++i) {
            double lower = std::max(S.F[idx][i].leftBound(), IF[i].leftBound());
            double upper = std::min(S.F[idx][i].rightBound(), IF[i].rightBound());
            // If the intersection is empty, return an empty interval
            if (lower > upper) {
                std::cout << "Error: full-enclsoure in bisect end is NOT match full-enclsoure when stage= "
                    << idx << std::endl;
            }
        }
    }
    S.F[idx] = IntersectB(S.F[idx], IF);

}




void Subroutine7(Stage& S, int idx, int dim, IMap f, std::vector<std::string> SVar,int debug )
{
    if (idx > 0) {
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }

    double delta = S.G[idx].delta;
    std::vector<double> tmp = center(S.G[idx].bfE[0]);
    IVector p(dim);  // 
    for (int j = 0; j < dim; ++j) {
        p[j] = interval(tmp[j], tmp[j]);
    }
    
    double r0 = wmax(S.G[idx].bfE[0]) / 2;
    IMap p3(Convert_to_IMap(SVar, S.G[idx].Xform.p3), 2.0);
    
    IMap inp3(Convert_to_IMap(SVar, S.G[idx].Xform.inp3), 2.0);
    IMap gg = S.G[idx].Xform.gg1;
  //  cout << "gg" << S.G[idx].Xform.gg << endl;
    IVector q = p3(p);
  //  cout << "q" << q << endl;
    IVector piE0 = p3(S.G[idx].bfE[0]);
    double pr0 = wmax(piE0) / 2;
    tmp = center(piE0);
    IVector cpiE0(dim);  //

    for (int j = 0; j < dim; ++j) {
        cpiE0[j] = interval(tmp[j], tmp[j]);
    }
    IVector Itmp;
    Itmp = p - inp3(cpiE0);
    double d = 0;
    for (int j = 0; j < dim; ++j) {
        d = d + pow((Itmp[j].rightBound() - Itmp[j].leftBound()), 2);
    }
    d = sqrt(d);
   // cout << "d:=" << d << endl;
   // cout << "r:=" << r0 << endl;
    if (debug == 1) {
        if (d > r0) {
            
            cout << "error p not belong to E0" << endl;
            throw std::runtime_error("Refine: p not belong to E0 (debug check failed)");
        }
    }
    int N = std::pow(2, S.G[idx].ell);
    //cout << S.G[idx].bfE.size() << endl;
   // cout << N << endl;
    double H = (S.T[idx] - S.T[idx - 1]) / N;
  // cout << "spe:" << S.E[idx] << endl;
    IVector IF= S.G[idx].bfF[1];
    q = cpiE0;
    for (int j = 1; j < N + 1; ++j) {
      //  cout << "q" << q << endl;
        q = q + gg(q) * H;
      //  cout << "H"<<H << endl;
       // cout << "q" << q << endl;
        if (debug == 1) {
            for (int i = 0; i < dim; ++i) {
                if (q[i].leftBound() < p3(S.G[idx].bfF[j])[i].leftBound() || q[i].rightBound() > p3(S.G[idx].bfF[j])[i].rightBound()) {
                    cout << "Warring: q not belong to pi(E0) when j="<<j << endl;
                    cout << "H" << H << endl;
                    cout << "q" << q << endl;
                    cout << p3(S.G[idx].bfF[j-1]) << endl;
                    cout << p3(S.G[idx].bfF[j]) << endl;
                                        throw std::runtime_error("Refine: q not belong to pi(E0) (debug check failed)");
                  //  cout << S.G[idx].bfE[j+1] << endl;
                }
            }
        }
        for (int i = 0; i < dim; ++i) {
            if (q[i].leftBound() < p3(S.G[idx].bfE[j])[i].leftBound() || q[i].rightBound() > p3(S.G[idx].bfE[j])[i].rightBound()) {
               double tt= (p3(S.G[idx].bfE[j])[i].leftBound() + p3(S.G[idx].bfE[j])[i].rightBound()) / 2;
               q[i] = interval(tt, tt);
            }
        }
        double delta1 = TransformBound(inp3, delta, S.G[idx].bfF[j]);

        IMatrix J = computeJacobian(gg, p3(S.G[idx].bfF[j]));
        double mu2;
        if (dim == 2) {
            mu2 = computemu(J);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J);
            mu2 = Imu.rightBound();
        }


        S.G[idx].Xform.mu2[j] = mu2;
        double r1 = (r0+d) * exp(j * S.G[idx].mu1[j] * H)+delta;
        double r2 = (pr0) * exp(j * mu2 * H) + delta1;
        IVector B(dim);  IVector B1(dim);
        for (int k = 0; k < dim; ++k) {
            B[k] = interval(-r1,r1);
            B1[k] = interval(-r2, r2);
        }
        tmp = center(p3(S.G[idx].bfE[j - 1]));
        for (int k = 0; k < dim; ++k) {
            Itmp[k] = interval(tmp[k], tmp[k]);
        }
        Itmp = Box(Itmp + B1, q + B1);
        Itmp = inp3(Itmp);
        if (debug == 1) {
            for (int i = 0; i < dim; ++i) {
                double lower = std::max(S.G[idx].bfF[j][i].leftBound(), Itmp[i].leftBound());
                double upper = std::min(S.G[idx].bfF[j][i].rightBound(), Itmp[i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {
                    cout << "Fullreal:=" << S.G[idx].bfF[j] << endl;
                    cout << "Fullreal:=" << S.G[idx].bfF[j-1] << endl;
                    cout << "Fullreal:=" << S.G[idx].bfF[j+1] << endl;
                    cout << "Fullnow:=" << Itmp << endl;
                    std::cout << "Error: full-enclsoure in EulerTube after transforming back is NOT match full-enclsoure when (stage,j)= "
                        << idx<<"," << j << std::endl;
                    throw std::runtime_error("Refine: full-enclosure mismatch in EulerTube (debug check failed)");
                }
            }
        }
        S.G[idx].bfF[j] = IntersectB(Itmp,S.G[idx].bfF[j] );
        IF = Box(IF, S.G[idx].bfF[j]);
         J = computeJacobian(f, S.G[idx].bfF[j]);
       
        if (dim == 2) {
            mu2 = computemu(J);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J);
            mu2 = Imu.rightBound();
        }


        S.G[idx].mu1[j] = mu2;
        IVector endenc = q + B1;
        endenc = inp3(endenc);
        if (debug == 1) {
            for (int i = 0; i < dim; ++i) {
                double lower = std::max(S.G[idx].bfE[j][i].leftBound(), endenc[i].leftBound());
                double upper = std::min(S.G[idx].bfE[j][i].rightBound(), endenc[i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {
                    cout << "endenc before:=" << S.G[idx].bfE[j] << endl;
                    cout << "endenc now:=" << endenc << endl;
                    std::cout << "Error: end-enclsoure in EulerTube after transforming back is NOT match end-enclsoure when (stage,j)= "
                        << idx << "," << j << std::endl;
                    throw std::runtime_error("Refine: end-enclosure mismatch in EulerTube (debug check failed)");
                }
            }
        }

        Itmp = IntersectB(inp3(q + B1),S.G[idx].bfE[j]);
        if (debug == 1) {
            for (int i = 0; i < dim; ++i) {
                double lower = std::max(Itmp[i].leftBound(), (inp3(q) + B)[i].leftBound());
                double upper = std::min(Itmp[i].rightBound(),(inp3(q) + B)[i].rightBound());
                // If the intersection is empty, return an empty interval
                if (lower > upper) {
                    std::cout << "Error: end-enclsoure in EulerTub  is NOT match end-enclsoure when (stage,j)= "
                        << idx << "," << j << std::endl;
                }
            }
        }
        S.G[idx].bfE[j] = IntersectB(inp3(q) + B,Itmp);

    }
    S.E[idx] = S.G[idx].bfE.back();
    S.F[idx] = IF;

    



}





void Refinenew(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug) {
    double r0 = wmax(S.E.back());
    while (veps < r0) {
        IVector E0(dim);
        for (int i = 0; i < dim; ++i) {
            E0[i] = interval(S.E[0][i].leftBound() + (S.E[0][i].rightBound() - S.E[0][i].leftBound()) / 4,
                S.E[0][i].rightBound() - (S.E[0][i].rightBound() - S.E[0][i].leftBound()) / 4);
        }
        S.E[0] = E0;
        S.F[0] = E0;
        S.G[0].bfE[0] = E0;
        S.G[0].bfF[0] = E0;

        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        for (int i = 1; i < numofstage; ++i) {

            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;
            if (h > S.G[i].heuler) {

                Bisect(S, i, dim, f, stepBtype, degree, debug);
                double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
                IMap inp3 = S.G[i].Xform.finp3;
                IMap p3 = S.G[i].Xform.fp3;
                IMap gg = S.G[i].Xform.gg1;
                double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
                IMatrix J = computeJacobian(gg, p3(S.F[i]));
                IVector coeffs = J * gg(p3(S.F[i]));
                capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

                interval M1 = euclNorm(coeffs); // 计算 2-范数
                // cout << "M1" << M1 << endl;

                double MM = M1.rightBound() / 2;
                if (MM == 0.0 || mu == 0.0) {

                }
                else {
                    double htmp = eulerstep(H, MM, mu, delta1);
                    //cout << "htmp=" << htmp << endl;
                    S.G[i].heuler = htmp;

                }

            }
            else {
                Subroutine7(S, i, dim, f, SVar, debug);
                S.G[i].delta = S.G[i].delta / 2;
                double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
                IMap inp3 = S.G[i].Xform.finp3;
                IMap p3 = S.G[i].Xform.fp3;
                IMap gg = S.G[i].Xform.gg1;
                double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
                IMatrix J = computeJacobian(gg, p3(S.F[i]));
                IVector coeffs = J * gg(p3(S.F[i]));
                capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

                interval M1 = euclNorm(coeffs); // 计算 2-范数
                // cout << "M1" << M1 << endl;

                double MM = M1.rightBound() / 2;
                if (MM == 0.0 || mu == 0.0) {

                }
                else {
                    double htmp = eulerstep(H, MM, mu, delta1);
                    // cout << "htmp=" << htmp << endl;
                    S.G[i].heuler = std::min(htmp, S.G[i].heuler);

                }
            }







        }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }

}







void Subroutine7notransform(Stage& S, int idx, int dim, IMap f, std::vector<std::string> SVar, int debug)
{
    if (idx > 0) {
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }

    double delta = S.G[idx].delta;
    std::vector<double> tmp = center(S.G[idx].bfE[0]);
    IVector p(dim);  // 
    for (int j = 0; j < dim; ++j) {
        p[j] = interval(tmp[j], tmp[j]);
    }

    double r0 = wmax(S.G[idx].bfE[0]) / 2;
    IMap p3(Convert_to_IMap(SVar, SVar), 2.0);
    IMap inp3(Convert_to_IMap(SVar, SVar), 2.0);
    IMap gg = f;
    IVector q = p3(p);
    IVector piE0 = p3(S.G[idx].bfE[0]);
    double pr0 = wmax(piE0) / 2;
    tmp = center(piE0);
    IVector cpiE0(dim);  //

    for (int j = 0; j < dim; ++j) {
        cpiE0[j] = interval(tmp[j], tmp[j]);
    }
    IVector Itmp;
    Itmp = q - cpiE0;
    double d = 0;
    for (int j = 0; j < dim; ++j) {
        d = d + pow((Itmp[j].rightBound() - Itmp[j].leftBound()), 2);
    }
    d = sqrt(d);
    int N = std::pow(2, S.G[idx].ell);
    double H = (S.T[idx] - S.T[idx - 1]) / N;
    IVector IF = S.G[idx].bfF[1];
    for (int j = 1; j < N + 1; ++j) {
        q = q + gg(q) * H;
        double delta1 = TransformBound(inp3, delta, S.G[idx].bfF[j]);

        IMatrix J = computeJacobian(gg, p3(S.G[idx].bfF[j]));
        double mu2;
        if (dim == 2) {
            mu2 = computemu(J);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J);
            mu2 = Imu.rightBound();
        }
        S.G[idx].Xform.mu2[j] = mu2;
        double r1 = r0 * exp(j * S.G[idx].mu1[j] * H) + delta;
        double r2 = (pr0 + d) * exp(j * mu2 * H) + delta1;
        IVector B(dim);  IVector B1(dim);
        for (int k = 0; k < dim; ++k) {
            B[k] = interval(-r1, r1);
            B1[k] = interval(-r2, r2);
        }
        tmp = center(p3(S.G[idx].bfE[j - 1]));
        for (int k = 0; k < dim; ++k) {
            Itmp[k] = interval(tmp[k], tmp[k]);
        }
        Itmp = Box(Itmp + B1, q + B1);
        Itmp = inp3(Itmp);
        S.G[idx].bfF[j] = IntersectB(Itmp,S.G[idx].bfF[j]);
        IF = Box(IF, S.G[idx].bfF[j]);
        J = computeJacobian(f, S.G[idx].bfF[j]);

        if (dim == 2) {
            mu2 = computemu(J);
        }
        else {
            capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
            interval Imu = euclLogNorm(J);
            mu2 = Imu.rightBound();
        }
        S.G[idx].mu1[j] = mu2;
        Itmp = IntersectB(inp3(q + B1),S.G[idx].bfE[j]);
        S.G[idx].bfE[j] = IntersectB(inp3(q) + B, Itmp);

    }
    S.E[idx] = S.G[idx].bfE.back();
    S.F[idx] = IF;





}



void Refinenewcp(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<double> target) {
    double r0 = wmax(S.E.back());
    while (veps < r0) {



        IVector E0(dim);
        for (int i = 0; i < dim; ++i) {
            double left = S.E[0][i].leftBound();
            double right = S.E[0][i].rightBound();
            double width = right - left;

            // 朝着 target[i] 收缩
            double newLeft = left + (target[i] - left) * 0.5;
            double newRight = right - (right - target[i]) * 0.5;



            E0[i] = interval(newLeft, newRight);
        }

        S.E[0] = E0;
        S.F[0] = E0;
        S.G[0].bfE[0] = E0;
        S.G[0].bfF[0] = E0;

        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        for (int i = 1; i < numofstage; ++i) {

            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;
            if (h > S.G[i].heuler) {

                Bisect(S, i, dim, f, stepBtype, degree, debug);
                double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
                IMap inp3 = S.G[i].Xform.finp3;
                IMap p3 = S.G[i].Xform.fp3;
                IMap gg = S.G[i].Xform.gg1;
                double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
                IMatrix J = computeJacobian(gg, p3(S.F[i]));
                IVector coeffs = J * gg(p3(S.F[i]));
                capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

                interval M1 = euclNorm(coeffs); // 计算 2-范数
                // cout << "M1" << M1 << endl;

                double MM = M1.rightBound() / 2;
                if (MM == 0.0 || mu == 0.0) {

                }
                else {
                    double htmp = eulerstep(H, MM, mu, delta1);
                    //cout << "htmp=" << htmp << endl;
                    S.G[i].heuler = htmp;

                }

            }
            else {
                Subroutine7(S, i, dim, f, SVar, debug);
                S.G[i].delta = S.G[i].delta / 2;
                double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
                IMap inp3 = S.G[i].Xform.finp3;
                IMap p3 = S.G[i].Xform.fp3;
                IMap gg = S.G[i].Xform.gg1;
                double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
                IMatrix J = computeJacobian(gg, p3(S.F[i]));
                IVector coeffs = J * gg(p3(S.F[i]));
                capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

                interval M1 = euclNorm(coeffs); // 计算 2-范数
                // cout << "M1" << M1 << endl;

                double MM = M1.rightBound() / 2;
                if (MM == 0.0 || mu == 0.0) {

                }
                else {
                    double htmp = eulerstep(H, MM, mu, delta1);
                    // cout << "htmp=" << htmp << endl;
                    S.G[i].heuler = std::min(htmp, S.G[i].heuler);

                }
            }







        }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }

}






void Refinenewsimple(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<double> target) {
    double r0 = wmax(S.E.back());
    while (veps < r0) {



        IVector E0(dim);
        for (int i = 0; i < dim; ++i) {
            double left = S.E[0][i].leftBound();
            double right = S.E[0][i].rightBound();
            double width = right - left;

            // 朝着 target[i] 收缩
            double newLeft = left + (target[i] - left) * 0.5;
            double newRight = right - (right - target[i]) * 0.5;



            E0[i] = interval(newLeft, newRight);
        }

        S.E[0] = E0;
        S.F[0] = E0;
        S.G[0].bfE[0] = E0;
        S.G[0].bfF[0] = E0;

        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        for (int i = 1; i < numofstage; ++i) {

            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;

            Subroutine7notransform(S, i, dim, f, SVar, debug);
            S.G[i].delta = S.G[i].delta / 2;
            double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());

            IMap gg = f;
            // double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
            IMatrix J = computeJacobian(gg, S.F[i]);
            IVector coeffs = J * gg(S.F[i]);
            capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

            interval M1 = euclNorm(coeffs); // 计算 2-范数
            // cout << "M1" << M1 << endl;

            double MM = M1.rightBound() / 2;
            if (MM == 0.0 || mu == 0.0) {

            }
            else {
                double htmp = eulerstep(H, MM, mu, S.G[i].delta);
                // cout << "htmp=" << htmp << endl;
                S.G[i].heuler = std::min(htmp, S.G[i].heuler);

            }








        }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }

}


void RefinenewsimpleT(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<double> target) {
    double r0 = wmax(S.E.back());
    while (veps < r0) {



        IVector E0(dim);
        for (int i = 0; i < dim; ++i) {
            double left = S.E[0][i].leftBound();
            double right = S.E[0][i].rightBound();
            double width = right - left;

            // 朝着 target[i] 收缩
            double newLeft = left + (target[i] - left) * 0.5;
            double newRight = right - (right - target[i]) * 0.5;



            E0[i] = interval(newLeft, newRight);
        }

        S.E[0] = E0;
        S.F[0] = E0;
        S.G[0].bfE[0] = E0;
        S.G[0].bfF[0] = E0;

        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        for (int i = 1; i < numofstage; ++i) {

            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;

            Subroutine7(S, i, dim, f, SVar, debug);
            S.G[i].delta = S.G[i].delta / 2;
            double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
            IMap inp3 = S.G[i].Xform.finp3;
            IMap p3 = S.G[i].Xform.fp3;
            IMap gg = S.G[i].Xform.gg1;
            double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
            IMatrix J = computeJacobian(gg, p3(S.F[i]));
            IVector coeffs = J * gg(p3(S.F[i]));
            capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

            interval M1 = euclNorm(coeffs); // 计算 2-范数
            // cout << "M1" << M1 << endl;

            double MM = M1.rightBound() / 2;
            if (MM == 0.0 || mu == 0.0) {

            }
            else {
                double htmp = eulerstep(H, MM, mu, delta1);
                // cout << "htmp=" << htmp << endl;
                S.G[i].heuler = std::min(htmp, S.G[i].heuler);

            }








        }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }

}



void Refinenewnotransform(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<double> target) {
    double r0 = wmax(S.E.back());
    while (veps < r0) {



        IVector E0(dim);
        for (int i = 0; i < dim; ++i) {
            double left = S.E[0][i].leftBound();
            double right = S.E[0][i].rightBound();
            double width = right - left;

            // 朝着 target[i] 收缩
            double newLeft = left + (target[i] - left) * 0.5;
            double newRight = right - (right - target[i]) * 0.5;



            E0[i] = interval(newLeft, newRight);
        }

        S.E[0] = E0;
        S.F[0] = E0;
        S.G[0].bfE[0] = E0;
        S.G[0].bfF[0] = E0;

        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        for (int i = 1; i < numofstage; ++i) {

            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;
            if (h > S.G[i].heuler) {

                Bisect(S, i, dim, f, stepBtype, degree, debug);
                double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
                //  IMap inp3 = S.G[i].Xform.finp3;
                //  IMap p3 = S.G[i].Xform.fp3;
                IMap gg = f;
                //  double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
                IMatrix J = computeJacobian(gg, S.F[i]);
                IVector coeffs = J * gg(S.F[i]);
                capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

                interval M1 = euclNorm(coeffs); // 计算 2-范数
                // cout << "M1" << M1 << endl;

                double MM = M1.rightBound() / 2;
                if (MM == 0.0 || mu == 0.0) {

                }
                else {
                    double htmp = eulerstep(H, MM, mu, S.G[i].delta);
                    //cout << "htmp=" << htmp << endl;
                    S.G[i].heuler = htmp;

                }

            }
            else {
                Subroutine7notransform(S, i, dim, f, SVar, debug);
                S.G[i].delta = S.G[i].delta / 2;
                double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());

                IMap gg = f;
                // double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
                IMatrix J = computeJacobian(gg, S.F[i]);
                IVector coeffs = J * gg(S.F[i]);
                capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

                interval M1 = euclNorm(coeffs); // 计算 2-范数
                // cout << "M1" << M1 << endl;

                double MM = M1.rightBound() / 2;
                if (MM == 0.0 || mu == 0.0) {

                }
                else {
                    double htmp = eulerstep(H, MM, mu, S.G[i].delta);
                    // cout << "htmp=" << htmp << endl;
                    S.G[i].heuler = std::min(htmp, S.G[i].heuler);

                }

            }







        }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }

}



void Refinenewnoeuler(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<double> target) {
    double r0 = wmax(S.E.back());
    double mmu = -1000;
    for (int i = 0; i < S.G.size(); ++i) {
        for (int j = 0; j < S.G[i].mu1.size(); ++j) {
            if(S.G[i].mu1[j]<100){
            mmu = std::max(mmu, S.G[i].mu1[j]);}
        }
    }
  //  cout << "mmu=" << mmu << endl;
    while (veps < r0) {
       

        double TmpR = exp((S.T.back() - S.T[0]) * mmu) * wmax(S.E[0]);
      //  cout << "TmpR=" << TmpR << endl;
        IVector E0(dim);
        if (TmpR > veps / 2) {
            for (int i = 0; i < dim; ++i) {
                double left = S.E[0][i].leftBound();
                double right = S.E[0][i].rightBound();
                double width = right - left;

                // 朝着 target[i] 收缩
                double newLeft = left + (target[i] - left) * 0.5;
                double newRight = right - (right - target[i]) * 0.5;



                E0[i] = interval(newLeft, newRight);
            }

            S.E[0] = E0;
            S.F[0] = E0;
            S.G[0].bfE[0] = E0;
            S.G[0].bfF[0] = E0;
        }
        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        for (int i = 1; i < numofstage; ++i) {

            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;


            Bisect(S, i, dim, f, stepBtype, degree, debug);
            double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
            IMap inp3 = S.G[i].Xform.finp3;
            IMap p3 = S.G[i].Xform.fp3;
            IMap gg = S.G[i].Xform.gg1;
            double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
            IMatrix J = computeJacobian(gg, p3(S.F[i]));
            IVector coeffs = J * gg(p3(S.F[i]));
            capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

            interval M1 = euclNorm(coeffs); // 计算 2-范数
            // cout << "M1" << M1 << endl;

            double MM = M1.rightBound() / 2;
            if (MM == 0.0 || mu == 0.0) {

            }
            else {
                double htmp = eulerstep(H, MM, mu, delta1);
                //cout << "htmp=" << htmp << endl;
                S.G[i].heuler = htmp;

            }










        }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }

}




void Refinenewnoeulerplus(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<double> target,double H) {
    S.E.back() = stepBcrlohner(f, degree, H, S.E[0]);
    double r0 = wmax(S.E.back());
    IVector E0(dim);
    //  cout << "mmu=" << mmu << endl;
    while (veps < r0) {

       
       
       
       
            for (int i = 0; i < dim; ++i) {
                double left = S.E[0][i].leftBound();
                double right = S.E[0][i].rightBound();
                double width = right - left;

                // 朝着 target[i] 收缩
                double newLeft = left + (target[i] - left) * 0.5;
                double newRight = right - (right - target[i]) * 0.5;



                E0[i] = interval(newLeft, newRight);
            }

            S.E[0] = E0;
            S.F[0] = E0;
            S.G[0].bfE[0] = E0;
            S.G[0].bfF[0] = E0;
        
        
        S.E.back() = stepBcrlohner(f, degree,H, S.E[0]);
        


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
    }
    S.T.back() = H;
}










void Refinenewnoeuler3D(Stage& S, std::vector<std::string> SVar, int dim, IMap f, int stepBtype, int degree, double veps, int debug, std::vector<Stage>& target) {
    double r0 = wmax(S.E.back());
    double mmu = -1000;
    for (int i = 0; i < S.G.size(); ++i) {
        for (int j = 0; j < S.G[i].mu1.size(); ++j) {
            if (S.G[i].mu1[j] < 100) {
                mmu = std::max(mmu, S.G[i].mu1[j]);
            }
        }
    }
    int couter=0;
    //  cout << "mmu=" << mmu << endl;
    while (veps < r0) {


        double TmpR = exp((S.T.back() - S.T[0]) * mmu) * wmax(S.E[0]);
        //  cout << "TmpR=" << TmpR << endl;
        IVector E0(dim);
        if (couter>0 ) {
            // 获取盒子的维数
           
            int n = S.E[0].dimension();
            IVector OrignE = S.E[0];
            // 计算每个维度需要分割的中点
            std::vector<double> midpoints(n);
            for (int i = 0; i < n; ++i) {
                midpoints[i] = (OrignE[i].leftBound() + OrignE[i].rightBound()) / 2.0;
            }

            // 保留的盒子E0（我们选择mask=0对应的盒子作为保留的盒子）
           IVector E0(n);
            for (int dim1 = 0; dim1 < n; ++dim1) {
                E0[dim1] = capd::interval(OrignE[dim1].leftBound(), midpoints[dim1]);
            }

            // 生成所有可能的子盒子索引组合（2^n个），跳过第一个
            
            for (int mask = 1; mask < (1 << n); ++mask) {  // 从1开始，跳过0
                // 创建新的盒子
               
                IVector newE(n);

                // 根据mask的每一位决定每个维度取左半还是右半
                for (int dim1 = 0; dim1 < n; ++dim1) {
                    if (mask & (1 << dim1)) {
                        // 该位为1，取右半部分 [mid, right]
                        newE[dim1] = capd::interval(midpoints[dim1], OrignE[dim1].rightBound());
                    }
                    else {
                        // 该位为0，取左半部分 [left, mid]
                        newE[dim1] = capd::interval(OrignE[dim1].leftBound(), midpoints[dim1]);
                    }
                }

                S.E[0] = newE;
                S.F[0] = newE;
                S.G[0].bfE[0] = newE;
                S.G[0].bfF[0] = newE;
               
              
                target.push_back(S);
            }

            // 用保留的盒子E0更新S的各个字段
            S.E[0] = E0;
            S.F[0] = E0;
            S.G[0].bfE[0] = E0;
            S.G[0].bfF[0] = E0;
        }
        // cout << "E0" << E0 << endl;
        int numofstage = S.T.size();
        // cout << "T=" << S.T << endl;
       //  cout << numofstage << endl;
        //for (int i = 1; i < numofstage; ++i) {

           // int N = std::pow(2, S.G[i].ell);
           // double H = S.T[i] - S.T[i - 1];
           // double h = H / N;
            //cout << "h=" << h << endl;
            //cout << "S.G[i].heuler=" << S.G[i].heuler << endl;


            //Bisect(S, i, dim, f, stepBtype, degree, debug);
            /*
            double mu = *std::max_element(S.G[i].Xform.mu2.begin(), S.G[i].Xform.mu2.end());
            IMap inp3 = S.G[i].Xform.finp3;
            IMap p3 = S.G[i].Xform.fp3;
            IMap gg = S.G[i].Xform.gg1;
            double delta1 = TransformBound(inp3, S.G[i].delta, S.F[i]);
            IMatrix J = computeJacobian(gg, p3(S.F[i]));
            IVector coeffs = J * gg(p3(S.F[i]));
            capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;

            interval M1 = euclNorm(coeffs); // 计算 2-范数
            // cout << "M1" << M1 << endl;

            double MM = M1.rightBound() / 2;
            if (MM == 0.0 || mu == 0.0) {

            }
            else {
                double htmp = eulerstep(H, MM, mu, delta1);
                //cout << "htmp=" << htmp << endl;
                S.G[i].heuler = htmp;

            }


            */



            S.E[1] = stepBcrlohner(f, degree, S.T.back(), E0);



       // }


        r0 = wmax(S.E.back());
        // cout << "r0=" << r0 << endl;
        couter = couter + 1;
    }

}

