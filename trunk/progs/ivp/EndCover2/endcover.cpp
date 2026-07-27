// ===========================================================================
//  End Cover for the Initial Value Problem  (Zhang & Yap)
//  ---------------------------------------------------------------------------
//  A clean implementation of the EndCover algorithm:
//
//      EndCover_f(B0, eps, H) -> C   such that
//          End(B0, H)  subset of  union(C)  subset of  End(B0,H) (+) [-eps,eps]^n.
//
//  Structure (see the per-file headers for the paper section references):
//      calD-calQ-new.h : scaffold data structures + interval/logNorm helpers
//      stepAB-new.h    : StepA and StepB           (Section 4)
//      Extend-new.h    : the Extend subroutine     (Section 4)
//      Refine-new.h    : Refine, Bisect, EulerTube (Section 5)
//      EndCover-new.h  : the global EndCover queue  (Section 3.3)
//
//  CLI (note the leading iflag; the algorithm itself is unchanged, only the
//  input parsing and the amount/format of output depend on iflag):
//      ./endcover.exe iflag stepB stepA n <vars..> <funs..> eps order T debug <lo hi ..>
//  e.g. Volterra:
//      ./endcover.exe 2 0 0 2 x y "2*x-2*x*y" "-y+x*y" 0.1 20 1 0 0.9 1.1 2.9 3.1
//
//  iflag is progressive (iflag >= k includes every output of smaller k):
//      0+: print total runtime (ms)
//      1+: print Hull(T): the minimal axis-aligned hull of the time-T cover
//      2+: print E0Boxes: number of initial sub-boxes
//      3+: write E0.txt and E1.txt (E1.txt contains ONLY the time-T cover)
//      4+: additionally write E_0.txt and E_1.txt for plotting:
//            - E_0.txt contains E0 (and in 2D also the 4 corner points)
//            - E_1.txt contains the time-T cover plus propagated images at
//              times 0.1/0.4/0.7 (those <= T), and in 2D also corner propagation
//
//  This matches the CLI / iflag convention of the sibling Boundary-method tool
//  (build/examples/Boundarymethod/ourcode), so the same example parameter files
//  and Makefile idioms (make eg, make run-egXXX) work here too.
// ===========================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>

#include "capd/capdlib.h"
#include "Extend-new.h"
#include "EndCover-new.h"

using namespace capd;
using namespace std;

// Taylor order assumed by StepA's default argument (paper uses k = 20).
int MAX_TAYLOR_ORDER = 20;

static void saveBoxes(const std::vector<IVector>& boxes, const std::string& filename) {
    std::ofstream fout(filename);
    if (!fout) { std::cerr << "cannot open file: " << filename << std::endl; return; }
    fout << std::fixed << std::setprecision(17);
    for (size_t i = 0; i < boxes.size(); i++) {
        fout << "Box " << i << ": ";
        for (int d = 0; d < boxes[i].dimension(); d++) {
            fout << "[" << boxes[i][d].leftBound() << ", " << boxes[i][d].rightBound() << "]";
            if (d + 1 < boxes[i].dimension()) fout << " x ";
        }
        fout << "\n";
    }
    std::cout << "write to -> " << filename << std::endl;
}

// ---------------------------------------------------------------------------
//  Plotting output (iflag >= 4).  Post-processing only: it reuses the cover
//  that EndCover already returned and adds a validated "tube" so a viewer can
//  see how End(B0, t) evolves.  Nothing here changes the algorithm.
//
//      E_1.txt = [ cover@T ]                         (the eps-cover)
//              + [ E0 sub-boxes ]                    (their initial boxes)
//              + [ image of each E0 sub-box at t = 0.1/0.4/0.7, those <= T ]
//              + [ 2D only: 4 corner points, then each corner's images ]
//      E_0.txt = [ E0 sub-boxes ] + [ 2D only: the 4 corners of B0 ]
//
//  This is the box layout the bundled MATLAB reader (Boundarymethod/
//  two_dim_E1.m) expects, so it can colour each E0 box and its images alike.
//  Every image is a rigorous C^r-Lohner time-t map, so the plot data is
//  validated as well.
// ---------------------------------------------------------------------------

// The intermediate times at which E0 boxes / corners are propagated for plots.
static const double kTrackTimes[3] = { 0.1, 0.4, 0.7 };

// Verified time-t image of a (possibly degenerate) box.  Falls back to the box
// itself if the Lohner map throws (e.g. filib overflow), so the per-box entry
// count in the plot files stays fixed (the MATLAB reader relies on that).
static IVector safeImage(IMap F, int degree, double t, const IVector& box) {
    try { return stepBcrlohner(F, degree, t, box); }
    catch (const std::exception&) { return box; }
}

