#pragma once

// ===========================================================================
//  stepAB-new.h  --  StepA and StepB (paper Section 4).
//
//  The A/B-iteration (paper Section 3.1) is the core motif of validated IVP:
//
//     StepA(E0, H, eps) -> (h, F1)   : choose a step h <= H and a candidate
//                                      full-enclosure F1 so that the triple
//                                      (E0, h, F1) is "eps-admissible"
//                                      (paper Section 4.1, Lemma 4.1).
//
//     StepB(E0, h, F1)  -> E1        : upgrade the admissible triple to an
//                                      admissible quad (E0, h, F1, E1) by
//                                      computing an end-enclosure E1 of
//                                      End(E0, h)  (paper Section 4.2).
//
//  A single A/B step is therefore   E0 --StepA--> (E0,h,F1) --StepB--> E1.
//
//  StepB here is realised with CAPD's C^r-Lohner solver (stepBcrlohner).  The
//  logNorm-tightened variants combine the Direct method (eq. (4.4)) with the
//  Fundamental Inequality ball of Lemma 2.1, and are selected by `stepBtype`.
// ===========================================================================

#include <iostream>
#include <cstdlib>
#include "capd/capdlib.h"
#include "calD-calQ-new.h"

using namespace capd;
using namespace std;

// Taylor order k of the method (paper Section 2.2 assumes k = 20, as in CAPD).
extern int MAX_TAYLOR_ORDER;

// Public StepB choices used by the CLI.
// Type 4 keeps the SIAM scaffold, but uses the stronger local logNorm StepB
// and endpoint-tightened splitting.
// Type 5 is an endpoint-refine path: the whole-horizon validated Lohner image
// drives acceptance / splitting directly.
// Type 6 first runs the SIAM scaffold, then applies the endpoint refine as a
// verified completion/splitting oracle.
static int localStepBType(int stepBtype) {
    if (stepBtype == 4 || stepBtype == 6) return 1;
    if (stepBtype == 5) return 0;
    return stepBtype;
}

static bool useEndpointLohnerTightening(int stepBtype) {
    return stepBtype == 4 || stepBtype == 6;
}

static bool useBoundaryEndpointRefine(int stepBtype) {
    return stepBtype == 5;
}

static bool useHybridEndpointRefine(int stepBtype) {
    return stepBtype == 6;
}

// StepB (type 0), plain C^r-Lohner (paper Section 4.2, "C^r-Lohner method"):
// CAPD's validated doubleton-set time-h map gives an end-enclosure E1 of
// End(E0, h).  This is the default StepB and the rigorous core of the method.
IVector stepBcrlohner(IMap f, int degree, double timeStep, IVector initialCondition) {
    ICnOdeSolver solver(f, degree);
    ICnTimeMap timeMap(solver);
    CnRect2Set s(initialCondition, 3.0);
    interval T(timeStep);
    return timeMap(T, s);
}

IVector centerTaylorImage(IMap f, int degree, double timeStep, IVector initialCondition) {
    int n = initialCondition.dimension();
    IVector point(n);
    std::vector<double> mid = center(initialCondition);
    for (int i = 0; i < n; ++i) point[i] = interval(mid[i], mid[i]);

    ICnOdeSolver solver(f, degree);
    solver.computeCoefficients(point, degree);

    IVector image(n);
    interval h(timeStep);
    for (int i = 0; i < n; ++i) {
        image[i] = solver.coefficient(i, degree - 1);
        for (int j = degree - 2; j >= 1; --j)
            image[i] = image[i] * h + solver.coefficient(i, j);
        image[i] = image[i] * h + point[i];
    }
    return image;
}

IVector taylorRemainderOnFullEnclosure(IMap f, int degree, double timeStep,
                                       IVector fullEnclosure) {
    int n = fullEnclosure.dimension();
    ICnOdeSolver solver(f, degree);
    solver.computeCoefficients(fullEnclosure, degree);

    IVector remainder(n);
    interval timeFactor(0, pow(timeStep, degree));
    for (int i = 0; i < n; ++i)
        remainder[i] = timeFactor * solver.coefficient(i, degree);
    return remainder;
}

IVector fundamentalInequalityBox(IMap f, int degree, double timeStep,
                                 IVector initialCondition,
                                 IVector fullEnclosure, double mu) {
    IVector q0 = centerTaylorImage(f, degree, timeStep, initialCondition);
    IVector remainder = taylorRemainderOnFullEnclosure(f, degree, timeStep, fullEnclosure);
    double r = euclideanRadius(initialCondition) * exp(mu * timeStep);

    IVector result(q0.dimension());
    for (int i = 0; i < q0.dimension(); ++i)
        result[i] = q0[i] + interval(-r, r) + remainder[i];
    return result;
}

// StepB (type 1), C^r-Lohner tightened by the Fundamental Inequality
// (paper Lemma 2.1 / the StepB pseudocode in Section 4.2): intersect the
// Lohner end-enclosure with the ball Ball_{q0}(r0) + h^k f^[k](F1), where
// q0 is the Taylor image of m(E0), r0 encloses E0 around m(E0), and mu is a
// logNorm-bound on F1.
IVector stepBcrlohnerwithmu(IMap f, int degree, double timeStep,
                            IVector initialCondition, IVector fullEnclosure,
                            double mu) {
    ICnOdeSolver solver(f, degree);
    ICnTimeMap timeMap(solver);
    CnRect2Set s(initialCondition, 3.0);
    interval T(timeStep);
    IVector result = timeMap(T, s);

    IVector result1 = fundamentalInequalityBox(f, degree, timeStep,
                                               initialCondition, fullEnclosure, mu);
    return IntersectB(result1, result);
}

