#ifndef BASE_H_
#define BASE_H_

#include <iostream>
#include <vector>

using namespace std;

class Oracle {
public:
    int ORACLE_CALLS=0;
    int EXCH_CALLS=0;
    int FREE_CALLS=0;

    inline          Oracle()  { ORACLE_CALLS = 0; EXCH_CALLS = 0; FREE_CALLS = 0; }
    virtual bool    Free(int b) = 0;
    virtual bool    Exchangeable(int a, int b) = 0;
    virtual bool    Exchangeable_Set(vector<int> A, int b) = 0;
    virtual int     Rank(vector<int> B) = 0;
    virtual void    Update_State(vector<int> S) = 0;
    virtual void    Temp_Update_State(int a, bool to_be_added) = 0;
    virtual void    show() = 0;
    inline  int     getOracleCalls()   { return ORACLE_CALLS; }
    inline  int     getExchangeCalls() { return EXCH_CALLS; }
    inline  int     getFreeCalls()     { return FREE_CALLS; }
};

#endif