#include "Graphic.h"

Graphic::Graphic(int N_,vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    lct = allocLCT(N);
}
bool Graphic::Exchangeable(int a, int b) {  // 3.logN amortized
    ORACLE_CALLS++;
    cut(lct, edges[a].u, edges[a].v);
    bool ok=true;
    if (linkedQ(lct, edges[b].u, edges[b].v) )
        ok = false;
    link(lct, edges[a].u, edges[a].v);
    return ok;
}
bool Graphic::Free(int b) {  // logN amortized
    ORACLE_CALLS++;
    if (linkedQ(lct, edges[b].u, edges[b].v) )
        return false;
    return true;
}
int Graphic::Rank(vector<int> B) {
    ORACLE_CALLS++;
    int r=0;
    for (int i=0; i<B.size(); i++)
    {
        if (!linkedQ(lct, edges[B[i]].u, edges[B[i]].v) )
        {
            r++;
            link(lct, edges[B[i]].u, edges[B[i]].v);
        }
    }
    for (int i=0; i<B.size(); i++)
        cut(lct, edges[B[i]].u, edges[B[i]].v);

    return r; //returns rank(B\cup S)
}
bool Graphic::Exchangeable_Set(vector<int> A, int b) {
    ORACLE_CALLS++;
    for (int i=0; i<A.size(); i++)
        cut(lct, edges[A[i]].u, edges[A[i]].v);
    bool ok = linkedQ(lct, edges[b].u, edges[b].v) == 0;
    for (int i=0; i<A.size(); i++)
        link(lct, edges[A[i]].u, edges[A[i]].v);
    return ok;
}
void Graphic::Update_State(vector<int> S) {
    cleanLCT(lct);
    for (size_t i = 0; i < S.size(); ++i)
    {
        link(lct, edges[S[i]].u, edges[S[i]].v);
    }
}
void Graphic::Temp_Update_State(int a, bool to_be_added) {
    if (to_be_added) {
        link(lct, edges[a].u, edges[a].v);
    }
    else {
        cut(lct, edges[a].u, edges[a].v);
    }
}
void Graphic::show() {
    std::cout << "Graphic matroid oracle (using LCTs)" << endl;
    link(lct,2,3);
    link(lct,3,4);
    //cleanLCT(lct);
    showLCT(lct,"teste");
    showRepTree(lct,"rep.dot"); //use with GraphViz online
}