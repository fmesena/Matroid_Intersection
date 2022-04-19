#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "Base.h"

using namespace std;

class Uniform final : public Oracle { //poweroracle <=> U(n,n)
public:
    int n;
    int r;
    vector<int> S;
    Uniform(int, int);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(vector<int>, int);
    int  Rank(vector<int>);
    void Update_State(vector<int>);
    void show();
};

#endif