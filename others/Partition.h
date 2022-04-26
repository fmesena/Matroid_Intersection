#ifndef PARTITION_H_
#define PARTITION_H_

#include "../oracles/Base.h"
#include "../ds/Edge.h"

class Partition final : public Oracle {
public:
    int N;
    std::vector<int> taken;
    std::vector<int> elements;
    Partition(int, std::vector<int>);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(std::vector<int>, int);
    int  Rank(std::vector<int>);
    void Update_State(std::vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif