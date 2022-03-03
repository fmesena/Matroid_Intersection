#ifndef SAP_H_
#define SAP_H_

#include "oracles/Base.h"

void okSAP();
void Init();
void Independent_Set_Greedy(Oracle*, Oracle*);
bool BFS_Without_Graph(Oracle*, Oracle*);
size_t SAP(int, Oracle*, Oracle*);

//bool BFS();
//bool Augment();
//void Build_Exchange_Graph();

#endif