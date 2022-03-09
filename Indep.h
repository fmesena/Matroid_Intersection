#ifndef INDEP_H_
#define INDEP_H_

#include "oracles/Base.h"

int FindExchange(Oracle*, int const, std::vector<int> const &A);
void GetDistancesIndep(int*, std::vector<int> candidates[]);
void OnePath(int*, std::vector<int> candidates[]);
void AugmentingPaths(Oracle*, Oracle*);
void okIndep();

#endif