#ifndef GEN_H_
#define GEN_H_

#include <stdlib.h>
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <cassert>
#include <math.h>
#include "ds/Edge.h"

std::pair<std::vector<std::vector<int>>, std::vector<Edge>> Generate_BipartiteGraph(int);
std::vector<std::vector<int>> Generate_Arborescence(int, int);
std::pair<int,std::vector<Edge>> Generate_SimpleGraph(int);
void assertArb(std::vector<Edge>, std::vector<std::vector<int>>,std::string);
void assertGraphic(std::vector<Edge>, std::string);
void assertMatching(std::vector<Edge>,int,std::string);

#endif