#ifndef CUN86_H_
#define CUN86_H_

#include "oracles/Base.h"
#include "utils.h"

void okCun();
bool GetDistances();
int  FindArc(int const);
void Augment();
size_t Cun86(int, Oracle*, Oracle*);

#endif