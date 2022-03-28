#include "RightMatching.h"

RightMatching::RightMatching(int N_, vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    right = vector<int>(N_,0);
}
void RightMatching::Update_State(vector<int> S) {
    fill (right.begin(),right.end(),0);
    for (size_t i = 0; i < S.size(); ++i)
        right [edges[S[i]].v] = 1;
}
bool RightMatching::Exchangeable(int a, int b) {
    ORACLE_CALLS++;
    return right[edges[b].v]==0 || edges[a].v == edges[b].v;
}
bool RightMatching::Free(int b) {
    ORACLE_CALLS++;
    return right[edges[b].v]==0;
}
int RightMatching::Rank(vector<int> B) {
    ORACLE_CALLS++;
    vector<int> occupied=right;
    int r=0;
    for (int i=0; i<B.size(); i++)
    {
        if (occupied[edges[i].v] == 1) continue;
        occupied[edges[i].v] = 1;
        r++;
    }
    return r;
}
bool RightMatching::Exchangeable_Set(vector<int> A, int b) {
    ORACLE_CALLS++;
    if (right[edges[b].v]==0) return true;
    for (int a:A)
        if (edges[a].v == edges[b].v)
            return true;
    return false;
}
void RightMatching::show() {
    cout << "Right Matching: " << N << " edges" << endl;
    for (Edge e : edges) cout << e.u << " " << e.v << endl;
    cout << endl << endl;
}
