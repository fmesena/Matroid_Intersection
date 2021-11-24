#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <assert.h>
#include <stdlib.h>

using namespace std;

struct Edge {
    int u;
    int v;
};

class Oracle {
public:
    int ORACLE_CALLS=0;
    virtual bool Exchangeable(vector<int> S, int a, int b) {
        cout << "Exchangeable virtual" << endl;
        return false;
    }
    virtual bool Free(vector<int> S, int b) {
        cout << "Free virtual" << endl;
        return false;
    }
    virtual int Rank(vector<int> X) {
        cout << "Rank virtual" << endl;
        return 0;
    }
    int getOracleCalls() {
        return ORACLE_CALLS;
    }
};


class LeftMatching_Oracle : public Oracle {
public:

    int N;
    vector<int> left;
    vector<Edge> edges;

    LeftMatching_Oracle(int _N, vector<Edge> _edges) {
        N = _N;
        edges = _edges;
    }
    
    //em média por cada augmentation pago r/2, r augmentations são feitas => additive factor to the complexity of O(r^2)
    void Update_State(vector<int> S) {
        left = {0};
        for (size_t i = 0; i < S.size(); ++i) left [edges[S[i]].u] = 1;
    }

    bool Exchangeable(vector<int> S, int a, int b) {
        return left[edges[b].u]==0 || edges[a].u == edges[b].u;
    }

    bool Free(vector<int> S, int free) {
        return left[edges[free].u]==0;
    }

    int Rank(vector<int> X) {
        vector<int> occupied(N,0);
        int res=0;
        for (int i=0; i<X.size(); i++)
        {
            if (occupied[edges[i].u] == 1) continue;
            res++;
        }
        return res;
    }

    /*
    bool Exchangeable_Set(vector<int> S, vector<int> A, int b) {  // O(|A|) :D
        if (left[edges[b].u]==0) return 1;
        for (int a:A) if (edges[a].u == edges[b].u) return 1;
        return 0;
    }*/

    void show() {
        cout << "LeftMatching_Oracle" << endl;
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
        right.clear();
        for (size_t i = 0; i < S.size(); ++i) right [edges[S[i]].u] = 1;
    }

    bool Exchangeable(vector<int> S, int a, int b) {
        return right[edges[b].v]==0 || edges[a].v == edges[b].v;
    }

    bool Free(vector<int> S, int free) {
        return right[edges[free].v]==0;
    }

    int Rank(vector<int> X) {
        vector<int> occupied(N,0);
        int res=0;
        for (int i=0; i<X.size(); i++)
        {
            if (occupied[edges[i].v - N/2] == 1) continue;
            res++;
        }
        return res;
    }

    /*
    bool Exchangeable_Set(vector<int> S, vector<int> A, int b) {  // O(|A|) :D
        if (left[edges[b].v]==0) return 1;
        for (int a:A) if (edges[a].v == edges[b].v) return 1;
        return 0;
    }*/

    void show() {
        cout << "RightMatching_Oracle" << endl;
    }
};


class Arborescence_Oracle : public Oracle {
public:

    //LCT lct;
    int N;

    Arborescence_Oracle(int _N/*, LCT _lct*/) {
        N = _N;
        //lct = _lct;
    }

    // 4*lgN amortized
    bool Exchangeable(vector<int> S, int a, int b) {
        /*
        lct[S] - a;
        if (lct[S]+b)
        {
            lct[S]-b;
            lct[S]+a; //clearly safe but maybe include assertion
            return true;  
        }
        lct[S]+a;
        */
        return false;      
    }

    // 2*lgN amortized
    bool Free(vector<int> S, int b) {
        /*
        if (lct[S]+b)
        {
            lct[S]-b;
            return true;  
        }
        */
        return false;
    }

    /* undefined
    int Rank(vector<int> X) {
    }
    */
};


class Rank_Oracle : public Oracle {
public:

    int H;

    RankOracle(int _H) {
        H = _H;
    }

    bool Oracle_Uniform(vector<int> S) { ORACLE_CALLS++; return S.size() <= H; }
    bool Exchangeable(vector<int> S, int a, int b) { ORACLE_CALLS++; return S.size() <= H; }
    bool Free(vector<int> S, int b) { ORACLE_CALLS++; return S.size() < H; }
    int  Rank(vector<int> X) { return X.size() <= H ? X.size() : H; }

};

class Uniform_Oracle : public Oracle {
public:
};

int main() {
    return 0;
}