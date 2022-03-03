#ifndef RANK_H_
#define RANK_H_

void FindGreedy();

void Init();

int FindExchange(int const b, std::vector<int> const &A);

int FindFree(std::vector<int> const &B);

int OutArc(int const a, const std::vector<int> L);

void GetDistancesRank();

void BlockFlow();

size_t ExactRank (int N, int (*a)(std::vector<int>),  bool (*b)(std::vector<int>, int), bool (*c)(int));

size_t ApproxRank(int N, int (*a)(std::vector<int>), bool (*b)(std::vector<int>, int), bool (*c)(int), double eps);

#endif