#ifndef SAP_H_
#define SAP_H_

#include "oracles/Base.h"
#include "utils.h"

void okSAP();
bool BFS_Augment();
size_t SAP(int, Oracle*, Oracle*);

//bool BFS();
//bool Augment();
//void Build_Exchange_Graph();

#endif