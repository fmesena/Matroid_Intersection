#include <vector>

#ifndef EDGE_H_
#define EDGE_H_

struct Edge {
    int u;
    int v;

    bool operator==(const Edge& e) const
    {
        return (u == e.u && v == e.v);
    }
    bool operator!=(const Edge& e) const
    {
        return (u != e.u || v != e.v);
    }
    /*ostream& print(ostream& os) {
        return os << "(" << u << ", " << v << ")" << endl;
    }*/
};

/*ostream& operator<<(ostream& os, Edge& e) {
    return e.print(os);
}*/

#endif