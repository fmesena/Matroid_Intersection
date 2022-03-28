#ifndef INDEP_H_
#define INDEP_H_

#include "oracles/Base.h"
#include "utils.h"

void okIndep();
int FindExchange(Oracle*, int const, std::vector<int> const &A);
void GetDistancesIndep();
void OnePath();
size_t AugmentingPaths(int, Oracle*, Oracle*);

#endif