#pragma once

// ===========================================================================
//  Refine-new.h  --  the Refine subroutine (paper Section 5) and its helpers.
//
//  S.Refine(eps) makes the scaffold "eps-small" (w_max(S.E.back()) <= eps).
//  It uses three subroutines (paper Section 5):
//
//     Bisect(i)    : one dyadic refinement of stage i -- each mini-step (mini-
//                    quad) is split into two by applying StepB twice
//                    (paper Section 5.1, S.Bisect).
//     EulerTube(i) : tighten stage i via the Euler-tube enclosures of
//                    Theorem 5.1 (paper Section 5.2, S.EulerTube); may only be
//                    invoked when the mini-step size is below h^euler_i.
//     Split()      : halve S.E[0] and spin off a parallel scaffold
//                    (paper Section 5).  In this implementation the spatial
//                    split is delegated to the outer EndCover queue (see
//                    EndCover-new.h): Refine shrinks S.E[0] and returns it,
//                    which signals EndCover to subdivide the box.
//
//  Each pass of the for-loop over stages i = 1..m is a "phase" (paper Section
//  5.1).  The paper's stopping conditions StageFreeze / NeedSplit /
//  CanTerminate (eqs. (5.4)-(5.6)) are realised implicitly here by the
//  while(eps < r0) loop together with the EndCover-level split.
// ===========================================================================

#include <iostream>
#include <cctype>
#include "capd/capdlib.h"
#include "stepAB-new.h"
#include "TaylorTube-new.h"   // the Taylor tube (higher-order EulerTube analogue)

using namespace capd;
using namespace std;

struct TubeStats {
    long long bisectCalls = 0;
    long long eulerTubeCalls = 0;
    long long rk2TubeCalls = 0;
    long long taylorTubeCalls = 0;
};

static TubeStats& tubeStats() {
    static TubeStats stats;
    return stats;
}

static bool tubeStatsEnabled() {
    static const bool enabled = (std::getenv("CAPD_TUBE_STATS") != nullptr);
    return enabled;
}

static void resetTubeStats() {
    tubeStats() = TubeStats();
}

static void printTubeStats() {
    if (!tubeStatsEnabled()) return;
    const TubeStats& s = tubeStats();
    std::cout << "TubeStats: bisect=" << s.bisectCalls
              << " euler=" << s.eulerTubeCalls
              << " rk2=" << s.rk2TubeCalls
              << " taylor=" << s.taylorTubeCalls << std::endl;
}

static int endpointMinPhasesOverride() {
    static const int value = []() {
        const char* raw = std::getenv("CAPD_ENDPOINT_MIN_PHASES");
        if (!raw) return -1;
        try {
            int v = std::stoi(raw);
            return (v > 0) ? v : -1;
        } catch (...) {
            return -1;
        }
    }();
    return value;
}

enum class TubeMethod {
    Euler,
    RK2Midpoint,
    Taylor          // higher-order Taylor tube (see TaylorTube-new.h)
};

static TubeMethod selectedTubeMethod() {
    static const TubeMethod method = []() {
        const char* value = std::getenv("CAPD_TUBE_METHOD");
        if (!value) return TubeMethod::Euler;
        std::string s(value);
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (s == "rk2" || s == "midpoint" || s == "improved-euler")
            return TubeMethod::RK2Midpoint;
        if (s == "taylor" || s == "taylortube" || s == "taylor-tube")
            return TubeMethod::Taylor;
        return TubeMethod::Euler;
    }();
    return method;
}

// Requested Taylor-tube degree (the order of the centre Taylor polynomial),
// set once from the CLI in main().  A value <= 0 means "auto" = order - 1,
// which reuses the coefficients StepB already needs (see TaylorTube-new.h).
inline int& taylorTubeDegreeRef() {
    static int degree = -1;
    return degree;
}

// Resolve the effective tube degree given the method's Taylor `order`.
// Default (auto) is order-1; an explicit request is clamped to [1, order-1].
static int resolveTaylorTubeDegree(int order) {
    int requested = taylorTubeDegreeRef();
    int deg = (requested > 0) ? std::min(requested, order - 1) : (order - 1);
    return std::max(1, deg);
}

