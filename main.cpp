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
#include "Rank.h"
#include "Indep.h"
#include "gen.h"
#include "Bipartite_Matching.h"

using namespace std;
using namespace std::chrono;

int V;
int E;

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

const int WRITE=0;

vector<Edge> solution;
vector<Edge> edgelist;

void printsol() {
	cout << "###Solution:\n";
	for (size_t i = 0; i < independent_set.size(); ++i) {
		cout << edgelist[independent_set[i]].u << " " << edgelist[independent_set[i]].v << endl;
	}
	cout << endl;
}

void pg(vector<vector<int>> adj) {
	cout << "Adj list\n";
	for (int i=0; i<V; i++)
	{
		cout << i << ": ";
		for (auto x : adj[i]) {
			cout << x << " ";
		}
		cout << endl;
	}
}

void BuildSolution() {
	solution.clear();
	for (size_t i = 0; i < independent_set.size(); ++i) {
		solution.push_back(edgelist[independent_set[i]]);
	}
}

vector<Edge> BuildSolution(vector<Edge> el, string name) {
	vector<Edge> res;
	solution.clear();
	cout << name << " Forest: ";
	for (size_t i = 0; i < independent_set.size(); ++i) {
		res.push_back(el[independent_set[i]]);
		cout << res[i].u << "-" << res[i].v << " | ";
	}
	cout << endl;
	return res;
}


int main() {

	size_t S;
	size_t C;
	size_t I;
	size_t R;
	//int HK;
	int EK;

	const int RUNS = 50;

	ofstream sap_file;
	ofstream cun_file;
	ofstream indep_file;
	ofstream rank_file;

	if (WRITE) {
		sap_file.open ("plots/stats/sap_ER_GRAPH.txt");
		cun_file.open ("plots/stats/cun_ER_GRAPH.txt");
		indep_file.open("plots/stats/indep_ER_GRAPH.txt");
		rank_file.open("plots/stats/rank_ER_GRAPH.txt");
		sap_file   << "p  n   r  #Q   O(#Q)   time(-6)  EXCHANGES" << endl;
		cun_file   << "p  n   r  #Q   O(#Q)   time(-6)  EXCHANGES" << endl;
		rank_file  << "p  n   r  #Q   O(#Q)   time(-6)  EXCHANGES" << endl;
		indep_file << "p  n   r  #Q   O(#Q)   time(-6)  EXCHANGES" << endl;
	}
	
	int w=1;

	/*E=10;
	while (E <= 200)
	{
		cout << "###Iteration number: " << w++ << ", Edges=" << E << endl;
		srand(73);
		
		pair<int,vector<Edge>> x = Generate_SimpleGraph(E);
		pair<int,vector<Edge>> y = Generate_SimpleGraph(E);
		for (auto e: x.second) cout << e.u << "-" << e.v << " | ";
		cout << "\n\n";
		for (auto e: y.second) cout << e.u << "-" << e.v << " | ";
		cout << "\n";

		Graphic *gr1 = new Graphic(x.first, x.second);
		Graphic *gr2 = new Graphic(y.first, y.second);
	
		assert(E==x.second.size());
		S = SAP(E,gr1,gr2);
		assertGraphic(BuildSolution(x.second,"SAP"),"SAP");
		assertGraphic(BuildSolution(y.second,"SAP"),"SAP");

		gr1 = new Graphic(x.first, x.second);
		gr2 = new Graphic(y.first, y.second);

		C = Cun86(E,gr1,gr2);
		assertGraphic(BuildSolution(x.second,"Cun"),"Cun");
		assertGraphic(BuildSolution(y.second,"Cun"),"Cun");
		assert(C==S);

		gr1 = new Graphic(x.first, x.second);
		gr2 = new Graphic(y.first, y.second);

		I = AugmentingPaths(E,gr1,gr2);
		assertGraphic(BuildSolution(x.second,"Indep"),"Indep");
		assertGraphic(BuildSolution(y.second,"Indep"),"Indep");
		assert(I==S);

		gr1 = new Graphic(x.first, x.second);
		gr2 = new Graphic(y.first, y.second);

		R = ExactRank(E,gr1,gr2);
		BuildSolution();
		assertGraphic(BuildSolution(x.second,"Rank"),"Rank");
		assertGraphic(BuildSolution(y.second,"Rank"),"Rank");
		assert(R==S);

		E++;
	}*/


	V=10; // RUN VALGRIND WITH SMALL VALUES OF V (hope to find seg fault on Cunningham's algorithm)
	int p=0;

	while (p <= 50)
	{
		int ct;

		pair<vector<vector<int>>,vector<Edge>> x = Generate_BipartiteGraph(V,p);
		vector<vector<int>> g=x.first;
		edgelist=x.second;
		
		cout << "###Iteration number: " << w++ << ", Edges=" << edgelist.size() << " with V=" << V << " and p=" << p << endl;

		auto rng = default_random_engine {}; //random_shuffle(edgelist.begin(), edgelist.end());

		LeftMatching  *lm;
		RightMatching *rm;

		int sumcallsavg  = 0;
		int maxcallsavg  = 0;
		int exchangesavg = 0;
		std::chrono::duration<long int, std::ratio<1, 1000000> >::rep totaltimeavg = 0;
		cout << "sap\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			S = SAP(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//printsol();
			BuildSolution();
			assertMatching(solution,V,"SAP");
			EK = EdmondsKarp(V,g);
			assert((int)S==EK);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		if (WRITE)
			sap_file << p << " " << edgelist.size() << " " << S << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cout << "cun\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			C = Cun86(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//printsol();
			BuildSolution();
			assertMatching(solution,V,"Cun");
			assert(C==S);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		if (WRITE)
			cun_file << p << " " << edgelist.size() << " " << C << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;
		
		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cout << "indep\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			I = AugmentingPaths(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//printsol();
			BuildSolution();
			assertMatching(solution,V,"Indep");
			assert(I==S);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		if (WRITE)
			indep_file << p << " " << edgelist.size() << " " << I << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cout << "rank\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(V,edgelist);
			rm = new RightMatching(V,edgelist);
			auto start = high_resolution_clock::now();
			//pg(g);
			R = ExactRank(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//printsol();
			BuildSolution();
			assertMatching(solution,V,"Rank");
			assert(R==S);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
 		}
		if (WRITE)
			rank_file << p << " " << edgelist.size() << " " << R << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;
		
		cout << endl;
		//V+=2;
		p+=5;
	}
	
	if (WRITE) {
		sap_file.close();
		cun_file.close();
		indep_file.close();
		rank_file.close();
	}

	cout << ">>SUCCESS\n";

	return 0;
}