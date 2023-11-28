#ifndef CLSSW_APPROX_H_
#define CLSSW_APPROX_H_

#include "oracles/Base.h"
#include "utils.h"

void	Refine();
void	Refine1(int);
void	Refine2(int);
size_t  AugmentingPathsApprox(int, Oracle*, Oracle*, double);

#endif