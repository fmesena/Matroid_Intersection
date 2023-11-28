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
#include "CLSSW_Indep.h"
#include "CLSSW_Approx.h"
#include "CLSSW_Rank.h"
#include "gen.h"
#include "EdmondsKarp.h"
#include "HopcroftKarp.h"

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

int V=10;
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

vector<Edge> BuildSolution(vector<Edge> el) {
	vector<Edge> res = vector<Edge>(independent_set.size());
	for (size_t i = 0; i < independent_set.size(); ++i) {
		//cout << independent_set[i] << endl;
		res[i] = el[independent_set[i]];
	}
	return res;
}


int main() {

	size_t 			  S;	
	LeftMatching 	*lm;
	RightMatching 	*rm;

	auto [G, el] = GenerateRandomBipartiteGraph(V,30);

	printGraph(G);

	lm = new LeftMatching (V, el);
	rm = new RightMatching(V, el);

	S  = SAP(el.size(),lm,rm);
	
	auto matching = BuildSolution(el);
	assertMatching(matching,V,"SAP");
	PrintMatching(matching);

	delete lm;
	delete rm;

	cout << ">>SUCCESS " << S << endl;

	return 0;
}


/*

int main() {

	size_t 		  S;	
	Partition 	*lm;
	Partition 	*rm;

	auto [G, el] = GenerateRandomBipartiteGraph(V,40);

	printGraph(G);

	vector<vector<int>>  left_side = vector<vector<int>>(V/2);
	vector<vector<int>> right_side = vector<vector<int>>(V/2);
	vector<int>  left_cap = vector<int>(V/2,1);
	vector<int> right_cap = vector<int>(V/2,1);

	for (size_t i = 0; i < el.size(); ++i)
	{
		Edge e = el[i];
		left_side[e.u].push_back(i);
		right_side[e.v-V/2].push_back(i);
	}

	for (Edge e : el)
		cout << e.u << "-" << e.v << " ";

	lm = new Partition( (int) el.size(),  left_side, left_cap);
	rm = new Partition( (int) el.size(), right_side, right_cap);

	S  = SAP(el.size(),lm,rm);
	
	auto matching = BuildSolution(el);
	PrintMatching(matching);
	assertMatching(matching,V,"SAP");

	delete lm;
	delete rm;

	cout << ">>SUCCESS " << S << endl;

	return 0;
}


int main() {

	size_t 			  S;	
	LeftMatching 	*lm;
	RightMatching 	*rm;

	auto [G, el] = GenerateRandomBipartiteGraph(V,30);

	printGraph(G);

	lm = new LeftMatching (V, el);
	rm = new RightMatching(V, el);

	S  = SAP(el.size(),lm,rm);
	
	auto matching = BuildSolution(el);
	assertMatching(matching,V,"SAP");
	PrintMatching(matching);

	delete lm;
	delete rm;

	cout << ">>SUCCESS " << S << endl;

	return 0;
}


int main() {

	size_t 		I;
	Graphic 	*gr1;
	Graphic 	*gr2;
	
	auto [V1, el1] = GenerateDuplicationDivergence(V);
	auto [V2, el2] = GenerateMultiGraph((int)el1.size());

    for (auto e:el1) cout << e.u << "-" << e.v << endl;
    cout << endl;
    for (auto e:el2) cout << e.u << "-" << e.v << endl;


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
*/





/*

int main() {

	size_t 		I;
	Graphic 	*gr1;
	Graphic 	*gr2;

	int E = 700;

	auto rng = default_random_engine {};

	cout << "VERTICES: " << V << endl;
	
	//auto [V1, el1] = GenerateDuplicationDivergence(V);
	auto [V, el1] = GenerateMultiGraph(E);
	auto el2 = el1;
	shuffle(el2.begin(), el2.end(), rng);

	gr1 = new Graphic(V, el1);
	gr2 = new Graphic(V, el2);

	
	LeftMatching 	*lm;
	RightMatching 	*rm;

	int V = 11000;
	auto [G, el] = GenerateRandomBipartiteGraph(V,65);
	auto rng = default_random_engine {};
	shuffle(begin(el), end(el), rng);

	lm = new LeftMatching(V, el);
	rm = new RightMatching(V, el);

	I 	= AugmentingPaths(E,gr1,gr2);
	
	assertGraphic(BuildSolution(el1),"CLSSW-independence");
	assertGraphic(BuildSolution(el2),"CLSSW-independence");
	//PrintForest(el1);
	//PrintForest(el2);
	
	delete gr1;
	delete gr2;

	cout << ">>SUCCESS " << I << endl;

	return 0;
}*/


/*

	LeftMatching 	*lm;
	RightMatching 	*rm;

	//sap_blocks_BIG

	ofstream sap_file;
	ofstream cun_file;
	ofstream indep_file;
	ofstream rank_file;
	sap_file.open("plots/stats/sap_blocks_BIG.txt");
	cun_file.open("plots/stats/cun_blocks_BIG.txt");
	indep_file.open("plots/stats/indep_blocks_BIG.txt");
	rank_file.open("plots/stats/rank_blocks_BIG.txt");
	sap_file   << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES   V  groups" << endl;
	cun_file   << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES   V  groups" << endl;
	indep_file << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES   V  groups" << endl;
	rank_file  << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES   V  groups" << endl;

	int  ct,S,C,I,R;
	int iter = 0;
	int RUNS = 5;
	double eps=0.1;
	int sumcallsavg  = 0;
	int maxcallsavg  = 0;
	int exchangesavg = 0;
	int itersavg = 0;
	std::chrono::duration<long int, std::ratio<1, 1000000> >::rep totaltimeavg = 0; //microseconds

	int V = 2560;
	int groups = 32;

	
	while (V <= 25600)
	{
		auto [G, edgelist] = GenerateBlocksBipartiteGraph(V,32); //GeneratePHPGraph(V); //auto [g, edgelist] = GenerateRandomBipartiteGraph(V,p);

		cout << "###Iteration number: " << iter++ << ", Edges=" << edgelist.size() << " with V=" << V << " and groups=" << groups << endl;

		int  ct;
		auto rng = default_random_engine {};

		int sumcallsavg  = 0;
		int maxcallsavg  = 0;
		int exchangesavg = 0;
		int itersavg=0;
		std::chrono::duration<long int, std::ratio<1, 1000000> >::rep totaltimeavg = 0; //microseconds

		cerr << "   Running SAP\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			S = SAP(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		sap_file << edgelist.size() << " " << S << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
				 << V << " " << groups << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Cun\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			C = Cun86(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		cun_file << edgelist.size() << " " << C << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
				 << V << " " << groups << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Indep\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			I = AugmentingPaths(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		indep_file << edgelist.size() << " " << I << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
				   << V << " " << groups << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Rank\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			R = ExactRank(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);;
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
 		}
		rank_file << edgelist.size() << " " << R << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
				  << V << " " << groups << endl;
		
		V+=64;
	}
*/




/*MEMORY PEAK:
valgrind --tool=massif --max-snapshots=200 --detailed-freq=5
https://github.com/KDE/massif-visualizer

VISUALIZE EXECUTION FLOW GRAPH WITH COUNTS
gprof
https://users.cs.duke.edu/~ola/courses/programming/gprof.html
*/