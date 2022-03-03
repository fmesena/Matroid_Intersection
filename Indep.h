#ifndef INDEP_H_
#define INDEP_H_

void FindGreedy();

void Init();

int FindExchange(bool (*a)(std::vector<int>, std::vector<int>, int), int const b, std::vector<int> const &A);

void GetDistancesIndep(bool (*Oracle1)(std::vector<int>, int), bool (*Oracle2)(std::vector<int>, int), int *distance, vector<int> std::candidates[]);

void OnePath(int *distance, std::vector<int> candidates[]);

void AugmentingPaths( bool (*Oracle1)(std::vector<int>, int), bool (*Oracle2)(std::vector<int>, int));


#endif