#pragma once

// ===========================================================================
//  Extend-new.h  --  the Extend subroutine (paper Section 4, the S.Extend
//  pseudocode).
//
//   S.Extend(eps, H): append one stage to the m-stage scaffold S, producing an
//   (m+1)-stage scaffold whose new terminal time is <= H and whose new stage
//   (E_m, dt_{m+1}, F_{m+1}, E_{m+1}) is an eps-admissible quad.  Concretely:
//
//       (h, F1) <- StepA(S.E.back(), H - S.t.back(), eps)   // admissible triple
//       E1      <- StepB(S.E.back(), h, F1)                 // -> admissible quad
//       initialise mini-scaffold G_{m+1} with mu <- mu_2(Jf(F1)) and h^euler.
//
//   Under the "refine-before-extend" principle (paper Section 3.2), Extend is
//   only called when the scaffold is already eps-small.
// ===========================================================================

#include <iostream>
#include "capd/capdlib.h"
#include "Refine-new.h"

using namespace capd;
using namespace std;

void Extendnew(IMap F, Stage& S, double veps, double delta, int degree,
               double H, int stepBtype, int stepAtype, int debuglevel) {
    int m = S.T.size();
    IVector B0 = S.E[m - 1];
    S.G.push_back(S.G[0]);

    // StepA: admissible (h, F1) for the current end-enclosure E0 = B0.
    std::pair<double, IVector> result1 =
        (stepAtype == 0) ? stepA(F, B0, H, veps, degree)
                         : stepA0(F, B0, H, veps, degree);
    double  h  = result1.first;
    IVector B1 = result1.second;

    if (debuglevel == 1) {
        for (int i = 0; i < B0.dimension(); ++i)
            if (B1[i].leftBound() > B0[i].leftBound() ||
                B1[i].rightBound() < B0[i].rightBound())
                std::cout << "Extend: E0 not contained in F1 at dim " << i
                          << " (t=" << S.T.back() << ")" << std::endl;
    }

    S.T.push_back(S.T[m - 1] + h);
    S.F.push_back(B1);
    S.G[m].bfF = { B1, B1 };
    S.G[m].ell = 0;
    S.G[m].delta = delta;

    // logNorm-bound  mu <- mu_2(Jf(F1))  for the new mini-scaffold ([G4]).
    double mu = logNorm(computeJacobian(F, B1));
    S.G[m].mu1 = { mu, mu };
    S.G[m].mu2 = { mu, mu };
    S.G[m].heuler = eulerstep(h, secondTaylorNorm(F, B1), mu, S.G[m].delta);

    // StepB: end-enclosure E1 of End(E0, h).
    IVector E1;
    switch (localStepBType(stepBtype)) {
    case 0: E1 = stepBcrlohner(F, degree, h, B0); break;
    case 1: E1 = stepBcrlohnerwithmu(F, degree, h, B0, B1, mu); break;
    case 2: E1 = directmethodwithmu(F, degree, h, B0, B1, mu); break;
    case 3: E1 = puredirectmethod(F, degree, h, B0); break;
    default: E1 = stepBcrlohner(F, degree, h, B0); break;
    }

    if (debuglevel == 1) {
        for (int i = 0; i < B0.dimension(); ++i)
            if (B1[i].leftBound() > E1[i].leftBound() ||
                B1[i].rightBound() < E1[i].rightBound())
                std::cout << "Extend: E1 not contained in F1 at dim " << i
                          << " (t=" << S.T.back() << ")" << std::endl;
    }

    S.E.push_back(E1);
    S.G[m].bfE = { B0, E1 };
}