// Diagnostic only: warn when two boxes that should overlap are disjoint.
static void warnIfDisjoint(const IVector& A, const IVector& B,
                           const char* what, int idx) {
    for (int i = 0; i < A.dimension(); ++i) {
        double lo = std::max(A[i].leftBound(),  B[i].leftBound());
        double hi = std::min(A[i].rightBound(), B[i].rightBound());
        if (lo > hi) {
            std::cout << "Bisect: " << what << " disjoint at stage " << idx << std::endl;
            return;
        }
    }
}

static double scaffoldMuMax(const Stage& S) {
    double mu = 0.0;
    bool seen = false;
    for (size_t i = 1; i < S.G.size(); ++i) {
        for (double v : S.G[i].mu2) {
            mu = seen ? std::max(mu, v) : v;
            seen = true;
        }
    }
    return seen ? mu : 0.0;
}

static bool NeedSplit(const Stage& S, int dim, double veps) {
    double threshold = veps / (2.0 * sqrt(static_cast<double>(dim)));
    return wmax(S.E[0]) > threshold;
}

static void ShrinkInitialBox(Stage& S, int dim, const std::vector<double>& target) {
    IVector E0(dim);
    for (int i = 0; i < dim; ++i) {
        double left  = S.E[0][i].leftBound();
        double right = S.E[0][i].rightBound();
        double newLeft  = left  + (target[i] - left ) * 0.5;
        double newRight = right - (right - target[i]) * 0.5;
        E0[i] = interval(newLeft, newRight);
    }
    S.E[0] = E0;
    S.F[0] = E0;
    S.G[0].bfE[0] = E0;
    S.G[0].bfF[0] = E0;
}

