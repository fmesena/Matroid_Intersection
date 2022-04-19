#include <algorithm>
#include <queue>
#include "SAP.h"

using namespace std;

static Oracle* O1;
static Oracle* O2;
static vector<bool> X1;
static vector<bool> X2;

bool BFS_Augment() {   // O (n + n.T + nr.T + 2r)

	const int NO_ONE = -4;
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

	O1->Update_State(independent_set);
	O2->Update_State(independent_set);

	vector<int> parent(N, NO_ONE);
	queue<int>  q;

	fill (X1.begin(),X1.end(),false);
	fill (X2.begin(),X2.end(),false);
	for (size_t i = 0; i < not_independent.size(); i++)
	{
		int e = not_independent[i];
		if (O1->Free(e)) X1[e] = true;
		if (O2->Free(e)) X2[e] = true;
		if (X1[e] && X2[e])
		{
			in_independent_set[not_independent[i]] = true;
			return true;
		}
		if (X1[e]) 
		{
			q.push(e);
			parent[e] = SOURCE;
		}
	}

	int current;
	int neighb;
	int endpoint = NO_AUGMENTATION;

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
    			if (parent[neighb] != NO_ONE) continue;
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
	return true;
}


size_t SAP(int N_, Oracle* O1_, Oracle* O2_) {  // O (n.r^2.T) 

	N  = N_;
	O1 = O1_;
	O2 = O2_;

	in_independent_set.resize(N, false);
	in_independent_set.clear();
	independent_set.clear();
	not_independent.clear();

	X1 = vector<bool>(N, false);
	X2 = vector<bool>(N, false);
	for (int i = 0; i < N; ++i) {
		not_independent.push_back(i);
	}

	while (BFS_Augment());

	return SZ;
}