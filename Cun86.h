#ifndef CUN86_H_
#define CUN86_H_

#include "oracles/Base.h"

void okCun();
void Build_Exchange_Graph();
bool BFS();
int  FindArc(int const);
bool Augment();
size_t Cun86(int, Oracle*, Oracle*);

#endif