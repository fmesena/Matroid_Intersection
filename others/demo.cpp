/*#include <fstream>
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
int ITER_CT;

vector<Edge> solution;
vector<Edge> edgelist;

void printsol() {
	cout << "###Solution: "<< independent_set.size() << "\n";
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
	//cout << name << " Forest: ";
	for (size_t i = 0; i < independent_set.size(); ++i) {
		res.push_back(el[independent_set[i]]);
		//cout << res[i].u << "-" << res[i].v << " | ";
	}
	//cout << endl;
	return res;
}


int main() {

	size_t S;
	size_t C;
	size_t I;
	size_t R;
	int HK;
	int EK;

	int iter;
	const int RUNS  = 20;
	const int WRITE = 1;

	ofstream sap_file;
	ofstream cun_file;
	ofstream indep_file;
	ofstream rank_file;

	ofstream ek;
	ofstream hk;

	if (WRITE) {
		sap_file.open ("plots/stats/sap_blocks2.txt");
		cun_file.open ("plots/stats/cun_blocks2.txt");
		indep_file.open("plots/stats/indep_blocks2.txt");
		rank_file.open("plots/stats/rank_blocks2.txt");
		ek.open("plots/stats/hk_blocks2.txt");
		hk.open("plots/stats/ek_blocks2.txt");
		sap_file   << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES  V   groups" << endl;
		cun_file   << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES  V   groups" << endl;
		rank_file  << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES  V   groups" << endl;
		indep_file << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES  V   groups" << endl;
		ek << "V   groups   E   Sol   time(-6)  " << endl;
		hk << "V   groups   E   Sol   time(-6)  " << endl;
		rank_file.open("plots/stats/rank_approx.txt");
		rank_file  << "n    r   #Q     O(#Q)    time(-6)   EXCHANGES  V   groups   epsilon  Iter_ct" << endl;
	}

	
	/*iter=1;
	V=50;
	Graphic *gr1;
	Graphic *gr2;
	while (V <= 1000)
	{
		srand(73);
		
		auto [V1, el1] = GenerateDuplicationDivergence(V);
		auto [V2, el2] = GenerateMultiGraph((int)el1.size());
		
		cout << "###Iteration number: " << iter++ << ", Edges=" << el1.size() << " " << el2.size() << ", Vertices=" << V1 << " " << V2 << endl;

		assert(el1.size() == el2.size());
		E = el1.size();

		int  ct;
		auto rng = default_random_engine {};
		
		int sumcallsavg  = 0;
		int maxcallsavg  = 0;
		int exchangesavg = 0;
		std::chrono::duration<long int, std::ratio<1, 1000000> >::rep totaltimeavg = 0; //microseconds

		cerr << "   Running SAP\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(el1), end(el1), rng);
			shuffle(begin(el2), end(el2), rng);
			gr1 = new Graphic(V1, el1);
			gr2 = new Graphic(V2, el2);
			auto start = high_resolution_clock::now();
			S = SAP(E,gr1,gr2);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			assertGraphic(BuildSolution(el1,"SAP"),"SAP");
			assertGraphic(BuildSolution(el2,"SAP"),"SAP");
			sumcallsavg  += gr1->getOracleCalls()+gr2->getOracleCalls();
			maxcallsavg  += max(gr1->getOracleCalls(),gr2->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += gr1->getExchangeCalls()+gr2->getExchangeCalls();
			assert(gr1->getExchangeCalls()+gr1->getFreeCalls()==gr1->getOracleCalls());
			assert(gr2->getExchangeCalls()+gr2->getFreeCalls()==gr2->getOracleCalls());
			delete gr1;
			delete gr2;
		}
		if (WRITE)
			sap_file << E << " " << S << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Cun\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(el1), end(el1), rng);
			shuffle(begin(el2), end(el2), rng);
			gr1 = new Graphic(V1, el1);
			gr2 = new Graphic(V2, el2);
			auto start = high_resolution_clock::now();
			C = Cun86(E,gr1,gr2);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			assertGraphic(BuildSolution(el1,"Cun"),"Cun");
			assertGraphic(BuildSolution(el2,"Cun"),"Cun");
			assert(C==S);
			sumcallsavg  += gr1->getOracleCalls()+gr2->getOracleCalls();
			maxcallsavg  += max(gr1->getOracleCalls(),gr2->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += gr1->getExchangeCalls()+gr2->getExchangeCalls();
			assert(gr1->getExchangeCalls()+gr1->getFreeCalls()==gr1->getOracleCalls());
			assert(gr2->getExchangeCalls()+gr2->getFreeCalls()==gr2->getOracleCalls());
			delete gr1;
			delete gr2;
 		}
		if (WRITE)
			cun_file << E << " " << C << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;
		//for (auto ap : AUGMENTATIONS) cun_file << ap << " ";
		//cun_file << endl;
		//cun_file << ITER_CT << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Indep\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(el1), end(el1), rng);
			shuffle(begin(el2), end(el2), rng);
			gr1 = new Graphic(V1, el1);
			gr2 = new Graphic(V2, el2);
			auto start = high_resolution_clock::now();
			I = AugmentingPaths(E,gr1,gr2);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			assertGraphic(BuildSolution(el1,"Indep"),"Indep");
			assertGraphic(BuildSolution(el2,"Indep"),"Indep");
			assert(I==S);
			sumcallsavg  += gr1->getOracleCalls()+gr2->getOracleCalls();
			maxcallsavg  += max(gr1->getOracleCalls(),gr2->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += gr1->getExchangeCalls()+gr2->getExchangeCalls();
			//assert(gr1->getExchangeCalls()+gr1->getFreeCalls()==gr1->getOracleCalls());
			//assert(gr2->getExchangeCalls()+gr2->getFreeCalls()==gr2->getOracleCalls());
			delete gr1;
			delete gr2;
 		}
		if (WRITE)
			indep_file << E << " " << I << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Rank\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(el1), end(el1), rng);
			shuffle(begin(el2), end(el2), rng);
			gr1 = new Graphic(V1, el1);
			gr2 = new Graphic(V2, el2);
			auto start = high_resolution_clock::now();
			R = ExactRank(E,gr1,gr2);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			assertGraphic(BuildSolution(el1,"Rank"),"Rank");
			assertGraphic(BuildSolution(el2,"Rank"),"Rank");
			assert(R==S);
			sumcallsavg  += gr1->getOracleCalls()+gr2->getOracleCalls();
			maxcallsavg  += max(gr1->getOracleCalls(),gr2->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += gr1->getExchangeCalls()+gr2->getExchangeCalls();
			//assert(gr1->getExchangeCalls()+gr1->getFreeCalls()==lm->getOracleCalls());
			//assert(gr2->getExchangeCalls()+gr2->getFreeCalls()==gr2->getOracleCalls());
			delete gr1;
			delete gr2;
 		}
		if (WRITE)
			rank_file << E << " " << R << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << endl;
		//for (auto ap : AUGMENTATIONS) rank_file << ap << " ";
		//rank_file << endl;
		//rank_file << ITER_CT << endl;
		
		cout << endl;
		//E += 10;
		V += 25;
	}
	
	
	int groups = 32;

	int p = 70;
	iter  = 1;
	V 	  = 9600;
	LeftMatching  *lm;
	RightMatching *rm;

	double eps = 0.1;
	pair<vector<vector<int>>,vector<Edge>> x = GenerateBlocksBipartiteGraph(V,groups); //GeneratePHPGraph(V); //auto [g, edgelist] = GenerateRandomBipartiteGraph(V,p);
	vector<vector<int>> g=x.first;
	edgelist=x.second;

	while (eps <= 1)
	{

		cout << "###Iteration number: " << iter++ << ", Edges=" << edgelist.size() << " with V=" << V << " and groups=" << groups << endl;

		int  ct;
		auto rng = default_random_engine {};

		int sumcallsavg  = 0;
		int maxcallsavg  = 0;
		int exchangesavg = 0;
		int itersavg=0;
		std::chrono::duration<long int, std::ratio<1, 1000000> >::rep totaltimeavg = 0; //microseconds

		cout << EdmondsKarp(2*V,g) << endl;

		cerr << "   Running ApproxRank\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(2*V,edgelist);
			rm = new RightMatching(2*V,edgelist);
			auto start = high_resolution_clock::now();
			R = ApproxRank(edgelist.size(),lm,rm,eps);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			itersavg += ITER_CT;
			delete lm;
			delete rm;
 		}
		if (WRITE)
			rank_file << edgelist.size() << " " << R << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
					  << V << " " << groups << " " << eps << " " << ITER_CT/RUNS << endl;
		cout << endl;
		eps += 0.1;

		cerr << "   Running SAP\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(2*V,edgelist);
			rm = new RightMatching(2*V,edgelist);
			auto start = high_resolution_clock::now();
			S = SAP(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			BuildSolution();
			//assertMatching(solution,V,"SAP");
			//assert((int)S==EK);
			//assert(HK==EK);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			//assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			//assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		if (WRITE)
			sap_file << edgelist.size() << " " << S << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
					 << V << " " << groups << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running EK\n";
		for (ct=0; ct<RUNS; ct++)
		{
			auto start = high_resolution_clock::now();
			EK = EdmondsKarp(2*V,g);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			assert(EK==S);
			totaltimeavg += duration.count();
 		}
		if (WRITE)
			ek << V << " " << groups << " " << edgelist.size() << " " << EK << " " << totaltimeavg/RUNS << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running HK\n";
		for (ct=0; ct<RUNS; ct++)
		{
			auto start = high_resolution_clock::now();
			HK = HopcroftKarp(2*V,g);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			assert(EK==HK);
			totaltimeavg += duration.count();
 		}
		if (WRITE)
			hk << V << " " << groups << " " << edgelist.size() << " " << HK << " " << totaltimeavg/RUNS << endl;

		sumcallsavg  = 0;
		maxcallsavg  = 0;
		exchangesavg = 0;
		totaltimeavg = 0;
		cerr << "   Running Cun\n";
		for (ct=0; ct<RUNS; ct++)
		{
			shuffle(begin(edgelist), end(edgelist), rng);
			lm = new LeftMatching(2*V,edgelist);
			rm = new RightMatching(2*V,edgelist);
			auto start = high_resolution_clock::now();
			C = Cun86(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//BuildSolution();
			//assertMatching(solution,V,"Cun");
			assert(C==S);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			//assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			//assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		if (WRITE)
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
			lm = new LeftMatching(2*V,edgelist);
			rm = new RightMatching(2*V,edgelist);
			auto start = high_resolution_clock::now();
			I = AugmentingPaths(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//BuildSolution();
			//assertMatching(solution,V,"Indep");
			assert(I==S);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			//assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			//assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
		}
		if (WRITE)
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
			lm = new LeftMatching(2*V,edgelist);
			rm = new RightMatching(2*V,edgelist);
			auto start = high_resolution_clock::now();
			R = ExactRank(edgelist.size(),lm,rm);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//printsol();
			//BuildSolution();
			//assertMatching(solution,V,"Rank");
			assert(R==S);
			sumcallsavg  += lm->getOracleCalls()+rm->getOracleCalls();
			maxcallsavg  += max(lm->getOracleCalls(),rm->getOracleCalls());
			totaltimeavg += duration.count();
			exchangesavg += lm->getExchangeCalls()+rm->getExchangeCalls();
			//assert(lm->getExchangeCalls()+lm->getFreeCalls()==lm->getOracleCalls());
			//assert(rm->getExchangeCalls()+rm->getFreeCalls()==rm->getOracleCalls());
			delete lm;
			delete rm;
 		}
		if (WRITE)
			rank_file << edgelist.size() << " " << R << " " << sumcallsavg/RUNS << " " << maxcallsavg/RUNS << " " << totaltimeavg/RUNS << " " << exchangesavg/RUNS << " "
					  << V << " " << groups << endl;
		
		cout << endl;
		V+=256;
		//p+=5;
	}
	
	if (WRITE) {
		//sap_file.close();
		//cun_file.close();
		//indep_file.close();
		rank_file.close();
		//ek.close();
		//hk.close();
	}

	cout << ">>SUCCESS\n";

	return 0;
}

*/