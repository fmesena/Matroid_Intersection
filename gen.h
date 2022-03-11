#ifndef GEN_H_
#define GEN_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
#include "ds/Edge.h"

using namespace std;

pair<vector<vector<int>>, vector<Edge>> GenerateGraph_Matchings(int);
vector<vector<int>> Generate_Arborescence(int, int);
void okGEN();
bool assertArb(vector<Edge>, vector<vector<int>>);
bool assertGraphic(vector<Edge>, vector<vector<int>>, vector<vector<int>>);
bool assertMatching(vector<Edge>);

#endif