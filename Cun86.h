#ifndef CUN86_H_
#define CUN86_H_

#include "oracles/Base.h"
#include "utils.h"

bool 	GetDistances();
int  	FindArc(int const);
void 	Augment();
size_t 	Cun86(int, Oracle*, Oracle*);
size_t 	ApproxCun86(int N_, Oracle* O1_, Oracle* O2_, double eps);

#endif