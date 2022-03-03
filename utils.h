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
#define 	IN_INDEPENDENT(a)	in_independent_set[a]
#define 	SOURCE 				-2
#define 	TARGET 				-3

static int N;
static vector<int>  AUGMENTATIONS;
static vector<int>  independent_set;
static vector<int>  not_independent;
static vector<bool> in_independent_set;
static vector<int>  index_;  // index[i]=-1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
static int  DISTANCE_TARGET;
static int *distances;
//static int **candidates; TODO
static int CURRENT_RANK;

template<typename T>
void DEBUG_VECTOR(const std::vector<T> &v);

template<typename T>
std::vector<T> Slice(std::vector<T> const &v, int m, int n);
void reset(std::vector<int> &v);

void addEdge(std::vector<int> graph[], int u, int v);

void printGraph(std::vector<int> graph[]);

void PrintIndependentSet();

bool equal_content_same_order(vector<int> &a, vector<int> &b);

void PrintCandidates(vector<int> c[]);

void UpdateIndependentSet();

vector<int> RemoveSubset(vector<int> S, vector<int> A);

//void Init(); TODO

#endif