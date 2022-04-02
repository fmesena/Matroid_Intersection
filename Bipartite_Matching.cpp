#include<bits/stdc++.h>
#include "Bipartite_Matching.h"

#define NIL 0
#define INF INT_MAX

using namespace std;

void okBM() {cout<<"okBM\n";}

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

void AE(int u, int v) {
	adj[u].push_back(v);
}

int main() {
	AE(0, 3);
	AE(0, 4);
	AE(1, 3);
	AE(1, 4);
	AE(2, 3);
	AE(2, 4);
	AE(2, 5);
	cout << MaxFlow(3,3) << endl;
}*/

/*
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











/*************************************/
/*********** HOPCROFT KARP ***********/ 
/*************************************/
/*
// m and n are number of vertices on left and right sides of Bipartite Graph
int m;
int n;

// adj[u] stores adjacents of left side vertex 'u'. The value of u ranges from 1 to m. 0 is used for dummy vertex
list<int> *adj;

int *pairU, *pairV, *dist;


// Returns true if there is an augmenting path, else returns false
bool BFS_() {

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
			for (i=adj[u].begin(); i!=adj[u].end(); ++i)
			{
				int v = *i;

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
bool DFS(int u)
{
	if (u != NIL)
	{
		list<int>::iterator i;
		for (i=adj[u].begin(); i!=adj[u].end(); ++i)
		{
			// Adjacent to u
			int v = *i;

			// Follow the distances set by BFS
			if (dist[pairV[v]] == dist[u]+1)
			{
				// If dfs for pair of v also returns true
				if (DFS(pairV[v]) == true)
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

//int HopcroftKarp(list<int> *adj_)

// returns size of maximum matching
int HopcroftKarp() {
	m = n = 3;
	adj = new list<int>[m+1];
	AddEdge_(1, 4);
	AddEdge_(1, 5);
	AddEdge_(2, 4);
	AddEdge_(2, 5);
	AddEdge_(3, 4);
	AddEdge_(3, 5);
	AddEdge_(3, 6);
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

	// Initialize result
	int result = 0;

	// Keep updating the result while there is an augmenting path.
	while (BFS_())
	{
		// Find a free vertex
		for (int u=1; u<=m; u++)

			// If current vertex is free and there is an augmenting path from current vertex
			if (pairU[u]==NIL && DFS(u))
				result++;
	}

	delete[] pairU;
	delete[] pairV;
	delete[] dist;

	return result;
}

void AddEdge_(int u, int v) {
	adj[u].push_back(v);
}

int main() {
	cout << "Size of maximum matching is " << HopcroftKarp() << endl;
	return 0;
}*/

/*************************************/
/************ EDMONDS KARP ***********/
/*************************************/

static int N;
static int s;
static int t;
//The matrix capacity stores the capacity for every pair of vertices
//adj is the adjacency list of the undirected graph, since we have also to use the reversed of directed edges when we are looking for augmenting paths
static int **capacity; //could be a 0-1 matrix in this particular case
static vector<vector<int>> adj;


int bfsEK(vector<int> &parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();
        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }
    return 0;
}

int EdmondsKarp(int N_, vector<vector<int>> g) {

	N = N_;
	adj = g;
	
	capacity = new int*[N+2];
	for (int i = 0; i < N+2; ++i) {
		capacity[i] = new int[N+2];
	}

	s = N;
	t = N+1;
	
	for (int i = 0; i < N/2; ++i)
	{
		for (auto x : adj[i]) {
			capacity[i][x] = 1;
			capacity[x][i] = 0;
		}
 	}
 	
    adj.push_back(vector<int>()); //source
    adj.push_back(vector<int>()); //target
	for (int i=0; i<N/2; i++) {
		adj[s].push_back(i); adj[i+N/2].push_back(t);
		adj[i].push_back(s); adj[t].push_back(i+N/2);
		capacity[s][i]=1; capacity[i+N/2][t]=1;
		capacity[i][s]=0; capacity[t][i+N/2]=0;
	}
 	
	int flow = 0;
	int new_flow;
	vector<int> parent(N+2);

    while (new_flow = bfsEK(parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow; // capacity[prev][cur] ^= 1;
            capacity[cur][prev] += new_flow; // capacity[cur][prev] ^= 1;
            cur = prev;
        }
    }
	for (int i = 0; i < N+2; i++) {
		delete[] capacity[i];
	}
	delete[] capacity;
    return flow;
}



/*void GenerateGraph_Matchings(int V) {
	assert(V%2==0);
	int arc;
	for (int i = 0; i < V; ++i)
		adj.push_back(vector<int>());
	for (int i = 0; i < V/2; ++i)
		for (int j = V/2; j < V; ++j) {
			arc = rand() % 2;
			if (!arc) continue;
			adj[i].push_back(j);
			adj[j].push_back(i);
		}
	return;
}

void pgg() {
	cout << "Adj list\n";
	for (int i=0; i<N; i++)
	{
		cout << i << ": ";
		for (auto x : adj[i])
			cout << x << " ";
		cout << endl;
	}
}

int main() {
	N=500;
	GenerateGraph_Matchings(N);
	pg();
	int res = EdmondsKarp();
	cout << "Flow: " << res << endl;
	return 0;
}*/