// ---------------------------------------------------------------------------
//  S.Bisect(i)  (paper Section 5.1).  One dyadic refinement of stage i: each
//  of the current 2^ell_i mini-quads is subdivided into two mini-quads of half
//  the step-size by applying StepB twice.  For each half-step we recompute the
//  mini full-enclosure tmpF (truncated Taylor series, order 5) and the mini
//  end-enclosure via StepB, update the logNorm arrays mu_i, and increment ell_i.
//  Bisect has no precondition (unlike EulerTube).
// ---------------------------------------------------------------------------
void Bisect(Stage& S, int idx, int dim, IMap f, int stepBtype, int degree, int debug) {
    tubeStats().bisectCalls += 1;
    if (idx > 0) {
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }
    std::vector<IVector> bfE = { S.G[idx].bfE[0] };
    std::vector<IVector> bfF = { S.G[idx].bfE[0] };
    std::vector<double>  mu1 = { S.G[idx].mu1[0] };
    std::vector<double>  mu2 = { S.G[idx].mu2[0] };
    int N = std::pow(2, S.G[idx].ell);
    double h = (S.T[idx] - S.T[idx - 1]) / (2 * N);
    IVector IF = S.G[idx].bfE[0];

    for (int j = 1; j < N + 1; ++j) {
        // --- first half-step ---
        ICnOdeSolver solver1(f, 5);
        solver1.computeCoefficients(bfE.back(), 5);
        IVector coeffs(dim);
        for (int k = 0; k < dim; ++k) {
            coeffs[k] = solver1.coefficient(k, 4);
            for (int d = 3; d >= 1; --d)
                coeffs[k] = coeffs[k] * interval(0, h) + solver1.coefficient(k, d);
            coeffs[k] = coeffs[k] * interval(0, h) + bfE.back()[k];
        }
        ICnOdeSolver solverB0(f, 5);
        solverB0.computeCoefficients(S.G[idx].bfF[j], 5);
        IVector B0bar_derivative_k(dim);
        for (int k = 0; k < dim; ++k)
            B0bar_derivative_k[k] = interval(0, std::pow(h, 5)) * solverB0.coefficient(k, 5);

        IVector tmpF1 = coeffs + B0bar_derivative_k;
        if (debug) warnIfDisjoint(tmpF1, S.G[idx].bfF[j], "full-encl (1st half)", idx);
        tmpF1 = IntersectB(tmpF1, S.G[idx].bfF[j]);
        IF = Box(IF, tmpF1);
        mu1.push_back(logNorm(computeJacobian(f, tmpF1)));
        mu2.push_back(S.G[idx].mu2[j]);

        IVector nextBox;
        switch (localStepBType(stepBtype)) {
        case 0: nextBox = stepBcrlohner(f, degree, h, bfE.back()); break;
        case 1: nextBox = stepBcrlohnerwithmu(f, degree, h, bfE.back(), tmpF1, mu1.back()); break;
        case 2: nextBox = directmethodwithmu(f, degree, h, bfE.back(), tmpF1, mu1.back()); break;
        case 3: nextBox = puredirectmethod(f, degree, h, bfE.back()); break;
        default: nextBox = stepBcrlohner(f, degree, h, bfE.back()); break;
        }
        if (debug) warnIfDisjoint(nextBox, S.G[idx].bfF[j], "end-encl (1st half)", idx);
        nextBox = IntersectB(nextBox, S.G[idx].bfF[j]);
        bfE.push_back(nextBox);
        bfF.push_back(tmpF1);

        // --- second half-step ---
        ICnOdeSolver solver2(f, 5);
        solver2.computeCoefficients(nextBox, 5);
        IVector coeffs2(dim);
        for (int k = 0; k < dim; ++k) {
            coeffs2[k] = solver2.coefficient(k, 4);
            for (int d = 3; d >= 1; --d)
                coeffs2[k] = coeffs2[k] * interval(0, h) + solver2.coefficient(k, d);
            coeffs2[k] = coeffs2[k] * interval(0, h) + nextBox[k];
        }
        IVector tmpF2 = coeffs2 + B0bar_derivative_k;
        if (debug) warnIfDisjoint(tmpF2, S.G[idx].bfF[j], "full-encl (2nd half)", idx);
        tmpF2 = IntersectB(tmpF2, S.G[idx].bfF[j]);
        IF = Box(IF, tmpF2);
        mu1.push_back(logNorm(computeJacobian(f, tmpF2)));
        mu2.push_back(S.G[idx].mu2[j]);

        IVector nextBox2;
        switch (localStepBType(stepBtype)) {
        case 0: nextBox2 = stepBcrlohner(f, degree, h, nextBox); break;
        case 1: nextBox2 = stepBcrlohnerwithmu(f, degree, h, nextBox, tmpF2, mu1.back()); break;
        case 2: nextBox2 = directmethodwithmu(f, degree, h, nextBox, tmpF2, mu1.back()); break;
        case 3: nextBox2 = puredirectmethod(f, degree, h, nextBox); break;
        default: nextBox2 = stepBcrlohner(f, degree, h, nextBox); break;
        }
        if (debug) warnIfDisjoint(nextBox2, S.G[idx].bfE[j], "end-encl (2nd half)", idx);
        nextBox2 = IntersectB(nextBox2, S.G[idx].bfE[j]);
        bfE.push_back(nextBox2);
        bfF.push_back(tmpF2);
    }

    S.G[idx].mu1  = mu1;
    S.G[idx].mu2  = mu2;
    S.G[idx].ell  = S.G[idx].ell + 1;
    S.G[idx].bfE  = bfE;
    S.G[idx].bfF  = bfF;
    S.E[idx] = IntersectB(bfE.back(), S.E[idx]);
    S.F[idx] = IntersectB(S.F[idx], IF);
}

