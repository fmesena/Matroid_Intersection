#include "Uniform.h"

//This is an example of a very simple oracle to help you implement one for your applications.

Uniform::Uniform(int _n, int _r):Oracle() {
    n   = _n;
    r   = _r;
    cur = 0;
}
bool Uniform::Exchangeable(int a, int b) { //returns true iff `a` and `b` are exchangeable wrt the current independent set S
    return cur <= r;
}
bool Uniform::Free(int b) { //returns true iff `b` is a free element wrt S
    return cur < r;
}
bool Uniform::Exchangeable_Set(vector<int> A, int b) {
    return cur <= r;
}
int Uniform::Rank(vector<int> B) { //returns rank(B U S) - |S|. B is assumed to be contained in V\S
    int b = (int) B.size();
    if (cur+b >= r)
        return r-cur;
    return b;
}
void Uniform::Temp_Update_State(int a, bool to_be_added) { //temporarily removes or adds an element to S. It is assumed that no element is added when S is of maximum possible cardinality.
    cur = to_be_added ? 1:-1;
}
void Uniform::Update_State(vector<int> S) {
    cur = (int) S.size();
}
void Uniform::show() {
    std::cout << "Uniform(" << n << "," << r << ")" << endl;
}