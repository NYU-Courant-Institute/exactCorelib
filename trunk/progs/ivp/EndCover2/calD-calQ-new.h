#pragma once

// ===========================================================================
//  calD-calQ-new.h
//  ---------------------------------------------------------------------------
//  Basic data structures and interval tools for the End Cover algorithm of
//
//     B. Zhang and C. Yap, "End Cover for Initial Value Problem:
//     Complete Validated Algorithm with Complexity Analysis".
//
//  This file provides:
//    * the "scaffold" S = (t, E, F, G)        (paper Section 3.2),
//    * the per-stage "mini-scaffold" G[i]      (paper Section 5.1, [G1]-[G5]),
//    * box operators  Box(.), IntersectB(.)    (paper Section 2.1),
//    * the max-width  w_max(.)                 (paper Section 2.1),
//    * the logarithmic 2-norm  mu_2(.)         (paper Section 2.4), and
//    * the Euler-tube step threshold h^euler   (paper Lemma 2.2, eq. (2.6)).
// ===========================================================================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "capd/capdlib.h"
#include "capd/vectalg/Norm.hpp"

using namespace capd;
using namespace std;

// ---------------------------------------------------------------------------
//  Scaffold  (paper Section 3.2, eq. (3.1)).
//  A scaffold S = (t, E, F, G) is a multi-stage cover of the time interval
//  [0, T].  Stage i (i = 1..m) is the "admissible quad" (E[i-1], dt_i, F[i],
//  E[i]) where dt_i = t_i - t_{i-1}; E[i] is the "end-enclosure" (it encloses
//  End(E0, t_i)) and F[i] is the "full-enclosure" (it encloses the whole
//  solution set over [t_{i-1}, t_i]).  G[i] is the per-stage refinement data,
//  the "mini-scaffold".
// ---------------------------------------------------------------------------

// Mini-scaffold  G[i] = ( (ell_i, E_i, F_i), mu_i, (delta_i, h^euler_i) )
// (paper Section 5.1, items [G1]-[G5]).  After ell_i bisections stage i is a
// chain of 2^ell_i uniform admissible "mini-quads" (mini-steps).
struct ministeps {
    std::vector<double> mu1;   // growth logNorm used for the end-enclosure radius r0*e^{mu*t}
    std::vector<double> mu2;   // [G4] mu_i[j]: logNorm bound on mu_2(Jf(F_i[j]))
    double              delta; // [G5] delta_i : target delta-tube radius; halved after each EulerTube(i)
    double              heuler;// [G5] h^euler_i : Euler-tube step threshold from eq. (2.6)
    int                 ell;   // [G1] ell_i : level; stage i is divided into 2^ell_i mini-steps
    std::vector<IVector> bfE;  // [G3] E_i[] : the 2^ell_i+1 mini end-enclosures, E_i[0] = E_{i-1}
    std::vector<IVector> bfF;  // [G3] F_i[] : the 2^ell_i+1 mini full-enclosures
};

// Scaffold  S = (t, E, F, G)  (paper eq. (3.1)); S.T.back() is the current
// final time, S.E.back() the current terminal end-enclosure.
struct Stage {
    std::vector<double>    T;  // t = (t_0 < t_1 < ... < t_m)
    std::vector<IVector>   E;  // E = (E_0, ..., E_m); E_i encloses End(E_0, t_i)
    std::vector<IVector>   F;  // F = (F_1, ..., F_m) full-enclosures
    std::vector<ministeps> G;  // G = (G_1, ..., G_m) refinement substructures
};

// ---------------------------------------------------------------------------
//  Symbolic helper: build a CAPD "var:...;fun:...;" string.
// ---------------------------------------------------------------------------
string Convert_to_IMap(std::vector<std::string> SVar,
                       std::vector<std::string> SFun)
{
    std::string sumF = "var:";
    for (size_t i = 0; i < SVar.size(); ++i) {
        sumF += SVar[i];
        if (i < SVar.size() - 1) sumF += ", ";
    }
    sumF += "; fun:";
    for (size_t i = 0; i < SFun.size(); ++i) {
        sumF += SFun[i];
        if (i < SFun.size() - 1) sumF += ", ";
    }
    sumF += "; ";
    return sumF;
}

// Box(.) operator (paper Section 2.1): the smallest axis-aligned box
// enclosing B1 and B2 (a 2-argument hull, used to accumulate full-enclosures).
IVector Box(const IVector& B1, const IVector& B2) {
    int n = B1.dimension();
    IVector B(n);
    for (int i = 0; i < n; ++i) {
        double lower = std::min(B1[i].leftBound(),  B2[i].leftBound());
        double upper = std::max(B1[i].rightBound(), B2[i].rightBound());
        B[i] = interval(lower, upper);
    }
    return B;
}