// ---------------------------------------------------------------------------
//  S.EulerTube(i)  (paper Section 5.2, Theorem 5.1).  Refine stage i using the
//  Euler-tube method: march the Euler polygon q_j (eq. (2.5)) of the centre
//  trajectory and grow a delta-tube of radius r0 e^{mu t} + delta around it.
//  Theorem 5.1(a) gives the mini end-enclosures, Theorem 5.1(b) the mini
//  full-enclosures; mu_i[j] is refreshed to mu_2(Jf(F_i[j])) and delta_i is
//  halved by the caller.  (Precondition: mini-step size < h^euler_i.)
//  If the Euler point leaves the stored mini end-enclosure, restart the tube
//  from the current mini-step's center as prescribed in Section 5.2.
// ---------------------------------------------------------------------------
void EulerTube(Stage& S, int idx, int dim, IMap f, int debug) {
    tubeStats().eulerTubeCalls += 1;
    if (idx > 0) {
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }
    double delta = S.G[idx].delta;
    std::vector<double> tmp = center(S.G[idx].bfE[0]);
    IVector q(dim);
    for (int k = 0; k < dim; ++k) q[k] = interval(tmp[k], tmp[k]);  // Euler start = centre

    double segmentR0 = wmax(S.G[idx].bfE[0]) / 2;
    int segmentStart = 0;
    int N = std::pow(2, S.G[idx].ell);
    double H = (S.T[idx] - S.T[idx - 1]) / N;
    IVector IF = S.G[idx].bfF[1];
    IVector Itmp(dim);

    for (int j = 1; j < N + 1; ++j) {
        if (!containsPoint(S.G[idx].bfE[j - 1], q)) {
            if (debug)
                std::cout << "EulerTube: restart at stage " << idx
                          << ", mini-step " << j << std::endl;
            tmp = center(S.G[idx].bfE[j - 1]);
            for (int k = 0; k < dim; ++k) q[k] = interval(tmp[k], tmp[k]);
            segmentR0 = wmax(S.G[idx].bfE[j - 1]) / 2;
            segmentStart = j - 1;
        }

        IVector qPrev = q;
        q = q + f(q) * H;                                   // Euler polygon, eq. (2.5)
        int segmentStep = j - segmentStart;

        double mu2 = logNorm(computeJacobian(f, S.G[idx].bfF[j]));
        S.G[idx].mu2[j] = mu2;
        double r1 = segmentR0 * exp(segmentStep * S.G[idx].mu1[j] * H) + delta;
        double r2 = segmentR0 * exp(segmentStep * mu2 * H) + delta;
        IVector B(dim), B1(dim);
        for (int k = 0; k < dim; ++k) { B[k] = interval(-r1, r1); B1[k] = interval(-r2, r2); }

        Itmp = Box(qPrev + B1, q + B1);                     // Theorem 5.1(b): full-enclosure
        S.G[idx].bfF[j] = IntersectB(Itmp, S.G[idx].bfF[j]);
        IF = Box(IF, S.G[idx].bfF[j]);

        S.G[idx].mu1[j] = logNorm(computeJacobian(f, S.G[idx].bfF[j]));
        Itmp = IntersectB(q + B1, S.G[idx].bfE[j]);
        S.G[idx].bfE[j] = IntersectB(q + B, Itmp);          // Theorem 5.1(a): end-enclosure
    }
    S.E[idx] = S.G[idx].bfE.back();
    S.F[idx] = IF;
}

static double maxAbsIntervalVector(const IVector& v) {
    double r = 0.0;
    for (int i = 0; i < v.dimension(); ++i) {
        r = std::max(r, std::abs(v[i].leftBound()));
        r = std::max(r, std::abs(v[i].rightBound()));
    }
    return r;
}

static IVector rk2MidpointStep(IMap f, const IVector& q, double h) {
    return q + f(q + f(q) * (h / 2.0)) * h;
}

static IVector rk2TaylorEndpointEnclosure(IMap f, int dim, const IVector& q,
                                           const IVector& fullEnclosure,
                                           double h) {
    ICnOdeSolver centerSolver(f, 3);
    centerSolver.computeCoefficients(q, 3);

    IVector exact(dim);
    for (int k = 0; k < dim; ++k)
        exact[k] = q[k]
                 + interval(h) * centerSolver.coefficient(k, 1)
                 + interval(h * h) * centerSolver.coefficient(k, 2);

    ICnOdeSolver fullSolver(f, 3);
    fullSolver.computeCoefficients(fullEnclosure, 3);
    interval timeFactor(0.0, h * h * h);
    for (int k = 0; k < dim; ++k)
        exact[k] += timeFactor * fullSolver.coefficient(k, 3);

    return exact;
}

