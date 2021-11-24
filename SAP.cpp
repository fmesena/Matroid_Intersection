#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <assert.h>
#include <chrono>
#include "SAP.h"

using namespace std;
using namespace std::chrono;

#define 	MAX_V					100000
#define 	min(u, v) 			    (u<v ? u:v)
#define		even(x)					x%2==0
#define		odd(x)					x%2==1
#define		SZ 						independent_set.size()
#define 	IN_INDEPENDENT(a)		in_independent_set[a]

#define		Oracle_M1(S)			(*M1)(S)
#define		Oracle_M2(S)			(*M2)(S)

int N;

vector<int> independent_set;
vector<int> not_independent;
vector<bool> in_independent_set;
vector<bool> X1;
vector<bool> X2;
vector<int> graph[MAX_V];

bool (*M1)(vector <int>);
bool (*M1_Free)(vector <int>, int);
bool (*M1_Exch)(vector <int>, int, int);

bool (*M2)(vector <int>);
bool (*M2_Free)(vector <int>, int);
bool (*M2_Exch)(vector <int>, int, int);



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
        cout << "Adjacency list of vertex "<< v << endl;
        for (auto x : graph[v])
           cout << x << " ";
        cout << endl;
    }
    cout << endl;
}

void PrintIndependentSet() {
	cout << "INDEPENDENT SET FOUND WITH SIZE " << independent_set.size() << ":" << endl;
	for (size_t i=0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
	cout << endl;
}

bool equal_content_same_order(vector<int> &a, vector<int> &b) {
	if (a.size() != b.size()) return false;
	for (size_t i=0; i < a.size(); ++i)
		if (a[i] != b[i])
			return false;
	return true;
}


/**************************************************/
/****************** ALGORITHMS ********************/
/**************************************************/


void Init() {
	return;
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

bool BFS() {   // O (n + nTind + nr + 2r) 

	const int SOURCE = -1;
	const int NOT_VISITED = -2;
	const int NO_AUGMENTATION = -3;

	int current;
	int endpoint = NO_AUGMENTATION;

	queue<int> q;
	int parent[N];
	
	for (int i = 0; i < N; ++i) parent[i] = NOT_VISITED;

	// Compute sources and sinks, i.e., free elements wrt current independent set and M1 and M2
	for (size_t i = 0; i < not_independent.size(); i++) 
	{
		if (X1[i] && X2[i])
		{
			in_independent_set[not_independent[i]] = true;
			cout << "giga free element " << not_independent[i] << endl;
			return true;
		}

		if (X1[i])
		{
			q.push(i);
			parent[i] = SOURCE;
		}
    }

    cout << "starting bfs\n";
	while (!q.empty()) 
	{
		current = q.front(); q.pop();

		if (X2[current])
		{
			endpoint = current;
			break;
		}

		for (auto neighb : graph[current])
		{
			if (parent[neighb] != NOT_VISITED) continue;
			q.push(neighb);
			parent[neighb] = current;
		}
	}

	if (endpoint == NO_AUGMENTATION) return false;
	cout << "ok\n";
    do { 
    	in_independent_set[endpoint] = in_independent_set[endpoint] == true ? false:true;
		endpoint = parent[endpoint];
	} while (endpoint != SOURCE);

	return true;
}


bool Augment() {
	return BFS();
	//return BFS_Without_Graph();
}


void Build_Exchange_Graph() {   // O (n + nrT_ind) 

	for (int i = 0; i < N; ++i) graph[i].clear();

	independent_set.clear();
	not_independent.clear();

	for (int i = 0; i < N; ++i)
	{
		if (in_independent_set[i])
			independent_set.push_back(i);
		else
			not_independent.push_back(i);
	}

	/*
	size_t n = independent_set.size();
	independent_set.resize(n+1);
	not_independent.resize(N-n);
	
	int x,y=0;
	for (int i = 0; i < N; ++i)
	{
		cout << i << endl;
		if (in_independent_set[i])
			independent_set[x++] = i;
		else
			not_independent[y++] = i;
	}
	*/

	//DEBUG_VECTOR(independent_set);
	//DEBUG_VECTOR(not_independent);

	// Compute X1 and X2
	X1 = vector<bool>(not_independent.size(), false);
	X2 = vector<bool>(not_independent.size(), false);
	DEBUG_VECTOR(not_independent);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		independent_set.push_back(not_independent[i]);
	
		//DEBUG_VECTOR(independent_set);
		//cout << Oracle_M1(independent_set) << endl;
		//cout << Oracle_M2(independent_set) << endl;
	
		if (Oracle_M1(independent_set)) X1[i] = true;
		if (Oracle_M2(independent_set)) X2[i] = true;
		independent_set.pop_back();
    }

	DEBUG_VECTOR(X1);
	DEBUG_VECTOR(X2);

	int aux;
	for (size_t i = 0; i < independent_set.size(); ++i)
	{
		aux = independent_set[i];
		for (size_t j = 0; j < not_independent.size(); ++j)
		{
			/* if the element not_independent[j] is free either in M1 or M2 it is obviously exchangeable, and we do not care about those arcs
			   since they will never be used in a shortest s-t path */

			// test if the pair (independent_set[i], not_independent[j]) is exchangeable
			independent_set[i] = not_independent[j];

			if (!X1[j])
				if (Oracle_M1(independent_set))
					AddEdge(aux, not_independent[j]);

			if (!X2[j])
				if (Oracle_M2(independent_set)) 
					AddEdge(not_independent[j], aux);

		}
		independent_set[i] = aux;
	}
	PrintGraph();
	return;
}


size_t SAP(int N_, bool (*I1)(vector<int>), bool (*I2)(vector<int>)) {  // O (nr^2T_ind) 

	cout << ">>SAP\n";

	N  = N_;
	M1 = I1;
	M2 = I2;

	in_independent_set.resize(N, false);
	//Init();
	//Independent_Set_Greedy();

	int s=0;
	do {
		Build_Exchange_Graph();
	}
	while (Augment());

	PrintIndependentSet();
	return independent_set.size();
}


/*
int main() {

	cin >> N;
	vector<int> graph[N];

	in_independent_set.resize(N,false);

	ORACLE_CALLS  = 0;
	AUGMENTATIONS = 0;


	auto start = high_resolution_clock::now();

	SAP(graph);

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);


	cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
	cout << "Total number of oracle calls: " << ORACLE_CALLS << endl;
	cout << "Total number of augmentations: " << AUGMENTATIONS << endl;

	PrintIndependentSet();
	//printGraph(graph);

	return 0;
}
*/

/*
std::cout <<"Values 1 & 3. Pointer function: Add:"<<operation (1,3,&add)<<" Sub:"<<operation (1,3,&sub) << std::endl;
std::cout <<"Values 1 & 3. std::function   : Add:"<<operation2(1,3,&add)<<" Sub:"<<operation2(1,3,&sub) << std::endl;

int add(int x, int y){return x+y;}
int sub(int x, int y){return x-y;}
int operation (int x, int y,int (*function)(int,int)){return function(x,y);}
int operation2(int x, int y,std::function<int(int, int)> function){return function(x,y);}
*/

/*
#include <functional>

void task(){
   std::cout << "Task" << std::endl;
}

void task2(){
   std::cout << "Task2" << std::endl;
}

void doTask(std::function<void()> func){
	func();
}

int main(){
    doTask(task);
    doTask(task2);
}

std::cout <<"Values 1 & 3. std::function   : Add:"<<operation2(1,3,&add)<<" Sub:"<<operation2(1,3,&sub) << std::endl;

int add(int x, int y){return x+y;}
int sub(int x, int y){return x-y;}
int operation (int x, int y,int (*function)(int,int)){return function(x,y);}*/




/*

bool BFS_Without_Graph() {   // O (n + nT_ind + nrT_ind + 2r) 

	const int SOURCE = -1;
	const int NOT_VISITED = -2;
	const int NO_AUGMENTATION = -3;

	int current;
	int endpoint = NO_AUGMENTATION;
	
	queue<int> q;
	int parent[N];

	independent_set.clear();
	not_independent.clear();

	for (int i = 0; i < N; ++i)
	{
		if (in_independent_set[i])
			independent_set.push_back(i);
		else
			not_independent.push_back(i);
	}

	for (int i = 0; i < N; ++i) parent[i] = NOT_VISITED;

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
		if (X1[i] && X2[i])
		{
			in_independent_set[not_independent[i]] = true;
			return true;
		}
		if (X1[i]) 
		{
			q.push(e);
			parent[e] = SOURCE;
		}
    }

    assert(independent_set.size() == for_debug.size());
    assert(equal_content_same_order(independent_set, for_debug));

    while (!q.empty())
    {	
    	int neighb;
    	current = q.front(); q.pop();

    	if (in_independent_set[current])
    	{
    		for (size_t i = 0; i < not_independent.size(); ++i)
    		{
    			neighb = not_independent[i];
    			if (parent[neighb] != NOT_VISITED) continue;
				if (!Oracle_M1_Exch(independent_set, current, neighb)) continue;
				q.push(neighb);
				parent[neighb] = current;
    		}
    	}
    	else
    	{
    		if (X2[current])
    		{
    			endpoint = current;
				break;
    		}
    		for (size_t i = 0; i < independent_set.size(); ++i)
    		{
    			neighb = independent_set[i];
    			if (parent[neighb] != NOT_VISITED) continue;
				if (!Oracle_M2_Exch(independent_set, neighb, current)) continue;
				q.push(neighb);
				parent[neighb] = current;
    		}
    	}
   	}

	if (endpoint == NO_AUGMENTATION) return false;

    do { 
    	in_independent_set[endpoint] = in_independent_set[endpoint] == true ? false:true;
		endpoint = parent[endpoint];
	} while (endpoint != SOURCE);

	return true;
}

*/