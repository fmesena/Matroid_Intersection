#ifndef LEFTMATCHING_H_
#define LEFTMATCHING_H_

#include "Base.h"
#include "../ds/Edge.h"

using namespace std;

class LeftMatching : public Oracle {
public:
    int N;
    vector<int> left;
    vector<Edge> edges;
    LeftMatching(int, vector<Edge>);
    bool Exchangeable(int, int);
    bool Free(int);
    int  Rank(vector<int>);
    bool Exchangeable_Set(vector<int>, int);
    void Update_State(vector<int>);
    void show();
};

#endif