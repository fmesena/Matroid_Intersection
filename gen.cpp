#include <cstdlib>
#include "gen.h"

using std::pair, std::vector, std::cout, std::endl, std::string, std::max, std::min;

#define rand_p() (((double) rand()) / RAND_MAX)
#define rand_n(n) ((int) (((double) (n)) * (rand() / (RAND_MAX + 1.0))))


/*** Generators *************************/

pair<vector<vector<int>>, vector<Edge>> GenerateRandomBipartiteGraph(int V, int p) {
	assert(V%2==0);
	vector<vector<int>> graph(V);
	vector<Edge> edges;
	int arc;
	const int range_from  = 1;
	const int range_to    = 100;
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(range_from, range_to);

	for (int i = 0; i < V/2; ++i) {
		for (int j = V/2; j < V; ++j) {
			arc = distr(generator) <= p? true:false;
			if (!arc) continue;
			edges.push_back({i,j});
			graph[i].push_back(j);
			graph[j].push_back(i);
		}
	}
	size_t E = edges.size();
	if (p==100) assert(E==V*V/4);
	if (p==0)   assert(E==0);
	/*cout << "Edge list\n";
	for (Edge e : edges)
		cout << e.u << " " << e.v << endl;*/

	/*cout << "\nAdj list\n";
	for (int i=0; i<V; i++)
	{
		cout << i << ": ";
		for (auto x : graph[i])
			cout << x << " ";
		cout << endl;
	}*/
	return {graph,edges};
}

// generates a "pigeonhole principle" graph, i.e. a K_{V,V+1} graph.
pair<vector<vector<int>>, vector<Edge>> GeneratePHPGraph(int V) {
	vector<vector<int>> graph(2*V+1);
	vector<Edge> edges;
	for (int i = 0; i < V; ++i) {
		for (int j = V; j < 2*V+1; ++j) {
			edges.push_back({i,j});
			graph[i].push_back(j);
			graph[j].push_back(i);
		}
	}
	return {graph,edges};
}

pair<vector<vector<int>>, vector<Edge>> GenerateBlocksBipartiteGraph(int V, int n1) {
	assert(V%2==0);
	assert(V>=20);
	assert(n1>=4);
	V=V/2;
	assert(V%n1==0);

	vector<vector<int>> graph;
	
	vector<Edge> edges;
	const int 	 n2 	 = V/n1;
	const int 	 degree	 = min(5,n2*3); //average degree
	cout << "Degree: " << degree << endl;
	const double p 		 = degree/V;
	
	for (int a=0; a<n1; a++) {
		for (int b=0; b<n2; b++) {
			int u  = a*n2+b;
			vector<int> neighb_group = {(-1+a+n1)%n1, a,(1+a+n1)%n1}; //find neighbours of "u" in blocks a-1,a,a+1
			for (auto j : neighb_group) {
				for (int v=n2*j; v<n2*(j+1); v++) {
					if (rand_p() > p)
						continue;
					edges.push_back({u,v+V});
				}
			}
		}
	}
	return {graph,edges};
}

//returns a vector of Edges with vertices in the range 1..V
pair<int,vector<Edge>> GenerateMultiGraph(int E) {
	vector<Edge> edges;
	int V = 0.5*(1 + sqrt(1+16*E)); //this is the minimum number of vertices needed to have a complete simple graph on 2*E edges
	int m = 0;
	int u,v;

	const int range_from  = 1;
	const int range_to    = V;
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(range_from, range_to);

	while (m++<E)
	{
		u = distr(generator);
		v = distr(generator);
		while (v==u) v = distr(generator);
		edges.push_back({u,v});
	}
	return {V,edges};
}

//returns a vector of Edges with vertices in the range 1..V
pair<int,vector<Edge>> GenerateDuplicationDivergence(int V) {
	assert(V>2);
	const double p=0.51;

	vector<vector<int>> adj = vector<vector<int>>(V+1);
	adj[1].push_back(2); adj[2].push_back(1);
	vector<Edge> edges;
	edges.push_back({1,2});

	int cur = 3;
	while (cur <= V)
	{
		int rnd_node = rand_n(cur)+1; cout << rnd_node << endl;
		for (auto v : adj[rnd_node])
		{
			if (rand_p() > p)
				continue;
			adj[cur].push_back(v);
			adj[v].push_back(cur);
			edges.push_back({v,cur});
		}
		cur++;
	}
	return {V,edges};
}


/*** Checkers *************************/

void assertMatching(vector<Edge> matching, int V, string name) {
	//cout << name << " ";
	vector<bool> matched = vector<bool>(V,false);
	assert((int)matching.size()<=V/2);
	for (Edge &e:matching)
	{
		assert(!matched[e.u] && !matched[e.v]);
		matched[e.u]=matched[e.v]=true;
	}
}

static void CheckBackEdge_DFS(vector<vector<int>> &adj, int u, int last, int* visited) {
	visited[u]=1;
	for (int v: adj[u]) {
		if (v!=last) {
			if (visited[v]) {
				delete[] visited;
				assert(!visited[v] && "back-edge");
			}
			CheckBackEdge_DFS(adj,v,u,visited);
		}
	}
}

void assertGraphic(vector<Edge> forest, string name) {
	//cout << name << " ";
	vector<vector<int>> adj; //should be dynamically allocated
	int V=0;
	for (Edge e:forest)
	{
		V = max(V,max(e.u,e.v));
		while (V>(int)adj.size()) adj.push_back(vector<int>());
		adj[e.u-1].push_back(e.v-1);
		adj[e.v-1].push_back(e.u-1);
	}
	int *visited = new int[V]();
	for (int u=0; u<V; u++) {
		if (!visited[u]) {
			visited[u] = 1;
			CheckBackEdge_DFS(adj, u, -1, visited);
		}
	}
	//cout << endl;
	delete[] visited;
}

void assertArb(vector<Edge> arb, vector<vector<int>> adj, string name) {
}
