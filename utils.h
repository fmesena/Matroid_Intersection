#ifndef UTILS_H_
#define UTILS_H_


template<typename T>
void DEBUG_VECTOR(const std::vector<T> &v);

template<typename T>
std::vector<T> Slice(std::vector<T> const &v, int m, int n);
void reset(std::vector<int> &v);

void addEdge(std::vector<int> graph[], int u, int v);

void printGraph(std::vector<int> graph[]);

void PrintIndependentSet();

bool equal_content_same_order(vector<int> &a, vector<int> &b);

void PrintCandidates(vector<int> c[]);

void UpdateIndependentSet();

vector<int> RemoveSubset(vector<int> S, vector<int> A);


#endif