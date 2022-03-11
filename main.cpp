#include <fstream>
#include <assert.h>
#include <time.h>
#include <chrono>
#include <unordered_map>
#include "SAP.h"
#include "Cun86.h"
#include "Rank.h"
#include "Indep.h"
#include "Bipartite_Matching.h"
#include "gen.h"
#include "oracleheaders.h"

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
int CURRENT_RANK;
//extern int **candidates; TODO


void printsol() {
	cout << "###Solution: ";
	for (int i = 0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
	cout << endl;
}

int main() {

	int RUNS = 0;
	int S  = 0;
	int C  = 0;
	int HK = 0;
	int EK = 0;

	ofstream myfile;
	myfile.open ("plots/stats.txt");

	V=20;

	/*okSAP();
	okCun();
	okGEN();
	okRank();
	okIndep();
	okBM();*/

	while (RUNS++ < 1) 
	{		
		V = rand() % 10+6;  //does not generate uniformly distributed random numbers in the span (since in most cases this operation makes lower numbers slightly more likely)
		if (V%2==1) V++;
		//int xx=EdmondsKarp(V/2,GenerateGraph_Matchings(V));
		//cout << "Flow: " << xx << endl;
		V=6;
		pair<vector<vector<int>>,vector<Edge>> x = GenerateGraph_Matchings(V);
		vector<vector<int>> g=x.first;
		vector<Edge> edgelist=x.second;

		LeftMatching  *lm = new LeftMatching(V,edgelist); //rewrite into PartitionMatroid(int V, vector<vector<int>> partition, vector<int> ks)  assert(ks==partition.size())
		RightMatching *rm = new RightMatching(V,edgelist);
		lm->show();
		rm->show();

		cout << "\n###Iteration number: " << RUNS << endl;

		Uniform *uo1 = new Uniform(5,3);
		Uniform *uo2 = new Uniform(5,4);
		//uo1->show();
		//uo2->show();

		auto start = high_resolution_clock::now();
		S = SAP(edgelist.size(),lm,rm);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "###SAP\n";
		cout << "###Independent set found with size " << S <<  endl;
		printsol();
		cout << "###Resources:\n";
		cout << "   Calls: " << uo1->getOracleCalls()+uo2->getOracleCalls() << endl; //max(uo1->getOracleCalls(),uo2->getOracleCalls())
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;
		myfile << V << " " << S << " " << duration.count() << " " << endl;
		myfile << endl;


		/*start = high_resolution_clock::now();
		C = Cun86(5,uo1,uo2);
		stop = high_resolution_clock::now();
		duration = duration_cast<microseconds>(stop - start);
		cout << "\n###Cun86\n";
		cout << "###Independent set found with size " << C <<  endl;
		printsol();
		cout << "###Resources:\n";
		cout << "   Calls: " << uo1->getOracleCalls()+uo2->getOracleCalls() << endl; //max(uo1->getOracleCalls(),uo2->getOracleCalls())
		cout << "   Time:  "   << duration.count() << " microseconds" << endl;*/

		myfile << V << " " << C << " " << duration.count() << " " << endl;

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
			//Update_State(); FIXME
		}
	}
	return;
}*/