#include "Graphic.h"
#include <cassert>

Graphic::Graphic(int N_,vector<Edge> edges_):Oracle() {
    N = N_;
    edges = edges_;
    lct = allocLCT(N);
}
Graphic::~Graphic() {
    freeLCT(lct);
}
bool Graphic::Free(int b) {  // log r
    ORACLE_CALLS++;
    FREE_CALLS++;
    if (linkedQ(lct, edges[b].u, edges[b].v) )
        return false;
    return true;
}
bool Graphic::Exchangeable(int a, int b) {  // 3.log r
    ORACLE_CALLS++;
    EXCH_CALLS++;
    cut(lct, edges[a].u, edges[a].v);
    bool ok = !linkedQ(lct, edges[b].u, edges[b].v);
    link(lct, edges[a].u, edges[a].v);
    return ok;
}
bool Graphic::Exchangeable_Set(vector<int> A, int b) { // 2.r.log r
    ORACLE_CALLS++;
    EXCH_CALLS++;
    for (size_t i=0; i<A.size(); i++) {
        cut(lct, edges[A[i]].u, edges[A[i]].v);
    }
    bool ok = !linkedQ(lct, edges[b].u, edges[b].v);
    for (size_t i=0; i<A.size(); i++) {
        link(lct, edges[A[i]].u, edges[A[i]].v);
    }
    return ok;
}
int Graphic::Rank(vector<int> B) { // 3.n.log r
    ORACLE_CALLS++;
    EXCH_CALLS++;
    int r=0;
    //LCT aux = lct;
    vector<int> links;
    for (size_t i=0; i<B.size(); i++)
    {
        if (!linkedQ(lct, edges[B[i]].u, edges[B[i]].v) )
        {
            r++;
            link(lct, edges[B[i]].u, edges[B[i]].v);
            links.push_back(i);
        }
    }
    for (size_t i=0; i<links.size(); i++) {
        cut(lct, edges[B[links[i]]].u, edges[B[links[i]]].v);
    }
    //assert(fastEqQ(aux,lct));
    return r;
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
    std::cout << "Graphic matroid oracle (using LCTs). Printing edge list:\n";
    for (Edge &e:edges) cout << e.u << "-" << e.v << " | ";
    cout << endl;
    char *name1 = (char*)"teste";
    char *name2 = (char*)"rep.dot";
    showLCT(lct,name1);
    showRepTree(lct,name2); //use with GraphViz online
}