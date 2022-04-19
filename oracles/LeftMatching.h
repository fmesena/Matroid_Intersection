#ifndef LEFTMATCHING_H_
#define LEFTMATCHING_H_

#include "Base.h"
#include "../ds/Edge.h"

using namespace std;

class LeftMatching final : public Oracle {
public:
    int N;
    vector<int> left;
    vector<Edge> edges;
    LeftMatching(int, vector<Edge>);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(vector<int>, int);
    int  Rank(vector<int>);
    void Update_State(vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif