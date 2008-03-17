#ifndef BFINTERVAL_H
#define BFINTERVAL_H

#include <vector>

CORE_BEGIN_NAMESPACE

typedef long extLong;
typedef std::pair<BigFloat, BigFloat> BFInterval;
typedef std::vector<BFInterval> BFVecInterval;

// global constant:
const BFInterval INVALID_BFInterval(1,0);

CORE_END_NAMESPACE

#endif
