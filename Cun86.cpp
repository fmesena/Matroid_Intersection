#include <algorithm>
#include <queue>
#include "Cun86.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;
static size_t hack;


bool GetDistances() {

	const int NO_ONE = -4;

	int 		current;
	int 		neighb;
	queue<int> 	q;
	vector<int> parent(N, NO_ONE);
	
	ClearCandidates();

	for (size_t i = 0; i < not_independent.size(); i++) 
	{
		int e = not_independent[i];
		if (distances[e]<=1)
		{
			if (O1->Free(e))
			{
				q.push(e);
				parent[e] = SOURCE;
				distances[e] = 1;
				candidates[1].push_back(e);
			}
		}
	}
	
	while (!q.empty())
	{	
		current = q.front(); q.pop();

    	if (IN_INDEPENDENT(current))
    	{
    		for (size_t i = 0; i < not_independent.size(); ++i)
    		{
    			neighb = not_independent[i];
    			if (parent[neighb] != NO_ONE) continue;
				if (!O1->Exchangeable(current, neighb)) continue;
				q.push(neighb);
				parent[neighb] = current;
				distances[neighb] = distances[current] + 1;
				candidates[distances[neighb]].push_back(neighb);
    		}
    	}
    	else
    	{
    		if (distances[current]>=DISTANCE_TARGET+1) {
        		if (O2->Free(current)) {
					DISTANCE_TARGET = distances[current] + 1;
					candidates[DISTANCE_TARGET].push_back(TARGET);
					assert(DISTANCE_TARGET%2==0);
					return true;
				}
    		}
    		for (size_t i = 0; i < independent_set.size(); ++i)
    		{
    			neighb = independent_set[i];
    			if (parent[neighb] != NO_ONE) continue;
				if (!O2->Exchangeable(neighb, current)) continue;
				q.push(neighb);
				parent[neighb] = current;
				distances[neighb] = distances[current] + 1;
				candidates[distances[neighb]].push_back(neighb);
			}
		}
	}
	return false;
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

	int	l 					= 	0;
	int	current 			= 	SOURCE;
	int	augmentations_ct 	=	 0;

	vector<int> path;
	vector<int> indexes;
	vector<int> prev_element;

	candidates[0].push_back(SOURCE);
	assert(candidates[0].size()==1);

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
			augmentations_ct++;
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
	AUGMENTATIONS.push_back(augmentations_ct);
	return;
}


size_t Cun86(int N_, Oracle* O1_, Oracle* O2_) {  // O (nr^{1.5}.T) 

	N  = N_;
	O1 = O1_;
	O2 = O2_;

	Init(N);
	for (int i = 0; i < N; ++i) {
		distances[i] = 1;
		not_independent.push_back(i);
	}
	DISTANCE_TARGET=0;
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