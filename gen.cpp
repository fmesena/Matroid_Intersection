#include <cstdlib>
#include "gen.h"

using std::pair, std::vector, std::cout, std::endl, std::string;


pair<vector<vector<int>>, vector<Edge>> Generate_BipartiteGraph(int V) {
	assert(V%2==0);
	vector<vector<int>> graph;
	vector<Edge> edges;
	int arc;
	for (int i = 0; i < V; ++i)
		graph.push_back(vector<int>());
	for (int i = 0; i < V/2; ++i)
		for (int j = V/2; j < V; ++j) {
			arc = rand() % 2;
			if (!arc) continue;
			edges.push_back({i,j});
			graph[i].push_back(j);
			graph[j].push_back(i);
		}
	size_t E = edges.size();
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


vector<vector<int>> Generate_SimpleGraph(int V, int type) {
	vector<vector<int>> graph;
	vector<Edge> edges;
	/*
	type 0 = random p=...,
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
	}
	*/
	return graph;
}


//TODO include designated root "r"
vector<vector<int>> Generate_Arborescence(int V, int root) {
	int arc1,arc2;
	vector<vector<int>> graph;
	vector<Edge> edges;
	Edge e;
	for (int i = 0; i < V; ++i) {
		for (int j = 0; j < V; ++j) {
			if (i==j) continue;
			arc1 = rand() % 2;
			arc2 = rand() % 2;
			if (arc1) { edges.push_back({i,j}); graph[i].push_back(j); }
			if (arc2) { edges.push_back({j,i}); graph[j].push_back(i); }
		}
	}
	size_t E = edges.size();
	cout << "Edge list\n";
	for (Edge e : edges)
		cout << e.u << " " << e.v << endl;

	cout << "\nAdj list\n";
	for (int i=0; i<V; i++)
	{
		cout << i << ": ";
		for (auto x : graph[i])
			cout << x << " ";
		cout << endl;
	}
	return graph;
}


/* Checkers */

void assertMatching(vector<Edge> matching, int V, string name) {
	cout << name << " ";
	vector<bool> matched = vector<bool>(V,false);
	assert(matching.size()<=V/2);
	for (Edge &e:matching)
	{
		assert(!matched[e.u] && !matched[e.v]);
		matched[e.u]=matched[e.v]=true;
	}
}

void DFS_aux(int cur, vector<vector<int>> adj) {

}

void DFS(vector<Edge> forest, vector<vector<int>> adj) {

}

void assertGraphic(vector<Edge> forest, vector<vector<int>> adj, string name) {
	//https://codeforces.com/contest/1375/submission/86366903
	cout << name << " ";
	DFS(forest, adj);
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
