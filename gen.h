#ifndef GEN_H_
#define GEN_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cassert>
#include "ds/Edge.h"

std::pair<std::vector<std::vector<int>>, std::vector<Edge>> GenerateGraph_Matchings(int);
std::vector<std::vector<int>> Generate_Arborescence(int, int);
void okGEN();
bool assertArb(std::vector<Edge>, std::vector<std::vector<int>>);
bool assertGraphic(std::vector<Edge>, std::vector<std::vector<int>>, std::vector<std::vector<int>>);
bool assertMatching(std::vector<Edge>);

#endif