void RK2Tube(Stage& S, int idx, int dim, IMap f, int debug) {
    tubeStats().rk2TubeCalls += 1;
    if (idx > 0) {
        S.G[idx].bfE[0] = S.G[idx - 1].bfE.back();
        S.G[idx].bfF[0] = S.G[idx - 1].bfF.back();
    }

    std::vector<double> tmp = center(S.G[idx].bfE[0]);
    IVector q(dim);
    for (int k = 0; k < dim; ++k) q[k] = interval(tmp[k], tmp[k]);

    double r1 = wmax(S.G[idx].bfE[0]) / 2.0;
    double r2 = r1;
    int N = std::pow(2, S.G[idx].ell);
    double h = (S.T[idx] - S.T[idx - 1]) / N;
    IVector IF = S.G[idx].bfF[1];

    for (int j = 1; j < N + 1; ++j) {
        if (!containsPoint(S.G[idx].bfE[j - 1], q)) {
            if (debug)
                std::cout << "RK2Tube: restart at stage " << idx
                          << ", mini-step " << j << std::endl;
            tmp = center(S.G[idx].bfE[j - 1]);
            for (int k = 0; k < dim; ++k) q[k] = interval(tmp[k], tmp[k]);
            r1 = wmax(S.G[idx].bfE[j - 1]) / 2.0;
            r2 = r1;
        }

        IVector qNext = rk2MidpointStep(f, q, h);
        IVector exact = rk2TaylorEndpointEnclosure(f, dim, q, S.G[idx].bfF[j], h);
        double localError = maxAbsIntervalVector(exact - qNext);

        double mu2 = logNorm(computeJacobian(f, S.G[idx].bfF[j]));
        S.G[idx].mu2[j] = mu2;
        r1 = r1 * exp(S.G[idx].mu1[j] * h) + localError;
        r2 = r2 * exp(mu2 * h) + localError;

        IVector B(dim), B1(dim);
        for (int k = 0; k < dim; ++k) {
            B[k] = interval(-r1, r1);
            B1[k] = interval(-r2, r2);
        }

        // Keep the existing full enclosure; the RK2 local-error bound above is
        // used for endpoint tightening only.  This avoids claiming a higher
        // order continuous-time tube theorem that we have not proved here.
        IF = Box(IF, S.G[idx].bfF[j]);

        S.G[idx].mu1[j] = logNorm(computeJacobian(f, S.G[idx].bfF[j]));
        IVector endpoint = IntersectB(qNext + B1, S.G[idx].bfE[j]);
        S.G[idx].bfE[j] = IntersectB(qNext + B, endpoint);
        q = qNext;
    }

    S.E[idx] = S.G[idx].bfE.back();
    S.F[idx] = IF;
}

// Extra endpoint tightening, still within the SIAM scaffold:
// StepB gives a verified enclosure of Phi^{T_i}(E0), so it is safe to
// intersect it with the scaffold's current endpoint enclosure.
static void TightenEndpointByLohner(Stage& S, IMap f, int degree,
                                    size_t idx, int debug) {
    if (idx == 0 || idx >= S.E.size() || idx >= S.T.size()) return;
    IVector endpoint = stepBcrlohner(f, degree, S.T[idx], S.E[0]);
    S.E[idx] = IntersectB(S.E[idx], endpoint);

    if (idx < S.G.size() && !S.G[idx].bfE.empty())
        S.G[idx].bfE.back() = IntersectB(S.G[idx].bfE.back(), S.E[idx]);

    if (idx + 1 < S.G.size() && !S.G[idx + 1].bfE.empty()) {
        S.G[idx + 1].bfE[0] = IntersectB(S.G[idx + 1].bfE[0], S.E[idx]);
        if (!S.G[idx + 1].bfF.empty())
            S.G[idx + 1].bfF[0] = S.G[idx + 1].bfE[0];
    }

    if (debug == 1)
        std::cout << "EndpointLohner: tightened stage " << idx
                  << " at t=" << S.T[idx]
                  << ", wmax=" << wmax(S.E[idx]) << std::endl;
}

static void TightenTerminalByEndpointLohner(Stage& S, IMap f, int degree,
                                            int debug) {
    if (S.E.size() <= 1) return;
    TightenEndpointByLohner(S, f, degree, S.E.size() - 1, debug);
}

