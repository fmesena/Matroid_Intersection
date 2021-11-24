#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <assert.h>
#include <limits>
#include <chrono>
#include "Rank.h"

using namespace std;
using namespace std::chrono;

#define 	MAX_V				100000
#define 	min(u, v) 			(u<v ? u:v)
#define		even(x)				x%2==0
#define		odd(x)				x%2==1
#define		SZ 					independent_set.size()
#define 	IN_INDEPENDENT(a)	in_independent_set[a]

#define		Rank_M1(S)					(*M1_Rank)(S)
#define		Oracle_M2(S)				(*M2_Indep)(S)
#define		Oracle_M2_Free(S,b)			(*M2_Free)(S,b)

int N;
vector<int> AUGMENTATIONS;

vector<int>  independent_set;
vector<bool> in_independent_set;
vector<int>  index_;				  // index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S

int exchangeable; //hack
int free_;		  //hack

int DISTANCE_TARGET;
int CURRENT_RANK;
bool TARGET_IN_B;

vector<int> B_S;
vector<int> B_not_S;

int *distances;

int  (*M1_Rank)(vector <int>);
bool (*M2_Indep)(vector <int>);
bool (*M2_Free)(vector<int> S, int b);

const int SOURCE = -2;
const int TARGET = -3;


template<typename T>
void DEBUG_VECTOR(const vector<T> &v) {
	cout << "\nDEBUG BEGIN" << endl;
	cout << "The vector size is " << v.size() << " and its " << "capacity is " << v.capacity() << endl;
	cout << "Vector content:" << endl;
	for (size_t i=0; i < v.size(); ++i) cout << v[i] << " ";
	cout << "\nDEBUG END" << endl;
}

template<typename T>
vector<T> Slice(vector<T> const &v, int m, int n) {
	auto first = v.cbegin() + m;
	auto last = v.cbegin() + n;
	vector<T> vec(first, last);
	return vec;
}


// index[i] = -1 if i-th element is not in S, index[i]=j 0<j<N-1 if i-th element is in S
void UpdateIndependentSet() {
	independent_set.clear();
	index_.clear();

	for (int i = 0, j = 0; i < N; ++i)
	{
		index_[i] = -1;
		if (IN_INDEPENDENT(i))
		{
			independent_set.push_back(i);
			index_[i] = j++;
		}
	}
}

void PrintIndependentSet() {
	cout << "INDEPENDENT SET FOUND WITH SIZE " << independent_set.size() << ":" << endl;
	for (size_t i = 0; i < independent_set.size(); ++i) cout << independent_set[i] << " ";
	cout << endl;
}

void PrintCandidates(vector<int> c[]) {
	for (size_t i = 0; i < 3; ++i)
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
	for (size_t i = 0; i < A.size(); i++)
		if (index_[A[i]] != -1) {
			S[index_[A[i]]] = -1; j++; }
	vector<int> S_;
	for (size_t i = 0; i < S.size(); i++)
		if (S[i] != -1)
			S_.push_back(i);
	return S_;
}


/*

def binary_search(arr, low, high, x):
 
    # Check base case
    if high >= low:
 
        mid = (high + low) // 2
 
        # If element is present at the middle itself
        if arr[mid] == x:
            return mid
 
        # If element is smaller than mid, then it can only
        # be present in left subarray
        elif arr[mid] > x:
            return binary_search(arr, low, mid - 1, x)
 
        # Else the element can only be present in right subarray
        else:
            return binary_search(arr, mid + 1, high, x)
 
    else:
        # Element is not present in the array
        return -1
 

arr = [ 2, 3, 4, 10, 40 ]
x = 10

result = binary_search(arr, 0, len(arr)-1, x)
 
if result != -1:
    print("Element is present at index", str(result))
else:
    print("Element is not present in array")

*/

