#include "Graphic.h"

Graphic::Graphic(int N_, LCT lct_):Oracle() {
    N = N_;
    lct = lct_;
}
bool Graphic::Exchangeable(int a, int b) {  // 4*lgN amortized
    /*
    lct[S] - a;
    if (lct[S]+b)
    {
        lct[S]-b;
        lct[S]+a; //safe but maybe include assertion
        return true;  
    }
    lct[S]+a;
    */
    return false;      
}
bool Graphic::Free(int b) {  // 2*lgN amortized
    /*
    if (lct[S]+b)
    {
        lct[S]-b;
        return true;  
    }
    */
    return false;
}
int Graphic::Rank(vector<int> B) { //TODO
    ORACLE_CALLS++;
    return 0;
}
bool Graphic::Exchangeable_Set(vector<int> A, int b) {  //TODO
    return true;
}
void Graphic::Update_State(vector<int> S_) {  //TODO
    //lct[S[i]]...
}
void Graphic::show() {
    std::cout << "Graphic matroid oracle (LCT)" << endl;
}