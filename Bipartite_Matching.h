#ifndef BIPARTITE_MATCHING_H_
#define BIPARTITE_MATCHING_H_

#include <stdlib.h>
#include<vector>

int bfsEK(std::vector<int>& parent);
bool DFS(int u);
int EdmondsKarp(int, std::vector<std::vector<int>>);
int HopcroftKarp_();
int HopcroftKarp();
void AddEdge_(int u, int v);
void okBM();

#endif