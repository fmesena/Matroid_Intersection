#include <fstream>
#include <assert.h>
#include <algorithm>
#include <random>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include "oracleheaders.h"
#include "SAP.h"
#include "Cun86.h"
#include "Indep.h"
#include "Rank.h"
#include "gen.h"
#include "EdmondsKarp.h"
#include "HopcroftKarp.h"

using namespace std;


int N;
vector<int>  independent_set;
vector<int>  not_independent;
vector<bool> in_independent_set;
vector<int>  index_;  // index[i]=-1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
int  DISTANCE_TARGET;
int *distances;
vector<vector<int>> candidates;
vector<int>  AUGMENTATIONS;
int CURRENT_RANK;
int MAX_DIST;
int ITER_CT;


int V=50;
int E;
vector<Edge> solution;


void PrintForest(vector<Edge> g) {
	cout << "###Solution size: "<< independent_set.size() << "\n";
	for (size_t i = 0; i < independent_set.size(); ++i) {
		cout << g[independent_set[i]].u << " " << g[independent_set[i]].v << endl;
	}
	cout << endl;
}

vector<Edge> BuildSolution(vector<Edge> el) {
	vector<Edge> res;
	solution.clear();
	for (size_t i = 0; i < independent_set.size(); ++i) {
		res.push_back(el[independent_set[i]]);
	}
	return res;
}


int main() {

	size_t 		I;
	Graphic 	*gr1;
	Graphic 	*gr2;
	
	auto [V1, el1] = GenerateDuplicationDivergence(V);
	auto [V2, el2] = GenerateMultiGraph((int)el1.size());

	gr1 = new Graphic(V1, el1);
	gr2 = new Graphic(V2, el2);
	
	I 	= AugmentingPaths(el1.size(),gr1,gr2);
	
	assertGraphic(BuildSolution(el1),"CLSSW-independence");
	assertGraphic(BuildSolution(el2),"CLSSW-independence");
	PrintForest(el1);
	PrintForest(el2);
	
	delete gr1;
	delete gr2;

	cout << ">>SUCCESS " << I << endl;

	return 0;
}