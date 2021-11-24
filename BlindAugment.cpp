#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <assert.h>
#include <limits>
#include <chrono>
#include "Rank.h"

using namespace std;
using namespace std::chrono;

#define 	min(u, v) 			(u<v ? u:v)
#define		even(x)				x%2==0
#define		odd(x)				x%2==1
#define		SZ 					independent_set.size()
#define 	IN_INDEPENDENT(a)	in_independent_set[a]

#define		Oracle_M1(S)				(*M1_Indep)(S)
#define		Oracle_M2(S)				(*M2_Indep)(S)
#define		Rank_M1(S)					(*M1_Rank)(S)
#define		M2_Exchangeable_Set(S,A,b)	(*M2_Exch_Set)(S,A,b)


int N;
vector<int> AUGMENTATIONS;

vector<int>  independent_set;
vector<bool> in_independent_set;
vector<int>  index_;				  // index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S

int exchangeable; //hack
int free_;		  //hack

int DISTANCE_TARGET;
int CURRENT_RANK;
bool TARGET_IN_B;

vector<int> B_S;
vector<int> B_not_S;

int  (*M1_Rank)(vector <int>);

bool (*M2_Indep)(vector <int>);
bool (*M2_Exch_Set)(vector<int> S, vector<int> A, int b);

const int SOURCE = -2;
const int TARGET = -3;



template<typename T>
void DEBUG_VECTOR(const vector<T> &v) {
	cout << "DEBUG BEGIN" << endl;
	cout << "The vector size is " << v.size() << " and its " << "capacity is " << v.capacity() << endl;
	cout << "Vector content:" << endl;
	for (size_t i = 0; i < v.size(); ++i) cout << v[i] << endl;
	cout << "DEBUG END" << endl;
}


template<typename T>
vector<T> Slice(vector<T> const &v, int m, int n) {
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n + 1;
	vector<T> vec(first, last);
	return vec;
}


// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
void UpdateIndependentSet() {
	int j=0;
	independent_set.clear();
	index_.clear();

	for (int i = 0; i < N; ++i)
	{
		index_[i] = -1;
		if (IN_INDEPENDENT(i))
		{
			independent_set.push_back(i);
			index_[i] = j++;
		}
	}
}


