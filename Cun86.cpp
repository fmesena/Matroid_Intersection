#include <algorithm>
#include <queue>
#include "utils.h"
#include "Cun86.h"

using namespace std;

size_t hack;

vector<int> graph[MAX_V];
vector<bool> X1;
vector<bool> X2;

Oracle* O1;
Oracle* O2;

void okCun() {
	cout <<"okCun\n";
	return;
}

void InitCun() {
	in_independent_set.resize(N,false);
	distances = new int[N]();
	//TODO allocate candidates here maybe
}

void Independent_Set_Greedy() {   // O (n.T) 
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

void Build_Exchange_Graph() {   // O (n + n.r.T) 

	for (int i = 0; i < N; ++i) graph[i].clear();	
	UpdateIndependentSet(); //needed to initialize not_independent

	// Compute X1 and X2
	X1 = vector<bool>(not_independent.size(), false);
	X2 = vector<bool>(not_independent.size(), false);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		int e = not_independent[i];
		if (O1->Free(e)) X1[i] = true;
		if (O2->Free(e)) X2[i] = true;
    }

	int aux;
	int a,b;
	for (size_t i = 0; i < independent_set.size(); ++i)
	{
		a = independent_set[i];
		for (size_t j = 0; j < not_independent.size(); ++j)
		{
			/* if the element not_independent[j] is free either in M1 or M2 it is obviously exchangeable. Unlike SAP, we, can't disregard "exchangeable" arcs
			   to/from free elements, as they can be used in an augmenting path (not the first) during a stage. You have an example showing this! */
			b = not_independent[j];

			if (O1->Exchangeable(a,b)) addEdge(graph,a,b);
			if (O2->Exchangeable(a,b)) addEdge(graph,b,a);
		}
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

	//DEBUG_VECTOR(X1);
	//DEBUG_VECTOR(X2);

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
			parent[neighb]     = current;
			distances[neighb]  = distances[current] + 1;
		}
	}

	if (endpoint == NO_AUGMENTATION) return false;

	assert(DISTANCE_TARGET%2==0);

	return true;
}


int FindArc(int a, vector<int> &next) {

	hack = 0;

	if (a == SOURCE)             while (hack < next.size()) if (O1->Free(next[hack++]))   return next[--hack];

	else if (IN_INDEPENDENT(a))  while (hack < next.size()) if (O1->Exchangeable(a,next[hack++])) return next[--hack];

	else if (!IN_INDEPENDENT(a) && O2->Free(a)) return TARGET;

	else if (!IN_INDEPENDENT(a)) while (hack < next.size()) if (O2->Exchangeable(next[hack++],a)) return next[--hack];

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

//size_t SAP(int N_, Oracle* oracle1, Oracle* oracle2)
size_t Cun86(int N_, Oracle* O1_, Oracle* O2_) {  // O (nr^{1.5}.T) 

	N = N_;

	O1 = O1_;
	O2 = O2_;

	InitCun();
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

//int main() {return 0;}