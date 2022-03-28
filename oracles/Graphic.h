#ifndef Graphic_H_
#define Graphic_H_

#include "Base.h"
#include "../ds/Edge.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "../ds/LinkCutTree.h"

#ifdef __cplusplus
} // end extern "C"
#endif

using namespace std;

class Graphic : public Oracle {
public:
    LCT lct;
    int N;
    Graphic(int);
    bool Exchangeable(int a, int b);
    bool Free(int b);
    int  Rank(vector<int> B);
    bool Exchangeable_Set(vector<int> A, int b);
    void Update_State(vector<int> S);
    void show();
};

#endif