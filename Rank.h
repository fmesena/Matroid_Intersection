#ifndef RANK_H_
#define RANK_H_


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

int FindExchange(bool (*a)(std::vector<int>, int), int const b, std::vector<int> A);

int FindFree(int (*a)(std::vector<int>), std::vector<int> S, std::vector<int> B);

int OutArc(int const a);

int OutArc2(int const a, const std::vector<int> L);

void GetDistancesRank();

void BlockFlow();

size_t ExactRank(int N, int (*a)(std::vector<int>),  bool (*b)(std::vector<int>), bool (*c)(std::vector<int>, int));

size_t ApproxRank(int N, int (*a)(std::vector<int>), bool (*b)(std::vector<int>), bool (*c)(std::vector<int>, int), double eps);

#endif