int FindExchange(bool (*Oracle)(vector<int>, int), int const b, vector<int> A) {

	if (A.empty()) return -1;

	int sz = A.size();
	int l=0;
	int r=sz-1;
	int m;

	vector<int> A1;
	vector<int> S = RemoveSubset(independent_set,A);

	if (!Oracle(S,b)) return -1;

	exchangeable = 0;

	while (sz > 1)
	{	
		m = l + (r-l)/2;    // (l+r)/2
		A1 = Slice(A, l, m);  // this takes O(|A|) operations overall. this could be deleted to improve running time, but then we would have to add the information of
							  // which subset of A we want to consider to remove from S. 
		S  = RemoveSubset(S, A1); // this takes (|S|+|A|)log|A| operations in total

		if (Oracle(S,b)) // Oracle(S,A1,b)
			r = exchangeable = m;
		else
			l = exchangeable = m;
		
		sz /= 2;
	}
	return A[exchangeable];
}


int FindFree(int (*RankOracle)(vector<int>), vector<int> S, vector<int> B) {
	
	if (B.empty()) return -1;

	int sz = B.size();
	int l=0;
	int r=sz-1;
	int m;
	vector<int> B_U_S = S;

	for (size_t i=0; i<B.size(); i++) B_U_S.push_back(B[i]);

	if (RankOracle(B_U_S) == RankOracle(S)) return -1;

	//CURRENT_RANK = RankOracle(S) + increment;
	//if (RankOracle(A) == CURRENT_RANK) return -1;

	free_ = 0;

	while (sz > 1)
	{
		B_U_S = S;
		m = l + (r-l)/2;  // (l+r)/2
		
		for (int i=l; i<m; i++)      // this takes O(|B|) operations in total
			B_U_S.push_back(B[i]);

		if (RankOracle(B_U_S) > RankOracle(S))
			r = free_ = m;
		else
			l = free_= m;

		sz /= 2;

	}
	return B[free_];
}


/*
int OutArc_OLD(int const a) {
	vector<int> S;

	if (a == SOURCE) { return FindFree(M1_Rank, independent_set, B_not_S); }

	S = independent_set; //is this not too bad?
	if (IN_INDEPENDENT(a))
	{	
		assert(S[index_[a]]!=-1);
		swap(S[index_[a]], S.back());
		S.pop_back();
		return FindFree(M1_Rank, S, B_not_S);
	}

	S.push_back(a);  //i dont like this, maybe change to Oracle_M2_Free(S,a)
	if (!IN_INDEPENDENT(a) && TARGET_IN_B && Oracle_M2(S)) { return TARGET; }
	S.pop_back();

	if (!IN_INDEPENDENT(a)) return FindExchange(M2_Exch_Set, a, B_S);

	return -1;
}*/


// DANGER: alterar independent_set sem alterar index_
int OutArc(int const a, const vector<int> L) {
	vector<int> S;

	if (a == SOURCE) return FindFree(M1_Rank, independent_set, L);
	
	S = independent_set;
	if (IN_INDEPENDENT(a))
	{
		// se nao quiseres fazer a cópia da linha S=independent_set e quiseres usar diretamente o independent_set, não te esqueças de atualizar o index_
		assert(S[index_[a]]!=-1);
		swap(S[index_[a]], S.back());
		S.pop_back();
		return FindFree(M1_Rank, S, L);
	}

	/* Maybe change the remainder of the routine to
		if(TARGET_IN_B && Oracle_M2(S)) { TARGET_IN_B=false; return TARGET; }  pois !IN_INDEPENDENT(a) vai ser sempre verdade
		return FindExchange(M2_Exch_Set, a, L); */

	if (!IN_INDEPENDENT(a) && TARGET_IN_B && Oracle_M2_Free(S,a)) { TARGET_IN_B=false; return TARGET; }

	if (!IN_INDEPENDENT(a)) return FindExchange(M2_Free, a, L);

	cerr << "something went wrong\n";
	return -1; // signals "FALSE". in theory, the code should never reach here
}


