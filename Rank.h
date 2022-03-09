#ifndef RANK_H_
#define RANK_H_

#include "oracles/Base.h"

int FindExchange(int const, std::vector<int> const &A);
int FindFree(std::vector<int> const &B);
int OutArc(int const, const std::vector<int>);
void GetDistancesRank();
void BlockFlow();
size_t ExactRank (int, Oracle*, Oracle*);
size_t ApproxRank(int, Oracle*, Oracle*, double eps);
void okRank();

#endif