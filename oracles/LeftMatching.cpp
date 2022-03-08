#include "LeftMatching.h"

LeftMatching::LeftMatching(int _N, vector<Edge> _edges):Oracle() {
    N = _N;
    edges = _edges;
}
void LeftMatching::Update_State(vector<int> S) {  //additive factor of O(r^2)
    left = {0};
    for (size_t i = 0; i < S.size(); ++i)
        left [edges[S[i]].u] = 1;
}
bool LeftMatching::Exchangeable(int a, int b) {  //O(1)
    return left[edges[b].u]==0 || edges[a].u == edges[b].u;
}
bool LeftMatching::Free(int b) {  //O(1)
    return left[edges[b].u]==0;
}
int LeftMatching::Rank(vector<int> B) {  //O(n)
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
    if (left[edges[b].u]==0) return true;
    for (int a:A)
        if (edges[a].u == edges[b].u)
            return true;
    return false;
}
void LeftMatching::show() {
    std::cout << "Left Matching: " << N << " edges" << endl;
}
