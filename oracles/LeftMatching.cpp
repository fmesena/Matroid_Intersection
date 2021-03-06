#include "LeftMatching.h"

LeftMatching::LeftMatching(int N_, vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    left = vector<int>(N_,0);
}
bool LeftMatching::Free(int b) {
    ORACLE_CALLS++;
    FREE_CALLS++;
    return left[edges[b].u]==0;
}
bool LeftMatching::Exchangeable(int a, int b) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    return left[edges[b].u]==0 || edges[a].u == edges[b].u;
}
bool LeftMatching::Exchangeable_Set(vector<int> A, int b) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    for (int a:A) {
        if (edges[a].u == edges[b].u) {
            return true;
        }
    }
    return false;
}
// S = 00010010
// B = 11010100
// r(B U S) = bitwise op. (S_i V B_i), (not S_i V B_i), (S_i V not B_i)
int LeftMatching::Rank(vector<int> B) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    vector<int> occupied=left;
    int r=0;
    for (size_t i=0; i<B.size(); i++)
    {
        if (occupied[edges[B[i]].u] == 1) continue;
        occupied[edges[B[i]].u] = 1;
        r++;
    }
    return r;
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
void LeftMatching::show() {
    cout << "Left Matching: " << N << " vertices" << endl;
    for (Edge e : edges) cout << e.u << " " << e.v << endl;
    cout << endl << endl;
}