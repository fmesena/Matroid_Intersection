#include <fstream>
#include <assert.h>
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
vector<int>  AUGMENTATIONS;
vector<int>  independent_set;
vector<int>  not_independent;
vector<bool> in_independent_set;
vector<int>  index_;  // index[i]=-1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
int  DISTANCE_TARGET;
int *distances;
vector<vector<int>> candidates;
int CURRENT_RANK;
int MAX_DIST;

vector<Edge> edgelist;
void printsol() {
	cout << "###Solution:\n";
	for (int i = 0; i < independent_set.size(); ++i) {
		cout << edgelist[independent_set[i]].u << " " << edgelist[independent_set[i]].v << endl;
	}
	cout << endl;
}

int main() {

	//Graphic *g = new Graphic(5);
	//g->show();

	int RUNS = 1;
	int S  = 0;
	int C  = 0;
	int I  = 0;
	int HK = 0;
	int EK = 0;

	ofstream myfile;
	myfile.open ("plots/stats.txt");

	/*okSAP();
	okCun();
	okGEN();
	okRank();
	okIndep();
	okBM();*/
	V=10;
	while (V <= 30) 
	{		
		//V = rand() % 10+6;  //does not generate uniformly distributed random numbers in the span (since in most cases this operation makes lower numbers slightly more likely)
		//V = RUNS*10;

		pair<vector<vector<int>>,vector<Edge>> x = GenerateGraph_Matchings(V);
		vector<vector<int>> g=x.first;
		edgelist=x.second;

		//TODO
		//shuffle(edgelist);

		LeftMatching  *lm = new LeftMatching(V,edgelist);  //rewrite into PartitionMatroid(int V, vector<vector<int>> partition, vector<int> ks)  assert(ks==partition.size())
		RightMatching *rm = new RightMatching(V,edgelist);
		//lm->show();
		//rm->show();
		V+=2;

		cout << "###Iteration number: " << RUNS++ << ", Edges = " << edgelist.size() << endl;

		//cout << "Flow: " << EdmondsKarp(V/2,g) << endl;

		/*Uniform *uo1 = new Uniform(5,3);
		Uniform *uo2 = new Uniform(5,4);
		uo1->show();
		uo2->show();*/

		/*auto start = high_resolution_clock::now();
		S = SAP(edgelist.size(),lm,rm);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		myfile << edgelist.size() << " " << S << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		cout << "###SAP\n";
		cout << "###Ground set size: " << edgelist.size() << endl;
		cout << "###Independent set found with size " << S <<  endl;
		//printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: " << lm->getOracleCalls()+rm->getOracleCalls() << endl; //max(lm->getOracleCalls(),rm->getOracleCalls())
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;*/

		auto start = high_resolution_clock::now();
		C = Cun86(edgelist.size(),lm,rm);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "###Cun86\n";
		cout << "###Independent set found with size " << C << endl;
		printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: "    << lm->getOracleCalls()+rm->getOracleCalls() << endl; 
		cout << "   O(Number of calls): " << max(lm->getOracleCalls(),rm->getOracleCalls()) << endl;
		cout << "   O1 calls: " << lm->getOracleCalls() << "; O2 calls: " << rm->getOracleCalls() << endl;
		cout << "   Time:  "   << duration.count() << " microseconds" << endl << endl;
		//myfile << edgelist.size() << " " << C << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		//myfile << endl;

		/*auto start = high_resolution_clock::now();
		I = AugmentingPaths(edgelist.size(),lm,rm);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "\n###FMI INDEP\n";
		cout << "###Independent set found with size " << I <<  endl;
		printsol();
		cout << "###Resources:\n";
		cout << "   Number of calls: "    << lm->getOracleCalls()+rm->getOracleCalls() << endl; 
		cout << "   O(Number of calls): " << max(lm->getOracleCalls(),rm->getOracleCalls()) << endl;
		cout << "   O1 calls: " << lm->getOracleCalls() << "; O2 calls: " << rm->getOracleCalls() << endl;
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;
		myfile << edgelist.size() << " " << I << " " << lm->getOracleCalls()+rm->getOracleCalls() << " " << max(lm->getOracleCalls(),rm->getOracleCalls()) << " " << duration.count() << endl;
		myfile << endl;*/

		//S  = ExactRank((int)E, &Rank_Uniform, &Oracle_PowerSet, &Oracle_PowerSet_Free);
		/*S  = Cun86((int)E, &Oracle_Transversal_Left,  &Oracle_Transversal_Left_Free, &Oracle_Transversal_Left_Exchangeable, 
					  &Oracle_Transversal_Right, &Oracle_Transversal_Right_Free, &Oracle_Transversal_Right_Exchangeable);*/
		//HK = HopcroftKarp_(); cout << "HK: " << HK << endl;
		//EK = EdmondsKarp(V, graph_);   cout << "EK: " << EK << endl;

		//myfile << V << " " << S << " " << duration.count() << " " << ORACLE_CALLS << " " << HK << " " << EK << endl;
		
	}
	
	myfile.close();
	cout << endl;
	
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