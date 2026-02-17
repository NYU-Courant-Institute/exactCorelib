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
