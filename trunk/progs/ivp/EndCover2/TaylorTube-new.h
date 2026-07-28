#pragma once

// ===========================================================================
//  TaylorTube-new.h  --  the Taylor Tube refine subroutine (tube degree p).
//  ---------------------------------------------------------------------------
//  The Taylor Tube of degree p >= 1 refines the end- and full-enclosures of a
//  scaffold stage.  It is the degree-p generalisation of the Euler tube (the
//  case p = 1): larger p gives tighter enclosures, and taking p = k-1 (one
//  below the Taylor order k used by StepB) lets the tube reuse the Taylor
//  coefficients StepB already computes -- which is why tubeDegree defaults to
//  order - 1.
//
//  Taylor curve.  The degree-p Taylor polynomial of f at q is
//        T^p_q(t) = sum_{i=0}^{p} t^i f^[i](q),
//  where the f^[i] are the normalised Taylor differential coefficients
//  (f^[0] = x, f^[i] = (1/i)(J_{f^[i-1]} . f)).  Marching the centre
//  q0 = m(E0) with mini-step h gives the Taylor sequence q_{i+1} = T^p_{q_i}(h)
//  and the piecewise Taylor curve T^p_{q0,h}(t).
//
//  Enclosures.  Given an admissible triple (E0, h, F1) with E0 = Ball_{q0}(r0),
//  a logNorm-bound mu on F1 and M >= ||f^[p+1](F1)||, and a mini-step h within
//  the Taylor-tube step bound h^taylor_p (see hTaylorStep below), the true
//  solution is enclosed by:
//     (a) end-enclosure  :  Ball_{ T^p_{q0,h}(h) } ( r0 e^{mu h} + delta ),
//     (b) full-enclosure :  T^p_{q0,h}([0,h])  (+)  [-r, r]^n,
//                           r = delta + max{ r0, r0 e^{mu h} }.
//  Both rest on the logNorm Fundamental IVP Inequality
//        || x1(t) - x2(t) ||_2  <=  || x1(0) - x2(0) ||_2 e^{mu t},
//  which bounds the spread of trajectories around the centre by r0 e^{mu t}.
//
//  Implementation (per mini-step [t_{j-1}, t_j]).  We split the step like the
//  Direct method  T^p(h) + (Point Error) + (Range Enclosure):
//     centreEnd  = T^p_{q0}(h)                    -- centre at the end time h,
//     centreTube = T^p_{q0,h}([0,h])              -- centre over [0, h],
//     pointError = [0,h]^{p+1} f^[p+1](F_j)       -- rigorous Taylor remainder,
//     rEnd = r0 e^{mu h},  rFull = max{r0, r0 e^{mu h}}   -- the spread ball,
//  and form  centre (+) pointError (+) (spread-ball ∩ old-enclosure residual).
//  Here the rigorous remainder plays the role of the tube tolerance delta, and
//  r0 is the *Euclidean* radius of E0 so the spread ball is an over-estimate.
//  Every result is intersected back into the stage's existing validated
//  enclosure, so the tube can only shrink it -- the output stays rigorous.
// ===========================================================================

#include <algorithm>
#include <cmath>
#include <vector>

#include "capd/capdlib.h"
#include "stepAB-new.h"   // taylorRemainderOnFullEnclosure, IntersectB, Box, ...

// Upper bound of the Taylor coefficient norm ||f^[p+1](B)|| in the normalised
// (differential-coefficient) scale, i.e. WITHOUT the (p+1)! factor, so that
// hTaylorStep below can use the analytically-cancelled formulas of eq. (7) and
// avoid factorial overflow.  This is the M >= ||f^[p+1](B1)|| of Lemma 2.
static double taylorCoeffNormUpperBound(IMap f, const IVector& B, int p) {
    const int n = B.dimension();
    ICnOdeSolver solver(f, p + 1);
    solver.computeCoefficients(B, p + 1);
    IVector coeffs(n);
    for (int i = 0; i < n; ++i) coeffs[i] = solver.coefficient(i, p + 1);
    capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;
    return std::max(0.0, euclNorm(coeffs).rightBound());
}

