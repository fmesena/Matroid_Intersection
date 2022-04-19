#ifndef RIGHTMATCHING_H_
#define RIGHTMATCHING_H_

#include "Base.h"
#include "../ds/Edge.h"

using namespace std;

class RightMatching final : public Oracle {
public:
    int N;
    vector<int> right;
    vector<Edge> edges;
    RightMatching(int, vector<Edge>);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(vector<int>, int);
    int  Rank(vector<int>);
    void Update_State(vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif