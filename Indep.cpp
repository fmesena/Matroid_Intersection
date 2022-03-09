#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>
#include <limits>
#include "Indep.h"
#include "utils.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;

#define		M1_Exch(A,b)				O1->Exchangeable_Set(A,b)
#define		M2_Exch(A,b)				O2->Exchangeable_Set(A,b)
#define		Oracle_M1_Free(b)			O1->Free(b)
#define		Oracle_M2_Free(b)			O2->Free(b)

static int exchangeable; //hack

void okIndep() {cout<<"okIndep\n";}

int FindExchange(Oracle* oracle, int const b, vector<int> &A) {

	if (A.empty()) { cerr << "A is empty\n"; return -1; }
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


void GetDistancesIndep(int *distances, vector<int> candidates[]) {

	int l = 0;
	int marked = 0;

	DISTANCE_TARGET = numeric_limits<int>::max();
	vector<int> updated_l_plus_one;

	// unnecessary, already done along the execution of GetDistancesIndep- CHECK THIS
	/*for (int i = 0; i < N; ++i)
		candidates[distances[i]].push_back(i);*/

	while (l >= 0)
	{
		cout << "\nMarked: " << marked << endl;
		cout << "DISTANCE_TARGET: " << DISTANCE_TARGET << endl;
		cout << "Layer: " << l << endl << endl;
		
		PrintCandidates(candidates);

		updated_l_plus_one.clear();
		if (odd(l))
		{	
			if (candidates[l+1].empty())
			{
				cout << "\nEND IN ODD" << endl;
				if (marked == N) { cout << "TARGET REACHED: " << l+1 << endl; DISTANCE_TARGET = min(DISTANCE_TARGET,l+1);} 
				return;
			}

			int a;
			vector<int> Q = candidates[l+1];
			DEBUG_VECTOR(Q);

			for (int b : candidates[l])
			{
				/*
				if (IN_INDEPENDENT(b))
				{
					candidates[l+1].push_back(b);
					continue;
				}*/
				cout << "\nVisiting " << b << endl;
				if (Q.empty()) { cout << "Q.empty()" << endl; break;} // irrelevant but why not

				while ( (a = FindExchange(O2, b, Q)) != -1)
				{
					cout << "\nArc from " << b << " to " << a << endl;
					
					distances[a] = l+1;
					marked++;
					
					swap(Q[exchangeable], Q.back());
					Q.pop_back();

					updated_l_plus_one.push_back(a);
				}
			}
			candidates[l+1] = updated_l_plus_one; 			 // L_l+1 <- L_l+1 - Q
			for (int q : Q)	  candidates[l+3].push_back(q);	 // L_l+3 <- L_l+3 + Q
		}

		else
		{

			if (candidates[l+1].empty())
			{
				cout << "\nEND IN EVEN" << endl;
				if (marked == N) cout << "\nVISITED ALL NODES || TARGET UNREACHABLE\n" << endl;
				return;
			}

			bool arc;
			DEBUG_VECTOR(candidates[l+1]);
			for (int b : candidates[l+1])
			{
				arc = l==0 ? (O1->Free(b) && !IN_INDEPENDENT(b)) : (FindExchange(O1, b, candidates[l]) != -1);
				cout << "l: " << l << " arc: " << arc << endl;
				if (arc)
				{
					cout << "arc to " << b << endl;
					distances[b] = l+1;
					updated_l_plus_one.push_back(b);
					marked++;
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


void OnePath(int *distances, vector<int> candidates[]) {
	assert(even(DISTANCE_TARGET));
	
	int l = DISTANCE_TARGET;
	int a = TARGET;
	bool arc;
	vector<int> Q;

	while (l>0)
	{
		cout << "\nWhile com l=" << l << endl;
		Q = candidates[l-1];
		DEBUG_VECTOR(candidates[l-1]);
		arc = false;

		int b;

		for (int i=0; i < candidates[l-1].size(); i++)
		{
			
			b = candidates[l-1][i];

			if (a == TARGET)
			{
				if (O2->Free(b))
				{
					cout << "setting " << b << " to true" << endl;
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
						in_independent_set[a] = true;
						in_independent_set[b] = false;
						arc=true;
					}
				}
				else if (even(l))
				{
					if (O2->Exchangeable(a,b))
					{
						in_independent_set[a] = false;
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
	return;
}


void AugmentingPaths(int N_, Oracle* O1_, Oracle* O2_) {

	cout << "Indep alive\n";

	N  = N_;
	O1 = O1_;
	O2 = O2_;

	Init(N);
	// Remark: the shortest path from SOURCE to TARGET has at most 2*(SZ+1) arcs.
	vector<int> candidates[200];
	//maybe  vector< vector<int> > e no final de cada iteraçao faço 2x. push_back de vector<int>

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

	//-----------------------------------------------------------//

	cout << "Starting AugmentingPaths" << endl;

	while (true)
	{
		GetDistancesIndep(distances,candidates);
		cout << "DistancesIndep finished. Target reached with distance " << DISTANCE_TARGET << endl;

		if (DISTANCE_TARGET < numeric_limits<int>::max())
		{
			OnePath(distances,candidates);
			UpdateIndependentSet();
			PrintIndependentSet();
			cout << "\ndistances:"<<endl;
			for (int i=0; i<N; i++) cout << distances[i] << " ";
			cout << endl << endl;
		}
		else break;
	}

	cout << "Finished AugmentingPaths" << endl;

	delete[] distances;
	//TODO delete[][] candidates;

	return;
}