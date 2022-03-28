#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <limits>

#define 	MAX_V				100000
#define 	min(u, v) 			(u<v ? u:v)
#define		even(x)				x%2==0
#define		odd(x)				!even(x)
#define		SZ 					independent_set.size()
#define 	IN_INDEPENDENT(i)	in_independent_set[i]==true
#define 	SOURCE 				-2
#define 	TARGET 				-3

extern int N;
extern std::vector<int>  AUGMENTATIONS;
extern std::vector<int>  independent_set;
extern std::vector<int>  not_independent;
extern std::vector<bool> in_independent_set;
extern std::vector<int>  index_;  // index[i]=-1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
extern int DISTANCE_TARGET;
extern int *distances;
extern std::vector<std::vector<int>> candidates;
extern int CURRENT_RANK;
extern int MAX_DIST;

template<typename T>
void DEBUG_VECTOR(const std::vector<T> &v) {
	std::cout << "DEBUG BEGIN" << std::endl;
	std::cout << "The vector size is " << v.size() << " and its " << "capacity is " << v.capacity() << std::endl;
	std::cout << "Vector content:" << std::endl;
	for (int i = 0; i < v.size(); ++i) std::cout << v[i] << std::endl;
	std::cout << "DEBUG END" << std::endl;
}

template<typename T>
std::vector<T> Slice(std::vector<T> const &v, int m, int n) {
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n+1;
	std::vector<T> vec(first, last);
	return vec;
}

void Init(int);
void clear_swap(std::vector<int> &v);
void addEdge(std::vector<int> graph[], int u, int v);
void printGraph(std::vector<int> graph[]);
void PrintIndependentSet();
bool equal_content_same_order(std::vector<int> &a, std::vector<int> &b);
void PrintCandidates();
void ClearCandidates();
void UpdateIndependentSet();
std::vector<int> RemoveSubset(std::vector<int> S, std::vector<int> A);
void okutils();

#endif