// StepB (type 2), the Direct method of Nedialkov (paper eq. (4.4)) tightened
// by the logNorm ball of Lemma 2.1.  This is the paper's recommended StepB.
IVector directmethodwithmu(IMap f, int degree, double timeStep,
                           IVector initialCondition, IVector fullEnclosure,
                           double mu) {
    IOdeSolver solver(f, degree);
    solver.setStep(timeStep);
    ITimeMap timeMap(solver);
    C0HORect2Set s(initialCondition);
    interval T(timeStep);
    IVector result = timeMap(T, s);

    IVector result1 = fundamentalInequalityBox(f, degree, timeStep,
                                               initialCondition, fullEnclosure, mu);
    return IntersectB(result1, result);
}

// StepB (type 3), the plain Direct method (paper eq. (4.4)) without the
// logNorm tightening of Lemma 2.1.
IVector puredirectmethod(IMap f, int degree, double timeStep, IVector initialCondition) {
    IOdeSolver solver(f, degree);
    solver.setStep(timeStep);
    ITimeMap timeMap(solver);
    C0HORect2Set s(initialCondition);
    interval T(timeStep);
    return timeMap(T, s);
}

// StepA (type 0), adaptive  (paper Section 4.1, the StepA pseudocode).
// It repeatedly halves the trial horizon H, recomputing the candidate
// full-enclosure F1 = sum_{i<k} [0,H]^i f^[i](E0) + Box(eps) and the step
// h = min(H, min_i (eps_i / M_i)^{1/k}), until (E0, h, F1) is eps-admissible
// while keeping h as large as possible (Lemma 4.1; complexity Lemma 4.2).
// Here the StepA Taylor order is fixed to 5 (the f^[i] are normalized Taylor
// coefficients, eq. (2.1)).
std::pair<double, IVector> stepA(IMap f, IVector B0, double H1, double epsilon,
                                 int degree1 = MAX_TAYLOR_ORDER) {
    int degree = 5;
    int n = B0.dimension();
    ICnOdeSolver solver(f, degree);
    solver.computeCoefficients(B0, degree);
    double H = H1;
    double h = 0.0;
    IVector B0_bar(n);
    while (h < H / 2) {
        IVector coeffs(n);
        for (int i = 0; i < n; ++i) {
            coeffs[i] = solver.coefficient(i, degree - 1);
            for (int j = degree - 2; j >= 1; --j)
                coeffs[i] = coeffs[i] * interval(0, H) + solver.coefficient(i, j);
            coeffs[i] = coeffs[i] * interval(0, H) + B0[i];
        }
        IVector epsilonVector(n);
        for (int i = 0; i < n; ++i) epsilonVector[i] = interval(-epsilon, epsilon);
        B0_bar = coeffs + epsilonVector;

        ICnOdeSolver solver1(f, degree);
        solver1.computeCoefficients(B0_bar, degree);
        IVector B0bar_derivative_k(n);
        for (int i = 0; i < n; ++i)
            B0bar_derivative_k[i] = solver1.coefficient(i, degree);

        capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;
        double M = sup(euclNorm(B0bar_derivative_k));
        h = pow((epsilon / M), (1.0 / degree));
        if (h > H) { h = H; break; }
        H = H / 2;
    }
    return std::make_pair(h, B0_bar);
}

// StepA (type 1), non-adaptive: a single evaluation of the admissible triple
// (E0, h, B(E0,h,eps)) at the full horizon (paper Lemma 4.1, without the
// adaptive horizon-halving loop).  Faster but may return a smaller h.
std::pair<double, IVector> stepA0(IMap f, IVector B0, double H1, double epsilon,
                                  int degree1 = MAX_TAYLOR_ORDER) {
    int degree = 5;
    int n = B0.dimension();
    ICnOdeSolver solver(f, degree);
    solver.computeCoefficients(B0, degree);
    double h = 0.0;
    IVector B0_bar(n);

    IVector coeffs(n);
    for (int i = 0; i < n; ++i) {
        coeffs[i] = solver.coefficient(i, degree - 1);
        for (int j = degree - 2; j >= 1; --j)
            coeffs[i] = coeffs[i] * interval(0, H1) + solver.coefficient(i, j);
        coeffs[i] = coeffs[i] * interval(0, H1) + B0[i];
    }
    IVector epsilonVector(n);
    for (int i = 0; i < n; ++i) epsilonVector[i] = interval(-epsilon, epsilon);
    B0_bar = coeffs + epsilonVector;

    ICnOdeSolver solver1(f, degree);
    solver1.computeCoefficients(B0_bar, degree);
    IVector B0bar_derivative_k(n);
    for (int i = 0; i < n; ++i)
        B0bar_derivative_k[i] = solver1.coefficient(i, degree);

    capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;
    double M = sup(euclNorm(B0bar_derivative_k));
    h = pow((epsilon / M), (1.0 / degree));
    if (h > H1) h = H1;
    return std::make_pair(h, B0_bar);
}
