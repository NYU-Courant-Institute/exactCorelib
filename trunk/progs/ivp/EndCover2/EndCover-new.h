#pragma once

// ---------------------------------------------------------------------------
//  The global EndCover algorithm (Section 3.3 of the paper).
//
//   EndCover_f(B0, eps, H) -> C
//   C <- {}                            // the eps-cover
//   Q0 <- { Init(B0) }                 // working queue of boxes/scaffolds
//   While Q0 != {}:
//       B <- Q0.pop()
//       (ulB, olB) <- endEnc(B)        // run one scaffold from 0 to H
//       If ulB != B:                   // Refine had to shrink E[0] -> split B
//           Q0.push( children of B )   // full-dimensional or longest-dim split
//       Else:
//           C.push(olB)                // leaf olB encloses End(B, H)
//   Return C
//
//  The union of the returned boxes C satisfies
//        End(B0, H)  subset of  union(C)  subset of  End(B0, H) (+) [-eps,eps]^n.
// ---------------------------------------------------------------------------

#include <vector>
#include <array>
#include <utility>
#include <functional>
#include <stdexcept>
#include <cmath>

#include "capd/capdlib.h"
#include "calD-calQ-new.h"

namespace endcover {

using capd::interval;
using capd::IVector;

inline std::vector<double> midpoint(const IVector& box) {
    const int n = box.dimension();
    std::vector<double> mid;
    mid.reserve(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i)
        mid.push_back((box[i].leftBound() + box[i].rightBound()) / 2.0);
    return mid;
}

inline bool almostEqualBox(const IVector& a, const IVector& b, double tol = 0.0) {
    const int n = a.dimension();
    if (n != b.dimension()) return false;
    for (int i = 0; i < n; ++i) {
        if (std::abs(a[i].leftBound()  - b[i].leftBound())  > tol ||
            std::abs(a[i].rightBound() - b[i].rightBound()) > tol) return false;
    }
    return true;
}

// Split a box into its 2^n equal sub-boxes (the paper-style spatial split).
inline std::vector<IVector> splitBoxAllDims(const IVector& box) {
    const int n = box.dimension();
    if (n <= 0) return {};

    const size_t outCount = static_cast<size_t>(1) << static_cast<size_t>(n);
    std::vector<IVector> out;
    out.reserve(outCount);

    std::vector<double> a(n), m(n), b(n);
    for (int i = 0; i < n; ++i) {
        a[i] = box[i].leftBound();
        b[i] = box[i].rightBound();
        m[i] = (a[i] + b[i]) / 2.0;
    }
    for (size_t mask = 0; mask < outCount; ++mask) {
        IVector child(n);
        for (int i = 0; i < n; ++i) {
            const bool upperHalf = ((mask >> static_cast<size_t>(i)) & 1u) != 0u;
            child[i] = upperHalf ? interval(m[i], b[i]) : interval(a[i], m[i]);
        }
        out.push_back(child);
    }
    return out;
}

inline std::array<IVector, 2> splitBoxLongestDim(const IVector& box) {
    const int n = box.dimension();
    if (n <= 0)
        throw std::runtime_error("EndCover: cannot split a zero-dimensional box");

    int bestDim = -1;
    double bestWidth = -1.0;
    for (int i = 0; i < n; ++i) {
        double width = box[i].rightBound() - box[i].leftBound();
        if (width > bestWidth) {
            bestWidth = width;
            bestDim = i;
        }
    }
    if (bestDim < 0 || bestWidth <= 0.0)
        throw std::runtime_error("EndCover: cannot split a degenerate box");

    const double a = box[bestDim].leftBound();
    const double b = box[bestDim].rightBound();
    const double m = (a + b) / 2.0;
    IVector lo = box;
    IVector hi = box;
    lo[bestDim] = interval(a, m);
    hi[bestDim] = interval(m, b);
    return { lo, hi };
}

struct EndCoverResult {
    std::vector<IVector> cover;        // C: union of outer end-enclosures
    std::vector<IVector> initialBoxes; // leaf initial boxes, aligned with cover
};

// EndCover_f(B0, eps, H) -> C   (paper Section 3.3, the EndCover pseudocode).
//   Q0 is the "working queue" of boxes; it starts as { Init(B0) } with
//   Init(B0).E[0] = B0.  D = { B_i } (the popped boxes that become leaves) is
//   the space-cover of B0; each leaf is handled by one scaffold (endEnc).
//   endEnc(B, eps, p, H) must return (ulB, olB) where olB is an eps-small
//   end-enclosure of End(B, H) and ulB == B iff the scaffold reached time H
//   eps-small without needing a Split.  By default B is subdivided into 2^n
//   boxes; endpoint-refine modes can request the faster longest-dimension
//   bisection instead.
inline EndCoverResult EndCover(
    const IVector& B0,
    double veps,
    double H,
    const std::function<std::pair<IVector, IVector>(const IVector&, double,
                                                    const std::vector<double>&, double)>& endEnc,
    int maxSplits = 2000000,
    double equalTol = 0.0,
    bool splitLongestDimOnly = false) {
    if (maxSplits <= 0)
        throw std::invalid_argument("EndCover: maxSplits must be positive");

    std::vector<IVector> Q0;
    Q0.push_back(B0);

    EndCoverResult result;
    result.cover.reserve(128);
    int splitCount = 0;

    while (!Q0.empty()) {
        IVector B = Q0.back();
        Q0.pop_back();

        const std::vector<double> p = midpoint(B);
        try {
            auto [ulB, olB] = endEnc(B, veps, p, H);
            if (!almostEqualBox(ulB, B, equalTol)) {
                if (++splitCount > maxSplits)
                    throw std::runtime_error("EndCover: exceeded maxSplits");
                if (splitLongestDimOnly) {
                    auto children = splitBoxLongestDim(B);
                    Q0.insert(Q0.end(), children.begin(), children.end());
                } else {
                    auto children = splitBoxAllDims(B);
                    Q0.insert(Q0.end(), children.begin(), children.end());
                }
            } else {
                result.cover.push_back(olB);
                result.initialBoxes.push_back(ulB);
            }
        } catch (const std::exception&) {
            // Enclosure failed (e.g. filib overflow): do not drop the box,
            // force a subdivision and retry on the children.
            if (++splitCount > maxSplits) throw;
            if (splitLongestDimOnly) {
                auto children = splitBoxLongestDim(B);
                Q0.insert(Q0.end(), children.begin(), children.end());
            } else {
                auto children = splitBoxAllDims(B);
                Q0.insert(Q0.end(), children.begin(), children.end());
            }
        }
    }
    return result;
}

} // namespace endcover
