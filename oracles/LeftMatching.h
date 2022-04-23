#ifndef LEFTMATCHING_H_
#define LEFTMATCHING_H_

#include "Base.h"
#include "../ds/Edge.h"

class LeftMatching final : public Oracle {
public:
    int N;
    std::vector<int> left;
    std::vector<Edge> edges;
    LeftMatching(int, std::vector<Edge>);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(std::vector<int>, int);
    int  Rank(std::vector<int>);
    void Update_State(std::vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif