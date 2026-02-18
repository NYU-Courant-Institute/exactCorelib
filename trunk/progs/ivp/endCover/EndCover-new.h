/* file: EndCover-new.h
 *
 *      Purpose:
 *              Implements a simple subdivision-based covering routine (EndCover)
 *              for validated end-enclosures.  Given an initial interval box B0,
 *              a tolerance veps, and a final time H, it repeatedly calls a user-
 *              supplied enclosure routine endEnc(B, veps, p, H) and subdivides
 *              B whenever the enclosure indicates it is not yet "resolved" or
 *              whenever endEnc throws an exception.
 *
 *      Core idea:
 *              Maintain a worklist S0 of boxes to be processed.
 *              For each box B in S0:
 *                (1) Let p = midpoint(B) (a representative point).
 *                (2) Call endEnc(B, veps, p, H) which returns (ulB, olB):
 *                        ulB : "inner / updated" box (used as a progress signal)
 *                        olB : outer end-enclosure at time H for initial set B
 *                (3) Always append olB to the output cover.
 *                (4) If ulB differs from B (within equalTol), split B in all
 *                    dimensions into 2^n children and push them back to S0.
 *                (5) If endEnc throws, do NOT abort and do NOT drop B; instead
 *                    force subdivision and retry on the children.
 *
 *      Main types:
 *
 *              EndCoverResult:
 *                      - cover : vector<IVector> storing the union (as a list of boxes)
 *                                of outer end-enclosures produced by endEnc.
 *
 *      Main routines:
 *
 *              midpoint(box):
 *                      Returns the coordinate-wise midpoint vector<double>.
 *
 *              almostEqualBox(a, b, tol):
 *                      Compares two interval boxes by endpoint differences
 *                      (|aL-bL| and |aR-bR|), with tolerance tol.
 *
 *              splitBoxAllDims(box):
 *                      Splits a box in every dimension at its midpoint, producing
 *                      2^n child boxes (cartesian product of lower/upper halves).
 *
 *              EndCover(B0, veps, H, endEnc, maxSplits=..., equalTol=...):
 *                      Runs the cover construction described above.  The split
 *                      counter is global across the run; exceeding maxSplits
 *                      raises an exception.
 *
 *      Required callback:
 *
 *              endEnc must have signature:
 *                  std::pair<IVector,IVector> endEnc(const IVector& B,
 *                                                   double veps,
 *                                                   const std::vector<double>& p,
 *                                                   double H);
 *              and returns (ulB, olB).
 *
 *      Notes / cautions:
 *              - The output "cover" is a list of boxes; it is not simplified by
 *                merging, intersection pruning, or redundancy removal.
 *              - Subdivision is exponential in dimension (2^n children per split).
 *              - The rule "split when ulB != B" is a protocol between EndCover
 *                and endEnc; it assumes endEnc uses ulB as a progress indicator.
 *              - On exceptions in endEnc, EndCover forces subdivision rather than
 *                dropping boxes; this is robust but may cause many splits.
 *              - almostEqualBox compares endpoints only; it does not reason about
 *                set containment when rounding effects occur.
 *
 *      Dependencies:
 *              - CAPD library: capd/capdlib.h
 *              - Project core definitions: calD-calQ-new.h (IVector, helpers, etc.)
 *
 *      Author: <Bingwei Zhang and Chee Yap>  (<Feb 2026>)
 */


#pragma once

#include <vector>
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
  for (int i = 0; i < n; ++i) {
    const double a = box[i].leftBound();
    const double b = box[i].rightBound();
    mid.push_back((a + b) / 2.0);
  }
  return mid;
}

inline bool almostEqualBox(const IVector& a, const IVector& b, double tol = 0.0) {
  const int n = a.dimension();
  if (n != b.dimension()) return false;
  for (int i = 0; i < n; ++i) {
    const double aL = a[i].leftBound();
    const double aR = a[i].rightBound();
    const double bL = b[i].leftBound();
    const double bR = b[i].rightBound();
    if (std::abs(aL - bL) > tol || std::abs(aR - bR) > tol) return false;
  }
  return true;
}

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

struct EndCoverResult {
  std::vector<IVector> cover; // S (union of outer end-enclosures)
};

// EndCover_f(B0, veps, H) -> S
// The callable `endEnc` must behave like:
//   std::pair<IVector, IVector> endEnc(const IVector& B,
//                                     double veps,
//                                     const std::vector<double>& p,
//                                     double H);
// where it returns (ulB, olB).
inline EndCoverResult EndCover(
    const IVector& B0,
    double veps,
    double H,
    const std::function<std::pair<IVector, IVector>(const IVector&, double, const std::vector<double>&, double)>& endEnc,
    int maxSplits = 200000,
    double equalTol = 0.0) {
  if (maxSplits <= 0) {
    throw std::invalid_argument("EndCover: maxSplits must be positive");
  }

  std::vector<IVector> S0;
  S0.push_back(B0);

  EndCoverResult result;
  result.cover.reserve(128);

  int splitCount = 0;

  while (!S0.empty()) {
    IVector B = S0.back();
    S0.pop_back();

    const std::vector<double> p = midpoint(B);
    try {
      auto [ulB, olB] = endEnc(B, veps, p, H);

      result.cover.push_back(olB);

      if (!almostEqualBox(ulB, B, equalTol)) {
        if (++splitCount > maxSplits) {
          throw std::runtime_error("EndCover: exceeded maxSplits; refine maxSplits or adjust endEnc");
        }

        auto children = splitBoxAllDims(B);
        S0.insert(S0.end(), children.begin(), children.end());
      }
    } catch (const std::exception&) {
      // If enclosure failed (e.g., filib overflow), do NOT abort and do NOT drop the box.
      // Instead, force subdivision and retry on children.
      if (++splitCount > maxSplits) {
        throw;
      }
      auto children = splitBoxAllDims(B);
      S0.insert(S0.end(), children.begin(), children.end());
    }
  }

  return result;
}

} // namespace endcover
