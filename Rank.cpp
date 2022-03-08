#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <assert.h>
#include <limits>
#include <chrono>
#include "Rank.h"
#include "utils.h"

using namespace std;
using namespace std::chrono;

static Oracle* O1;
static Oracle* O2;

#define		M1_Rank(X)					O1->Rank(X)
#define		M2_Exch(A,b)				O2->Exchangeable_Set(A,b)
#define		Oracle_M2_Free(b)			O2->Free(b)

static int exchangeable; //hack
static int free_;		  //hack
bool TARGET_IN_B;
vector<int> B_S;
vector<int> B_not_S;


int FindExchange(int const b, vector<int> const &A) {

	if (A.empty()) { cerr << "A is empty\n"; return -1; } // if A is empty, then the oracle essentially tests whether b is free or not
	if (!M2_Exch(A,b)) return -1;

	int M;
	int L=0;
	int R=A.size()-1;

	exchangeable = 0;
	vector<int> A_;

	while (L < R)
	{	
		M = L + (R-L)/2;      // (L+R)/2
		A_ = Slice(A, L, M);  // this takes O(|A|) operations overall
		
		if (M2_Exch(A_,b)) // is S-A+b independent?
 			R = exchangeable = M;
		else
			L = exchangeable = M+1;
	}
	return A[exchangeable];
}


int FindFree(vector<int> const &B) {
	
	if (B.empty()) { cerr << "B is empty\n"; return -1; }

	if (M1_Rank(B) == CURRENT_RANK) return -1;

	int L=0;
	int R=B.size()-1;
	int M;

	vector<int> B_;
	free_ = 0;

	while (L < R)
	{
		M = L + (R-L)/2;
		B_ = Slice(B,L,M);		// this takes O(|B|) operations in total

		if (M1_Rank(B_) > CURRENT_RANK)
			R = free_ = M;
		else
			L = free_ = M+1;
	}
	return B[free_];
}


// DANGER: alterar independent_set sem alterar index_
// TODO: work with addresses: vector<int> const &B
int OutArc(int const a, const vector<int> B) {
	vector<int> S;

	if (a == SOURCE) return FindFree(B); // M1_Rank 
	
	S = independent_set;
	if (IN_INDEPENDENT(a))
	{	
		// need to check this for the BlockFlow routine:
		// se nao quiseres fazer a cópia da linha S=independent_set e quiseres usar diretamente o independent_set, não te esqueças de atualizar o index_
		assert(S[index_[a]]!=-1);
		swap(S[index_[a]], S.back());
		S.pop_back();
		return FindFree(B); // M1_Rank
	}

	if (!IN_INDEPENDENT(a) && TARGET_IN_B && Oracle_M2_Free(a)) { TARGET_IN_B=false; return TARGET; }

	if (!IN_INDEPENDENT(a)) return FindExchange(a, B); // M2_Exch 

	cerr << "Something went wrong.\n"; //throw BadOraclesException(a,B);
	return -1; // signals "FALSE". in theory, the code should never reach this line
}


void GetDistancesRank() {

	B_S.clear();
	B_not_S.clear();

	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i)) B_S.push_back(i);
		else B_not_S.push_back(i);
	}

	for (int i=0; i<N; i++) distances[i] = numeric_limits<int>::max(); //FIXME INT_MAX
	DISTANCE_TARGET = numeric_limits<int>::max(); //FIXME INT_MAX
	for (int i=0; i<N; i++) cout << distances[i] << " "; //FIXME INT_MAX
	cout << endl;

	TARGET_IN_B = true;

	queue<int> q;
	q.push(SOURCE);

	int a;
	int b;
	vector<int> B;

	cout << "Starting BFS" << endl;

	while(!q.empty())	// Remark: B.pop_back() "funciona como" visited[i] = true numa BFS simples
	{
		a = q.front(); q.pop();
		cout << "Expanding " << a << endl;

		// Inefficient, work with pointers
		if (a==SOURCE) B = B_not_S;
		else if (IN_INDEPENDENT(a))  B = B_not_S;
		else if (!IN_INDEPENDENT(a)) B = B_S;

		while ( (b = OutArc(a,B)) != -1 )
		{
			DEBUG_VECTOR(B_S);
			DEBUG_VECTOR(B_not_S);
			cout << "Found arc to " << b << endl;
			// Although we have reached the target, we don't break the loop since we want to compute the distances to ALL the vertices for the BlockFlow
			// (i think we could break but whatever)
			if (b==TARGET)
			{
				DISTANCE_TARGET = distances[a]+1;
				TARGET_IN_B = false;
				continue;
			}
			
			distances[b] = a==SOURCE? 1:distances[a]+1;
			q.push(b);

			/*
			if (distance[b] > DISTANCE_TARGET) halt
			*/

			if (a==SOURCE) // || IN_INDEPENDENT(a))  >>> watch out segfault <<<
			{
				swap(B.back(), B[free_]);
				B.pop_back();
				B_not_S = B;
			}
			else if (IN_INDEPENDENT(a))
			{
				swap(B.back(), B[free_]);
				B.pop_back();
				B_not_S = B;
			}
			else if (!IN_INDEPENDENT(a))
			{
				swap(B.back(), B[exchangeable]);
				B.pop_back();
				B_S = B;
			}
		}
	}
	//cout << "\nDistances:\n";
	//for (int i = 0; i < N; ++i) {cout << distances[i] << " ";} cout << endl;
	return;
}


