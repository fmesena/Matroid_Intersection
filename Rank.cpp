#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include "Rank.h"

using std::vector;
using std::cout;
using std::endl;

static Oracle* O1;
static Oracle* O2;
static int  free_;		  
static int  exchangeable;
static vector<int> B_S;
static vector<int> B_not_S;

int FindExchange(int const b, vector<int> const &A) {

	if (A.empty()) return -1;

	if (!O2->Exchangeable_Set(A,b)) return -1;

	int M;
	int L=0;
	int R=A.size()-1;

	exchangeable = 0;
	vector<int> A_;

	while (L < R)
	{	
		M = L + (R-L)/2;      // (L+R)/2
		A_ = Slice(A, L, M);  // this takes O(|A|) operations overall
		
		if (O2->Exchangeable_Set(A_,b)) // is S-A+b independent?
 			R = exchangeable = M;
		else
			L = exchangeable = M+1;
	}
	return A[exchangeable];
}

int FindFree(vector<int> const &B) {
	
	if (B.empty()) return -1;

	if (O1->Rank(B) == 0) return -1; //O->Rank(B) returns rank(B\cup S)

	int L=0;
	int R=B.size()-1;
	int M;

	vector<int> B_;
	free_ = 0;

	while (L < R)
	{
		M = L + (R-L)/2;
		B_ = Slice(B,L,M);		// this takes O(|B|) operations in total

		if (O1->Rank(B_) > 0)
			R = free_ = M;
		else
			L = free_ = M+1;
	}
	return B[free_];
}

int OutArc(int const a, const vector<int> B) {

	if (a == SOURCE) return FindFree(B);

	if (IN_INDEPENDENT(a))
	{	
		assert(independent_set[index_[a]]!=-1);

		swap(independent_set[index_[a]], independent_set.back());
		independent_set.pop_back();
		O1->Temp_Update_State(a,false);
		O2->Temp_Update_State(a,false);
		
		int e = FindFree(B);
		
		independent_set.push_back(a);
		swap(independent_set.back(), independent_set[index_[a]]);
		O1->Temp_Update_State(a,true);
		O2->Temp_Update_State(a,true);
		
		return e;
	}

	if (!IN_INDEPENDENT(a) && O2->Free(a)) return TARGET;

	if (!IN_INDEPENDENT(a)) 			   return FindExchange(a, B);

	cerr << ">>ERROR: Rank::OutArc" << endl;
	return -1;
}

void GetDistancesRank() {

	B_S.clear();
	B_not_S.clear();

	for (int i = 0; i < N; ++i)
	{
		if (IN_INDEPENDENT(i)) B_S.push_back(i);
		else B_not_S.push_back(i);
	}

	for (int i=0; i<N; i++)
		distances[i] = numeric_limits<int>::max();
	DISTANCE_TARGET = numeric_limits<int>::max();

	int a;
	int b;
	queue<int> q;
	q.push(SOURCE);

	while(!q.empty())
	{
		a = q.front(); q.pop();
		if (a==SOURCE)
		{
			while ( (b = OutArc(a,B_not_S)) != -1 )
			{	
				distances[b] = 1;
				q.push(b);
				swap(B_not_S.back(), B_not_S[free_]);
				B_not_S.pop_back();
			}
		}
		else if (IN_INDEPENDENT(a))
		{
			while ( (b = OutArc(a,B_not_S)) != -1 ) {
				distances[b] = distances[a]+1;
				q.push(b);
				swap(B_not_S.back(), B_not_S[free_]);
				B_not_S.pop_back();
			}
		}
		else if (!IN_INDEPENDENT(a))
		{
			while ( (b = OutArc(a,B_S)) != -1 )
			{
				if (b==TARGET)
				{
					DISTANCE_TARGET = distances[a]+1;
					return;
				}
				distances[b] = distances[a]+1;
				q.push(b);
				swap(B_S.back(), B_S[exchangeable]);
				B_S.pop_back();
			}
		}
	}
	return;
}

// An iterative DFS-like routine
void BlockFlow() {

	GetDistancesRank();
	if (DISTANCE_TARGET == numeric_limits<int>::max())
		return;

	ClearCandidates();
	for (int i = 0; i < N; ++i)
		if (distances[i] != numeric_limits<int>::max())
			candidates[distances[i]].push_back(i);

	candidates[0].push_back(SOURCE);
	candidates[DISTANCE_TARGET].push_back(TARGET);

	int augmenting_paths_count = 0;
	vector<int>  path;
	vector<int>  indexes;
	vector<int>  prev_element;
	int current = SOURCE;
	int l = 0;
	while (l>=0)
	{	
		if (l==DISTANCE_TARGET-1)
		{	
			if (!O2->Free(current)) {
				swap(candidates[l].back(), candidates[l][indexes.back()]);
				candidates[l].pop_back();
				current = prev_element.back();
				path.pop_back();
				indexes.pop_back();
				prev_element.pop_back();
				l--;
			}
			else {
				prev_element.push_back(current);
				l++;
			}
		}
		else if (l<DISTANCE_TARGET)
		{
			if (candidates[l+1].empty()) {
        		return;
      		}
			int next = OutArc(current, candidates[l+1]);
			if (next == -1)
			{	
				// this block corresponds to the part of removing the so called "useless" elements (Cunningham)
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
				path.push_back(next);
				//indexes.push_back(exchangeble);
				indexes.push_back(l%2==0 ? free_:exchangeable); //even layer correspond to SOURCE and elements in S, odd correspond to elements in \overline{S}
				prev_element.push_back(current);
				current = next;
				l++;
			}
		}
		else if (l==DISTANCE_TARGET)
		{
			augmenting_paths_count++;
			assert(path.size() == indexes.size());
			assert(path.size() > 0);
			for (size_t j=0; j<path.size(); j++)
			{
				in_independent_set[path[j]] = IN_INDEPENDENT(path[j]) ? false:true;
				swap(candidates[distances[path[j]]].back(), candidates[distances[path[j]]][indexes[j]]);
				candidates[distances[path[j]]].pop_back();
			}

			UpdateIndependentSet();
			O1->Update_State(independent_set);
			O2->Update_State(independent_set);
			
			vector<int>().swap(path);
			vector<int>().swap(indexes);
			vector<int>().swap(prev_element);
			
			current = SOURCE;

			l=0;
		}
		else if (l>DISTANCE_TARGET)
		{
			cerr << ">>ERROR: Rank::BlockFlow, l>DISTANCE_TARGET" << endl;
		}
	}
	
	AUGMENTATIONS.push_back(augmenting_paths_count);

	return;
}

size_t ExactRank(int N_, Oracle* O1_, Oracle* O2_) {
	
	size_t s;
	N  = N_;
	O1 = O1_;
	O2 = O2_;

	Init(N);
	AUGMENTATIONS.clear();

	do
	{
		s = SZ;
		BlockFlow();
	} while (s != SZ);
	
	delete[] distances;
	/*for (int i = 0; i < ?; i++) {
        delete[] candidates[i];
    }
    delete[] candidates;*/

	return SZ;
}

size_t ApproxRank(int N_, Oracle* O1_, Oracle* O2_, double eps=0.1) {

	size_t s;
	N  = N_;
	O1 = O1_;
	O2 = O2_;

	Init(N);
	AUGMENTATIONS.clear();
	
	int i = 0;
	while (i++ != (int)(1/eps) or s!=SZ)
	{
		s=SZ;
		BlockFlow();
	}

	delete[] distances;
	/*for (int i = 0; i < ?; i++) {
        delete[] candidates[i];
    }
    delete[] candidates;*/	

	return SZ;
}