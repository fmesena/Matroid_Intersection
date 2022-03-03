#include "utils.h"

template<typename T>
void DEBUG_VECTOR(const vector<T> &v) {
	cout << "DEBUG BEGIN" << endl;
	cout << "The vector size is " << v.size() << " and its " << "capacity is " << v.capacity() << endl;
	cout << "Vector content:" << endl;
	for (int i = 0; i < v.size(); ++i) cout << v[i] << endl;
	cout << "DEBUG END" << endl;
}

template<typename T>
vector<T> Slice(vector<T> const &v, int m, int n) {
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n+1;
	vector<T> vec(first, last);
	return vec;
}

void reset(vector<int> &v) { vector<int>().swap(v); }

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

void PrintIndependentSet() {
	cout << "INDEPENDENT SET FOUND WITH SIZE " << independent_set.size() << ":" << endl;
	for (int i = 0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
	cout << endl;
}

bool equal_content_same_order(vector<int> &a, vector<int> &b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); ++i)
		if (a[i] != b[i])
			return false;
	return true;
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

void PrintCandidates(vector<int> c[]) {
	for (int i = 0; i < 3; ++i)
	{
		cout << "Distance " << i << endl;
		for (int k : c[i])
			cout << k << " ";
		cout << endl;
	}
	cout << endl;
}

// returns V \subseteq S where all elements from A \subseteq S are removed from S
// does not change the values of "index_" since the altered S is supposed only for a "quick call" to the oracle
vector<int> RemoveSubset(vector<int> S, vector<int> A) {
	// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
	int j=0;
	for (int i = 0; i < A.size(); i++)
		if (index_[A[i]] != -1) {
			S[index_[A[i]]] = -1; j++; }
	vector<int> S_;
	for (int i = 0; i < S.size(); i++)
		if (S[i] != -1)
			S_.push_back(i);
	return S_;
}

/*
void Init() {
	in_independent_set.resize(N,false);
	distances = new int[N]();
	//TODO allocate candidates here maybe
}
*/