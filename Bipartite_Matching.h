#ifndef BIPARTITE_MATCHING_H_
#define BIPARTITE_MATCHING_H_

#include <stdlib.h>
#include<vector>

using namespace std;

int bfsEK(vector<int>& parent);
bool DFS(int u);
int EdmondsKarp(int, vector<vector<int>>);
int HopcroftKarp_();
int HopcroftKarp();
void AddEdge_(int u, int v);
void okBM();

#endif