// An iterative DFS-like routine
void BlockFlow() {

	cout << "Computing distances..." << endl;
	GetDistancesRank();
	for (int i=0; i<N; i++) cout << distances[i] << " "; //FIXME INT_MAX
	cout << endl;

	// Target is not reachable
	if (DISTANCE_TARGET == numeric_limits<int>::max()) { cout << "Target unreachable\n"; return; }

	// TODO: update candidates along the execution of GetDistancesRank
	vector<int> candidates[2*SZ+3]; //FIXME magic numbers, 2*SZ+3 is just a safe upper bound on the number of distinc distancess
	for (int i = 0; i < N; ++i)
		if (distances[i] != numeric_limits<int>::max())
			candidates[distances[i]].push_back(i);

	candidates[0].push_back(SOURCE);
	candidates[DISTANCE_TARGET].push_back(TARGET);

	int current = SOURCE;
	int l = 0;
	int k;
	TARGET_IN_B = true;
	vector<int>  path;
	vector<int>  indexes;
	vector<int>  prev_element;

	//PrintCandidates(candidates);

	int augmenting_paths_count = 0;

	cout << "Beginning BlockFlow with DISTANCE_TARGET equal to " << DISTANCE_TARGET << endl;

	while (l>=0)
	{
		if (l<DISTANCE_TARGET)
		{	
			// i think this condition is useless
			if (candidates[l].empty()) {
				cout << "not useless after all...\n";
        		return;
      		}
      		cout << "Current:" << current << ", at level " << l << endl;
			int next = OutArc(current, candidates[l+1]);
			cout << "Next:" << next << endl;
			if (next == -1)
			{
				if (current != SOURCE)
				{
					swap(candidates[l].back(), candidates[l][indexes.back()]);
					candidates[l].pop_back();
					
					current = prev_element.back();

					path.pop_back();
					indexes.pop_back();
					prev_element.pop_back();
				}
				l--;
			}
			else
			{
				if (next != TARGET)
				{
					path.push_back(next);
					indexes.push_back(l%2==0 ? free_:exchangeable); //even layer correspond to SOURCE and elements in S, odd correspond to elements in \overline{S}
					prev_element.push_back(current);

					current = next;
				}
				l++;
			}
		}

		if (l==DISTANCE_TARGET)
		{
			augmenting_paths_count++;
			cout << "l==DISTANCE_TARGET, apc: " << augmenting_paths_count << endl;
			//Augment along the path s,a1,a2,...,a_dt-1,t to obtain bigger S
			//Update candidates: for all i=0 up to DISTANCE_TARGET-1, set candidates[i] = candidates[i] - a_i
			assert(path.size() == indexes.size());
			assert(path.size() > 0);
			for (size_t j=0; j<path.size(); j++)
			{
				in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;
				swap(candidates[distances[path[j]]].back(), candidates[distances[path[j]]][indexes[j]]); //Safe because no two vertices lying on the path have the same distances
				candidates[distances[path[j]]].pop_back();
			}

			UpdateIndependentSet();

			TARGET_IN_B = true;

			vector<int>().swap(path);
			vector<int>().swap(indexes);
			vector<int>().swap(prev_element);
			
			current = SOURCE;

			l=0;
		}
	}
	
	AUGMENTATIONS.push_back(augmenting_paths_count);

	return;
}

size_t ExactRank(int N_, Oracle* O1_, Oracle* O2_) {
	
	size_t s;
	N  = N_;
	O1 = O1_;
	O2 = O2_;

	cout << "Init...\n";
	Init(N);
	//UpdateIndependentSet();

	int x=0;
	do
	{
		cout << "x:" << ++x << endl;
		cout << "SZ, s = " << SZ << ", " << s << endl;
		s = SZ;
		BlockFlow(); //change BlockFlow to return SZ, then 1 liner s=BlockFlow();
	} while (s != SZ);
	
	//UpdateIndependentSet(); //useless
	PrintIndependentSet();

	delete[] distances;
	//TODO free candidates here maybe?	

	return SZ;
}


size_t ApproxRank(int N_, Oracle* O1_, Oracle* O2_, double eps=0.1) {

	size_t s;
	N  = N_;
	O1 = O1_;
	O2 = O2_;
 
	//TODO allocate candidates here

	int i = 0;

	Init(N);
	//UpdateIndependentSet();
	
	while (i++ != (int)(1/eps) or s!=SZ) //FIXME 1/eps
	{
		s=SZ;
		BlockFlow(); //change BlockFlow to return SZ, then 1 liner s=BlockFlow();
	}

	//UpdateIndependentSet(); //useless
	PrintIndependentSet();

	delete[] distances;
	//TODO free candidates here	

	return SZ;
}