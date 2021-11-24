#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>
#include <limits>
#include <chrono>
#include "Indep.h"

using namespace std;
using namespace std::chrono;


#define 	min(u, v) 			(u<v ? u:v)
#define		even(x)				x%2==0
#define		odd(x)				x%2==1
#define		SZ 					independent_set.size()
#define 	IN_INDEPENDENT(a)	in_independent_set[a]

#define		Oracle_M1(S)			Oracle_PowerSet(S)
#define		Oracle_M1_Free(S,a)		Oracle_PowerSet_Free(S,a)
#define		Oracle_M1_Exch(S,a,b)	Oracle_PowerSet_Exchangeable(S,a,b)
#define		Oracle_M2(S)			Oracle_Uniform(S)
#define		Oracle_M2_Free(S,a)		Oracle_Uniform_Free(S,a)
#define		Oracle_M2_Exch(S,a,b)	Oracle_Uniform_Exchangeable(S,a,b)


const int SOURCE = -2;
const int TARGET = -3;


int N;
vector<int> independent_set;      // all elements in current independent set
vector<int> not_independent;	  // all elements not in current independent set
vector<int> index_;				  // index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
vector<bool> in_independent_set;

int exchangeable; //hack
int DISTANCE_TARGET;


/**************************************************/
/******************** ORACLES *********************/
/**************************************************/

const int H = 4;
bool Oracle_Uniform(vector<int> S) { return S.size() <= H; }
bool Oracle_Uniform_Free(vector<int> S, int free) { return S.size() < H; }
bool Oracle_Uniform_Exchangeable(vector<int> S, int a, int b) { return S.size() <= H; }

bool Oracle_PowerSet(vector<int> S) { return true; }
bool Oracle_PowerSet_Free(vector<int> S, int free) { return true; }
bool Oracle_PowerSet_Exchangeable(vector<int> S, int a, int b) { return true; }



bool Oracle_Cycle(vector<int> S) {
	//Use Edge List to store the graph, helps a lot here
	vector<int> graph[N];
	queue<int> q;
	q.push(0);
	bool visited[N] = {false};
	int predecessor[N] = {-1};
	int current;
	visited[0] = true;

	while (!q.empty())
	{
		current = q.front(); q.pop();
		for (auto neighbour : graph[current])
		{
			if (visited[neighbour] && predecessor[current] != neighbour) return false;
			if (visited[neighbour]) continue;
			q.push(neighbour);
			visited[neighbour] = true;
			predecessor[neighbour] = current;
		}
	}
	return true;
}

bool Oracle_Cycle_Free(vector<int> S, int free) {
	//free = HashMap(free);  //HashMap maps integers to its elements in the context of the problem
	S.push_back(free);
	return Oracle_Cycle(S);
}

bool Oracle_Cycle_Exchangeable(vector<int> S, int a, int b) {
	//a = HashMap(a);  //HashMap maps integers to its elements in the context of the problem
	//b = HashMap(b); 
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S.back());
			S.pop_back();
			break;
		}
	return Oracle_Cycle_Free(S,b);
}


// N colours are identified with integers from 0 to N-1
bool Oracle_Colour(vector<int> S) {
	bool counter[N] = {false};
	for (int e : S)
	{
		if (counter[e]==true)
			return false;
		counter[e] = true;
	}
	return true;
}

bool Oracle_Colour_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Colour(S);
}
bool Oracle_Colour_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Colour(S);
	if (Oracle_Colour(S))
	{
		S.push_back(b);
		return Oracle_Colour(S);
	}
	return false;
}

struct Edge {
	int u;
	int v;
};

bool Oracle_Transversal_Left(vector<int> S) {
	vector<Edge> Edges;
	int M = N/2;
	bool left[M] = {false}; //M is the number of vertices on the LEFT bipartition
	for (Edge edge : Edges)
	{
		if (left[edge.u] == true) return false;
		left[edge.u] = true;
	}
	return true;
}

bool Oracle_Transversal_Left_Free(vector<int> S, int free) {
	//free = HashMap(free);  //HashMap maps integers to its elements in the context of the problem
	S.push_back(free);
	return Oracle_Transversal_Left(S);
}

