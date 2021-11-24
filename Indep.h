#ifndef INDEP_H_
#define INDEP_H_

template<typename T>
void DEBUG_VECTOR(const std::vector<T> &v);

template<typename T>
std::vector<T> Slice(std::vector<T> const &v, int m, int n);
void reset(std::vector<int> &v);

void AddEdge(std::vector<int> graph[], int u, int v);

void PrintGraph(std::vector<int> graph[]);

void PrintIndependentSet();

void UpdateIndependentSet();

void PrintCandidates(std::vector<int> c[]);

std::vector<int> RemoveSubset(std::vector<int> S, std::vector<int> A);

void FindGreedy();

void Init();


//int FindExchange(bool (*void)(vector<int>, std::vector<int>, int), int const b, std::vector<int> A);

///void GetDistancesIndep(bool (*Oracle1)(std::vector<int>, int), bool (*Oracle2)(std::vector<int>, int), int *distance, vector<int> std::candidates[]);

//void OnePath(int *distance, std::vector<int> candidates[]);

void AugmentingPaths( bool (*Oracle1)(std::vector<int>, int), bool (*Oracle2)(std::vector<int>, int));


#endif