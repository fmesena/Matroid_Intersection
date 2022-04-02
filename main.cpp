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

vector<Edge> solution;

vector<Edge> edgelist;
void printsol() {
	cout << "###Solution:\n";
	for (int i = 0; i < independent_set.size(); ++i) {
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
	for (int i = 0; i < independent_set.size(); ++i) {
		solution.push_back(edgelist[independent_set[i]]);
	}
}


int main() {

	int RUNS = 1;
	int S  = 0;
	int C  = 0;
	int I  = 0;
	int R  = 0;
	int HK = 0;
	int EK = 0;

	ofstream sap_file;
	ofstream cun_file;
	ofstream indep_file;
	ofstream rank_file;
	sap_file.open ("plots/sap.txt");
	cun_file.open ("plots/cun.txt");
	indep_file.open("plots/indep.txt");
	rank_file.open("plots/rank.txt");
	sap_file   << "n  r  total_calls  O(calls)  time(microseconds)" << endl;
	cun_file   << "n  r  total_calls  O(calls)  time(microseconds)" << endl;
	rank_file  << "n  r  total_calls  O(calls)  time(microseconds)" << endl;
	indep_file << "n  r  total_calls  O(calls)  time(microseconds)" << endl;

	/*Graphic *gr= new Graphic(5);
	gr->show();*/

	V=4;
	while (V <= 200) 
	{
		//V = rand() % 10+6;  does not generate uniformly distributed random numbers in the span (since in most cases this operation makes lower numbers slightly more likely)
		pair<vector<vector<int>>,vector<Edge>> x = Generate_BipartiteGraph(V);
		vector<vector<int>> g=x.first;
		edgelist=x.second;

		cout << "###Iteration number: " << RUNS++ << ", Edges=" << edgelist.size() << " with V=" << V << endl;

		//random_shuffle(edgelist.begin(), edgelist.end());
		auto rng = default_random_engine {};
		shuffle(begin(edgelist), end(edgelist), rng);
		//cout << "Shuffled:\n";
		/*for (Edge e : edgelist)
			cout << e.u << " " << e.v << endl;*/

		LeftMatching  *lm = new LeftMatching(V,edgelist);
		RightMatching *rm = new RightMatching(V,edgelist);

		auto start = high_resolution_clock::now();
		S = SAP(edgelist.size(),lm,rm);
		//printsol();
		BuildSolution();
		assertMatching(solution,V,"SAP");
		EK = EdmondsKarp(V,g);
		assert(S==EK);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		sap_file << edgelist.size() << " " << S << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		/*cout << "###SAP\n";
		cout << "###Ground set size: " << edgelist.size() << endl;
		cout << "###Independent set found with size " << S <<  endl;
		////printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: " << lm->getOracleCalls()+rm->getOracleCalls() << endl; //max(lm->getOracleCalls(),rm->getOracleCalls())
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;*/

		lm = new LeftMatching(V,edgelist);
		rm = new RightMatching(V,edgelist);
		start = high_resolution_clock::now();
		C = Cun86(edgelist.size(),lm,rm);
		//printsol();
		BuildSolution();
		assertMatching(solution,V,"Cun");
		assert(C==S);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cun_file << edgelist.size() << " " << C << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		/*cout << "###Cun86\n";
		cout << "###Independent set found with size " << C << endl;
		//printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: "    << lm->getOracleCalls()+rm->getOracleCalls() << endl; 
		cout << "   O(Number of calls): " << max(lm->getOracleCalls(),rm->getOracleCalls()) << endl;
		cout << "   O1 calls: " << lm->getOracleCalls() << "; O2 calls: " << rm->getOracleCalls() << endl;
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;*/

		lm = new LeftMatching(V,edgelist);
		rm = new RightMatching(V,edgelist);
		start = high_resolution_clock::now();
		I = AugmentingPaths(edgelist.size(),lm,rm);
		//printsol();
		BuildSolution();
		assertMatching(solution,V,"Indep");
		assert(I==S);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		indep_file << edgelist.size() << " " << I << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		/*cout << "\n###FMI Indep\n";
		cout << "###Independent set found with size " << I <<  endl;
		////printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: "    << lm->getOracleCalls()+rm->getOracleCalls() << endl; 
		cout << "   O(Number of calls): " << max(lm->getOracleCalls(),rm->getOracleCalls()) << endl;
		cout << "   O1 calls: " << lm->getOracleCalls() << "; O2 calls: " << rm->getOracleCalls() << endl;
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;*/

		lm = new LeftMatching(V,edgelist);
		rm = new RightMatching(V,edgelist);
		start = high_resolution_clock::now();
		//pg(g);
		R = ExactRank(edgelist.size(),lm,rm);
		//printsol();
		BuildSolution();
		//assertMatching(solution,V,"Rank");
		assert(R==S);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		rank_file << edgelist.size() << " " << R << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		
		/*cout << "\n###FMI Rank\n";
		cout << "###Independent set found with size " << R <<  endl;
		//printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: "    << lm->getOracleCalls()+rm->getOracleCalls() << endl; 
		cout << "   O(Number of calls): " << max(lm->getOracleCalls(),rm->getOracleCalls()) << endl;
		cout << "   O1 calls: " << lm->getOracleCalls() << "; O2 calls: " << rm->getOracleCalls() << endl;
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;*/
		cout << endl;
		V+=2;
	}
	
	sap_file.close();
	cun_file.close();
	indep_file.close();
	rank_file.close();

	return 0;
}


/*void FindGreedy() {   // O (n.T) 
	for (int i = 0; i < N; ++i)
	{
		if (O1->Free(i) && O2->Free(i))
		{
			independent_set.push_back(i);
			in_independent_set[i]=true;
			//Update_State();
		}
	}
	return;
}*/