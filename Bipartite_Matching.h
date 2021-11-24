#ifndef BIPARTITE_MATCHING_H_
#define BIPARTITE_MATCHING_H_


bool bfs();

bool DFS(int u);

int EdmondsKarp(int V, std::vector<std::vector<int>> &graph);

int HopcroftKarp_();

int HopcroftKarp();

void AddEdge_(int u, int v);


#endif