bool Oracle_Transversal_Left_Exchangeable(vector<int> S, int a, int b) {
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S.back());
			S.pop_back();
			break;
		}
	return Oracle_Transversal_Left_Free(S,b);
}

bool Oracle_Transversal_Right(vector<int> S) {
	vector<Edge> Edges;
	int M = N/2;
	bool right[M] = {false}; //M is the number of vertices on the RIGHT bipartition
	for (Edge edge : Edges)
	{
		if (right[edge.v] == true) return false;
		right[edge.v] = true;
	}
	return true;
}

bool Oracle_Transversal_Right_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Transversal_Right(S);
}

bool Oracle_Transversal_Right_Exchangeable(vector<int> S, int a, int b) {
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S.back());
			S.pop_back();
			break;
		}
	return Oracle_Transversal_Right_Free(S,b);
}


bool Oracle_Transversal_MSA(vector<int> S) {
	return Oracle_Transversal_Right(S);
}

bool Oracle_Transversal_MSA_Free(vector<int> S, int free) {
	return Oracle_Transversal_Right_Free(S,free);
}

bool Oracle_Transversal_MSA_Exchangeable(vector<int> S, int a, int b) {
	return Oracle_Transversal_Right_Exchangeable(S,a,b);
}



/**************************************************/
/******************* UTILITIES ********************/
/**************************************************/


template<typename T>
void DEBUG_VECTOR(const vector<T> &v) {
	cout << "\nDEBUG BEGIN" << endl;
	cout << "The vector size is " << v.size() << " and its " << "capacity is " << v.capacity() << endl;
	cout << "Vector content:" << endl;
	for (size_t i=0; i < v.size(); ++i) cout << v[i] << " ";
	cout << "\nDEBUG END" << endl;
}

