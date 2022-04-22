#include "HopcroftKarp.h"

#define NIL 0
#define INF INT_MAX

using namespace std;


// m and n are number of vertices on left and right sides of the bipartite graph
static int m;
static int n;
static vector<vector<int>> adj;
static int *pairU, *pairV, *dist;


// Returns true if there is an augmenting path, else returns false
bool bfsHK() {

	queue<int> Q;

	for (int u=1; u<=m; u++)
	{
		if (pairU[u]==NIL)
		{
			dist[u] = 0;
			Q.push(u);
		}
		else dist[u] = INF;
	}

	dist[NIL] = INF;

	// Q is going to contain vertices of left side only
	while (!Q.empty())
	{
		// Dequeue a vertex
		int u = Q.front();
		Q.pop();

		if (dist[u] < dist[NIL])
		{
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
			if (dist[pairV[v]] == dist[u]+1)
			{
				if (dfsHK(pairV[v]) == true)
				{
					pairV[v] = u;
					pairU[u] = v;
					return true;
				}
			}
		}
		// If there is no augmenting path beginning with u
		dist[u] = INF;
		return false;
	}
	return true;
}

// returns size of maximum matching
int HopcroftKarp(int N, vector<vector<int>> g) {
	assert(N%2==0);
	N 	= N/2;
	m 	= n = N;
	adj = vector<vector<int>>(m+1);
	for (size_t i=0; i<g.size()/2; i++) {
		for (auto v : g[i]) {
			adj[i+1].push_back(v-N+1);
		}
	}

	pairU = new int[m+1];
	pairV = new int[n+1];

	// dist[u] stores distance of left side vertices
	// dist[u] is one more than dist[u'] if u is next to u'in augmenting path
	dist = new int[m+1];

	for (int u=0; u<=m; u++)
		pairU[u] = NIL;
	for (int v=0; v<=n; v++)
		pairV[v] = NIL;

	int res = 0;
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



/*
queue<int> q;
vector<int> adj[500];
int pair1[500], pair2[500], dist[500];
//int odd1[500], odd2[500]; Vertex Cover

int BFS(int n1) {
	for (int i=0; i<n1; i++)
		if (pair1[i]==-1)
			dist[i]=0, q.push(i);
		else
			dist[i]=n1*3;
	dist[n1]=n1*3;

	while(!q.empty()) {
		int nx = q.front();
		q.pop();

		if (dist[nx] < dist[n1])
			for (int i=0; i<(int)adj[nx].size(); i++) {
				int u = adj[nx][i];
				if (dist[pair2[u]] == n1*3) {
					dist[pair2[u]] == dist[nx]+1;
					q.push(pair2[u]);
				}
			}
	}
	return dist[n1] != n1*3;
}

int DFS(int v, int n1) {
	if (v!=n1) {
		for (int i=0; i<(int)adj[v].size(); i++) {
			int u=adj[v][i];
			if (dist[pair2[u]]==dist[v]+1 && DFS(pair2[u],n1)) {
				pair2[u]=v;
				pair1[v]=u;
				return 1;
			}
		}
		dist[v] = -1;
		return 0;
	}
	return 1;
}

int MaxFlow(int n1, int n2) {
	int match = 0;
	for (int i=0; i<n1; i++) pair1[i]=-1;
	for (int i=0; i<n2; i++) pair2[i]=-1;

	while(BFS(n1)) {
		for (int i=0; i<n1; i++)
			if (pair1[i] == -1 && DFS(i,n1))
				match++;
	}
	return match;
}


//calculates the vertex cover of G1 | G2
void VertexCover(int n1, int n2) {
	int ans = maxflow(n1,n2);
	memset(odd1,0,sizeof odd1), memset(odd2,0,sizeof odd2);

	for (int i=0; i<n1; i++)
		if (pair1[i]==-1)
			q.push(i);

	while (!q.empty()) {
		int nx = q.front(), q.pop();

		for (int i=0; i<(int)adj[nx].size();i++)
			if (pair2[adj[nx][i]] != n1 && !odd2[adj[nx][i]]) {
				q.push(pair2[adj[nx][i]]);
				odd2[adj[nx][i]] = 1;
				printf("%d ", adj[nx][i]);
			}
	}

	for (int i=0; i<n2; i++)
		if (pair2[i]==n1)
			q.push(i);

	while (!q.empty()) {
		int nx = q.front(), q.pop();

		for (i=0; i<(int)adj[nx].size(); i++) {
			if (pair1[adj[nx][i]] != -1 && !odd1[adj[nx][i]]) {
				q.push(pair1[adj[nx][i]])
			}
		}
	}
}
*/