static void writePlotFiles(const endcover::EndCoverResult& C, IMap F, int degree,
                           double T, const IVector& B0) {
    const int n = B0.dimension();

    // Which of 0.1/0.4/0.7 fall inside the horizon [0, T].
    std::vector<double> track;
    for (double t : kTrackTimes) if (t <= T) track.push_back(t);

    // E_1.txt: cover, then the E0 boxes, then each E0 box's intermediate images.
    std::vector<IVector> plot1 = C.cover;
    for (const auto& e0 : C.initialBoxes) plot1.push_back(e0);
    for (const auto& e0 : C.initialBoxes)
        for (double t : track) plot1.push_back(safeImage(F, degree, t, e0));

    // E_0.txt: the E0 boxes (initial sub-boxes).
    std::vector<IVector> plot0 = C.initialBoxes;

    // In 2D we can also trace the exact-set boundary via the 4 corners of B0.
    if (n == 2) {
        std::vector<IVector> corners;
        for (int i = 0; i < 4; ++i) {
            IVector c(2);
            c[0] = (i & 1) ? interval(B0[0].rightBound()) : interval(B0[0].leftBound());
            c[1] = (i & 2) ? interval(B0[1].rightBound()) : interval(B0[1].leftBound());
            corners.push_back(c);
        }
        for (const auto& c : corners) plot0.push_back(c);   // E_0 corner tail

        for (const auto& c : corners) plot1.push_back(c);   // corner points ...
        for (const auto& c : corners) {                     // ... then their images
            for (double t : track) plot1.push_back(safeImage(F, degree, t, c));
            plot1.push_back(safeImage(F, degree, T, c));
        }
    }

    saveBoxes(plot1, "E_1.txt");
    saveBoxes(plot0, "E_0.txt");
}

static void BoundaryEndpointRefine(Stage& S, IMap F, int degree, double H,
                                   double veps, int dim,
                                   const std::vector<double>& target) {
    if (S.T.size() == 1) {
        S.T.push_back(H);
        S.F.push_back(S.E[0]);
        S.G.push_back(S.G[0]);
        S.E.push_back(S.E[0]);
    }

    S.T.back() = H;
    S.E.back() = stepBcrlohner(F, degree, H, S.E[0]);

    int iter = 0;
    const int maxIter = 64;
    while (veps < wmax(S.E.back()) && iter < maxIter) {
        ++iter;
        ShrinkInitialBox(S, dim, target);
        S.E.back() = stepBcrlohner(F, degree, H, S.E[0]);
    }
}

// Run one scaffold for the box B from time 0 to H.  This is the paper's
// per-scaffold A/B loop under the refine-before-extend principle (Section 3):
// repeatedly Extend (append a stage) then Refine (make eps-small) until the
// terminal time reaches H.  Returns the final scaffold; S.E.front() is the
// (possibly shrunken) initial box ulB and S.E.back() the eps-small end-
// enclosure olB of End(B, H).
static Stage runScaffoldToH(const IVector& B, IMap F, const ministeps& templateG,
                            double veps, double delta, int degree, double H,
                            int stepBtype, int stepAtype, int debug, int dim,
                            const std::vector<double>& target) {
    ministeps G = templateG;
    G.bfE[0] = B;
    G.bfF[0] = B;
    Stage S = { {0.0}, {B}, {B}, {G} };

    if (useBoundaryEndpointRefine(stepBtype)) {
        BoundaryEndpointRefine(S, F, degree, H, veps, dim, target);
        return S;
    }

    double T = 0.0;
    double h = H - T;
    while (T < H) {
        if (h > 1.0) h = 1.0;                       // cap the trial horizon at 1
        Extendnew(F, S, veps, delta, degree, h, stepBtype, stepAtype, debug);

        // Tighten the new terminal end-enclosure with a direct time-T image.
        TightenTerminalByEndpointLohner(S, F, degree, debug);

        Refine(S, dim, F, stepBtype, degree, veps, debug, target);
        if (useHybridEndpointRefine(stepBtype)) {
            BoundaryEndpointRefine(S, F, degree, H, veps, dim, target);
            return S;
        }
        if (!endcover::almostEqualBox(S.E.front(), B)) return S;
        T = S.T.back();
        h = H - T;
    }
    return S;
}

