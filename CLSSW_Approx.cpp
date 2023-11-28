#include <iostream>
#include <algorithm>
#include <vector>
#include "CLSSW_Approx.h"

static Oracle* O1;
static Oracle* O2;
static int l;


/*
We maintain three types of elements in each layer:
	-selected elements denoted as Ak,Bk;
	-removed elements denoted as Rk;
	-fresh elements Fk.

The selected elements Ak \subseteq D2k, Bk \subseteq D2k−1 form a partial augmenting set, and our algorithm incrementally finds better and better partial
augmenting sets with larger B_l+1.
The removed elements Rk \subseteq Dk are deemed to be useless for finding better partial augmenting sets.
Finally, fresh elements Fk \subseteq Dk are those that are neither selected nor removed.

The type of an element can change from fresh to selected and from selected to removed but never the other way.
*/


void Refine1(int k) {

}

void Refine2(int k) {

}

void Refine() {
	for (int k=1; k<=l; k++)
	{
		Refine1(k);
		Refine2(k);
	}
	Refine1(0);
}

static void GetDistancesIndep() {
	return;
}

size_t AugmentingPathsApprox(int N_, Oracle* O1_, Oracle* O2_, double eps) {
	N  = N_;
	O1 = O1_;
	O2 = O2_;

	//First, compute the distances
	GetDistancesIndep();
	l = DISTANCE_TARGET/2 - 1;

	vector<vector<int>> B(N);
	vector<vector<int>> F(N);

	//Find B1 \subseteq \overline{S} such that S+B1 is independent in M1
	for (int e : not_independent)
		if (O1->Free(e))
			B[1].push_back(e);
		
	//F[1] = \emptyset;
	//R[1] = \emptyset;
	//A[0] = R[0] = F[0] = candidates[0] = \emptyset;
	//A[l+1] = R[2l+2] = F[2l+2] = candidates[2l+2] = \emptyset;
	for (int k=2; k<=2*l+1; k++)
	{
		//A[k] = B[k] = R[k] = \emptyset
		F[k] = candidates[k];
	}

	int 	i 	= 0;
	size_t 	s 	= 0;

	while ( i++ < (int)(1/eps))
	{	
		s=SZ;
		Refine();
		if (s==SZ) break;
	}

	return 0;
}
