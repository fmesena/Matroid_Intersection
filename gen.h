#ifndef GEN_H_
#define GEN_H_

#include <stdlib.h>
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <cassert>
#include <random>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "ds/Edge.h"

std::pair<std::vector<std::vector<int>>, std::vector<Edge>> GenerateRandomBipartiteGraph(int,int);
std::pair<std::vector<std::vector<int>>, std::vector<Edge>> GenerateBlocksBipartiteGraph(int,int);
std::pair<std::vector<std::vector<int>>, std::vector<Edge>> GeneratePHPGraph(int);
std::vector<std::vector<int>> Generate_Arborescence(int,int);
std::pair<int,std::vector<Edge>> GenerateMultiGraph(int);
void assertArb(std::vector<Edge>, std::vector<std::vector<int>>,std::string);
void assertGraphic(std::vector<Edge>, std::string);
void assertMatching(std::vector<Edge>,int,std::string);

#endif