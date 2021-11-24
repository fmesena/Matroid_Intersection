#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>
#include <chrono>
#include "Cun86.h"

using namespace std;
using namespace std::chrono;


#define 	MAX_V					100000
#define 	min(u, v) 			    (u<v ? u:v)
#define		even(x)					x%2==0
#define		odd(x)					x%2==1
#define		SZ 						independent_set.size()
#define 	IN_INDEPENDENT(a)		in_independent_set[a]

#define		Oracle_M1(S)			(*M1)(S)
#define		Oracle_M1_Free(S,a)		(*M1_Free)(S,a)
#define		Oracle_M1_Exch(S,a,b)	(*M1_Exch)(S,a,b)
#define		Oracle_M2(S)			(*M2)(S)
#define		Oracle_M2_Free(S,a)		(*M2_Free)(S,a)
#define		Oracle_M2_Exch(S,a,b)	(*M2_Exch)(S,a,b)

int N;
int DISTANCE_TARGET;
size_t hack;

vector<int> independent_set;
vector<int> not_independent;
vector<bool> in_independent_set;
vector<bool> X1;
vector<bool> X2;
vector<int> graph[MAX_V];
int *distances;

bool (*M1)(vector <int>);
bool (*M1_Free)(vector <int>, int);
bool (*M1_Exch)(vector <int>, int, int);

bool (*M2)(vector <int>);
bool (*M2_Free)(vector <int>, int);
bool (*M2_Exch)(vector <int>, int, int);

const int SOURCE = -2;
const int TARGET = -3;

//ORACLE_CALLS++;


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

void reset(vector<int> &v) { vector<int>().swap(v); }

void AddEdge(int u, int v) { graph[u].push_back(v); }

void PrintGraph() {
    for (int v = 0; v < N; ++v)
    {
        cout << "\n Adjacency list of vertex "
             << v << "\n head ";
        for (auto x : graph[v])
           cout << "-> " << x;
        printf("\n");
    }
}

void PrintIndependentSet() {
	cout << "INDEPENDENT SET FOUND WITH SIZE " << independent_set.size() << ":" << endl;
	for (size_t i=0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
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


/**************************************************/
/****************** ALGORITHMS ********************/
/**************************************************/


void Init() {
	in_independent_set.resize(N,false);
	distances = new int[N]();
	//TODO allocate candidates here maybe?
}

void Independent_Set_Greedy() {   // O (n T_ind) 
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

void UpdateIndependentSet() {
	independent_set.clear();
	not_independent.clear();

	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i))
			independent_set.push_back(i);
		else
			not_independent.push_back(i);
	}
}

void Build_Exchange_Graph() {   // O (n + nrT_ind) 

	for (int i = 0; i < N; ++i) graph[i].clear();	
	UpdateIndependentSet(); //remove this line and in main call Init() before main cycle of augmentations

	// Compute X1 and X2
	X1 = vector<bool>(not_independent.size(), false);
	X2 = vector<bool>(not_independent.size(), false);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		int e = not_independent[i];

		independent_set.push_back(e);
		if (Oracle_M1(independent_set)) X1[i] = true;
		if (Oracle_M2(independent_set)) X2[i] = true;
		independent_set.pop_back();
    }

	int aux;
	for (size_t i = 0; i < independent_set.size(); ++i)
	{
		aux = independent_set[i];
		for (size_t j = 0; j < not_independent.size(); ++j)
		{
			/* if the element not_independent[j] is free either in M1 or M2 it is obviously exchangeable. Unlike SAP, we, can't disregard "exchangeable" arcs
			   to/from free elements, as they can be used in an augmenting path (not the first) during a stage. You have an example showing this! */
			independent_set[i] = not_independent[j];

			if (Oracle_M1(independent_set)) AddEdge(aux, not_independent[j]);
			if (Oracle_M2(independent_set)) AddEdge(not_independent[j], aux);
		}
		independent_set[i] = aux;
	}
	return;
}

