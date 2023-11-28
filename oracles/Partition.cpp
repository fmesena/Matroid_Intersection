#include "Partition.h"

Partition::Partition(int N_, vector<vector<int>> category_, vector<int> capacity_):Oracle() {
    N = N_;
    category_raw = category_;
    capacity     = capacity_;
    
    category     = vector<int>(N);
    M            = (int) category_raw.size();
    cur_cap      = vector<int>(M,0);

    cout << "\nprinting...\n";
    int idx = 0;
    for (int i = 0; i < M; ++i) {
        for (size_t j = 0; j < category_raw[i].size(); ++j) {
            category[idx++] = i; //make sure that N = \sum |C_i|
            //cout << i << " " << j << " " << category[idx-1] << " ";
        }
        //cout << endl;
    }
    for (int i = 0; i < N; ++i)
    {
        cout << category[i] << " ";
    }
}
bool Partition::Free(int b) {
    int category_of_bth = category[b];
    return cur_cap[category_of_bth] < capacity[category_of_bth];
}
bool Partition::Exchangeable(int a, int b) {
    return category[a] == category[b];
}
bool Partition::Exchangeable_Set(vector<int> A, int b) {
    for (int a:A) {
        if (category[a] == category[b]) {
            return true;
        }
    }
    return false;
}
int Partition::Rank(vector<int> B) {//returns rank(B U S) - |S|. B is assumed to be contained in V\S. this returns the number of elements in B that can be freely added to S
    vector<int> taken=cur_cap;
    int r=0;
    for (size_t i=0; i < B.size(); i++)
    {
        int c = category[B[i]];
        if (taken[c] == capacity[c]) continue; // > cannot happen, thus the rest is when <
        taken[c]++;
        r++;
    }
    return r;
}
void Partition::Update_State(vector<int> S) {
    fill (cur_cap.begin(),cur_cap.end(),0);
    for (size_t i = 0; i < S.size(); ++i)
        cur_cap [category[S[i]]]++;
}
void Partition::Temp_Update_State(int a, bool to_be_added) {
    if (to_be_added) {
        cur_cap [category[a]]++;
    }
    else {
        cur_cap [category[a]]--;
    }
}
void Partition::show() {
    cout << "Partition matroid with " << N << " elements" << endl;
    cout << "Capacity - Elements in the respective category";
    for (int i = 0; i < M; ++i)
    {
        cout << capacity[i] << " -";
        for (size_t j = 0; j < category_raw[i].size(); ++j)
        {
            cout << " " << category_raw[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
}