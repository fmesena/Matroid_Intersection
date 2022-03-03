#include <algorithm>
#include <queue>
#include "utils.h"
#include "SAP.h"

using namespace std;

//vector<int> graph[MAX_V]; needed for the other version (see last comment at the end of the file)

void okSAP() {
	cout <<"okSAP\n";
	DISTANCE_TARGET=2;
	cout << DISTANCE_TARGET << endl;
	return;
}

void Init() {
	in_independent_set.resize(N, false);
	return;
}

void Find_Greedy(Oracle* O1, Oracle* O2) {   // O (n.T) 
	for (int i = 0; i < N; ++i)
	{
		if (O1->Free(i) && O2->Free(i))
		{
			independent_set.push_back(i);
			in_independent_set[i]=true;
			//Update_State(); FIXME
		}
	}
	return;
}

bool BFS_Without_Graph(Oracle* O1, Oracle* O2) {   // O (n + n.T + nr.T + 2r)

	const int NOT_VISITED = -4;
	const int NO_AUGMENTATION = -5;

	independent_set.clear();
	not_independent.clear();

	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i))
			independent_set.push_back(i);
		else
			not_independent.push_back(i);
	}

	int parent[N];
	queue<int> q;

	for (int i = 0; i < N; ++i)
		parent[i] = NOT_VISITED;

	// Compute free elements wrt M1 and M2
	vector<bool> X1 = vector<bool>(not_independent.size(), false);
	vector<bool> X2 = vector<bool>(not_independent.size(), false);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		int e = not_independent[i];

		if (O1->Free(e)) X1[i] = true;
		if (O2->Free(e)) X2[i] = true;

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

	int current;
	int endpoint = NO_AUGMENTATION;

    while (!q.empty())
    {	
    	int neighb;
    	current = q.front(); q.pop();

    	if (IN_INDEPENDENT(current))
    	{
    		for (size_t i = 0; i < not_independent.size(); ++i)
    		{
    			neighb = not_independent[i];
    			if (parent[neighb] != NOT_VISITED) continue;
				if (!O1->Exchangeable(current, neighb)) continue;
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
				if (!O2->Exchangeable(neighb, current)) continue;
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

	UpdateIndependentSet();

	return true;
}


size_t SAP(int N_, Oracle* oracle1, Oracle* oracle2) {  // O (n.r^2.T) 

	N = N_;

	Init();
	//Independent_Set_Greedy();

	while (BFS_Without_Graph(oracle1, oracle2));

	PrintIndependentSet();
	return independent_set.size();
}



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




/* this is a version builiding the graph explicitly

bool BFS() {   // O (n + n.T + nr + 2r) 

	const int NOT_VISITED = -4;
	const int NO_AUGMENTATION = -5;

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
		if (IN_INDEPENDENT(i))
			independent_set.push_back(i);
		else
			not_independent.push_back(i);
	}
	
	//DEBUG_VECTOR(independent_set);
	//DEBUG_VECTOR(not_independent);

	// Compute X1 and X2
	X1 = vector<bool>(not_independent.size(), false);
	X2 = vector<bool>(not_independent.size(), false);
	DEBUG_VECTOR(not_independent);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		int e = not_independent[i];
		if (M1_Free(e)) X1[i] = true;
		if (M2_Free(e)) X2[i] = true;
    }

	DEBUG_VECTOR(X1);
	DEBUG_VECTOR(X2);

	int aux;
	int a,b;
	for (size_t i = 0; i < independent_set.size(); ++i)
	{
		a = independent_set[i];
		for (size_t j = 0; j < not_independent.size(); ++j)
		{
			// test whether the pair (a,b) is exchangeable or not
			b = not_independent[j];
			// Note: if the element not_independent[j] is free either in M1 or M2 it is obviously exchangeable, and we do not care about those arcs
			//   since they will never be used in a shortest s-t path

			if (!X1[j])
				if (M1_Exch(a,b))
					addEdge(a,b);

			if (!X2[j])
				if (M2_Exch(a,b)) 
					addEdge(b,a);
		}
	}
	PrintGraph();
	return;
}*/
