#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include "Indep.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;
static int exchangeable; //hack


void okIndep() {cout<<"okIndep\n";}


int FindExchange(Oracle* oracle, int const b, vector<int> &A) {

	if (A.empty()) { cout << "A is empty\n"; return -1; }
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
		//cout << "Layer: " << l << endl << endl;
		//PrintCandidates(N+5,candidates);

		updated_l_plus_one.clear();
		
		if (odd(l))
		{	
			if (candidates[l].empty()) return;
			for (int b : candidates[l]) if (O2->Free(b)) { /*cout << "FREE M2\n";*/ DISTANCE_TARGET=min(DISTANCE_TARGET,l+1);}

			if (candidates[l+1].empty())
				return;

			int a;
			vector<int> Q = candidates[l+1];

			for (int b : candidates[l])
			{
				if (Q.empty()) break;
				//cout << "SIZE: " << Q.size() << " " << l << endl;
				while ( (!Q.empty()) && (a = FindExchange(O2, b, Q)) != -1)
				{
					//cout << "arc M2" << endl;
					
					distances[a] = l+1;

					swap(Q[exchangeable], Q.back());
					Q.pop_back();

					updated_l_plus_one.push_back(a);
				}
			}
			candidates[l+1] = updated_l_plus_one; 			 // L_l+1 <- L_l+1 - Q
			for (int q : Q)   candidates[l+3].push_back(q);	 // L_l+3 <- L_l+3 + Q, not done in O(1) :(
		}
		//even(l)
		else
		{
			if (candidates[l].empty() && l!=0) cerr << "even layer candidates[l] is empty " << l << endl;
			bool arc;
			for (int b : candidates[l+1])
			{
				bool x=l==0;
				//cout << "SIZE: " << candidates[l].size() << " " << l << endl;
				arc = x ? O1->Free(b) : FindExchange(O1, b, candidates[l]) != -1; // O1->Exchangeble_Set(candidates[l],b);
				cout << arc << " " << b << " " << x << endl;
				if (arc==1)
				{
					distances[b] = l+1;
					updated_l_plus_one.push_back(b);
					//cout << "arc M1\n";
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
	//cout << "OnePath begin\n";
	int b;
	bool arc;
	int a = TARGET;
	int l = DISTANCE_TARGET;
	vector<int> Q;

	while (l>0)
	{
		//cout << "\nWhile com l=" << l << endl;
		Q = candidates[l-1];
		DEBUG_VECTOR(candidates[l-1]);
		arc = false;

		for (int i=0; i < candidates[l-1].size(); i++) //when l=1 we do not enter the loop since any element with distane 1 is free in M1
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
	PrintCandidates();
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

	Init(N);

	//distance lower bounds
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
		GetDistancesIndep();
		//cout << "DistancesIndep finished. Target reached with distance " << DISTANCE_TARGET << endl;

		if (DISTANCE_TARGET < numeric_limits<int>::max())
		{
			OnePath();
			//PrintIndependentSet();
			//cout << "\ndistances:"<<endl;
			//for (int i=0; i<N; i++) cout << distances[i] << " ";
			//cout << endl << endl;
		}
		else break;
	}

	//cout << "Finished AugmentingPaths" << endl;

	delete[] distances;
	/*for (int i = 0; i < ?; i++) {
        delete[] candidates[i];
    }
    delete[] candidates;*/

	return SZ;
}