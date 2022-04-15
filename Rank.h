#ifndef RANK_H_
#define RANK_H_

#include "oracles/Base.h"
#include "utils.h"

int FindExchange(int, std::vector<int> const &A);
int FindFree(std::vector<int> const &B);
int OutArc(int, std::vector<int> const &B);
void GetDistancesRank();
void BlockFlow();
size_t ExactRank (int, Oracle*, Oracle*);
size_t ApproxRank(int, Oracle*, Oracle*, double eps);

#endif