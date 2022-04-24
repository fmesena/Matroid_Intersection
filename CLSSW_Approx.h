#ifndef CLSSW_APPROX_H_
#define CLSSW_APPROX_H_

#include "oracles/Base.h"
#include "utils.h"

void	Refine();
size_t  AugmentingPathsApprox(int, Oracle*, Oracle*, double);

#endif