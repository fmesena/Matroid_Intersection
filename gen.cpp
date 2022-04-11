#include <cstdlib>
#include "gen.h"

using std::pair, std::vector, std::cout, std::endl, std::string, std::max;

/*** Generators *************************/

pair<vector<vector<int>>, vector<Edge>> Generate_BipartiteGraph(int V, int p) {
	assert(V%2==0);
	vector<vector<int>> graph;
	vector<Edge> edges;
	int arc;
	const int range_from  = 1;
	const int range_to    = 100;
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  distr(range_from, range_to);

	for (int i = 0; i < V; ++i)
		graph.push_back(vector<int>());
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
	if (p==100) {
		assert(E==V*V/4);
	}
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


//returns a vector of Edges with vertices in the range 1..V
pair<int,vector<Edge>> Generate_SimpleGraph(int E) {
	vector<Edge> edges;
	int V = 0.5*(1 + sqrt(1+16*E));
	int m = 0;
	int u,v;
	while (m++<E)
	{
		u = rand() % V + 1;
		v = rand() % V + 1;
		while (v==u) v = rand() % V + 1;
		edges.push_back({u,v});
	}

	/*type 0 = random p=...,
	type 1 = random p=...,
	type 2 = random p=...,
	type 3 = SFN    ...,
	...
	type n = ...
	
	https://en.cppreference.com/w/cpp/language/enum
	enum Color { red, green, blue };
	Color r = red;
	 
	switch(r)
	{
	    case red  : std::cout << "red\n";   break;
	    case green: std::cout << "green\n"; break;
	    case blue : std::cout << "blue\n";  break;
	}*/
	return {V,edges};
}


//TODO include designated root "r"
vector<vector<int>> Generate_Arborescence(int V, int root) {
	int arc1,arc2;
	vector<vector<int>> graph;
	vector<Edge> edges;
	for (int i = 0; i < V; ++i) {
		for (int j = 0; j < V; ++j) {
			if (i==j) continue;
			arc1 = rand() % 2;
			arc2 = rand() % 2;
			if (arc1) { edges.push_back({i,j}); graph[i].push_back(j); }
			if (arc2) { edges.push_back({j,i}); graph[j].push_back(i); }
		}
	}
	//size_t E = edges.size();
	/*cout << "Edge list\n";
	for (Edge e : edges)
		cout << e.u << " " << e.v << endl;

	cout << "\nAdj list\n";
	for (int i=0; i<V; i++)
	{
		cout << i << ": ";
		for (auto x : graph[i])
			cout << x << " ";
		cout << endl;
	}*/
	return graph;
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

void DFS(vector<vector<int>> adj, int u, int last, bool *visited) {
	visited[u]=true;
	for (int v: adj[u]) {
		if (v!=last) {
			//assert(!visited[v] && "back-edge");
			if (visited[v]==true) {
				cout << "ERROR: back-edge " << u+1 << "-" << v+1 << endl;
				assert(!visited[v] && "back-edge");
			}
			DFS(adj,v,u,visited);
		}
	}
}

void assertGraphic(vector<Edge> forest, string name) {
	//cout << name << " ";
	vector<vector<int>> adj; //should be dynamically allocated
	int V=0;
	for (Edge e:forest)
	{
		V = max(e.u,e.v);
		while (V>(int)adj.size()) adj.push_back(vector<int>());
		adj[e.u-1].push_back(e.v-1);
		adj[e.v-1].push_back(e.u-1);
	}
	bool *visited = new bool[V]();
	for (int u=0; u<V; u++) {
		if (!visited[u]) {
			visited[u] = true;
			DFS(adj, u, -1, visited);
		}
	}
	delete[] visited; //may leak if assertion fails, lazy
}

void assertArb(vector<Edge> arb, vector<vector<int>> adj, string name) {
	/*int current;
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
	return arborescence;*/
}
