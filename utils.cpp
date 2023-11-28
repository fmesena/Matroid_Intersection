#include "utils.h"

using namespace std;

void Init(int N_) {
	MAX_DIST = N_+10; //the maximum possible distance in any point is just the number of elements plus one (because of the target). because of the "l+3" operation in CLSSW_Indep, max_dist=N+4. therefore, we opted to play it safe and make it N+10
	independent_set.clear();
	not_independent.clear();
	in_independent_set.clear();
	in_independent_set.resize(N_, false);
	index_.resize(N_,-1); //value initialization
	distances = new int[N_](); //default value initialization
	candidates = vector<vector<int>>(MAX_DIST);
	ITER_CT = 0;
}

void clear_swap(vector<int> &v) { vector<int>().swap(v); }

void addEdge(vector<int> graph[], int u, int v) { graph[u].push_back(v); }

void printGraph(vector<vector<int>> graph) {
	cout << "\nAdjacency list representation\n";
    for (size_t v = 0; v < graph.size(); ++v)
    {	
    	cout << v << " :";
        for (auto x : graph[v])
           cout << " " << x;
        cout << endl;
    }
}

bool equal_content_same_order(vector<int> &a, vector<int> &b) {
	if (a.size() != b.size()) return false;
	for (size_t i = 0; i < a.size(); ++i)
		if (a[i] != b[i])
			return false;
	return true;
}

void PrintIndependentSet() {
	cout << "###Curent solution: ";
	for (size_t i = 0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
	cout << endl;
}

// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
void UpdateIndependentSet() {
	independent_set.clear();
	not_independent.clear();
	index_.clear();

	for (int i=0, j=0; i < N; ++i)
	{
		index_[i] = -1;
		if (IN_INDEPENDENT(i))
		{
			independent_set.push_back(i);
			index_[i] = j++;
		}
		else not_independent.push_back(i);
	}
}

void PrintCandidates() {
	cout << "Printing candidates\n";
	for (int i = 0; i < MAX_DIST; ++i)
	{
		cout << "Distance " << i << endl;
		for (int k : candidates[i])
			cout << k << " ";
		cout << endl;
	}
	cout << endl;
}

void ClearCandidates() {
	assert((int)candidates.size()==MAX_DIST);
	for (size_t i = 0; i < candidates.size(); ++i)
		candidates[i].clear();
}