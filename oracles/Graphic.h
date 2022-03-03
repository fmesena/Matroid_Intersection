#ifndef Graphic_H_
#define Graphic_H_

#include "Base.h"
#include "../ds/LinkCutTree.h"
#include "../ds/Edge.h"

using namespace std;

class Graphic : public Oracle {
public:
    LCT lct;
    int N;
    bool Exchangeable(int a, int b);
    bool Free(int b);
    int Rank(vector<int> B);
    bool Exchangeable_Set(vector<int> A, int b);
    void Update_State(vector<int> S);
};

#endif