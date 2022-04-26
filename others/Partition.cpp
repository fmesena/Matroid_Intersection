#include "Partition.h"

Partition::Partition(int N_, vector<int> elements_):Oracle() {
    N = N_;
    taken  = vector<int>(N_,0);
    elements = elements_;
}
bool Partition::Free(int b) {
    ORACLE_CALLS++;
    FREE_CALLS++;
    return taken[b]==0;
}
bool Partition::Exchangeable(int a, int b) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    return false;
}
bool Partition::Exchangeable_Set(vector<int> A, int b) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    return false;
}
int Partition::Rank(vector<int> B) {
    ORACLE_CALLS++;
    EXCH_CALLS++;
    vector<int> occupied=taken;
    int r=0;
    for (size_t i=0; i<B.size(); i++)
    {
        if (occupied[B[i]] == 1) continue;
        occupied[B[i]] = 1;
        r++;
    }
    return r;
}
void Partition::Update_State(vector<int> S) {
}
void Partition::Temp_Update_State(int a, bool to_be_added) {
}
void Partition::show() {
    cout << "Partition matroid: " << N << " elements" << endl;
    for (int e: elements) cout << e << " ";
    cout << endl << endl;
}