/** @file Header file for the Link Cut Tree ADT. */

#ifndef LINKCUTTREE_H
#define LINKCUTTREE_H


/*
#ifdef __cplusplus
extern "C" {
#endif
*/


//#include "graph.h"

typedef struct _LCT* LCT; /** Link-Cut tree abstract data type. */
/* typedef ... LCTAux;  */  /* Bad idea exposess splay tree internals. AVOID. */

/** @return Empty LCT, over vertexes labelled 1 to V */
LCT
allocLCT(int V /**< [in] Number of vertexes of the underlying graph */
         );

/** @return Empty LCT, over vertexes labelled 1 to V */
void
cleanLCT(LCT t /** [in] The tree to clean up */
         );

/** Makes a copy of the LCT tree. */
//LCT
//clone(LCT t /** [in] Original tree. */
//     );

/** Make an LCT from array info */
void
array2LCT(int v /** [in] number of vertexes */,
          int* A,
          LCT O /** [in] original tree to keep hash values */
          );

/** Tests to see if the trees are equivalent. */
int
fastEqQ(LCT t /** [in] */,
        LCT c /** [in] */
        );

/*uint64_t
getLCTHash(LCT t
);*/

#ifndef NDEBUG
/** VERY SLOW O(V^2), a series of asserts to verify if both LCTs represent the
    same tree. */
void
equivalentQ(LCT t /** [in] */,
            LCT c /** [in] */
            );

/** SLOW O(V),
 @return distance of represented trees. */
int
slowD(LCT t /** [in] */,
      LCT c /** [in] */
      );
#endif /* NDEBUG */

/** Free the LCT */
void
freeLCT(LCT t /** [in] */
        );

/** @return Number of vertexes in t. */
int
vertexNr(LCT t
         );

/** Prints out the current represented tree. */
void
displayRepTree(LCT t /** [in] */
               );

/** Produces a graphviz representation of the LCT to a file. */
void
showLCT(LCT t /** [in] */,
        char* fname /** [in] Name of file to store the representation. */
        );

/** Produces a graphviz representation of the represented Tree to a file. */
void
showRepTree(LCT t /** [in] */,
            char* fname /** [in] Name of file to store the representation. */
            );

/** Contain the path from root to v in a splay tree. */
void access(LCT t /**< [in] */,
       int v /**< [in] */
       );

/** Changes the root of the LCT to node u. Out of range values checked by
    asserts. */
void
reRoot(LCT t /**< [in] */,
       int v /**< [in] The node that will become the new root */
       );

/** @return 1 if there is a path linking u to v.
            0 otherwise */
int
linkedQ(LCT t /**< [in] */,
        int u /**< [in] */,
        int v /**< [in] */
        );

/** Add the edge (r,v) to the LCT. IMPORTANT: Sub-trees containing r and v
    must not be linked. Therefore linkedQ(t, r, v) must be false. */
void link(LCT t /**< [in] */,
     int r /**< [in] Becomes the root of its sub-tree, before linking. */,
     int v /**< [in] */
     );

/** Contains the cycle between u and v in a splay tree. IMPORANT: this is
    only possible if linkedQ(t, u, v) is true, otherwise results are
    inconsistent.

    In general this function could be called pathify. I use only on cycles,
    but coding is general.

    @return the size of the resulting cycle. Number of vertexes involved. */
int
cycle(LCT t /**< [in] */,
      int u /**< [in] */,
      int v /**< [in] */
      );

/** Obtains the size of an existing path, where a certain vertex exists.

    @return the size of the existing path.
 */
int
sizePath(LCT t /**< [in] */,
         int u /**< [in] */
         );
/** @return The position of v in its tree. */
int
position(LCT t /** [in] */,
         int v /** [in] */
         );

/** @return the i-th vertex of the tree containing vertex v. Vertex 1 is
    the root. If out of bounds returns 0. */
int
selectAux(LCT t /**< [in] */,
          int v /**< [in] */,
          int i /**< [in] The index of the edge, starting at 1 */
          );

int selectRootAux(LCT t /**< [in] */,
                  int v /**< [in] */);

/** @return The next element in depth order. If out of bounds returns 0. */
int
successor(LCT t /**< [in] */,
          int v
          );

/** @return The previous element in depth order. If out of bounds returns 0. */
int
predecessor(LCT t /**< [in] */,
            int v
            );

/** @return 0 if (u,v) is not on the LCT
            1 otherwise */
int
edgeQ(LCT t /**< [in] */,
      int u /**< [in] */,
      int v /**< [in] */
      );

/** IMPORTANT: before this function can be applied, the aux tree containing
    (u,v) must be selected, with a cycle call. Works across sub-trees.
    @return 0 if (u,v) is not on this Aux tree.
            1 otherwise. */
int
edgeQAux(LCT t /**< [in] */,
         int s /**< [in] vertex from the aux tree. */,
         int u /**< [in] */,
         int v /**< [in] */
         );

/** Remove the edge (u,v) from the LCT, from whatever aux tree that
    contains it. Does nothing if edge is not there. */
void
cut(LCT t /**< [in] */,
    int u /**< [in] Becomes the root of its sub-tree. */,
    int v /**< [in] */
    );

/* This final part of the API is necessary to support coupling
    simulation. */

/* This function is even harder to compute than marking. Given two cycles
   it returns an edge that is in t and its path but not in the path a.

   IMPORTANT: the vertexes of both trees are shared. */
void
selectExcluded(LCT t /** [in] The tree to choose from */,
               int tp /** [in] The node indicating the path in t */,
               int cp /** [in] size of path in t. */,
               LCT a  /** [in] The tree to avoid. */,
               int ta /** [in] The node indicating the path in a */,
               int* ou /** [out] a vertex of the chosen edge. */,
               int* ov /** [out] another vertex of the chosen edge. */
               );

/* Marking starts here */

/** If the edge (u,v) exists in t then it becomes Marked. */
void
mark(LCT t /** [in] */,
     int u /** [in] */,
     int v /** [in] */
     );

/** @return u from an edge (u,u') marked. Then u' can be obtained by
    successor, which returns 0 if the edge does not exist. */
int
selectMarkedAux(LCT t /** [in] */,
                LCT m /** [in] Marking tree */,
                int start /** [in] path indicating vertex */,
                int end /** [in] path indicating vertex */
                );

/** @return u from an edge (u,u') not marked. Then u' can be obtained by
    successor, which returns 0 if the edge does not exist. */
int
selectUnmarkedAux(LCT t /** [in] */,
                  LCT m /** [in] Marking tree */,
                  int start /** [in] path indicating vertex */,
                  int end /** [in] path indicating vertex */
                  );

/** @return u from an edge (u,u') not marked. If there is more than one
    such node it returns -1. */
int
singularUnmarkedAux(LCT t /** [in] */,
                    LCT m /** [in] Marking tree */,
                    int start /** [in] path indicating vertex */,
                    int end /** [in] path indicating vertex */
                    );

/*
#ifdef __cplusplus
} // end extern "C"
#endif
*/

#endif /* LINKCUTTREE_H */