int main(int argc, char* argv[]) {
    // iflag controls how much is printed / written; it does NOT affect what
    // EndCover computes (see the file header for the progressive levels).
    int iflag = 2;
    int stepB = 0, stepA = 0, n = 0, order = 0, debug = 0;
    double eps = 0.0, T = 0.0;
    std::vector<std::string> SVar, SFun;
    IVector B;

    try {
        if (argc < 11) throw std::runtime_error("not enough arguments");
        int a = 1;
        iflag = std::stoi(argv[a++]);
        stepB = std::stoi(argv[a++]);
        stepA = std::stoi(argv[a++]);
        n     = std::stoi(argv[a++]);
        if (n <= 0) throw std::runtime_error("n must be positive");
        for (int i = 0; i < n; ++i) SVar.push_back(argv[a++]);
        for (int i = 0; i < n; ++i) SFun.push_back(argv[a++]);
        eps   = std::stod(argv[a++]);
        order = std::stoi(argv[a++]);
        T     = std::stod(argv[a++]);
        debug = std::stoi(argv[a++]);
        if (argc < a + 2 * n) throw std::runtime_error("not enough interval bounds");
        B.resize(n);
        for (int i = 0; i < n; ++i) {
            double lo = std::stod(argv[a++]);
            double hi = std::stod(argv[a++]);
            B[i] = capd::interval(lo, hi);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n"
                  << "Usage: endcover.exe iflag stepB stepA n <vars..> <funs..> "
                     "eps order T debug <lo hi ..>\n"
                  << "Falling back to the default Volterra example.\n";
        iflag = 2; stepB = 0; stepA = 0; n = 2; eps = 0.1; order = 20; T = 1.0; debug = 0;
        SVar = { "x", "y" };
        SFun = { "2*x-2*x*y", "-y+x*y" };
        B.resize(2);
        B[0] = capd::interval(0.9, 1.1);
        B[1] = capd::interval(2.9, 3.1);
    }

    double veps  = eps;
    double delta = veps / 10.0;
    // The function strings are passed directly to CAPD's parser; write them in
    // CAPD syntax (explicit '*', '^' and parentheses are fine, e.g. "(x+y)^2").
    IMap F(Convert_to_IMap(SVar, SFun), 3.0);
    int degree = order;

    // Template mini-scaffold for stage 0 of every fresh scaffold.
    ministeps G;
    G.mu1 = { 1000.0 };
    G.mu2 = { 1000.0 };
    G.delta = delta;
    G.heuler = 0.0;        // stage thresholds are computed when Extend creates them
    G.ell = 0;
    G.bfE = { B };
    G.bfF = { B };

    // endEnc is the per-box callback used by the EndCover queue (Section 3.3):
    // it runs one scaffold for the box and reports (ulB, olB).  ulB != box tells
    // EndCover that a spatial Split is required.
    auto endEnc = [&](const IVector& box, double epsLocal,
                      const std::vector<double>& p, double Hlocal)
        -> std::pair<IVector, IVector> {
        Stage s = runScaffoldToH(box, F, G, epsLocal, delta, degree, Hlocal,
                                 stepB, stepA, debug, n, p);
        return { s.E.front(), s.E.back() };
    };

    resetTubeStats();
    auto t0 = std::chrono::high_resolution_clock::now();
    endcover::EndCoverResult C;
    try {
        C = endcover::EndCover(B, veps, T, endEnc, 2000000, 0.0,
                               useEndpointLohnerTightening(stepB) ||
                               useBoundaryEndpointRefine(stepB) ||
                               useHybridEndpointRefine(stepB));
    } catch (const std::exception& e) {
        std::cerr << "[EndCover] failed: " << e.what() << std::endl;
        return 1;
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    printTubeStats();   // silent unless CAPD_TUBE_STATS is set

    // --- Progressive output (iflag >= k prints/writes everything for <= k) ---

    // 0+: total runtime.
    if (iflag >= 0)
        std::cout << "time(ms)= " << ms << std::endl;

    // 1+: the minimal axis-aligned hull of the time-T cover (and its max width).
    if (iflag >= 1 && !C.cover.empty()) {
        IVector hull = C.cover.front();
        for (size_t i = 1; i < C.cover.size(); ++i) hull = Box(hull, C.cover[i]);
        std::cout << "Hull(T)= " << hull << std::endl;
        std::cout << "wmax= "    << wmax(hull) << std::endl;
    }

    // 2+: number of initial sub-boxes produced by the space cover of B0.
    if (iflag >= 2)
        std::cout << "E0Boxes= " << C.initialBoxes.size() << std::endl;

    // 3+: machine-readable box lists.  E1.txt is ONLY the time-T cover; E0.txt
    //     holds the matching initial sub-boxes (same order, one box per line).
    if (iflag >= 3) {
        saveBoxes(C.cover, "E1.txt");
        saveBoxes(C.initialBoxes, "E0.txt");
    }

    // 4+: plotting files with the validated propagation tube (see writePlotFiles).
    if (iflag >= 4)
        writePlotFiles(C, F, degree, T, B);

    return 0;
}
