#ifndef UNIFORM_H_
#define UNIFORM_H_

#include "Base.h"

class Uniform final : public Oracle {
public:
    int n;
    int r;
    int cur;
    Uniform(int, int);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(std::vector<int>, int);
    int  Rank(std::vector<int>);
    void Update_State(std::vector<int>);
    void show();
};

#endif