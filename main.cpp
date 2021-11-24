#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <unordered_map>
//#include "Rank.h"
#include "SAP.h"
#include "Cun86.h"
#include "Bipartite_Matching.h"
#include "LinkCutTree.h"

using namespace std;
using namespace std::chrono;


int V;
int E;
int ORACLE_CALLS;

struct Edge {
	int u;
	int v;
};

//vector<Edge> graph;
vector<vector<int>> graph_;
vector<Edge> edges;


const int H = 4;

int  Rank_Uniform(vector<int> S) { return S.size() <= H ? S.size() : H; }
bool Oracle_Uniform(vector<int> S) { ORACLE_CALLS++; return S.size() <= H; }
bool Oracle_Uniform_Free(vector<int> S, int free) { ORACLE_CALLS++; return S.size() < H; }
bool Oracle_Uniform_Exchangeable(vector<int> S, int a, int b) { ORACLE_CALLS++; return S.size() <= H; }
bool Oracle_Uniform_Exchangeable(vector<int> S, vector<int> B, int b) { assert(S.size()>=B.size()); ORACLE_CALLS++; if (B.size()==0) return false; return S.size() <= H; }

int  Rank_PowerSet(vector<int> S) { ORACLE_CALLS++; return S.size(); }
bool Oracle_PowerSet(vector<int> S) { ORACLE_CALLS++; return true; }
bool Oracle_PowerSet_Free(vector<int> S, int free) { ORACLE_CALLS++; return true; }
bool Oracle_PowerSet_Exchangeable(vector<int> S, int a, int b) { ORACLE_CALLS++; return true; }
bool Oracle_PowerSet_Exchangeable(vector<int> A, vector<int> B, int b) { ORACLE_CALLS++; return true; }


void GenerateGraph_Matchings() {
	//V = rand() % 1000 + 50;  //does not generate uniformly distributed random numbers in the span (since in most cases this operation makes lower numbers slightly more likely)
	int arc;
	if (V%2==1) V++;
	for (int i = 0; i < V+1; ++i) graph_.push_back(vector<int>());
	for (int i = 1; i <= V/2; ++i)
		for (int j = 1+V/2; j <= V; ++j) {
			arc = rand() % 2;
			if (!arc) continue;
			edges.push_back({i,j});
			graph_[i].push_back(j);
			graph_[j].push_back(i);
		}
	E = edges.size();
	cout << "Edge list\n";
	for (Edge e : edges)
		cout << e.u << " " << e.v << endl;
	
	//For flows we add soure "0" and target "V+1"
	for (int i = 1; i <= V/2; ++i)
	{
		graph_[0].push_back(i);
		graph_[i].push_back(0);
	}

	for (int i = 1+V/2; i <= V; ++i)
	{
		graph_[V+1].push_back(i);
		graph_[i].push_back(V+1);
	}

	cout << "\nAdj list\n";
	for (int i=0; i<=V+1; i++)
	{
		cout << i << ": ";
		for (auto x : graph_[i])
			cout << x << " ";
		cout << endl;
	}
}

//TODO include designated root "r"
void Generate_Arborescence() {
	V = 10;
	int arc1,arc2;
	Edge e;
	for (int i = 0; i < V; ++i) {
		for (int j = 0; j < V; ++j) {
			if (i==j) continue;
			arc1 = rand() % 2;
			arc2 = rand() % 2;
			if (arc1) { edges.push_back({i,j}); graph_[i].push_back(j); }
			if (arc2) { edges.push_back({j,i}); graph_[j].push_back(i); }
		}
	}
	E = edges.size();
	cout << "Edge list\n";
	for (Edge e : edges)
		cout << e.u << " " << e.v << endl;

	cout << "\nAdj list\n";
	for (int i=0; i<V; i++)
	{
		cout << i << ": ";
		for (auto x : graph_[i])
			cout << x << " ";
		cout << endl;
	}
}


//////////////////////
/* 		ORACLES  	*/
//////////////////////

bool Oracle_Transversal_Right(vector<int> S) {
	assert(V%2==0);
	int M = V/2;
	bool right[M] = {false};
	for (int i=0; i < S.size(); i++)
	{
		Edge edge = edges[S[i]];
		if (right[edge.v-1-M] == true) return false;
		right[edge.v-1-M] = true;
	}
	return true;
}

bool Oracle_Transversal_Right_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Transversal_Right(S);
}

bool Oracle_Transversal_Right_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Transversal_Right(S);
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S[S.size()-1]);
			S.pop_back();
			break;
		}
	return Oracle_Transversal_Right_Free(S,b);
}

bool Oracle_Transversal_Left(vector<int> S) {
	assert(V%2==0);
	int M = V/2;
	bool left[M] = {false};
	for (int i=0; i < S.size(); i++)
	{
		Edge edge = edges[S[i]];
		if (left[edge.u-1] == true) return false;
		left[edge.u-1] = true;
	}
	return true;
}

bool Oracle_Transversal_Left_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Transversal_Left(S);
}

bool Oracle_Transversal_Left_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Transversal_Left(S);
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S[S.size()-1]);
			S.pop_back();
			break;
		}
	return Oracle_Transversal_Left_Free(S,b);
}


/*
Oracle(S) S \in I?
Oracle(S,b)
Oracle(S,a,b)
Oracle(S,A,b)

Oracle(X) -> rank(X)
*/


// "Edmonds algorithm"
vector<Edge> BFS_Directed(int root) {
	int current;
	queue<int> q; 
	q.push(root);
	bool visited[N] = {false};
	visited[root] = true;
	vector<Edge> arborescence;
	while (!q.empty())
	{
		current = q.front(); q.pop();
		for (auto neighbour : graph_[current])
		{
			if (visited[neighbour]) continue;
			q.push(neighbour);
			visited[neighbour] = true;
			arborescence.push_back({i,j});
		}
	}
	return arborescence;
}





int main() { // M = (V,I)

	int RUNS = 0;
	int S  = 0;
	int HK = 0;
	int EK = 0;

	ofstream myfile;
	myfile.open ("stats.txt");

	V=20;
	GenerateGraph_Matchings();
	//GenerateGraph_MSA();

	while (RUNS++ < 1) 
	{
		ORACLE_CALLS = 0;
		
		//GenerateGraph();

		cout << "Computing independent set...\n";

		auto start = high_resolution_clock::now();
		
		S  = SAP((int)E, &Oracle_Transversal_Left, &Oracle_Transversal_Right);

		//S  = ExactRank((int)E, &Rank_Uniform, &Oracle_PowerSet, &Oracle_PowerSet_Free);
		/*S  = Cun86((int)E, &Oracle_Transversal_Left,  &Oracle_Transversal_Left_Free, &Oracle_Transversal_Left_Exchangeable, 
					  &Oracle_Transversal_Right, &Oracle_Transversal_Right_Free, &Oracle_Transversal_Right_Exchangeable);*/
		//HK = HopcroftKarp_(); cout << "HK: " << HK << endl;
		EK = EdmondsKarp(V, graph_);   cout << "EK: " << EK << endl;

		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(stop - start);

		//myfile << V << " " << S << " " << duration.count() << " " << ORACLE_CALLS << " " << HK << " " << EK << endl;
		myfile << V << " " << S << " " << duration.count() << " " << ORACLE_CALLS << endl;

		cout << "Cardinality: " << S << endl;
		cout << "Oracle calls: " << ORACLE_CALLS << endl;
		cout << "Time taken: " << duration.count() << " microseconds" << endl;
	}

	myfile.close();

	return 0;
}