// Taylor Tube Step Size Bound h^taylor_p (Lemma 2, eq. (7)): the largest step
// h <= H for which the degree-p Taylor curve is guaranteed to stay inside the
// delta-tube of the true trajectory.  Three cases on the sign of the logNorm
// mu; Cbar is the normalised coefficient bound above (the (p+1)! cancels).
// The Euler tube is the special case p = 1.
static double hTaylorStep(double H, double Cbar, double muBar, double eps, int p) {
    if (H <= 0.0 || eps <= 0.0 || p <= 0) return 0.0;
    if (Cbar <= 0.0) return H;

    double candidate = H;
    const double tiny = 1e-12;
    if (std::abs(muBar) < tiny) {
        const double ratio = eps / (Cbar * H);
        if (ratio > 0.0) candidate = std::pow(ratio, 1.0 / static_cast<double>(p));
    } else if (muBar >= 0.0) {
        const double den = Cbar * (std::exp(muBar * H) - 1.0);
        const double ratio = (den > 0.0) ? (muBar * eps / den) : -1.0;
        if (ratio > 0.0) candidate = std::pow(ratio, 1.0 / static_cast<double>(p));
    } else {
        const double den = Cbar * (std::exp(muBar * H) - 1.0) - (muBar * muBar) * eps;
        const double ratio = (std::abs(den) > tiny) ? (2.0 * muBar * eps / den) : -1.0;
        if (ratio > 0.0) candidate = std::pow(ratio, 1.0 / static_cast<double>(p));
    }
    if (!std::isfinite(candidate) || candidate <= 0.0) return H;
    return std::min(H, candidate);
}

// f(B) has no component that straddles 0 => each coordinate is monotone in
// time on B, so Box(start, end) already encloses the sub-trajectory and we can
// skip the (wider) polynomial full-enclosure branch.
static bool taylorTubeAvoidsZero(IMap f, const IVector& B) {
    IVector v = f(B);
    for (int i = 0; i < v.dimension(); ++i)
        if (v[i].leftBound() <= 0.0 && v[i].rightBound() >= 0.0) return false;
    return true;
}

// The centred box [-r, r]^dim.
static IVector taylorTubeSymBox(int dim, double r) {
    IVector B(dim);
    for (int i = 0; i < dim; ++i) B[i] = interval(-r, r);
    return B;
}

