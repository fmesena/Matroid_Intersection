#include "LeftMatching.h"

LeftMatching::LeftMatching(int N_, vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    left = vector<int>(N_,0);
}
void LeftMatching::Update_State(vector<int> S) {
    fill (left.begin(),left.end(),0);
    for (size_t i = 0; i < S.size(); ++i)
        left [edges[S[i]].u] = 1;
}
void LeftMatching::Temp_Update_State(int a, bool to_be_added) {
    if (to_be_added) {
        left [edges[a].u] = 1;
    }
    else {
        left [edges[a].u] = 0;
    }
}
bool LeftMatching::Exchangeable(int a, int b) {  //O(1)
    ORACLE_CALLS++;
    return left[edges[b].u]==0 || edges[a].u == edges[b].u;
}
bool LeftMatching::Free(int b) {  //O(1)
    ORACLE_CALLS++;
    return left[edges[b].u]==0;
}
int LeftMatching::Rank(vector<int> B) {  //O(n)
    ORACLE_CALLS++;
    vector<int> occupied=left;
    int r=0;
    for (int i=0; i<B.size(); i++)
    {
        if (occupied[edges[B[i]].u] == 1) continue;
        occupied[edges[B[i]].u] = 1;
        r++;
    }
    return r; //returns rank(B\cup S)
}
bool LeftMatching::Exchangeable_Set(vector<int> A, int b) {  // O(|A|)
    ORACLE_CALLS++;
    for (int a:A) {
        if (edges[a].u == edges[b].u) {
            return true;
        }
    }
    return false;
}
void LeftMatching::show() {
    cout << "Left Matching: " << N << " edges" << endl;
    for (Edge e : edges) cout << e.u << " " << e.v << endl;
    cout << endl << endl;
}