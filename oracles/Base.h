#ifndef BASE_H_
#define BASE_H_

#include <iostream>
#include <vector>

using namespace std;

class Oracle {
public:
    int ORACLE_CALLS=0;
    //vector<int> independent_set; to be defined in each concrete class
    inline Oracle() { ORACLE_CALLS = 0; }
    virtual int Rank(vector<int> B) = 0;
    virtual bool Exchangeable(int a, int b) = 0;
    virtual bool Free(int b) = 0;
    virtual bool Exchangeable_Set(vector<int> A, int b) = 0;
    virtual void Update_State(vector<int> S) = 0;
    virtual void show() = 0;
    inline int getOracleCalls() { return ORACLE_CALLS; }
};

#endif