// Intersection of two boxes (returns B2 if the intersection is empty).  Empty
// intersections should be rare after the scaffold invariants are maintained;
// callers use this as a conservative fallback rather than dropping coverage.
IVector IntersectB(const IVector& B1, const IVector& B2) {
    int n = B1.dimension();
    IVector B(n);
    for (int i = 0; i < n; ++i) {
        double lower = std::max(B1[i].leftBound(),  B2[i].leftBound());
        double upper = std::min(B1[i].rightBound(), B2[i].rightBound());
        if (lower > upper) {
            cout << "intersecterror" << endl;
            return B2;
        }
        B[i] = interval(lower, upper);
    }
    return B;
}

bool containsPoint(const IVector& B, const IVector& p) {
    if (B.dimension() != p.dimension()) return false;
    for (int i = 0; i < B.dimension(); ++i) {
        if (p[i].leftBound() < B[i].leftBound() ||
            p[i].rightBound() > B[i].rightBound())
            return false;
    }
    return true;
}

// Jacobian Jf(B) of the vector field f evaluated on the box B (paper Section
// 2.4 uses mu_2(Jf(.)) for the logNorm; here we just build the interval matrix).
IMatrix computeJacobian(IMap f, IVector B) {
    int n = B.dimension();
    IJet jet(n, n, 2);
    f(B, jet);
    IMatrix J(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            J[i][j] = jet(i, j);
    return J;
}

// Max-width  w_max(B) = max_i w(I_i)  (paper Section 2.1).  Refine stops once
// w_max of the terminal end-enclosure is <= eps (the scaffold is "eps-small").
double wmax(IVector B) {
    double m = 0.0;
    for (size_t i = 0; i < B.dimension(); ++i)
        m = std::max(m, B[i].rightBound() - B[i].leftBound());
    return m;
}

double euclideanRadius(IVector B) {
    double r2 = 0.0;
    for (size_t i = 0; i < B.dimension(); ++i) {
        double halfWidth = (B[i].rightBound() - B[i].leftBound()) / 2.0;
        r2 += halfWidth * halfWidth;
    }
    return std::sqrt(r2);
}

// Midpoint  m(B)  (paper Section 2.1), returned as plain doubles.  Used as the
// Euler initial point q0 in the Euler-tube method and as the "target" of a split.
std::vector<double> center(IVector B) {
    std::vector<double> v(B.dimension());
    for (size_t i = 0; i < B.dimension(); ++i)
        v[i] = (B[i].rightBound() + B[i].leftBound()) / 2.0;
    return v;
}

// ---------------------------------------------------------------------------
//  Logarithmic 2-norm  mu_2(J)  (paper Section 2.4; also called the matrix
//  measure / Lozinskii measure).  It is the largest eigenvalue of the
//  symmetric part (J + J^T)/2, works in any dimension, and -- unlike a norm --
//  may be negative, which sharpens the Fundamental Inequality (Lemma 2.1).
//  A value mu >= mu_2(Jf(B)) is a "logNorm-bound" on B.
// ---------------------------------------------------------------------------
double logNorm(const IMatrix& J) {
    capd::vectalg::EuclLNorm<IVector, IMatrix> euclLogNorm;
    return euclLogNorm(J).rightBound();
}

// ---------------------------------------------------------------------------
//  Euler-tube step threshold  h^euler(H, M, mu, delta)  (paper Lemma 2.2,
//  eq. (2.6)).  Returns the largest step h <= H for which the Euler polygon
//  Q_{q0,h} (the piecewise-linear Euler approximation, eq. (2.5)) is
//  guaranteed to stay inside the delta-tube of the exact trajectory.
//  Inputs:  M >= ||f^[2](F1)|| (f^[i] are the normalized Taylor coefficients,
//  eq. (2.1)) and mu a logNorm-bound on the full-enclosure F1.
//  EulerTube(i) may only be invoked when the mini-step size is below h^euler.
//  Degenerate M values mean the Euler polygon has no second-order tube growth
//  on this enclosure, so the full stage is admissible.
// ---------------------------------------------------------------------------
double eulerstep(double H, double M, double mu, double delta) {
    if (!(H > 0.0) || !(delta > 0.0)) return 0.0;
    if (!(M > 0.0)) return H;

    double h;
    if (mu > 0.0)
        h = (mu * delta) / (M * (exp(mu * H) - 1.0));
    else if (mu == 0.0)
        h = delta / (M * H);
    else
        h = (2.0 * mu * delta) / (2.0 * M * (exp(mu * H) - 1.0) - mu * mu * delta);

    if (!std::isfinite(h) || h <= 0.0) return 0.0;
    return std::min(H, h);
}

double secondTaylorNorm(IMap f, IVector B) {
    IVector coeffs = computeJacobian(f, B) * f(B);  // 2 f^[2](B)
    capd::vectalg::EuclNorm<IVector, IMatrix> euclNorm;
    return sup(euclNorm(coeffs)) / 2.0;
}
