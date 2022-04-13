#include "EdmondsKarp.h"

#define INF INT_MAX

using namespace std;


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

    while ( (new_flow = bfsEK(parent)) ) {
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