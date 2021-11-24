#include <vector>
#include <queue>
#include "oracles.h"


bool Oracle_Uniform(vector<int> S) { ORACLE_CALLS++; return S.size() <= H; }
bool Oracle_Uniform_Free(vector<int> S, int free) { ORACLE_CALLS++; return S.size() < H; }
bool Oracle_Uniform_Exchangeable(vector<int> S, int a, int b) { ORACLE_CALLS++; return S.size() <= H; }

int  Rank_PowerSet(vector<int> S) { ORACLE_CALLS++; return S.size(); }
bool Oracle_PowerSet(vector<int> S) { ORACLE_CALLS++; return true; }
bool Oracle_PowerSet_Free(vector<int> S, int free) { ORACLE_CALLS++; return true; }
bool Oracle_PowerSet_Exchangeable(vector<int> S, int a, int b) { ORACLE_CALLS++; return true; }


bool Oracle_Cycle(vector<int> S) {
	//Use Edge List to store the graph, helps a lot here
	vector<int> graph[N];
	queue<int> q;
	q.push(0);
	bool visited[N] = {false};
	int predecessor[N] = {-1};
	int current;
	visited[0] = true;

	while (!q.empty())
	{
		current = q.front(); q.pop();
		for (auto neighbour : graph[current])
		{
			if (visited[neighbour] && predecessor[current] != neighbour) return false;
			if (visited[neighbour]) continue;
			q.push(neighbour);
			visited[neighbour] = true;
			predecessor[neighbour] = current;
		}
	}
	return true;
}

bool Oracle_Cycle_Free(vector<int> S, int free) {
	//free = HashMap(free);  //HashMap maps integers to its elements in the context of the problem
	S.push_back(free);
	return Oracle_Cycle(S);
}

bool Oracle_Cycle_Exchangeable(vector<int> S, int a, int b) {
	//a = HashMap(a);  //HashMap maps integers to its elements in the context of the problem
	//b = HashMap(b); 
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S[S.size()-1]);
			S.pop_back();
			break;
		}
	return Oracle_Cycle_Free(S,b);
}


bool Oracle_Transversal_MSA(vector<int> S) {
	return Oracle_Transversal_Right(S);
}

bool Oracle_Transversal_MSA_Free(vector<int> S, int free) {
	return Oracle_Transversal_Right_Free(S,free);
}

bool Oracle_Transversal_MSA_Exchangeable(vector<int> S, int a, int b) {
	return Oracle_Transversal_Right_Exchangeable(S,a,b);
}


bool Oracle_Transversal_Left(vector<int> S) {
	vector<Edge> Edges;
	int M = 5;
	bool left[M] = {false}; //M should be the number of vertices in the LEFT bipartition
	for (int i=0; i < S.size(); i++)
	{
		Edge edge = graph[S[i]];
		if (left[edge.u] == true) return false;
		left[edge.u] = true;
	}
	return true;
}

bool Oracle_Transversal_Left_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Transversal_Left(S);
}

bool Oracle_Transversal_Left_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Transversal_Left(S);
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S[S.size()-1]);
			S.pop_back();
			break;
		}
	return Oracle_Transversal_Left_Free(S,b);
}


bool Oracle_Transversal_Right(vector<int> S) {
	vector<Edge> Edges;
	int M = 5; //M should be number of vertices in the LEFT bipartition
	bool right[M] = {false}; //M is the number of vertices in the RIGHT bipartition
	for (int i=0; i < S.size(); i++)
	{
		Edge edge = graph[S[i]];
		if (right[edge.v] == true) return false;
		right[edge.v] = true;
	}
	return true;
}

bool Oracle_Transversal_Right_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Transversal_Right(S);
}

bool Oracle_Transversal_Right_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Transversal_Right(S);
	for (size_t i = 0; i < S.size(); ++i)
		if (S[i] == a) 
		{
			swap(S[i], S[S.size()-1]);
			S.pop_back();
			break;
		}
	return Oracle_Transversal_Right_Free(S,b);
}


///////////////////////////
/* TODO General Partition Matroid (k different from 1) */
///////////////////////////


// N colours are identified with integers from 0 to N-1
bool Oracle_Colour(vector<int> S) {
	bool counter[N] = {false};
	for (int e : S)
	{
		if (counter[e]==true)
			return false;
		counter[e] = true;
	}
	return true;
}

bool Oracle_Colour_Free(vector<int> S, int free) {
	S.push_back(free);
	return Oracle_Colour(S);
}
bool Oracle_Colour_Exchangeable(vector<int> S, int a, int b) {
	if (a==b) return Oracle_Colour(S);
	if (Oracle_Colour(S))
	{
		S.push_back(b);
		return Oracle_Colour(S);
	}
	return false;
}

