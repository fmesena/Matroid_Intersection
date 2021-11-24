#ifndef SAP_H_
#define SAP_H_

template<typename T>
void DEBUG_VECTOR(const std::vector<T> &v);

template<typename T>
std::vector<T> Slice(std::vector<T> const &v, int m, int n);
void reset(std::vector<int> &v);

void Init();

void AddEdge(int u, int v);

void PrintGraph();

void PrintIndependentSet();

bool BFS_Without_Graph();

bool BFS();

bool Augment();

size_t SAP(int N, bool (*a)(std::vector<int>), bool (*d)(std::vector<int>));

void Build_Exchange_Graph();


#endif