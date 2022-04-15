#include "utils.h"

using namespace std;

void Init(int N_) {
	MAX_DIST = N_+10;
	independent_set.clear();
	not_independent.clear();
	in_independent_set.resize(N_, false);
	in_independent_set.clear();
	index_.resize(N_,-1); //default value initialization
	distances = new int[N_]();
	candidates = vector<vector<int>>(MAX_DIST);
	CURRENT_RANK=0;
}

void clear_swap(vector<int> &v) { vector<int>().swap(v); }

void addEdge(vector<int> graph[], int u, int v) { graph[u].push_back(v); }

void printGraph(vector<int> graph[]) {
    for (int v = 0; v < N; ++v)
    {
        cout << "\n Adjacency list of vertex "
             << v << "\n head ";
        for (auto x : graph[v])
           cout << "-> " << x;
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
	CURRENT_RANK++;
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

// returns V \subseteq S where all elements from A \subseteq S are removed from S
// does not change the values of "index_" since the altered S is supposed only for a "quick call" to the oracle
vector<int> RemoveSubset(vector<int> S, vector<int> A) {
	// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
	int j=0;
	for (size_t i = 0; i < A.size(); i++)
		if (index_[A[i]] != -1) {
			S[index_[A[i]]] = -1; j++; }
	vector<int> S_;
	for (size_t i = 0; i < S.size(); i++)
		if (S[i] != -1)
			S_.push_back(i);
	return S_;
}