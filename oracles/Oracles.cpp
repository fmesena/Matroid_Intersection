#include <iostream>
#include <vector>
#include "../ds.h"
//#include "LinkCutTree.h"

using namespace std;

virtual class Oracle {
public:
    int ORACLE_CALLS=0;
    //vector<int> independent_set;
    virtual bool Exchangeable(int a, int b) {
        ORACLE_CALLS++;
        cout << "Exchangeable virtual" << endl;
        return false;
    }
    virtual bool Free(int b) {
        ORACLE_CALLS++;
        cout << "Free virtual" << endl;
        return false;
    }
    virtual int Rank(vector<int> B) {
        ORACLE_CALLS++;
        cout << "Rank virtual" << endl;
        return 0;
    }
    virtual bool Exchangeable_Set(vector<int> A, int b) {
        ORACLE_CALLS++;
        cout << "Exchangeable_Set virtual" << endl;
        return false;
    }
    virtual void Update_State(vector<int> S) {
        cout << "Update_State virtual" << endl;
        return;
    }
    virtual void show() {
        cout << "Virtual Oracle" << endl;
    }
    int getOracleCalls() {
        return ORACLE_CALLS;
    }
};


class LeftMatching : public Oracle {
public:

    int N;
    vector<int> left;
    vector<Edge> edges;

    LeftMatching(int _N, vector<Edge> _edges) {
        N = _N;
        edges = _edges;
    }
    void Update_State(vector<int> S) {  //additive factor of O(r^2)
        left = {0};
        for (size_t i = 0; i < S.size(); ++i)
            left [edges[S[i]].u] = 1;
    }
    bool Exchangeable(int a, int b) {  //O(1)
        return left[edges[b].u]==0 || edges[a].u == edges[b].u;
    }
    bool Free(int b) {  //O(1)
        return left[edges[b].u]==0;
    }
    int Rank(vector<int> B) {  //O(n)
        vector<int> occupied=left;
        int r=0;
        for (int i=0; i<B.size(); i++)
        {
            if (occupied[edges[i].u] == 1) continue;
            occupied[edges[i].u] = 1;
            r++;
        }
        return r;
    }
    bool Exchangeable_Set(vector<int> S, vector<int> A, int b) {  // O(|A|)
        if (left[edges[b].u]==0) return true;
        for (int a:A)
            if (edges[a].u == edges[b].u)
                return true;
        return false;
    }
    void show() {
        cout << "Left Matching" << endl;
    }
};


class RightMatching_Oracle : public Oracle {
public:

    int N;
    vector<int> right;
    vector<Edge> edges;

    RightMatching_Oracle(int _N, vector<Edge> _edges) {
        N = _N;
        edges = _edges;
    }
    void Update_State(vector<int> S) {
        right = {0};
        for (size_t i = 0; i < S.size(); ++i)
            right [edges[S[i]].v] = 1;
    }
    bool Exchangeable(int a, int b) {
        return right[edges[b].v]==0 || edges[a].v == edges[b].v;
    }
    bool Free(int b) {
        return right[edges[b].v]==0;
    }
    int Rank(vector<int> B) {
        vector<int> occupied=right;
        int r=0;
        for (int i=0; i<B.size(); i++)
        {
            if (occupied[edges[i].v] == 1) continue;
            occupied[edges[i].v] = 1;
            r++;
        }
        return r;
    }
    bool Exchangeable_Set(vector<int> S, vector<int> A, int b) {
        if (right[edges[b].v]==0) return true;
        for (int a:A)
            if (edges[a].v == edges[b].v)
                return true;
        return false;
    }
    void show() {
        cout << "RightMatching_Oracle" << endl;
    }
};


class Graphic_Oracle : public Oracle {
public:

    //LCT lct;
    int N;

    Graphic_Oracle(int _N/*, LCT _lct*/) {
        N = _N;
        //lct = _lct;
    }
    bool Exchangeable(int a, int b) {  // 4*lgN amortized
        /*
        lct[S] - a;
        if (lct[S]+b)
        {
            lct[S]-b;
            lct[S]+a; //safe but maybe include assertion
            return true;  
        }
        lct[S]+a;
        */
        return false;      
    }
    bool Free(int b) {  // 2*lgN amortized
        /*
        if (lct[S]+b)
        {
            lct[S]-b;
            return true;  
        }
        */
        return false;
    }
    int Rank(vector<int> B) { //TODO
        ORACLE_CALLS++;
        return 0;
    }
    bool Exchangeable_Set(vector<int> A, int b) {  //TODO
        return true;
    }
    void Update_State(vector<int> _S) {  //TODO
        //lct[S[i]]...
    }
};


class Uniform_Oracle : public Oracle {
public:

    int n;
    int r;
    vector<int> S;

    Uniform_Oracle(int _n, int _r) {
        n = _n;
        r = _r;
        ORACLE_CALLS=73;
    }

    bool Exchangeable(int a, int b) {
        ORACLE_CALLS++;
        return S.size() <= r;
    }
    bool Free(int b) {
        ORACLE_CALLS++;
        return S.size() < r;
    }
    int Rank(vector<int> B) {
        ORACLE_CALLS++;
        return S.size()+B.size() <= r ? S.size()+B.size() : r;
    }
    bool Exchangeable_Set(vector<int> A, int b) {
        ORACLE_CALLS++;
        return S.size() <= r;
    }
    void Update_State(vector<int> _S) {
        S=_S;
    }
    void show() {
        cout << "Uniform\n";
    }
};

class Power_Oracle : public Oracle {
public:

    int N;
    vector<int> S;

    Power_Oracle(int _N) {
        N = _N;
    }

    bool Exchangeable(int a, int b) {
        ORACLE_CALLS++;
        return true;
    }
    bool Free(int b) {
        ORACLE_CALLS++;
        return true;;
    }
    int Rank(vector<int> B) {
        ORACLE_CALLS++;
        return S.size()+B.size();
    }
    bool Exchangeable_Set(vector<int> A, int b) {
        ORACLE_CALLS++;
        return true;
    }
    void Update_State(vector<int> _S) {
        S=_S;
    }
};


/*int main() {
    Uniform_Oracle *uo = new Uniform_Oracle(5,3);
    uo->show();
    cout << uo->getOracleCalls() << endl;
    return 0;
}*/