void PrintIndependentSet() {
	cout << "INDEPENDENT SET FOUND WITH SIZE " << independent_set.size() << ":" << endl;
	for (size_t i = 0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
	cout << endl;
}

void PrintCandidates(vector<int> c[]) {
	for (size_t i = 0; i < 3; ++i)
	{
		cout << "Distance " << i << endl;
		for (int k : c[i])
			cout << k << " ";
		cout << endl;
	}
	cout << endl;
}

// returns V \subseteq S where all elements from A \subseteq S are removed from S
// does not change the values of "index_" since the altered S is supposed only for a "quick call" to the oracle
vector<int> RemoveSubset(vector<int> S, vector<int> A) {
	// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
	int j=0;
	for (size_t i = 0; i < A.size(); i++)
		if (index_[A[i]] != -1) {
			S[index_[A[i]]] = -1; j++; }
	vector<int> S_;
	for (size_t i = 0; i < S.size(); i++)
		if (S[i] != -1)
			S_.push_back(i);
	return S_;
}



int FindExchange(bool (*Oracle)(vector<int>, vector<int>, int), int const b, vector<int> A) {

	if (A.empty()) return -1;

	int sz = A.size();
	int l=0;
	int r=sz-1;
	int m;

	vector<int> S = independent_set;
	vector<int> A1;

	if (!Oracle(S,A,b)) return -1; // "FALSE" - signals that no element is exchangeable

	exchangeable = 0;

	while (sz > 1)
	{	

		m = l + (r-l)/2;     // (l+r)/2

		A1 = Slice(A, l, m); // this takes (sz-1) operations overall

		if (Oracle(S,A1,b))
		{
			r = m;
			exchangeable = r;
		}
		else
		{
			l = m + 1;
			exchangeable = l;
		}

		sz /= 2;

	}
	return A[exchangeable];
}


int FindFree(int (*RankOracle)(vector<int>), vector<int> B) {
	
	if (B.empty()) return -1;

	int sz = B.size();
	int l=0;
	int r=sz-1;
	int m;

	vector<int> S = independent_set;
	vector<int> A = independent_set;
	vector<int> B1;

	//Remark: B is always a subset of \overline S

	int qwe = A.size();
	A.resize(A.size()+B.size());

	for (size_t i=0; i<B.size(); i++)
		A[i+qwe]=B[i];

	//CURRENT_RANK = RankOracle(S);

	if (RankOracle(A) == RankOracle(S)) return -1;

	A = S;
	free_ = 0;

	while (sz > 1)
	{	

		m = l + (r-l)/2;  // (l+r)/2

		A  = S;
		B1 = Slice(B, l, m); // this takes (sz-1) operations in total
							 // change this to just one loop from "l" to "m" copying elements from B[l..m] to A
		
		for (int b : B1)	 // this takes (sz-1) operations in total
			A.push_back(b);

		if (RankOracle(A) > RankOracle(S))
		{
			r = m;
			free_ = r;
		}
		else
		{
			l = m + 1;
			free_ = l;
		}

		sz /= 2;

	}
	return B[free_];
}


// DANGER: vector S is not made const //
// DANGER: alterar independent_set sem alterar index_
int OutArc(int const a) {
	vector<int> S = independent_set;

	if (a == SOURCE) { return FindFree(M1_Rank, B_not_S); }

	if (IN_INDEPENDENT(a))
	{
		swap(S[index_[a]], S.back());
		S.pop_back();
		int res = FindFree(M1_Rank, B_not_S);
		S.push_back(a); //useless because S is a copy
		return res;
	}

	S.push_back(a);
	if (!IN_INDEPENDENT(a) && TARGET_IN_B && Oracle_M2(S)) { S.pop_back(); return TARGET; }
	S.pop_back();

	if (!IN_INDEPENDENT(a)) return FindExchange(M2_Exch_Set, a, B_S);

	return -1; // signals "FALSE"
}




// An iterative DFS-like routine
void BlindAugment() {

	int  next;
	int  l = 0;
	int  current = SOURCE;
	vector<int>  path;
	vector<int>  prev_element;

	while (l>=0)
	{

  		if (l==0)
  			next = FindFree(independent_set, not_independent); //FindFree em M1

  		else if (odd(l))
  		{
  			independent_set.push_back(current);
  			if (Oracle_M2(independent_set)) //Check if current is free wrt M2
  			{
  				independent_set.pop_back();
				
				for (size_t j=0; j<path.size(); j++)
					in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;

				UpdateIndependentSet();

				ground_set = reset(ground_set);

				vector<int>().swap(path);
				vector<int>().swap(prev_element);

				l=0;
				current = SOURCE;
  			}

  			else
  			{
  				independent_set.pop_back();
  				next = FindExchange(independent_set, current, independent_set); //first argument still has the element in the path in layer = l-2. need a way to "ignore" those elements when moving forward
  			}
  			
		}

		else if (even(l))
		{
			independent_set = independent_set - current;
			next = FindFree(independent_set, not_independent); //FindFree em M1
		}

		if (next == -1)
		{
			if (current != SOURCE)
			{	
				ground_set = ground_set - current; //deadend
				
				current = prev_element.back();

				path.pop_back();
				prev_element.pop_back();
			}
			l--;
		}
	}
	
	delete[] distance;
	AUGMENTATIONS.push_back(augmenting_paths_count);

	return;
}



void FixCycle(vector<int> graph[]) {
	//Fix Cycle
}



void FindGreedy() {
	for (int i = 0; i < N; ++i)
	{
		independent_set.push_back(i);
		in_independent_set[i]=true;
		if (Rank_M1(independent_set)!=SZ || !Oracle_M2(independent_set))
		{
			independent_set.pop_back();
			in_independent_set[i]=false;
		}
	}
	return;
}


void Init() {
	in_independent_set.resize(N,false);
	index_.resize(N,-1);
}



int main() {
	Init();
	FindGreedy();
	BlindAugment();
	return 0;
}