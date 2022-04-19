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

class Graphic final : public Oracle {
public:
    LCT lct;
    int N;
    vector<Edge> edges;
    Graphic(int,vector<Edge>);
    ~Graphic();
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(vector<int>, int);
    int  Rank(vector<int>);
    void Update_State(std::vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif