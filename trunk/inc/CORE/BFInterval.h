#ifndef BFINTERVAL_H
#define BFINTERVAL_H

#include <vector>

CORE_BEGIN_NAMESPACE

typedef long extLong;
typedef std::pair<BigFloat, BigFloat> BFInterval;
typedef std::vector<BFInterval> BFVecInterval;

CORE_END_NAMESPACE

#endif
