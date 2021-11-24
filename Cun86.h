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

void FindGreedy();

void Init();

void Build_Exchange_Graph();

bool BFS();

int FindArc(int const a);

bool Augment();

size_t Cun86(int N_, bool (*a)(std::vector<int>), bool (*b)(std::vector<int>, int), bool (*c)(std::vector<int>, int, int),
			 		 bool (*d)(std::vector<int>), bool (*e)(std::vector<int>, int), bool (*f)(std::vector<int>, int, int));


#endif