template<typename T>
vector<T> Slice(vector<T> const &v, int m, int n) {
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n;
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


// returns V \subseteq S where all elements from A \subseteq S are removed from S
// does not change the values of "index_" since the altered S is supposed only for a "quick call" to the oracle
vector<int> RemoveSubset(vector<int> S, vector<int> A) {
	// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
	int j=0;
	cout << "\nRemoveSubset" << endl;
	DEBUG_VECTOR(S);
	DEBUG_VECTOR(A);
	for (size_t i = 0; i < A.size(); i++)
		if (index_[A[i]] != -1) {
			S[index_[A[i]]] = -1; j++; }
	vector<int> S_;
	for (size_t i = 0; i < S.size(); i++)
		if (S[i] != -1)
			S_.push_back(i);
	DEBUG_VECTOR(S);
	DEBUG_VECTOR(S_);
	cout << endl;
	return S_;
}


// Outpus a \in A such that (a,b) is exchangeable wrt M
// int FindExchange(M=(V,I), S \in I, b \notin S, A \subseteq S)
int FindExchange(bool (*Oracle)(vector<int>, int), int const b, vector<int> A) {
	//assert(A.size() > 0);

	if (A.empty()) return -1;
	if (IN_INDEPENDENT(b)) return -1;

	int sz = A.size();
	int l=0;
	int r=sz-1;
	int m;

	cout << "\nDebuging in FindExchange" << endl;
	DEBUG_VECTOR(A);
	vector<int> S = RemoveSubset(independent_set, A);
	DEBUG_VECTOR(S);
	vector<int> A1;

	if (!Oracle(S,b)) return -1; // "FALSE" - signals that no element is exchangeable

	cout << "FindExchange: exists exchange\n\n";

	exchangeable = 0;

	while (sz > 1)
	{	

		m = l + (r-l)/2;  // (l+r)/2

		//FIXME next three lines are stupid, make use of array "index_" to remove A[l..m] from S (since A \subseteq S) and then push_back b to it
		A1 = Slice(A, l, m); // this takes O(|A|) operations overall. this could be deleted to improve running time, but then we would have to add the information of
							 // which subset of A we want to consider to remove from S. 
		S  = RemoveSubset(independent_set, A1); // this takes (|S|+|A|)log|A| operations in total

		if (Oracle(S,b))
		{
			r = m;
			exchangeable = r;
		}
		else
		{
			l = m;
			exchangeable = l;
		}

		sz /= 2;

	}
	cout << "exchangeable element: " << A[exchangeable] << " with index " << exchangeable << endl;
	return A[exchangeable];
}



void FindGreedy() {
	for (int i = 0; i < N; ++i)
	{
		independent_set.push_back(i);
		in_independent_set[i]=true;
		if (!Oracle_M1(independent_set) || !Oracle_M2(independent_set))
		{
			independent_set.pop_back();
			in_independent_set[i]=false;
		}
	}
	return;
}


void PrintCandidates(vector<int> c[]) {
	for (int i = 0; i < 7; ++i)
	{
		cout << "Distance " << i << endl;
		for (int k : c[i])
			cout << k << " ";
		cout << endl;
	}
	cout << endl;
}


void GetDistancesIndep(bool (*Oracle1)(vector<int>, int), bool (*Oracle2)(vector<int>, int), int *distance, vector<int> candidates[]) {

	int l = 0;
	int marked = 0;

	DISTANCE_TARGET = numeric_limits<int>::max();
	vector<int> updated_l_plus_one;

	// unnecessary, already done along the execution of GetDistancesIndep- CHECK THIS
	/*for (int i = 0; i < N; ++i)
		candidates[distance[i]].push_back(i);*/

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
				if (marked == N) { cout << "TARGET REACHED: " << l+1 << endl; DISTANCE_TARGET = l+1;}
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

				while ( (a = FindExchange(Oracle2, b, Q)) != -1)
				{
					cout << "\nArc from " << b << " to " << a << endl;
					
					distance[a] = l+1;
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
				arc = l==0 ? (Oracle_M1_Free(independent_set, b) && !IN_INDEPENDENT(b)) : (FindExchange(Oracle1, b, candidates[l]) != -1);
				cout << "l: " << l << " arc: " << arc << endl;
				if (arc)
				{
					cout << "arc to " << b << endl;
					distance[b] = l+1;
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



void OnePath(int *distance, vector<int> candidates[]) {
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
				if (Oracle_M2_Free(independent_set,b))
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
					if (Oracle_M1_Exch(independent_set,b,a))
					{
						in_independent_set[a] = true;
						in_independent_set[b] = false;
						arc=true;
					}
				}
				else if (even(l))
				{
					if (Oracle_M2_Exch(independent_set,a,b))
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
				candidates[l-1].pop_back();
				candidates[l].push_back(b);
				distance[b]++;
				a=b;
				break; 
			}
		}
		l--;
	}
	// quando l é odd,  S - b + a_l \in I_1
	// quando l é even, S - a_l + b \in I_2
	return;
}


void AugmentingPaths(bool (*Oracle1)(vector<int>, int), bool (*Oracle2)(vector<int>, int)) {

	FindGreedy();
	UpdateIndependentSet();
	PrintIndependentSet();

	int *distance = new int[N]();
	// Remark: the shortest path from SOURCE to TARGET has at most 2*(SZ+1) arcs.
	vector<int> candidates[200];
	//maybe  vector< vector<int> > e no final de cada iteraçao faço 2x. push_back de vector<int>

	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i))
		{
			distance[i] = 2;
			candidates[2].push_back(i);
		}
		else
		{
			distance[i] = 1;
			candidates[1].push_back(i);
		}
	}

	//-----------------------------------------------------------//

	cout << "Starting AugmentingPaths" << endl;

	while (true)
	{
		GetDistancesIndep(Oracle1,Oracle2,distance,candidates);
		cout << "DistancesIndep finished. Target reached with distance " << DISTANCE_TARGET << endl;

		if (DISTANCE_TARGET < numeric_limits<int>::max())
		{
			OnePath(distance,candidates);
			UpdateIndependentSet();
			PrintIndependentSet();
			cout << "\ndistance:"<<endl;
			for (int i=0; i<N; i++) cout << distance[i] << " ";
			cout << endl << endl;
		}
		else break;
	}

	cout << "Finished AugmentingPaths" << endl;

	delete[] distance;
	//TODO free(candidates);

	return;
}



int main() {

	cin >> N;

	in_independent_set.resize(N,false);
	index_.resize(N,-1);

	AugmentingPaths(&Oracle_PowerSet_Free,&Oracle_Uniform_Free);

	PrintIndependentSet();

	return 0;
}
