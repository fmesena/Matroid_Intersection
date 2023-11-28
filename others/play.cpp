#include <bits/stdc++.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "ds/LinkCutTree.h"

#ifdef __cplusplus
} // end extern "C"
#endif

using namespace std;

#define NIL 0
#define INF INT_MAX
#define rand_p() (((double) rand()) / RAND_MAX)
#define rand_n(n) ((int) (((double) (n)) * (rand() / (RAND_MAX + 1.0))))

using namespace std;


// m and n are number of vertices on left and right sides of Bipartite Graph
static int m;
static int n;

// adj[u] stores adjacents of left side vertex 'u'. The value of u ranges from 1 to m. NIL is used for dummy vertex
//static list<int> *adj;
static vector<vector<int>> adj;

static int *pairU, *pairV, *dist;


// Returns true if there is an augmenting path, else returns false
bool bfsHK() {

	queue<int> Q;

	// First layer of vertices (set distance as 0)
	for (int u=1; u<=m; u++)
	{
		// If this is a free vertex, add it to queue
		if (pairU[u]==NIL)
		{
			// u is not matched
			dist[u] = 0;
			Q.push(u);
		}

		// Else set distance as infinite so that this vertex is considered next time
		else dist[u] = INF;
	}

	// Initialize distance to NIL as infinite
	dist[NIL] = INF;

	// Q is going to contain vertices of left side only.
	while (!Q.empty())
	{
		// Dequeue a vertex
		int u = Q.front();
		Q.pop();

		// If this node is not NIL and can provide a shorter path to NIL
		if (dist[u] < dist[NIL])
		{
			// Get all adjacent vertices of the dequeued vertex u
			list<int>::iterator i;
			for (auto v:adj[u])
			{
				// If pair of v is not considered so far (v, pairV[V]) is not yet explored edge.
				if (dist[pairV[v]] == INF)
				{
					// Consider the pair and add it to queue
					dist[pairV[v]] = dist[u] + 1;
					Q.push(pairV[v]);
				}
			}
		}
	}

	// If we could come back to NIL using alternating path of distinct vertices then there is an augmenting path
	return (dist[NIL] != INF);
}


// Returns true if there is an augmenting path beginning with free vertex u
bool dfsHK(int u)
{
	if (u != NIL)
	{
		for (auto v: adj[u])
		{
			// Follow the distances set by BFS
			if (dist[pairV[v]] == dist[u]+1)
			{
				// If dfs for pair of v also returns true
				if (dfsHK(pairV[v]) == true)
				{
					pairV[v] = u;
					pairU[u] = v;
					return true;
				}
			}
		}
		// If there is no augmenting path beginning with u.
		dist[u] = INF;
		return false;
	}
	return true;
}

// returns size of maximum matching
int HopcroftKarp(int N, vector<vector<int>> g) {
	m 	= n = N/2;
	adj = vector<vector<int>>(m+1);
	for (size_t i=0; i<g.size()/2; i++) {
		for (auto v : g[i]) {
			adj[i+1].push_back(v-N/2+1);
		}
	}
	
	// pairU[u] stores pair of u in matching where u is a vertex on left side of Bipartite Graph. If u doesn't have any pair, then pairU[u] is NIL
	pairU = new int[m+1];

	// pairV[v] stores pair of v in matching. If v doesn't have any pair, then pairU[v] is NIL
	pairV = new int[n+1];

	// dist[u] stores distance of left side vertices
	// dist[u] is one more than dist[u'] if u is next to u'in augmenting path
	dist = new int[m+1];

	// Initialize NIL as pair of all vertices
	for (int u=0; u<=m; u++)
		pairU[u] = NIL;
	for (int v=0; v<=n; v++)
		pairV[v] = NIL;

	int res = 0;
	// Keep updating the result while there is an augmenting path.
	while (bfsHK())
	{
		// Find a free vertex
		for (int u=1; u<=m; u++)

			// If current vertex is free and there is an augmenting path from current vertex
			if (pairU[u]==NIL && dfsHK(u))
				res++;
	}

	delete[] pairU;
	delete[] pairV;
	delete[] dist;

	return res;
}


void addEdge(vector<vector<int>> &x, int u, int v) {
	x[u].push_back(v);
	x[v].push_back(u);
}


void t() {
	vector<int> *x = new vector<int>(5,-1);
	vector<int> *y = new vector<int>(5);
	vector<int> *z = new vector<int>();
	vector<int> *w = new vector<int>;

	cout << "x\n";
	cout << x->size() << endl;
	for (auto el : *x) cout << el << " ";
	cout << endl;
	cout << "y\n";
	cout << y->size() << endl;
	for (auto el : *y) cout << el << " ";
	cout << endl;
	cout << "z\n";
	cout << z->size() << endl;
	for (auto el : *z) cout << el << " ";
	cout << endl;
	cout << "w\n";
	cout << w->size() << endl;
	for (auto el : *w) cout << el << " ";
	cout << endl;
	delete x;
	delete y;
	delete z;
	delete w;
}


int main() {
	//t();
	vector<int> x(10);
	x.push_back(5);
	for (auto y:x) cout << y << " ";
	cout << "------------------------\n";
	int V=10;
	/*	int n1=5;
	const int 	 n2 	 = V/n1;
	const int 	 degree	 = min(5,n2*3); //average degree
	const double p 		 = degree/V;
	//Oracle o;  error: cannot declare variable ‘o’ to be of abstract type ‘Oracle’ 
	for (int a=0; a<n1; a++) {
		for (int b=0; b<n2; b++) {
			int u  = a*n2+b;
			cout << "u: " << u << endl;
			int ct = 0;
			vector<int> neighb_group = {(-1+a+n1)%n1, a, (1+a+n1)%n1}; //find neighbours of "u" in blocks a-1,a,a+1
			for (auto j : neighb_group) {
				for (int v=n2*j; v<n2*(j+1); v++) {
					cout << v << " ";
					if (rand_p() > p)
						continue;
					
				}
			}
			cout << endl;
		}
	}*/
	V=8;
	vector<vector<int>> g = vector<vector<int>>(V);
    addEdge(g, 0, 5);
    addEdge(g, 0, 6);
    addEdge(g, 1, 4);
    addEdge(g, 2, 5);
    addEdge(g, 3, 5);
    addEdge(g, 3, 7	);
    for (int i=0; i<V; i++) {
    	for (auto v:g[i]) {
    		cout << v << " ";
    	}
    	cout << endl;
    }
	cout << HopcroftKarp(V,g) << endl;

	return 0;
}


/*
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