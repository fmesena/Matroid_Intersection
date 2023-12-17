#include "Partition.h"
#include "../ds/Edge.h"
#include <cassert>

template<typename T>
Partition<T>::Partition(int N_, vector<T> gs, vector<vector<T>> category_, vector<int> capacity_):Oracle() {
    N        = N_;
    C        = (int) category_.size();
    ground_set   = gs;
    category_raw = category_;
    capacity = capacity_;
    category = vector<int>(N,-1); // elements with category -1 are assigned to a special category such that no element from there can be added
    cur_cap  = vector<int>(C,0);

    for (int i = 0; i < C; ++i)
    {
        for (size_t j = 0; j < category_raw[i].size(); ++j)
        {
            //build mapping
            int idx = 0;
            while (idx < N)
            {
                if (category_raw[i][j] == ground_set[idx]) //find the index of element category_raw[i][j] in ground_set, i.e. its identifier
                    category[idx] = i;
                idx++;
            }
        }
    }
}
template<typename T>
bool Partition<T>::Free(int b) {
    int category_of_bth = category[b];
    if (category_of_bth == -1) return false;
    return cur_cap[category_of_bth] < capacity[category_of_bth];
}
template<typename T>
bool Partition<T>::Exchangeable(int a, int b) {
    if (category[b] == -1) return false;
    return category[a] == category[b];
}
template<typename T>
bool Partition<T>::Exchangeable_Set(vector<int> A, int b) {
    if (category[b] == -1) return false;
    for (int a:A) {
        if (category[a] == category[b]) {
            return true;
        }
    }
    return false;
}
template<typename T>
int Partition<T>::Rank(vector<int> B) { // returns rank(B U S) - |S|. B is assumed to be contained in V\S. this returns the number of elements in B that can be freely added to S
    vector<int> taken=cur_cap;
    int r=0;
    for (size_t i=0; i < B.size(); i++)
    {
        int c = category[B[i]];
        if (taken[c] == capacity[c]) continue; // > cannot happen, thus the rest is when <
        if (c == -1) continue; // if the current element is in the null category
        taken[c]++;
        r++;
    }
    return r;
}
template<typename T>
void Partition<T>::Update_State(vector<int> S) {
    fill (cur_cap.begin(),cur_cap.end(),0);
    for (size_t i = 0; i < S.size(); ++i) 
        cur_cap [category[S[i]]]++; //DANGER. category[S[i]] might evaluate to -1 (i dont think so but still...)
}
template<typename T>
void Partition<T>::Temp_Update_State(int a, bool to_be_added) {
    if (to_be_added) {
        cur_cap [category[a]]++;
    }
    else {
        cur_cap [category[a]]--;
    }
}
template<typename T>
void Partition<T>::show() {
    cout << "Partition matroid with " << N << " elements and " << C << " categories" << endl;
    cout << "Capacity - Elements in the respective category";
    for (int i = 0; i < C; ++i)
    {
        cout << capacity[i] << " -";
        for (size_t j = 0; j < category_raw[i].size(); ++j)
        {
            cout << " " << "TODO";//category_raw[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
}

template class Partition<Edge>;
template class Partition<pair<int,int>>;