#include<bits/stdc++.h>

using namespace std;

#define NIL 0
#define INF INT_MAX


/*************************************/
/*********** HOPCROFT KARP ***********/ 
/*************************************/


/*
int main() {
	m = n = 4;
	adj = new list<int>[m+1];
	AddEdge(1, 2);
	AddEdge(1, 3);
	AddEdge(2, 1);
	AddEdge(3, 2);
	AddEdge(4, 2);
	AddEdge(4, 4);
	HopcroftKarp();

	cout << "Size of maximum matching is " << HopcroftKarp() << endl;

	return 0;
}


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

void AddEdge_(int u, int v) {
	adj[u].push_back(v);
}

//int HopcroftKarp(list<int> *adj_)

// returns size of maximum matching
int HopcroftKarp() {
	m = n = 4;
	//adj = adj_; 
	adj = new list<int>[m+1];
	AddEdge_(1, 2);
	AddEdge_(1, 3);
	AddEdge_(2, 1);
	AddEdge_(3, 2);
	AddEdge_(4, 2);
	AddEdge_(4, 4);

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
}*/


/*************************************/
/************ EDMONDS KARP ***********/
/*************************************/


int n;
int capacity[10000][10000] = {0}; //could be a 0-1 matrix
vector<vector<int>> adj;

int bfs(int s, int t, vector<int>& parent) {
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

void InitCapacities() {
	for (int i = 1; i <= n/2; ++i)
	{
		capacity[0][i] = 1;
		for (auto x : adj[i])
			capacity[i][x] = 1;
		capacity[i+n/2][n+1] = 1;
	}
}


int EdmondsKarp(int V, vector<vector<int>> &g) {
	
	n   = V;
	adj = g;

	int s = 0;
	int t = n+1;
    int flow = 0;
    vector<int> parent(n+2);
    int new_flow;

	InitCapacities();

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow; // capacity[prev][cur] ^= 1;
            capacity[cur][prev] += new_flow; // capacity[cur][prev] ^= 1;
            cur = prev;
        }
    }
    return flow;
}


int main2() {
	
	n = 8;

	adj[0].push_back(1); adj[1].push_back(0);
	adj[0].push_back(2); adj[2].push_back(0);
	adj[0].push_back(3); adj[3].push_back(0);
	adj[1].push_back(4); adj[4].push_back(1);
	adj[1].push_back(5); adj[5].push_back(1);
	adj[2].push_back(5); adj[5].push_back(2);
	adj[3].push_back(5); adj[5].push_back(3);
	adj[4].push_back(7); adj[7].push_back(4);
	adj[5].push_back(7); adj[7].push_back(5);
	adj[6].push_back(7); adj[7].push_back(6);

	adj[3].push_back(6); adj[6].push_back(3);
	capacity[3][6] = 1; capacity[6][3] = 0;

	capacity[0][1] = capacity[0][2] = capacity[0][3] = capacity[1][4] = capacity[1][5] = capacity[2][5] = capacity[3][5] = capacity[4][7] = capacity[5][7] = capacity[6][7] = 1;
	capacity[1][0] = capacity[2][0] = capacity[3][0] = capacity[4][1] = capacity[5][1] = capacity[5][2] = capacity[5][3] = capacity[7][4] = capacity[7][5] = capacity[7][6] = 0;

	//cout << "EK: " << EdmondsKarp() << endl;

	return 0;
}



/*

#include<bits/stdc++.h>

using namespace std;

#define NIL 0
#define INF INT_MAX

queue<int> q;
int odd1[500], odd2[500];
vector<int> adj[500];
int pair1[500], pair2[500], dist[500];

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
			if (dist[pair2[u]]==dist[v]+1 && dfs(pair2[u],n1)) {
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