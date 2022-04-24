#ifndef CLSSW_INDEP_H_
#define CLSSW_INDEP_H_

#include "oracles/Base.h"
#include "utils.h"

int 	FindExchange(Oracle*, int const, std::vector<int> const &A);
void 	GetDistancesIndep();
void 	OnePath();
size_t  AugmentingPaths(int, Oracle*, Oracle*);

#endif