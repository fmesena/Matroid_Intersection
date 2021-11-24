#include <vector>
#include <queue>
#include "ufds.h"

const int MAX = 300000 + 3;
vector<int> link(MAX);
vector<int> size(MAX);
vector<pair<string,string>> info(MAX);


int find(int i) {
    return ((link[i]==i) ? i : (link[i] = find(link[i])));
}

bool sameSet(int a, int b) {
	return find(a) == find(b);
}

void unite(int i, int j) {
    if (!sameSet(i,j)) {
        int x = find(i), y = find(j);
        if (size[x] > size[y]) link[y] = x;
        else {
            link[x] = y;
            if (size[x]==size[y])
                size[y]++;
        }
    }
}

/*
void Update_State(vector<int> S) {
	for (size_t i = 0; i < S.size(); ++i)
		unite(edges[S[i]].u, edges[S[i]].v);
}
*/

bool Oracle_Cycle_Free(vector<int> S, int free) {
	return !(find(edges[free].u) == find(edges[free].v));
}

//Link Cut Trees pleaseeeeeeeeeeeee
bool Oracle_Cycle_Exchangeable(vector<int> S, int a, int b) {
	return 1;
}



//use case for ufds
int main() {
	/*
	vector<Edge> edges;
	sort(edges.begin(), edges.end());

	for (int i=0; i<E; i++) {
		unite(edges[i].u, edges[i].v);
	}
	*/
	return 0;	
}