#ifndef PARTITION_H_
#define PARTITION_H_

#include "../oracles/Base.h"
#include "../ds/Edge.h"

template<typename T>
class Partition final : public Oracle {
public:
    int N;                                      // the number of elements in the ground set
    int C;                                      // the number of categories  

    std::vector<T> ground_set;                  //a vector of elements of type T. each element t \in T is represented internally by the index of t in the ground_set
    std::vector<std::vector<T>> category_raw;   //a matrix with m rows where category[i] is a vector with the elements in the i-th category. further, this matrix must induce a partition of the elements row-wise
    std::vector<int> capacity;                  //a vector where capacity[i] for 0 <= i < m denotes the capacity of the i-th category
    std::vector<int> category;                  //a vector where category[i]:=j means that the element t having index i in ground_set is in category j     
    std::vector<int> cur_cap;                   //cur_cap[i] denotes the current number of elements from category i in the independent set. invariant is that cur_cap[i] <= capacity[i] for all i

    Partition(int, std::vector<T>, std::vector<std::vector<T>>, std::vector<int>);
    bool Free(int);
    bool Exchangeable(int, int);
    bool Exchangeable_Set(std::vector<int>, int);
    int  Rank(std::vector<int>);
    void Update_State(std::vector<int>);
    void Temp_Update_State(int, bool);
    void show();
};

#endif