#ifndef UFDS_H_
#define UFDS_H_

template<typename T>
void DEBUG_VECTOR(const std::vector<T> &v);

template<typename T>
std::vector<T> Slice(std::vector<T> const &v, int m, int n);
void reset(std::vector<int> &v);

void AddEdge(std::vector<int> graph[], int u, int v);

void PrintGraph(std::vector<int> graph[]);


#endif