void GetDistancesRank() {

	B_S.clear();
	B_not_S.clear();

	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i)) B_S.push_back(i);
		else B_not_S.push_back(i);
	}

	for (int i=0; i<N; i++) distances[i] = numeric_limits<int>::max(); //FIXME INT_MAX
	DISTANCE_TARGET = numeric_limits<int>::max(); //FIXME INT_MAX
	for (int i=0; i<N; i++) cout << distances[i] << " "; //FIXME INT_MAX
	cout << endl;

	TARGET_IN_B = true;

	queue<int> q;
	q.push(SOURCE);

	int a;
	int b;
	vector<int> B;

	cout << "Starting BFS..." << endl;

	while(!q.empty())	// Remark: B.pop_back() "funciona como" visited[i] = true numa BFS simples
	{
		a = q.front(); q.pop();
		cout << "Expanding " << a << endl;

		// Inefficient, work with pointers
		if (a==SOURCE) B = B_not_S;
		else if (IN_INDEPENDENT(a))  B = B_not_S;
		else if (!IN_INDEPENDENT(a)) B = B_S;

		while ( (b = OutArc(a,B)) != -1 )
		{
			DEBUG_VECTOR(B_S);
			DEBUG_VECTOR(B_not_S);
			cout << "Found arc to " << b << endl;
			// Although we have reached the target, we don't break the loop since we want to compute the distances to ALL the vertices for the BlockFlow
			// (i think we could break but whatever)
			if (b==TARGET)
			{
				DISTANCE_TARGET = distances[a]+1;
				TARGET_IN_B = false;
				continue;
			}
			
			distances[b] = a==SOURCE? 1:distances[a]+1;
			q.push(b);

			if (a==SOURCE) // || IN_INDEPENDENT(a))  >>> watch out segfault <<<
			{
				swap(B.back(), B[free_]);
				B.pop_back();
				B_not_S = B;
			}
			else if (IN_INDEPENDENT(a))
			{
				swap(B.back(), B[free_]);
				B.pop_back();
				B_not_S = B;
			}
			else if (!IN_INDEPENDENT(a))
			{
				swap(B.back(), B[exchangeable]);
				B.pop_back();
				B_S = B;
			}
		}
	}
	//cout << "\nDistances:\n";
	//for (int i = 0; i < N; ++i) {cout << distances[i] << " ";} cout << endl;
	return;
}


