#include <algorithm>
#include <queue>
#include "Cun86.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;
static vector<bool> X1;
static vector<bool> X2;
static size_t hack;


bool GetDistances() {

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
				parent[neighb] = current;
				distances[neighb] = distances[current] + 1;
    		}
    	}
    	else
    	{
    		if (X2[current])
    		{
				DISTANCE_TARGET = distances[current]+1;
				break;
    		}
    		for (size_t i = 0; i < independent_set.size(); ++i)
    		{
    			neighb = independent_set[i];
    			if (parent[neighb] != NOT_VISITED) continue;
				if (!O2->Exchangeable(neighb, current)) continue;
				q.push(neighb);
				parent[neighb] = current;
				distances[neighb] = distances[current] + 1;
			}
		}
	}

	if (DISTANCE_TARGET == numeric_limits<int>::max()) return false;
	assert(DISTANCE_TARGET != numeric_limits<int>::max()); //finite distance
	assert(DISTANCE_TARGET%2==0);						   //DISTANCE_TARGET is always even
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

	ClearCandidates();
	for (int i = 0; i < N; ++i)
		if (distances[i] != numeric_limits<int>::max())
			candidates[distances[i]].push_back(i);

	candidates[0].push_back(SOURCE);
	candidates[DISTANCE_TARGET].push_back(TARGET);
	assert(candidates[0].size()==1);

	int l = 0;
	int current = SOURCE;
	int augmenting_paths_count = 0;
	vector<int> path;
	vector<int> indexes;
	vector<int> prev_element;

	while (l>=0)
	{	
		if (l==DISTANCE_TARGET-1)
		{
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
				prev_element.push_back(current);
				l++;
			}
		}
		else if (l<DISTANCE_TARGET)
		{      		
			if (candidates[l+1].empty())
				return;
			int next = FindArc(current, candidates[l+1]);
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
			augmenting_paths_count++;
			for (size_t j=0; j<path.size(); j++)
			{
				in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;
				swap(candidates[distances[path[j]]].back(), candidates[distances[path[j]]][indexes[j]]);
				candidates[distances[path[j]]].pop_back();
			}

			UpdateIndependentSet();
			O1->Update_State(independent_set);
			O2->Update_State(independent_set);

			vector<int>().swap(path);
			vector<int>().swap(indexes);
			vector<int>().swap(prev_element);
			
			current = SOURCE;

			l=0;
		}
		else if (l>DISTANCE_TARGET)
		{
			cerr << ">>ERROR: Cunningham::Augment, l>DISTANCE_TARGET" << endl;
		}
	}
	AUGMENTATIONS.push_back(augmenting_paths_count);
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
	AUGMENTATIONS.clear();
	
	while (GetDistances()) {
		Augment();
	}
	delete[] distances;

	/*for (int i = 0; i < N+1; i++) {
        delete[] candidates[i];
    }
    delete[] candidates;*/

	return SZ;
}