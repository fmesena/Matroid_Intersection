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
    bool Exchangeable(int, int);
    bool Free(int);
    int Rank(vector<int>);
    bool Exchangeable_Set(vector<int>, int);
    void Update_State(vector<int>);
    void show();
};

#endif