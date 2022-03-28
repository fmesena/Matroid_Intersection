#include "LeftMatching.h"

LeftMatching::LeftMatching(int N_, vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    left = vector<int>(N_,0);
}
void LeftMatching::Update_State(vector<int> S) {  //additive factor of O(r^2)
    fill (left.begin(),left.end(),0);
    for (size_t i = 0; i < S.size(); ++i)
        left [edges[S[i]].u] = 1;
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
        if (occupied[edges[i].u] == 1) continue;
        occupied[edges[i].u] = 1;
        r++;
    }
    return r;
}
bool LeftMatching::Exchangeable_Set(vector<int> A, int b) {  // O(|A|)
    ORACLE_CALLS++;
    cerr << "exch set LM1!\n";
    if (left[edges[b].u]==0) return true;
    for (int a:A)
        if (edges[a].u == edges[b].u) {
            cerr << "exch set LM2\n";
            return true;
        }
    return false;
}
void LeftMatching::show() {
    cout << "Left Matching: " << N << " edges" << endl;
    for (Edge e : edges) cout << e.u << " " << e.v << endl;
    cout << endl << endl;
}
