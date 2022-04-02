#ifndef GEN_H_
#define GEN_H_

#include <stdlib.h>
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <cassert>
#include "ds/Edge.h"

std::pair<std::vector<std::vector<int>>, std::vector<Edge>> Generate_BipartiteGraph(int);
std::vector<std::vector<int>> Generate_SimpleGraph(int, int);
std::vector<std::vector<int>> Generate_Arborescence(int, int);
void assertArb(std::vector<Edge>, std::vector<std::vector<int>>,std::string);
void assertGraphic(std::vector<Edge>, std::vector<std::vector<int>>, std::vector<std::vector<int>>,int,std::string);
void assertMatching(std::vector<Edge>,int,std::string);

#endif