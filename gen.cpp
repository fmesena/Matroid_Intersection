#include <cstdlib>
#include "gen.h"

void okGEN() {cout << "okGEN\n";}

/* Generators */

//vector<vector<int>>

pair<vector<vector<int>>, vector<Edge>> GenerateGraph_Matchings(int V) {
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
	cout << "Edge list\n";
	for (Edge e : edges)
		cout << e.u << " " << e.v << endl;
	
	//For flows we add soure and target with ids "0" and "V+1"
	/*for (int i = 1; i <= V/2; ++i)
	{
		graph[0].push_back(i);
		graph[i].push_back(0);
	}

	for (int i = 1+V/2; i <= V; ++i)
	{
		graph[V+1].push_back(i);
		graph[i].push_back(V+1);
	}*/

	cout << "\nAdj list\n";
	for (int i=0; i<V; i++)
	{
		cout << i << ": ";
		for (auto x : graph[i])
			cout << x << " ";
		cout << endl;
	}
	return {graph,edges};
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

bool assertMatching(vector<Edge> matching) {
	cout << "assertMatching\n";
	return true;
}

bool assertGraphic(vector<Edge> forest, vector<vector<int>> g1, vector<vector<int>> g2) {
	return true;
}

bool assertArb(vector<Edge> arb, vector<vector<int>> g) {
	return true;
}

/*
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
*/

/*
// N colours are identified with integers from 0 to N-1
bool Oracle_Colour(vector<int> S) {
	bool counter[N] = {false};
	for (int e : S)
	{
		if (counter[e]==true)
			return false;
		counter[e] = true;
	}
	return true;
}

bool Oracle_Colour_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Colour(S);
}
bool Oracle_Colour_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Colour(S);
	if (Oracle_Colour(S))
	{
		S.push_back(b);
		return Oracle_Colour(S);
	}
	return false;
}
*/