#include "RightMatching.h"

RightMatching::RightMatching(int _N, vector<Edge> _edges):Oracle() {
    N = _N;
    edges = _edges;
}
void RightMatching::Update_State(vector<int> S) {
    right = {0};
    for (size_t i = 0; i < S.size(); ++i)
        right [edges[S[i]].v] = 1;
}
bool RightMatching::Exchangeable(int a, int b) {
    return right[edges[b].v]==0 || edges[a].v == edges[b].v;
}
bool RightMatching::Free(int b) {
    return right[edges[b].v]==0;
}
int RightMatching::Rank(vector<int> B) {
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
    if (right[edges[b].v]==0) return true;
    for (int a:A)
        if (edges[a].v == edges[b].v)
            return true;
    return false;
}
void RightMatching::show() {
    std::cout << "Right Matching" << endl;
}