bool BFS() {   // O(nr)

	cout << "BFS\n";

	const int NOT_VISITED = -1;
	const int NO_AUGMENTATION = -4;

	int current;
	int endpoint = NO_AUGMENTATION;

	queue<int> q;
	int parent[N];

	DISTANCE_TARGET = numeric_limits<int>::max();
	for (int i = 0; i < N; ++i)
	{
		parent[i]    = NOT_VISITED;
		distances[i] = numeric_limits<int>::max();
	}

	DEBUG_VECTOR(X1);
	DEBUG_VECTOR(X2);

	// Compute sources and sinks, i.e., free elements wrt current independent set and M1 and M2
	for (size_t i = 0; i < not_independent.size(); i++) 
	{
		if (X1[i] && X2[i])
		{
			in_independent_set[not_independent[i]] = true;
			return true;
		}
		if (X1[i])
		{
			q.push(i);
			parent[i] = SOURCE;
		}
    }

	while (!q.empty()) 
	{	
		current = q.front(); q.pop();
		cout << "Expanding " << current << endl;
		if (X2[current])
		{
			DISTANCE_TARGET = distances[current] + 1;
			endpoint = current;
			break; // If we found a free element wrt M2, then we reached the target. 
				   // Moreover, we have found all the vertices that have distances less than DISTANCE_TARGET.
		}

		for (auto neighb : graph[current])
		{
			if (parent[neighb] != NOT_VISITED) continue;
			q.push(neighb);
			parent[neighb]    = current;
			distances[neighb] = distances[current] + 1;
		}
	}

	if (endpoint == NO_AUGMENTATION) return false;

	assert(DISTANCE_TARGET%2==0);

	return true;
}


int FindArc(int a, vector<int> &next) {

	hack = 0;

	if (a == SOURCE)             while (hack < next.size()) if (Oracle_M1_Free(independent_set,next[hack++]))   return next[--hack];

	else if (IN_INDEPENDENT(a))  while (hack < next.size()) if (Oracle_M1_Exch(independent_set,a,next[hack++])) return next[--hack];

	else if (!IN_INDEPENDENT(a) && Oracle_M2_Free(independent_set,a)) return TARGET;

	else if (!IN_INDEPENDENT(a)) while (hack < next.size()) if (Oracle_M2_Exch(independent_set,next[hack++],a)) return next[--hack];

	return -1;
}


bool Augment() {

	if (!BFS()) return false;

	// happens when we have found a free element in M1 and M2
	if (DISTANCE_TARGET == numeric_limits<int>::max())
	{
		UpdateIndependentSet();
		return true;
	}

	cout << "BFS ok\n";
	cout << "Printing distances before augmentation phase\n";
	for (int i=0; i<N; i++) cout << "Vertex " << i << ": " << distances[i] << endl;
	cout << endl;

	vector<int> candidates[2*SZ+3]; //FIXME magic numbers, 2*SZ+3 is just a safe upper bound on the number of distinc distancess
	for (int i = 0; i < N; ++i)
		if (distances[i] != numeric_limits<int>::max())
			candidates[distances[i]].push_back(i);

	candidates[0].push_back(SOURCE);
	cout << "DISTANCE_TARGET: " << DISTANCE_TARGET << endl;	
	candidates[DISTANCE_TARGET].push_back(TARGET);
	cout << "alive\n";
	PrintCandidates(candidates);

	assert(candidates[0].size()==1);
	assert(candidates[DISTANCE_TARGET].size()==1);

	int l = 0;
	int current = SOURCE;
	vector<int> path;
	vector<int> indexes;
	vector<int> prev_element;

	while (l>=0)
	{
		if (l<DISTANCE_TARGET)
		{
			// i think this condition is useless
			if (candidates[l].empty()) {
				cout << "not useless after all...\n";
        		return false;
      		}
			int next = FindArc(current, candidates[l+1]);
			
			if (next == -1)
			{	
				// this block corresponds to removing the so called "useless" elements in Cunningham's terminology
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
					indexes.push_back(hack);
					prev_element.push_back(current);

					current = next;
				}
				l++;
			}
		}

		if (l==DISTANCE_TARGET)
		{
			for (size_t j=0; j<path.size(); j++)
			{
				in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;
				swap(candidates[distances[path[j]]].back(), candidates[distances[path[j]]][indexes[j]]);
				candidates[distances[path[j]]].pop_back();
			}

			UpdateIndependentSet();

			vector<int>().swap(path);
			vector<int>().swap(indexes);
			vector<int>().swap(prev_element);
			
			current = SOURCE;

			l=0;
		}
	}
	return true;
}

void Augment_Recursive(int current) {
	if (current == TARGET)
		return;
}

size_t Cun86(int N_, bool (*a)(vector<int>), bool (*b)(vector<int>, int), bool (*c)(vector<int>, int, int),
			 bool (*d)(vector<int>), bool (*e)(vector<int>, int), bool (*f)(vector<int>, int, int)) {  // O (nr^1.5T_ind) 

	N = N_;

	M1 = a;
	M1_Free = b;
	M1_Exch = c;

	M2 = d;
	M2_Free = e;
	M2_Exch = f;	

	Init();
	//for (int i=0; i<N; i++) distances[i]=numeric_limits<int>::max();
	//Independent_Set_Greedy();

	do {
		Build_Exchange_Graph();
	}
	while (Augment());

	delete[] distances;
	PrintIndependentSet();

	return SZ;
}


int main3() {
	return 0;
}