// An iterative DFS-like routine
void BlockFlow() {

	cout << "Computing distances..." << endl;
	GetDistancesRank();
	for (int i=0; i<N; i++) cout << distances[i] << " "; //FIXME INT_MAX
	cout << endl;

	// Target is not reachable
	if (DISTANCE_TARGET == numeric_limits<int>::max()) { cout << "Target unreachable\n"; return; }

	// TODO: update candidates along the execution of GetDistancesRank
	vector<int> candidates[2*SZ+3]; //FIXME magic numbers, 2*SZ+3 is just a safe upper bound on the number of distinc distancess
	for (int i = 0; i < N; ++i)
		if (distances[i] != numeric_limits<int>::max())
			candidates[distances[i]].push_back(i);

	candidates[0].push_back(SOURCE);
	candidates[DISTANCE_TARGET].push_back(TARGET);

	int current = SOURCE;
	int l = 0;
	int k;
	TARGET_IN_B = true;
	vector<int>  path;
	vector<int>  indexes;
	vector<int>  prev_element;

	//PrintCandidates(candidates);

	int augmenting_paths_count = 0;

	cout << "Beginning BlockFlow with DISTANCE_TARGET equal to " << DISTANCE_TARGET << endl;

	while (l>=0)
	{
		if (l<DISTANCE_TARGET)
		{	
			// i think this condition is useless
			if (candidates[l].empty()) {
				cout << "not useless after all...\n";
        		return;
      		}
      		cout << "Current:" << current << ", at level " << l << endl;
			int next = OutArc(current, candidates[l+1]);
			cout << "Next:" << next << endl;
			if (next == -1)
			{
				if (current != SOURCE)
				{
					swap(candidates[l].back(), candidates[l][indexes.back()]);
					candidates[l].pop_back();
					
					current = prev_element.back();

					path.pop_back();
					indexes.pop_back();
					prev_element.pop_back();
				}
				l--;
			}
			else
			{
				if (next != TARGET)
				{
					path.push_back(next);
					indexes.push_back(l%2==0 ? free_:exchangeable); //even layer correspond to SOURCE and elements in S, odd correspond to elements in \overline{S}
					prev_element.push_back(current);

					current = next;
				}
				l++;
			}
		}

		if (l==DISTANCE_TARGET)
		{
			augmenting_paths_count++;
			cout << "l==DISTANCE_TARGET, apc: " << augmenting_paths_count << endl;
			//Augment along the path s,a1,a2,...,a_dt-1,t to obtain bigger S
			//Update candidates: for all i=0 up to DISTANCE_TARGET-1, set candidates[i] = candidates[i] - a_i
			assert(path.size() == indexes.size());
			assert(path.size() > 0);
			for (size_t j=0; j<path.size(); j++)
			{
				in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;
				swap(candidates[distances[path[j]]].back(), candidates[distances[path[j]]][indexes[j]]); //Safe because no two vertices lying on the path have the same distances
				candidates[distances[path[j]]].pop_back();
			}

			UpdateIndependentSet();
			//CURRENT_RANK++

			TARGET_IN_B = true;

			vector<int>().swap(path);
			vector<int>().swap(indexes);
			vector<int>().swap(prev_element);
			
			current = SOURCE;

			l=0;
		}
	}
	
	AUGMENTATIONS.push_back(augmenting_paths_count);

	return;
}


void FindGreedy() {
	for (int i = 0; i < N; ++i)
	{
		independent_set.push_back(i);
		in_independent_set[i]=true;
		if (Rank_M1(independent_set)!=SZ || !Oracle_M2(independent_set))
		{
			independent_set.pop_back();
			in_independent_set[i]=false;
		}
	}
	return;
}

void Init() {
	in_independent_set.resize(N,false);
	index_.resize(N,-1);
	distances = new int[N]();
	//TODO allocate candidates here maybe?
}

size_t ExactRank(int N_, int (*a)(vector<int>), bool (*b)(vector<int>), bool (*c)(vector<int>, int)) {
	
	size_t s;
	N = N_;
	M1_Rank = a;
	M2_Indep = b;
	M2_Free  = c;

	cout << "Init...\n";
	Init();
	//FindGreedy();
	//UpdateIndependentSet();
	
	//TODO allocate candidates here maybe?

	int x=0;
	do
	{
		cout << "x:" << ++x << endl;
		cout << "SZ, s = " << SZ << ", " << s << endl;
		s = SZ;
		BlockFlow();
	} while (s != SZ);
	
	//UpdateIndependentSet(); //useless
	PrintIndependentSet();

	delete[] distances;
	//TODO free candidates here maybe?	

	return SZ;
}


size_t ApproxRank(int N_, int (*a)(vector<int>), bool (*b)(vector<int>), bool (*c)(vector<int>, int), double eps=0.1) {

	N = N_;

	M1_Rank = a;

	M2_Indep = b;
	M2_Free  = c;

	int i = 0;

	Init();
	FindGreedy();
	UpdateIndependentSet();
	
	while (i++ != (int)(1/eps) ) BlockFlow();  //FIXME 1/eps -_-

	//UpdateIndependentSet(); //useless
	PrintIndependentSet();

	delete[] distances;
	//TODO free candidates here maybe?	

	return SZ;
}


//int main(){}

/*
int main() {

	cin >> N;

	in_independent_set.resize(N,false);
	index_.resize(N,-1);


	auto start = high_resolution_clock::now();

	ExactRank();
	//ApproxRank(0.1);

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

	PrintIndependentSet();

	return 0;
}
*/
