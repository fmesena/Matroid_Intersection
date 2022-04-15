#include "RightMatching.h"

RightMatching::RightMatching(int N_, vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    right = vector<int>(N_,0);
}
bool RightMatching::Free(int b) {
    ORACLE_CALLS++;
    FREE_CALLS++;
    return right[edges[b].v]==0;
}
bool RightMatching::Exchangeable(int a, int b) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    return right[edges[b].v]==0 || edges[a].v == edges[b].v;
}
bool RightMatching::Exchangeable_Set(vector<int> A, int b) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    for (int a:A) {
        if (edges[a].v == edges[b].v) {
            return true;
        }
    }
    return false;
}
int RightMatching::Rank(vector<int> B) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    vector<int> occupied=right;
    int r=0;
    for (size_t i=0; i<B.size(); i++)
    {
        if (occupied[edges[B[i]].v] == 1) continue;
        occupied[edges[B[i]].v] = 1;
        r++;
    }
    return r; //returns rank(B\cup S)
}
void RightMatching::Update_State(vector<int> S) {
    fill (right.begin(),right.end(),0);
    for (size_t i = 0; i < S.size(); ++i)
        right [edges[S[i]].v] = 1;
}
void RightMatching::Temp_Update_State(int a, bool to_be_added) {
    if (to_be_added) {
        right [edges[a].v] = 1;
    }
    else {
        right [edges[a].v] = 0;
    }
}
void RightMatching::show() {
    cout << "Right Matching: " << N << " edges" << endl;
    for (Edge e : edges) cout << e.u << " " << e.v << endl;
    cout << endl << endl;
}
