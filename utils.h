#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

#define 	MAX_V				100000
#define 	min(u, v) 			(u<v ? u:v)
#define		even(x)				x%2==0
#define		odd(x)				!even(x)
#define		SZ 					independent_set.size()
#define 	IN_INDEPENDENT(a)	in_independent_set[a]==true
#define 	SOURCE 				-2
#define 	TARGET 				-3

extern int N;
extern vector<int>  AUGMENTATIONS;
extern vector<int>  independent_set;
extern vector<int>  not_independent;
extern vector<bool> in_independent_set;
extern vector<int>  index_;  // index[i]=-1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
extern int  DISTANCE_TARGET;
extern int *distances;
extern int CURRENT_RANK;
//extern int **candidates; TODO

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

void Init(int);
void clear_swap(std::vector<int> &v);
void addEdge(std::vector<int> graph[], int u, int v);
void printGraph(std::vector<int> graph[]);
void PrintIndependentSet();
bool equal_content_same_order(vector<int> &a, vector<int> &b);
void PrintCandidates(vector<int> c[]);
void UpdateIndependentSet();
vector<int> RemoveSubset(vector<int> S, vector<int> A);
void okutils();

#endif