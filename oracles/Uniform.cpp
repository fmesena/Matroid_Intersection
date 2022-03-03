#include "Uniform.h"

Uniform::Uniform(int _n, int _r):Oracle() {
    n = _n;
    r = _r;
}
bool Uniform::Exchangeable(int a, int b) {
    ORACLE_CALLS++;
    return S.size() <= r;
}
bool Uniform::Free(int b) {
    ORACLE_CALLS++;
    return S.size() < r;
}
int Uniform::Rank(vector<int> B) {
    ORACLE_CALLS++;
    return S.size()+B.size() <= r ? S.size()+B.size() : r;
}
bool Uniform::Exchangeable_Set(vector<int> A, int b) {
    ORACLE_CALLS++;
    return S.size() <= r;
}
void Uniform::Update_State(vector<int> _S) { //TODO
    S=_S;
}
void Uniform::show() {
    std::cout << "Uniform\n";
}