// ---------------------------------------------------------------------------
//  S.Refine(eps)  (paper Section 5).  Make the scaffold eps-small.  Each
//  "phase" (the for-loop over stages i = 1..m) refines every stage: EulerTube(i)
//  when the mini-step is below h^euler_i (so Lemma 2.2 applies), otherwise
//  Bisect(i); afterwards h^euler_i is refreshed from eq. (2.6).  When the
//  terminal end-enclosure E_m is still wider than eps, the initial box E[0] is
//  shrunk toward 'target' (its midpoint) -- this realises the Split() of the
//  paper: returning a shrunken E[0] signals the outer EndCover queue to
//  subdivide the box spatially.
// ---------------------------------------------------------------------------
void Refine(Stage& S, int dim, IMap f, int stepBtype, int degree,
            double veps, int debug, std::vector<double> target) {
    double r0 = wmax(S.E.back());
    int phases = 0;
    int minPhasesBeforeSplit = useEndpointLohnerTightening(stepBtype) ? 1 : 4;
    if (useEndpointLohnerTightening(stepBtype) && endpointMinPhasesOverride() > 0)
        minPhasesBeforeSplit = endpointMinPhasesOverride();
    const int maxPhasesBeforeForcedSplit =
        useEndpointLohnerTightening(stepBtype)
            ? std::max(12, minPhasesBeforeSplit + 4)
            : 32;
    const TubeMethod tubeMethod = selectedTubeMethod();
    while (veps < r0) {
        int numofstage = S.T.size();
        for (int i = 1; i < numofstage; ++i) {
            int N = std::pow(2, S.G[i].ell);
            double H = S.T[i] - S.T[i - 1];
            double h = H / N;

            if (tubeMethod == TubeMethod::RK2Midpoint) {
                RK2Tube(S, i, dim, f, debug);
                S.G[i].delta = S.G[i].delta / 2;
            } else if (h > S.G[i].heuler) { // Lemma 2.2 not yet applicable -> Bisect
                Bisect(S, i, dim, f, stepBtype, degree, debug);
            } else if (tubeMethod == TubeMethod::Taylor) {
                // mini-step admissible -> tighten with the higher-order Taylor tube
                tubeStats().taylorTubeCalls += 1;
                TaylorTube(S, i, dim, f, resolveTaylorTubeDegree(degree), debug);
                S.G[i].delta = S.G[i].delta / 2;
            } else {                        // mini-step small enough -> EulerTube
                EulerTube(S, i, dim, f, debug);
                S.G[i].delta = S.G[i].delta / 2;
            }

            // Update the tube-admissibility threshold h^euler for the next
            // phase.  The Taylor tube of degree p tolerates larger steps, so it
            // uses the Taylor step (Cbar = ||f^[p+1]||) instead of the order-2
            // Euler step; without this the tube branch would almost never fire.
            double mu = *std::max_element(S.G[i].mu2.begin(), S.G[i].mu2.end());
            double htmp;
            if (tubeMethod == TubeMethod::Taylor) {
                int td = resolveTaylorTubeDegree(degree);
                double Cbar = taylorCoeffNormUpperBound(f, S.F[i], td);
                htmp = hTaylorStep(H, Cbar, mu, S.G[i].delta, td);
            } else {
                double MM = secondTaylorNorm(f, S.F[i]);
                htmp = eulerstep(H, MM, mu, S.G[i].delta);
            }
            S.G[i].heuler = (h > S.G[i].heuler) ? htmp
                                                : std::min(htmp, S.G[i].heuler);
        }

        r0 = wmax(S.E.back());
        if (veps >= r0) break;

        ++phases;
        bool stillTooWide = useEndpointLohnerTightening(stepBtype)
                                ? (r0 > veps)
                                : (r0 > 2.0 * veps);
        if ((phases >= minPhasesBeforeSplit &&
             (NeedSplit(S, dim, veps) || stillTooWide)) ||
            phases >= maxPhasesBeforeForcedSplit) {
            ShrinkInitialBox(S, dim, target);
            return;
        }
    }
}
