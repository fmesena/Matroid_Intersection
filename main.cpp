#include <fstream>
#include <assert.h>
#include <algorithm>
#include <random>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include <set>
#include "SAP.h"
#include "Cun86.h"
#include "CLSSW_Indep.h"
#include "CLSSW_Approx.h"
#include "CLSSW_Rank.h"
#include "gen.h"
#include "EdmondsKarp.h"
#include "HopcroftKarp.h"
#include "oracleheaders.h"

using namespace std;
using namespace std::chrono;


vector<int>  		independent_set;
vector<int>  		not_independent;
vector<bool> 		in_independent_set;
vector<int>  		index_;  // index[i]=-1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
vector<vector<int>> candidates;
vector<int>  		AUGMENTATIONS;

int  				DISTANCE_TARGET;
int 				*distances;
int 				MAX_DIST;
int 				ITER_CT;

int V=8;
int E;
int N;

void PrintForest(vector<Edge> g) {
	cout << "###Solution size: "<< independent_set.size() << "\n";
	for (size_t i = 0; i < independent_set.size(); ++i) {
		cout << g[independent_set[i]].u << " " << g[independent_set[i]].v << endl;
	}
	cout << endl;
}

void PrintMatching(vector<Edge> sol) {
	cout << "###Solution size: "<< independent_set.size() << "\n";
	for (Edge e : sol) cout << e.u << " " << e.v << endl;
	cout << endl;
}

vector<Edge> BuildMatching(vector<Edge> el) {
	vector<Edge> res = vector<Edge>(independent_set.size());
	for (size_t i = 0; i < independent_set.size(); ++i) {
		//cout << independent_set[i] << endl;
		res[i] = el[independent_set[i]];
	}
	return res;
}

vector<pair<int,int>> BuildAllocation(vector<pair<int,int>> Z) {
	vector<pair<int,int>> res = vector<pair<int,int>>(independent_set.size());
	for (size_t i = 0; i < independent_set.size(); ++i) {
		res[i] = Z[independent_set[i]];
	}
	return res;
}

void PrintAllocation(vector<pair<int,int>> sol) {
	cout << "###Solution size: "<< independent_set.size() << "\n";
	for (auto p : sol) cout << "(" << p.first+1 << ", " << p.second+1 << ")" << endl;
	cout << endl;
}


int main() {

	int n = 4; //the number of agents
	int m = 5; //the number of items (i.e., categories)

	vector<pair<int,int>> ground_set;

	vector<int> items_capacity  = vector<int>(m,1); //ensures that every item is given to at most one agent
	vector<int> agents_capacity = vector<int>(n,2); //each agent can be given at most 2 items (in general, the capacities need not be all the same, and can be read e.g. from a textfile)

	vector<vector<pair<int,int>>> category_a = vector<vector<pair<int,int>>>(m);
	vector<vector<pair<int,int>>> category_b = vector<vector<pair<int,int>>>(n);

	vector<set<int>> availability = vector<set<int>>(n); // availability[i] is a set with the items that agent i can be allocated to. this information could be read e.g. from a textfile
	for (int i = 0; i < n; i++)
		for (int g = 0; g < m; g++)
			if ( i-g >= 0 ) //item g can be allocated to agent i if this condition holds
				availability[i].insert(g);

	for (int g = 0; g < m; g++)
	{
		for (int i = 0; i < n; i++)
		{
			auto p = make_pair(i,g);
			ground_set.push_back(p);
			category_a[g].push_back(p);

			if (availability[i].count(g) > 0) // check if item g can be allocated to agent i. if so, add the element (i,g) to the i-th category of matroid_b
				category_b[i].push_back(p);
		}
	}

	Partition<pair<int,int>> *partition_a = new Partition<pair<int,int>> ( (int) ground_set.size(), ground_set, category_a, items_capacity);
	Partition<pair<int,int>> *partition_b = new Partition<pair<int,int>> ( (int) ground_set.size(), ground_set, category_b, agents_capacity);

	size_t S  = SAP(ground_set.size(),partition_a,partition_b);
	
	auto allocation = BuildAllocation(ground_set);
	PrintAllocation(allocation);

	delete partition_a;
	delete partition_b;

	cout << ">>SUCCESS " << S << endl;

	return 0;
}


/*

int main() {

	size_t S;
	auto [G, el] = GenerateRandomBipartiteGraph(V,40);

	printGraph(G);

	//asssume that there is an equal number of vertices on the left and right side of the bipartition
	vector<vector<Edge>>  left_side = vector<vector<Edge>>(V/2);
	vector<vector<Edge>> right_side = vector<vector<Edge>>(V/2);
	vector<int>  left_cap = vector<int>(V/2,1);
	vector<int> right_cap = vector<int>(V/2,1);

	for (size_t i = 0; i < el.size(); ++i)
	{
		Edge e = el[i];
		left_side[e.u].push_back(e);
		right_side[e.v-V/2].push_back(e);
	}

	Partition<Edge> *lm = new Partition<Edge> ( (int) el.size(), el,  left_side,  left_cap);
	Partition<Edge> *rm = new Partition<Edge> ( (int) el.size(), el, right_side, right_cap);

	S  = SAP(el.size(),lm,rm);
	
	auto matching = BuildMatching(el);
	PrintMatching(matching);
	assertMatching(matching,V,"SAP");

	delete lm;
	delete rm;

	cout << ">>SUCCESS " << S << endl;

	return 0;
}


*/