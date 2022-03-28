#include <algorithm>
#include <queue>
#include "Cun86.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;
static vector<bool> X1;
static vector<bool> X2;
static size_t hack;

static int exchanges;

void okCun() {
	cout <<"okCun\n";
	//DISTANCE_TARGET++;
	//cout << DISTANCE_TARGET << endl;
	return;
}

bool GetDistances() {   // O (...)

	const int NOT_VISITED = -4;

	queue<int> q;
	int current;
	int neighb;
	int parent[N];
	DISTANCE_TARGET = numeric_limits<int>::max();
	
	for (int i = 0; i < N; ++i)
	{
		parent[i]    = NOT_VISITED;
		distances[i] = numeric_limits<int>::max();
	}

	fill (X1.begin(),X1.end(),false);
	fill (X2.begin(),X2.end(),false);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		int e = not_independent[i];
		if (O1->Free(e))
		{
			X1[e] = true;
			q.push(e);
			parent[e] = SOURCE;
			distances[e]=1;
		}
		if (O2->Free(e)) {
			X2[e] = true;
		}
	}

	while (!q.empty())
	{	
		current = q.front(); q.pop();
		//cout <<"current: " << current << " with distance " << distances[current] << endl;

    	if (IN_INDEPENDENT(current))
    	{
    		for (size_t i = 0; i < not_independent.size(); ++i)
    		{
    			neighb = not_independent[i];
    			if (parent[neighb] != NOT_VISITED) continue;
				if (!O1->Exchangeable(current, neighb)) continue;
				q.push(neighb);
				exchanges++;
				parent[neighb] = current;
				distances[neighb] = distances[current] + 1;
    		}
    	}
    	else
    	{
    		if (X2[current])
    		{
				//DISTANCE_TARGET = min(distances[current]+1, DISTANCE_TARGET);
				//cout << "Updating DT\n";
				DISTANCE_TARGET = distances[current]+1;
				break;
    		}
    		for (size_t i = 0; i < independent_set.size(); ++i)
    		{
    			neighb = independent_set[i];
    			if (parent[neighb] != NOT_VISITED) continue;
				if (!O2->Exchangeable(neighb, current)) continue;
				q.push(neighb);
				exchanges++;
				parent[neighb] = current;
				distances[neighb] = distances[current] + 1;
			}
		}
	}

	if (DISTANCE_TARGET == numeric_limits<int>::max()) return false;
	assert(DISTANCE_TARGET != numeric_limits<int>::max()); //finite distance
	assert(DISTANCE_TARGET%2==0);						   //DT is always even
	return true;
}


int FindArc(int a, const vector<int> &next) {

	hack = 0;

	if (a == SOURCE) {
		while (hack < next.size()) {
			if (O1->Free(next[hack++])) {
				return next[--hack];
			}
		}
	}

	else if (IN_INDEPENDENT(a)) {
		while (hack < next.size()) {
			if (O1->Exchangeable(a,next[hack++])) {
				return next[--hack];
			}
		}
	}

	/*else if (!IN_INDEPENDENT(a) && O2->Free(a)) {
		return TARGET;
	}*/

	else if (!IN_INDEPENDENT(a)) {
		while (hack < next.size()) {
			if (O2->Exchangeable(next[hack++],a)) {
				return next[--hack];
			}
		}
	}

	return -1;
}


void Augment() {

	/*cout << "Augmenting\nPrinting distances\n";
	for (int i=0; i<N; i++) cout << "Vertex " << i << ": " << distances[i] << endl;
	cout << endl;*/

	ClearCandidates();
	for (int i = 0; i < N; ++i)
		if (distances[i] != numeric_limits<int>::max())
			candidates[distances[i]].push_back(i);

	candidates[0].push_back(SOURCE);
	candidates[DISTANCE_TARGET].push_back(TARGET);
	//cout << "DT: " << DISTANCE_TARGET << endl;
	assert(candidates[0].size()==1);
	//PrintCandidates();

	int l = 0;
	int current = SOURCE;
	vector<int> path;
	vector<int> indexes;
	vector<int> prev_element;

	while (l>=0)
	{	
		//cout << endl;
		//cout << "current: " << current << " at layer " << l << endl;

		//cout << "next layer: ";
		//for (int x:candidates[l+1]) cout << x << " ";
		//cout << endl;

		/*if (candidates[l+1].empty() && l!=DISTANCE_TARGET) {
    		return;
  		}*/

		if (l==DISTANCE_TARGET-1)
		{
			//cout << "CLOSE\n";
			if (!O2->Free(current)) {
				swap(candidates[l].back(), candidates[l][indexes.back()]);
				candidates[l].pop_back();
				current = prev_element.back();
				path.pop_back();
				indexes.pop_back();
				prev_element.pop_back();
				l--;
			}
			else {
				//cout << "finished\n";
				prev_element.push_back(current);
				l++;
			}
		}
		else if (l<DISTANCE_TARGET)
		{      		
			if (candidates[l+1].empty()) return;
      		//cout << "current vertex:" << current << " at layer " << l << "\n";
      		//DEBUG_VECTOR(candidates[l+1]);
			int next = FindArc(current, candidates[l+1]);
			//cout << "next: " << next << " at layer " << l+1 << endl;
			if (next == -1)
			{	
				// this block corresponds to the part of removing the so called "useless" elements
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
				path.push_back(next);
				indexes.push_back(hack);
				prev_element.push_back(current);

				current = next;
				l++;
			}
		}
		else if (l==DISTANCE_TARGET)
		{
			//cout << "xd2\n";
			for (size_t j=0; j<path.size(); j++)
			{
				in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;
				swap(candidates[distances[path[j]]].back(), candidates[distances[path[j]]][indexes[j]]);
				candidates[distances[path[j]]].pop_back();
			}

			UpdateIndependentSet();
			O1->Update_State(independent_set);
			O2->Update_State(independent_set);
			//PrintIndependentSet();

			vector<int>().swap(path);
			vector<int>().swap(indexes);
			vector<int>().swap(prev_element);
			
			current = SOURCE;

			l=0;
		}
		else if (l>DISTANCE_TARGET) {
			cerr << "ERROR: l>DISTANCE_TARGET" << endl;
		}
	}
	return;
}


size_t Cun86(int N_, Oracle* O1_, Oracle* O2_) {  // O (nr^{1.5}.T) 

	N  = N_;
	O1 = O1_;
	O2 = O2_;

	Init(N);
	for (int i = 0; i < N; ++i) {
		not_independent.push_back(i);
	}
	X1 = vector<bool>(N, false);
	X2 = vector<bool>(N, false);

	exchanges=0;

	while (GetDistances()) {
		//cout << "Target is reachable with distance " << DISTANCE_TARGET << "\n";
		Augment();
	}
	cout << "Number of exchanges: " << exchanges << endl;
	delete[] distances;

	/*for (int i = 0; i < N+1; i++) {
        delete[] candidates[i];
    }
    delete[] candidates;*/

	return SZ;
}

/*
Cunningham needs fixes: Augment() bool function makes no sense here, wrong halt condition in main cycle
*/

/*  OLD VERSION WHERE THE EXCHANGE GRAPH IS BUILT EXPLICITLY (even with the redudndant arcs)
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
			b = not_independent[j];

			if (O1->Exchangeable(a,b)) addEdge(graph,a,b);
			if (O2->Exchangeable(a,b)) addEdge(graph,b,a);
		}
	}
	return;
}

bool BFS() {   // O(nr)

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
*/