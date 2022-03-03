#include <fstream>
#include <assert.h>
#include <time.h>
#include <chrono>
#include <unordered_map>
//#include "Rank.h"
//#include "Indep.h"
#include "Bipartite_Matching.h"
#include "SAP.h"
#include "Cun86.h"
#include "gen.h"
#include "oracleheaders.h"

using namespace std;
using namespace std::chrono;

int V;
int E;

int main() { // M = (V,I)

	int RUNS = 0;
	int S  = 0;
	int HK = 0;
	int EK = 0;

	ofstream myfile;
	myfile.open ("plots/stats.txt");

	V=20;

	okSAP();
	okCun();

	while (RUNS++ < 1) 
	{		
		//GenerateGraph();

		cout << "Computing independent set...\n";

		auto start = high_resolution_clock::now();
		
		//S  = SAP((int)E, &Oracle_Transversal_Left, &Oracle_Transversal_Right);

		//S  = ExactRank((int)E, &Rank_Uniform, &Oracle_PowerSet, &Oracle_PowerSet_Free);
		/*S  = Cun86((int)E, &Oracle_Transversal_Left,  &Oracle_Transversal_Left_Free, &Oracle_Transversal_Left_Exchangeable, 
					  &Oracle_Transversal_Right, &Oracle_Transversal_Right_Free, &Oracle_Transversal_Right_Exchangeable);*/
		//HK = HopcroftKarp_(); cout << "HK: " << HK << endl;
		//EK = EdmondsKarp(V, graph_);   cout << "EK: " << EK << endl;

		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(stop - start);

		//myfile << V << " " << S << " " << duration.count() << " " << ORACLE_CALLS << " " << HK << " " << EK << endl;
		myfile << V << " " << S << " " << duration.count() << " " << endl;

		cout << "Cardinality: " << S << endl;
		//cout << "Oracle calls: " << ORACLE_CALLS << endl;
		cout << "Time taken: " << duration.count() << " microseconds" << endl;
	}

	Uniform *uo = new Uniform(5,3);
	uo->show(); //needs include. this line was just to test if include worked as i thought
	okGEN();

	myfile.close();

	return 0;
}



/*vector<vector<int>> graph_;

const int H = 4;

int  Rank_Uniform(vector<int> S) { return S.size() <= H ? S.size() : H; }
bool Oracle_Uniform(vector<int> S) { return S.size() <= H; }
bool Oracle_Uniform_Free(vector<int> S, int free) { return S.size() < H; }
bool Oracle_Uniform_Exchangeable(vector<int> S, int a, int b) { return S.size() <= H; }
bool Oracle_Uniform_Exchangeable(vector<int> S, vector<int> B, int b) { assert(S.size()>=B.size()); if (B.size()==0) return false; return S.size() <= H; }

int  Rank_PowerSet(vector<int> S) { return S.size(); }
bool Oracle_PowerSet(vector<int> S) { return true; }
bool Oracle_PowerSet_Free(vector<int> S, int free) { return true; }
bool Oracle_PowerSet_Exchangeable(vector<int> S, int a, int b) { return true; }
bool Oracle_PowerSet_Exchangeable(vector<int> A, vector<int> B, int b) { return true; }
*/
