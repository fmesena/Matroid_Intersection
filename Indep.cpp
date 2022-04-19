#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include "Indep.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;
static int exchangeable; //hack


int FindExchange(Oracle* oracle, int const b, vector<int> &A) {

	if (A.empty()) { cout << ">>ERROR: Indep::FindExchange, A is empty" << endl; return -1; }
	if (!oracle->Exchangeable_Set(A,b)) return -1;

	int M;
	int L=0;
	int R=A.size()-1;

	exchangeable = 0;
	vector<int> A_;

	while (L < R)
	{	
		M = L + (R-L)/2;      // (L+R)/2
		A_ = Slice(A, L, M);  // this takes O(|A|) operations overall
		
		if (oracle->Exchangeable_Set(A_,b)) // is S-A+b independent?
			R = exchangeable = M;
		else
			L = exchangeable = M+1;
	}
	return A[exchangeable];
}


void GetDistancesIndep() {

	int l = 0;
	DISTANCE_TARGET = numeric_limits<int>::max();
	vector<int> updated_l_plus_one;

	while (l >= 0)
	{
		updated_l_plus_one.clear();
		
		if (odd(l))
		{
			if (candidates[l].empty()) return;
			for (int b : candidates[l]) {
				if (O2->Free(b)) { 
					DISTANCE_TARGET=l+1;
					return;
				}
			}

			if (candidates[l+1].empty())
				return;

			int a;
			vector<int> Q = candidates[l+1];

			for (int b : candidates[l])
			{
				if (Q.empty()) break;
				while ( (!Q.empty()) && (a = FindExchange(O2, b, Q)) != -1)
				{
					distances[a] = l+1;

					swap(Q[exchangeable], Q.back());
					Q.pop_back();

					updated_l_plus_one.push_back(a);
				}
			}
			candidates[l+1] = updated_l_plus_one; 			 // L_l+1 <- L_l+1 - Q
			for (int q : Q)   candidates[l+3].push_back(q);	 // L_l+3 <- L_l+3 + Q, not done in O(1) :(
		}
		else //even(l)
		{
			bool arc;
			if (candidates[l+1].empty() || (candidates[l].empty() && l!=0))
				return;
			for (int b : candidates[l+1])
			{
				if (l==0)
					arc = O1->Free(b);
				else
					arc = O1->Exchangeable_Set(candidates[l],b);
				if (arc==true)
				{
					distances[b] = l+1;
					updated_l_plus_one.push_back(b);
				}
				else 
					candidates[l+3].push_back(b);
			}
			candidates[l+1] = updated_l_plus_one;
		}
		l++;
	}
	return;
}


void OnePath() {
	assert(even(DISTANCE_TARGET));
	assert(candidates[0].size()==0);
	int  a 	= TARGET;
	int  l 	= DISTANCE_TARGET;
	int  b;
	bool arc;
	vector<int> Q;

	while (l>0)
	{
		Q 	= candidates[l-1];
		arc = false;

		for (size_t i=0; i < candidates[l-1].size(); i++) //when l=1 we do not enter the loop since any element with distane 1 is free in M1
		{
			
			b = candidates[l-1][i];

			if (a == TARGET)
			{
				if (O2->Free(b))
				{
					in_independent_set[b] = true;
					arc=true;
				}
			}

			else
			{
				if (odd(l))
				{
					if (O1->Exchangeable(b,a))
					{
						in_independent_set[b] = false;
						arc=true;
					}
				}
				else if (even(l))
				{
					if (O2->Exchangeable(a,b))
					{
						in_independent_set[b] = true;
						arc=true;		
					}
				}
			}

			if (arc)
			{
				swap(candidates[l-1][i], candidates[l-1].back());
				candidates[l-1].pop_back(); //
				candidates[l].push_back(b);	/*if b was in S, then it entered V\S. if b was in V\S, then it entered S. in both cases, its new distance is at least its current distance plus one*/
				distances[b]++;				//
				a=b;
				break;
			}
		}
		l--;
	}
	// when l is odd,  S - b + a_l \in I_1
	// when l is even, S - a_l + b \in I_2
	UpdateIndependentSet();
	O1->Update_State(independent_set);
	O2->Update_State(independent_set);
	return;
}

size_t AugmentingPaths(int N_, Oracle* O1_, Oracle* O2_) {

	N  = N_;
	O1 = O1_;
	O2 = O2_;

	if (N==0) return 0;

	Init(N);

	//initial distance lower bounds
	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i))
		{
			distances[i] = 2;
			candidates[2].push_back(i);
		}
		else
		{
			distances[i] = 1;
			candidates[1].push_back(i);
		}
	}

	while (true)
	{
		GetDistancesIndep();
		if (DISTANCE_TARGET < numeric_limits<int>::max())
			OnePath();
		else
			break;
	}

	delete[] distances;
	/*for (int i = 0; i < ?; i++) {
        delete[] candidates[i];
    }
    delete[] candidates;*/

	return SZ;
}