// One Taylor-tube pass over stage `idx`: refine every mini-step's end- and
// full-enclosure with the degree-`tubeDegree` Taylor Tube (Theorem 1).  The
// Euler tube is recovered at tubeDegree = 1.
void TaylorTube(Stage& S, int idx, int dim, IMap f, int tubeDegree, int debug) {
    if (idx > 0) {                       // chain the stage onto its predecessor
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }
    tubeDegree = std::max(1, tubeDegree);

    int N = std::pow(2, S.G[idx].ell);            // number of mini-steps
    double H = (S.T[idx] - S.T[idx - 1]) / N;     // mini-step size
    const IVector stageE_old = S.E[idx];
    const IVector stageF_old = S.F[idx];
    IVector IF = S.G[idx].bfF[0];

    const interval iH(0.0, H);       // time interval for the "tube" polynomial
    const interval HFixed(H, H);     // end time for the "end" polynomial

    for (int j = 1; j < N + 1; ++j) {
        const IVector curE  = S.G[idx].bfE[j - 1];
        const IVector oldEj = S.G[idx].bfE[j];
        const IVector oldFj = S.G[idx].bfF[j];

        // Centre point p0 = m(curE) as a degenerate box.
        std::vector<double> mid = center(curE);
        IVector p0(dim);
        for (int k = 0; k < dim; ++k) p0[k] = interval(mid[k], mid[k]);

        // Taylor coefficients of the centre trajectory at p0 (order tubeDegree).
        ICnOdeSolver pointSolver(f, tubeDegree);
        pointSolver.computeCoefficients(p0, tubeDegree);

        // Horner evaluation: centreEnd at fixed time H, centreTube over [0, H].
        IVector centerEnd(dim), centerTube(dim);
        for (int k = 0; k < dim; ++k) {
            interval polyTube(0.0, 0.0), polyEnd(0.0, 0.0);
            for (int d = tubeDegree; d >= 1; --d) {
                polyTube = polyTube * iH     + pointSolver.coefficient(k, d);
                polyEnd  = polyEnd  * HFixed + pointSolver.coefficient(k, d);
            }
            centerTube[k] = polyTube * iH     + p0[k];
            centerEnd[k]  = polyEnd  * HFixed + p0[k];
        }

        // Point Error term (eq. (11)): the rigorous Taylor remainder, the
        // order-(tubeDegree+1) coefficient over the validated full enclosure oldFj.
        IVector pointError = taylorRemainderOnFullEnclosure(f, tubeDegree + 1, H, oldFj);

        // logNorm spread radius r0 * e^{mu H} (Fundamental IVP Inequality).
        // r0 is the *Euclidean* radius of curE around its centre p0 (mu is the
        // Euclidean logNorm), so |x_i(t) - c_i(t)| <= ||x(t)-c(t)||_2 <= r0 e^{mu t}
        // holds componentwise -- this is what makes the symmetric box below a
        // rigorous over-estimate of the spread (not an under-estimate).
        double muBar = logNorm(computeJacobian(f, oldFj));
        double r0 = euclideanRadius(curE);
        double rEnd  = r0 * std::exp(muBar * H);
        double rFull = (muBar >= 0.0) ? rEnd : r0;

        // End enclosure = centre(H) (+) remainder (+) spread, the spread capped
        // by the logNorm ball AND the residual of the old validated enclosure.
        IVector rangeEnd = IntersectB(oldEj - centerEnd - pointError,
                                      taylorTubeSymBox(dim, rEnd));
        IVector endBox = centerEnd + pointError + rangeEnd;

        // Full enclosure over the mini-step.
        IVector fullBox;
        if (taylorTubeAvoidsZero(f, oldFj)) {
            fullBox = Box(curE, endBox);
        } else {
            IVector rangeFull = IntersectB(oldFj - centerTube - pointError,
                                           taylorTubeSymBox(dim, rFull));
            fullBox = centerTube + pointError + rangeFull;
        }

        // Tighten by intersection (never enlarge) -> stays a rigorous enclosure.
        S.G[idx].bfF[j] = IntersectB(oldFj, fullBox);
        IF = Box(IF, S.G[idx].bfF[j]);

        // Refresh the logNorm arrays used by the h^euler update in Refine.
        if (static_cast<int>(S.G[idx].mu1.size()) <= j) S.G[idx].mu1.resize(j + 1, muBar);
        if (static_cast<int>(S.G[idx].mu2.size()) <= j) S.G[idx].mu2.resize(j + 1, muBar);
        double mu2 = logNorm(computeJacobian(f, S.G[idx].bfF[j]));
        S.G[idx].mu1[j] = std::min(S.G[idx].mu1[j], mu2);
        S.G[idx].mu2[j] = std::min(S.G[idx].mu2[j], mu2);

        S.G[idx].bfE[j] = IntersectB(oldEj, endBox);

        if (debug == 1)
            std::cout << "TaylorTube: stage " << idx << " j=" << j
                      << " (deg " << tubeDegree << ") wmax(end)="
                      << wmax(S.G[idx].bfE[j]) << std::endl;
    }

    // Keep the stage enclosures monotone (never wider than before this pass).
    S.E[idx] = IntersectB(stageE_old, S.G[idx].bfE.back());
    S.F[idx] = IntersectB(stageF_old, IF);
}
