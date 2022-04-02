#ifndef RIGHTMATCHING_H_
#define RIGHTMATCHING_H_

#include "Base.h"
#include "../ds/Edge.h"

using namespace std;

class RightMatching : public Oracle {
public:
    int N;
    vector<int> right;
    vector<Edge> edges;
    RightMatching(int, vector<Edge>);
    bool Exchangeable(int, int);
    bool Free(int);
    int  Rank(vector<int>);
    bool Exchangeable_Set(vector<int>, int);
    void Update_State(vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif