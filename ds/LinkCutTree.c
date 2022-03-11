#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "LinkCutTree.h"

/*** file scope macro definitions *************************/

/*** file scope typedefs  *********************************/

/*int assert(int c)
{
    if(!c)
    {

    }
}*/

typedef struct _LCT* LCTAux; /** Used to represent splay trees. */

struct _LCT { /** This structure is used to represent a node in the LCT. */
  //uint64_t h; /** h value for node number */
  //uint64_t Hf; /** hash value for the sequence on the sub-tree (forward). */
  //uint64_t Hb; /** hash value for the sequence on the sub-tree (forward). */
  LCT left; /** Child */
  LCT right; /** Child */
  LCT* hook; /** Sort of parent pointer. Points directly to the position
                 that points back to this node. If NULL means that it is the
                 root of its sub-tree. It may point to a position that does
                 not point back, in wich case it is an accross Aux parent
                 pointer. */
  int sum; /** For now the size of this sub-tree. Can be changed to sum of
               weights. The value is assume to be >= 0. Negative values are
               used to indicate that left and right pointers should be
               swapped on the sub-tree. */
};

/*** global variables (externs or externables) ************/

/*** file scope variables (static) ************************/

static int ct = 1;
static char sstat[100];

/*** file scope functions declarations (static) ***********/

/** Resets invariant properties of v */
static void
resetNode(LCTAux v /**< [in] */
          );

/** @return 1 if this struct is flipped, 0 otherwise */
static int
flippedQ(LCTAux v /**< [in] */
         );

/** Changes a logical bit in the struct so that it behaves as flipped */
static void
flip(LCTAux v /**< [in] */
         );

/** Physically changes struct so that flip bit can be disabled. */
static void
unflip(LCTAux v /**< [in] */
     );

/** @return The size of the aux sub-tree rooted at v. */
static int
sizeAux(LCTAux v /**< [in] */
        );

/** IMPORTANT: This function only applies if v is the root of a splay tree.
    Changes the lower pointer of v to to. */
static void
setLower(LCTAux v /**< [in] Must be root of a splay tree. */,
         LCTAux to /**< [in] */
         );

/** @return 0 if s is NOT the root of the splay tree
    1 otherwise. */
static int
auxRootQ(LCTAux s /**< [in] */
         );

/** IMPORTANT: Assumes that the node is not flipped. Rotates node v, upwards. */
static void
rotate(LCT t /**< [in] */,
       LCTAux v /**< [in] */
       );

/** Splays u within its aux tree*/
static void
splay(LCT t /**< [in] */,
      int v /**< [in] */
      );

/** @return r, so that p points inside t[r]. */
static int
ptrToIndex(LCT t /**< [in] */,
           void *p /**< [in] */
           );

/** @return The aux parent of s. */
static int
auxParent(LCT t /**< [in] */,
          LCTAux s /**< [in] */
          );

/** @return rotated __X */
static __inline unsigned long long
__attribute__((__gnu_inline__, __always_inline__, __artificial__))
__rolq (unsigned long long __X, int __C)
  ;

/** @return The hash code */
static uint64_t
getHashPr(LCTAux v
          );

#ifndef NDEBUG
/** Asserts to test edgeQAux */
static void
edgeQAuxAssert(LCT t /**< [in] */,
               int s /**< [in] vertex from the aux tree. */,
               int u /**< [in] */,
               int v /**< [in] */,
               int bool /** [in] The value obtained */
               );

static void
sizeAssert(LCTAux v /**< [in] */
           );

#endif /* NDEBUG */

/*** implementation ***************************************/

static __inline unsigned long long
__attribute__((__gnu_inline__, __always_inline__, __artificial__))
__rolq (unsigned long long __X, int __C)

{
  __C &= 0x0F;
  return (__X << __C) | (__X >> (64 - __C));
}


static void
resetNode(LCTAux v /**< [in] */
          )
{
  int b;

  b = 1;
  if (flippedQ(v))
    b = -1;

  v->sum = 0;

  if (NULL != v->left) {
    v->sum += sizeAux(v->left);
    v->left->hook = &(v->left);
  }

  v->sum++; /* Adding the node */

  if (NULL != v->right) {
    v->sum += sizeAux(v->right);
    v->right->hook = &(v->right);
  }

  v->sum *= b; /* Flip back */

  /* Deal with Hash values */
  /* Forward Hash */
  /*v->Hf = 0;
  if (NULL != v->left) {
    v->Hf = getHashPr(v->left);
  }*/

/*  v->Hf = __rolq(v->Hf, 1);
  v->Hf ^= v->h;

  if (NULL != v->right) {
    v->Hf = __rolq(v->Hf, sizeAux(v->right));
    v->Hf ^= getHashPr(v->right);
  }
*/
  /* Backward Hash */
/*  v->Hb = 0;
  if (NULL != v->right) {
    flip(v->right);
  //  v->Hb = getHashPr(v->right);
    flip(v->right);
  }
*/
/*  v->Hb = __rolq(v->Hb, 1);
  v->Hb ^= v->h;

  if (NULL != v->left) {
    v->Hb = __rolq(v->Hb, sizeAux(v->left));
    flip(v->left);
    v->Hb ^= getHashPr(v->left);
    flip(v->left);
  }*/
}

static int
flippedQ(LCTAux v)
{
  return 0 > v->sum;
}

static void
flip(LCTAux v
     )
{
  v->sum *= -1;
}

static void
unflip(LCTAux v
     )
{
  LCTAux t;
  uint64_t ti;

  if (flippedQ(v))
    {
      flip(v);
      t = v->left;
      v->left = v->right;
      v->right = t;

      if (NULL != v->left){
        flip(v->left);
        v->left->hook = &(v->left);
      }
      if (NULL != v->right){
        flip(v->right);
        v->right->hook = &(v->right);
      }

      /* resetNode(v); */
      /*ti = v->Hf;
      v->Hf = v->Hb;
      v->Hb = ti;*/
    }
}

static int
sizeAux(LCTAux v
        )
{
  int r;

  r = 0;
  if (NULL != v)
    r = abs(v->sum);

  return r;
}

static void
setLower(LCTAux v,
         LCTAux to
         )
{
  unflip(v); /* Now branches are not flipped */
  v->sum -= sizeAux(v->right);
  v->right = to;
  v->sum += sizeAux(v->right);

  if (NULL != v->right)
    v->right->hook = &(v->right);

#ifndef NDEBUG
  sizeAssert(v);
#endif /* NDEBUG */

  /* resetNode(v); */
  /* Lazy Hash computation */
//  v->Hf = 0; /* Use this value to mean non-computed Hash */
//  v->Hb = 0; /* Use this value to mean non-computed Hash */
}

static int
auxRootQ(LCTAux s
         )
{
  int r;
  r = 0;
  if(NULL == s->hook)
    r = 1;
  else if (s != *(s->hook))
    r = 1;

  return r;
}

static void
rotate(LCT t,
       LCTAux v
       )
{
  int p; /* Parent node */
  int vs; /* Temporary v size */

  assert(!flippedQ(v) && "Trying to rotate flipped node.");

  if (!auxRootQ(v)) { /* Rotation is possible */
    p = auxParent(t, v);
    assert(!flippedQ(&t[p]) && "Trying to rotate flipped node.");

    vs = sizeAux(v);
    /* Reseting copies values for v */
    v->sum = t[p].sum;
    //v->Hf  = t[p].Hf;
    //v->Hb  = t[p].Hb;

    /* Reseting updates values for v */
    t[p].sum -= vs;
    /* Lazy Hash computation */
    //t[p].Hf = 0; /* Use this value to mean non-computed Hash */
    //t[p].Hb = 0; /* Use this value to mean non-computed Hash */

    if (v->hook == &(t[p].left)) {
      t[p].sum += sizeAux(v->right);

      *(v->hook) = v->right;
      if(NULL != *(v->hook))
        (*(v->hook))->hook = v->hook;

      v->hook = t[p].hook;
      if (NULL != v->hook && (LCT)&t[p] == *v->hook)
        *(v->hook) = v;

      v->right = &(t[p]);
      v->right->hook = &(v->right);
    } else {
      t[p].sum += sizeAux(v->left);

      *(v->hook) = v->left;
      if(NULL != *(v->hook))
        (*(v->hook))->hook = v->hook;

      v->hook = t[p].hook;
      if (NULL != v->hook && (LCT)&t[p] == *v->hook)
        *(v->hook) = v;

      v->left = &(t[p]);
      v->left->hook = &(v->left);
    }

    assert(0 != t[p].sum && "Size set to 0");
    assert(0 != v->sum && "Size set to 0");

    /* resetNode(&t[p]); */
    /* resetNode(v); */
  }

#ifndef NDEBUG
  sizeAssert(v);
#endif /* NDEBUG */
}

static void
splay(LCT t,
      int v
      )
{
  int p; /* Parent node */
  int g; /* Grand parent node */
  intptr_t pt; /* Integer pointer */

  while(0 == auxRootQ(&t[v])) { /* Not at the root of a splay tree */
    p = auxParent(t, &t[v]);

    if (0 == auxRootQ(&t[p])) { /* Then v has a grand parent */
      g = auxParent(t, &t[p]);
      unflip(&t[g]);
      unflip(&t[p]);
      unflip(&t[v]);

      pt = (intptr_t) t[v].hook;
      pt -= (intptr_t) t[p].hook;
      pt %= sizeof(struct _LCT);

      if (0 == pt) /* Zig Zig case */
        rotate(t, &t[p]);
      else /* Zig Zag case */
        rotate(t, &t[v]);
    }
    unflip(&t[p]); /* Zig case precaution */
    unflip(&t[v]);
    rotate(t, &t[v]); /* Both cases and Zig */
  }

#ifndef NDEBUG
  sizeAssert(&t[v]);
#endif /* NDEBUG */
}

static int
ptrToIndex(LCT t,
           void *p
           )
{
  int r;
  intptr_t pr; /* Integer pointer */
  r = 0;

  if (NULL != p) {
    pr = (intptr_t) p;
    pr -= (intptr_t) t;
    pr /= sizeof(struct _LCT);
    r = (int) pr;
  }

  return r;
}

static int
auxParent(LCT t,
          LCTAux s
          )
{
  return ptrToIndex(t, s->hook);
}

#ifndef NDEBUG
void
edgeQAuxAssert(LCT t,
               int s,
               int u,
               int v,
               int bool
               )
{
  int i;
  int r = 0;
  int n;

  i = selectAux(t, s, 1);
  while(i > 0 && 0 == r) {
    n = successor(t, i);
    if(u == i && v == n)
      r = 1;
    if(u == n && v == i)
      r = 1;
    i = n;
  }

  assert(bool == r);
}

static void
sizeAssert(LCTAux v /**< [in] */)
{
  int r;

  r = 0;
  if (NULL != v){
    r = 1;

    r += sizeAux(v->left);
    r += sizeAux(v->right);

    assert(r == sizeAux(v) && "Size error.\n");

    if (NULL != v->left)
      sizeAssert(v->left);
    if (NULL != v->right)
      sizeAssert(v->right);
  }
}

#endif /* NDEBUG */

/* End of Static Functions */

LCT
allocLCT(int V /**< [in] Number of vertexes of the underlying graph */
         )
{
  LCT t;
  int i;

  t = (LCT) malloc((V+1)*sizeof(struct _LCT));
  *(int*)t = V; /* Store number of vertexes */

  /*i = V;
  while(0 < i){
    arc4random_buf(&(t[i].h), sizeof(uint64_t));
    i--;
  }*/

  cleanLCT(t);

  return t;
}

void
cleanLCT(LCT t /** [in] The tree to clean up */
         )
{
  int i;
  int V;
  V = vertexNr(t);

  i = V;
  while(0 < i){
    t[i].left = NULL;
    t[i].right = NULL;
    t[i].hook = NULL;
    t[i].sum = 1;
    //t[i].Hf = t[i].h;
    //t[i].Hb = t[i].h;
    i--;
  }
}

int
fastEqQ(LCT t,
        LCT c
        )
{
  int V;
  int i;
  int p;
  int m;

  V = vertexNr(t);

  m = 1;
  i = 1;
  while(i <= V && 1 == m) {
    if (0 != successor(t, i) && !edgeQ(c, i, successor(t, i)))
      m = 0;
    if (1 == m && auxRootQ(&t[i]) && NULL != t[i].hook) {
      p = auxParent(t, &t[i]);
      if (!edgeQ(c, selectAux(t, i, 1), p))
        m = 0;
    }
    i++;
  }

  return m;
}

/*uint64_t
getLCTHash(LCT t /
           )
{
  uint64_t r;
  int i;

  r = 0;

  reRoot(t, 1);

  i = 2;
  while(i <= vertexNr(t)){
    access(t, i);
    i++;
  }

  i = 2;
  while(i <= vertexNr(t)){
    splay(t, i);
    unflip(&t[i]);

    if(NULL == t[i].right){
      r = __rolq(r, sizeAux(&t[i]));
      r ^= getHashPr(&t[i]);
    }

    if(NULL == t[i].left && 0 != auxParent(t, &t[i])){
      r = __rolq(r, 1);
      r ^= t[i].h;
      r = __rolq(r, 1);
      r ^= t[auxParent(t, &t[i])].h;
    }

    i++;
  }

  return r;
}*/

#ifndef NDEBUG
void
equivalentQ(LCT t,
            LCT c
            )
{
  int i;
  int j;

  assert(vertexNr(t) == vertexNr(c));

  i = 1;
  while(vertexNr(t) >= i) {
    j = 1;
    while(vertexNr(t) >= j) {
      assert(edgeQ(t, i, j) == edgeQ(t, i, j));
      assert(linkedQ(t, i, j) == linkedQ(t, i, j));
      j++;
    }
    i++;
  }
}

int
slowD(LCT t,
      LCT c
      )
{
  int V;
  int i;
  int p;
  int d;

  d = 0;
  V = vertexNr(t);

  i = 1;
  while(i <= V) {
    if (0 != successor(t, i) && !edgeQ(c, i, successor(t, i)))
      d++;

    if (auxRootQ(&t[i]) && NULL != t[i].hook) {
      p = auxParent(t, &t[i]);
      if (!edgeQ(c, selectAux(t, i, 1), p))
        d++;
    }
    i++;
  }

  return d;
}
#endif /* NDEBUG */


/*
LCT
clone(LCT t
      )
{
  LCT c; //The clone
  int i;
  int V;
  ptrdiff_t delta;

  V = vertexNr(t);
  c = (LCT) malloc((V+1)*sizeof(struct LCT));
  *(int*)c = V; //Store number of vertexes

  delta = (ptrdiff_t)c - (ptrdiff_t)t;

  i = V;
  while(0 < i) {
      c[i].left = NULL;
      if(NULL != t[i].left)
        c[i].left = (LCT)(delta + (ptrdiff_t)t[i].left);
      c[i].right = NULL;
      if(NULL != t[i].right)
        c[i].right = (LCT)(delta + (ptrdiff_t)t[i].right);
      c[i].hook = NULL;
      if(NULL != t[i].hook)
        c[i].hook = (LCT*)(delta + (ptrdiff_t)t[i].hook);

      c[i].sum = t[i].sum;
      //c[i].h = t[i].h;
      //c[i].Hf = t[i].Hf;
      //c[i].Hb = t[i].Hb;
      i--;
    }

  return c;
}*/

void
array2LCT(int v,
          int* A,
          LCT O
          )
{
  int i; /* counter */

  cleanLCT(O);
  i = 1;
  while(i <= v){
    if(0 < A[i])
      link(O, i, A[i]);
    i++;
  }
}

int
vertexNr(LCT t
         )
{
  return *(int*)t;
}

void
freeLCT(LCT t)
{
  free(t);
}

void
displayRepTree(LCT t
               )
{
  int V;
  int i;
  int p;

  V = vertexNr(t);

  i = 1;
  while(i <= V)
    {
      if (0 != successor(t, i))
        printf( "%d %d\n", i, successor(t, i));
      if (auxRootQ(&t[i]) && NULL != t[i].hook) {
        p = auxParent(t, &t[i]);
        printf( "%d %d\n", selectAux(t, i, 1), p);
      }
      i++;
    }

  printf("\n");

}

void
showLCT(LCT t,
        char* fname
        )
{
  int V;
  FILE* f;
  int i;
  int p;

  fprintf(stderr, "%s.pdf\n", fname);

  V = vertexNr(t);

  f = fopen(fname, "w");
  if (NULL != f)
    {
      fprintf(f, "digraph %s { \n", fname);
      fprintf(f, "graph [ rankdir = \"LR\" ];\n");
      fprintf(f, "node [ fontsize = \"16\" shape = \"record\" ];\n");
      fprintf(f, "edge [ ];\n");
      fprintf(f, "label =  \"%s\" \n", fname);

      i = 1;
      while(i <= V)
        {
          fprintf(f, "\"node%d\" ", i);
          fprintf(f, "[ label = \"");
          fprintf(f, "I: %d |", i);
          fprintf(f, "<M> M: %p |", (void *)&t[i]);
          if (NULL == t[i].left)
            fprintf(f, "<L> L: NULL |");
          else
            fprintf(f, "<L> L: |");

          if (NULL == t[i].right)
            fprintf(f, "<R> R: NULL |");
          else
            fprintf(f, "<R> R: |");

          if (auxRootQ(&t[i]))
            fprintf(f, " <H> H: |");
          else
            fprintf(f, " <H> H: %d |", auxParent(t, &t[i]));

          fprintf(f, " <F> F: %d |", flippedQ(&t[i]));
          fprintf(f, " <S> S: %d ", sizeAux(&t[i]));
          fprintf(f, "\" ]\n");
          i++;
        }

      i = 1;
      while(i <= V)
        {
          if (NULL != t[i].left)
            fprintf(f, "\"node%d\":L -> \"node%d\":M;\n", i,
                    ptrToIndex(t, t[i].left));
          if (NULL != t[i].right)
            fprintf(f, "\"node%d\":R -> \"node%d\":M;\n", i,
                    ptrToIndex(t, t[i].right));

          if (auxRootQ(&t[i]) && NULL != t[i].hook) {
              p = auxParent(t, &t[i]);
              if (t[i].hook == &(t[p].left))
                fprintf(f, "\"node%d\":H -> \"node%d\":L[style=dashed];\n", i, p);
              if (t[i].hook == &(t[p].right))
                fprintf(f, "\"node%d\":H -> \"node%d\":R[style=dashed];\n", i, p);
            }
          i++;
        }

      fprintf(f, "}\n");
      fclose(f);
    }
}

void
showRepTree(LCT t,char* fname)
{
  int V;
  FILE* f;
  int i;
  int p;

  if(NULL == fname)
    fname = sstat;

  if(0 == strcmp("Begin", fname)){
    sprintf(sstat, "tree%.4d_BeforePathStep_dot", ct++);
    fname = sstat;
  }

  V = vertexNr(t);

  printf("%s.ps\n", fname);

  f = fopen(fname, "w");
  if (NULL != f)
    {
      fprintf(f, "digraph %s { \n", "lct");
      fprintf(f, "graph [ rankdir = \"LR\" ];\n");
      fprintf(f, "node [ fontsize = \"16\" shape = \"record\" ];\n");

      fprintf(f, "edge [ ];\n");

      fprintf(f, "label =  \"%s\" \n", fname);

      i = 1;
      while(i <= V)
        {
          fprintf(f, "\"node%d\" ", i);
          fprintf(f, "[ label = \"");
          fprintf(f, " I: %d ", i);
          fprintf(f, "\" ]\n");
          i++;
        }

      i = 1;
      while(i <= V)
        {
          if (0 != successor(t, i))
            fprintf(f, "\"node%d\" -> \"node%d\";\n",
                    successor(t, i),i);
          if (auxRootQ(&t[i]) && NULL != t[i].hook) {
              p = auxParent(t, &t[i]);
              fprintf(f, "\"node%d\" -> \"node%d\" [style=dashed];\n",
                      selectAux(t, i, 1), p);
            }
          i++;
        }

      fprintf(f, "}\n");
        printf("}\n");
      fclose(f);
    }
}

void access(LCT t, int v)
{
  int w;

  splay(t, v);
  setLower(&t[v], NULL);

  while(NULL != t[v].hook) {
    w = auxParent(t, &t[v]);
    splay(t, w);
    setLower(&t[w], &t[v]);
    splay(t, v);
  }

#ifndef NDEBUG
  sizeAssert(&t[v]);
#endif /* NDEBUG */

  assert(flippedQ(&t[v]) || (NULL == t[v].right && "Failed access invariant."));
  assert(!flippedQ(&t[v]) || (NULL == t[v].left && "Failed access invariant."));
}

void
reRoot(LCT t,
       int v
       )
{
  access(t, v);
  flip(&t[v]);
  access(t, v);

#ifndef NDEBUG
  sizeAssert(&t[v]);
#endif /* NDEBUG */
}

int
linkedQ(LCT t,
       int u,
       int v
       )
{
    access(t,u);
    int root = selectRootAux(t,u);

  cycle(t, u, v);
  int r = selectAux(t, v, 1) == u;

  reRoot(t,root);
  return r;
}

void link(LCT t, int r, int v)
{
  /* sprintf(sstat, "tree%.4d_Before_Link_%d_%d_dot", ct++, r, v); */
  /* showRepTree(t, NULL); */

  assert(r != v && "Cutting no edge");
  assert(!linkedQ(t, r, v) && "Link failed. Trees already linked.");
  reRoot(t, r);
  access(t, v);
  setLower(&t[v], &t[r]);

  /* sprintf(sstat, "tree%.4d_After_Link_%d_%d_dot", ct++, r, v); */
  /* showRepTree(t, NULL); */

#ifndef NDEBUG
  sizeAssert(&t[v]);
#endif /* NDEBUG */
}

int
cycle(LCT t,
      int u,
      int v
      )
{
  reRoot(t, u);
  access(t, v);
  return sizeAux(&t[v]);
}

int
sizePath(LCT t /**< [in] */,
      int u /**< [in] */
      )
{
  splay(t, u);
  return sizeAux(&t[u]);
}

/*static uint64_t
getHashPr(LCTAux v
        )
{
  uint64_t r;

  if (!flippedQ(v)){
    if (0 == v->Hf){
      if (NULL != v->left)
        v->Hf = getHashPr(v->left);

      v->Hf = __rolq(v->Hf, 1);
      v->Hf ^= v->h;

      if (NULL != v->right) {
        v->Hf = __rolq(v->Hf, sizeAux(v->right));
        v->Hf ^= getHashPr(v->right);
      }
    }
    r = v->Hf;
  } else {
    if (0 == v->Hb){
      if (NULL != v->right) {
        flip(v->right);
        v->Hb = getHashPr(v->right);
        flip(v->right);
      }

      v->Hb = __rolq(v->Hb, 1);
      v->Hb ^= v->h;

      if (NULL != v->left) {
        v->Hb = __rolq(v->Hb, sizeAux(v->left));
        flip(v->left);
        v->Hb ^= getHashPr(v->left);
        flip(v->left);
      }
    }
    r = v->Hb;
  }

  assert(0 != r && "Non-Critical Assert, gives probability of hashing to 0.");

  return r;
}

uint64_t
getHash(LCT t,
        int u
        )
{
  assert(NULL != t && "Calling Hash from NULL pointer");

  return getHashPr(&(t[u]));
}

uint64_t
getCycleHash(LCT t,
             int u,
             int v
             )
{
  int h;

  if(v < u){
    h = v;
    v = u;
    u = h;
  }

  cycle(t, u, v);

  return getHash(t, v);
}

/**uint64_t
getCycleHashByE(LCT t,
                edge e
                )
{
  return getCycleHash(t, e[0], e[1]);
}**/

int
position(LCT t,
         int v
         )
{
  int r;
  r = 1;

  splay(t, v);
  unflip(&t[v]);
  if (NULL != t[v].left)
    r += sizeAux(t[v].left);

  return r;
}

int selectRootAux(LCT t /**< [in] */,
                  int v /**< [in] */)
{
    LCTAux s;

    splay(t, v);
    s = &t[v];

    return selectAux(t,v,1);
}

int
selectAux(LCT t /**< [in] */,
          int v /**< [in] */,
          int i /**< [in] The index of the edge, starting at 1 */
          )
{
  int r;
  LCTAux s;
  r = 0; /* Default for out of bounds */

  splay(t, v);
  s = &t[v];
  if (0 < i && sizeAux(s) >= i) {
    i--; /* Converted to how many elements do you need on the left */
    unflip(s);
    while (sizeAux(s->left) != i) {
      if (sizeAux(s->left) > i)
        s = s->left;
      else {
        i -= sizeAux(s);
        s = s->right;
        i += sizeAux(s);
      }
      unflip(s);
    }
    r = (int) (s-t);
    splay(t, r);
  }

  return r;
}

int
successor(LCT t,
          int v
          )
{
  splay(t, v);
  unflip(&t[v]);
  return selectAux(t, v, 2 + sizeAux(t[v].left));
}

int
predecessor(LCT t,
            int v
            )
{
  splay(t, v);
  unflip(&t[v]);
  return selectAux(t, v, sizeAux(t[v].left));
}

int
edgeQ(LCT t,
      int u,
      int v
      )
{
  int r;

  r = 0;
  if (u != v)
    {
      access(t,u);
      int root = selectRootAux(t,u);
      cycle(t, u, v);

      unflip(&t[v]);
      if(t[v].left == &t[u]){
        unflip(t[v].left);
        r = (NULL == t[v].left->right);
      }
      reRoot(t,root);
      /* r = (predecessor(t, v) == u); */
    }
  return r;
}

int
edgeQAux(LCT t,
         int s,
         int u,
         int v
         )
{
  int r;
  r = 0;

  splay(t, s);

  if (successor(t, u) == v)
    r = 1;
  else if (predecessor(t, u) == v)
    r = 1;

  if(1 == r && s != u && s != v)
    r = !auxRootQ(&t[s]);

#ifndef NDEBUG
  edgeQAuxAssert(t, s, u, v, r);
#endif /* NDEBUG */
  return r;
}

void
cut(LCT t,
    int u,
    int v
    )
{
  /* sprintf(sstat, "tree%.4d_Before_Cut_%d_%d_dot", ct++, u, v); */
  /* showRepTree(t, NULL); */

    assert(u != v && "Cutting no edge");
    assert(edgeQ(t, u, v) && "Cutting edges that are not there");
    if(1 == edgeQ(t, u, v)) {
        access(t, u);
        int root = selectRootAux(t, u);

        cycle(t, u, v);
        unflip(&t[v]);
        assert(NULL != t[v].left && "Cut guard.");

        t[v].sum -= sizeAux(t[v].left);

        assert(0 != t[v].sum && "Size set to 0");

        t[v].left->hook = NULL;
        t[v].left = NULL;

        assert(sizeAux(&t[v]) > sizeAux(t[v].right) && "Size order");
        assert(sizeAux(&t[v]) > sizeAux(t[v].left) && "Size order");

        reRoot(t, root);


    /* sprintf(sstat, "tree%.4d_After_Cut_%d_%d_dot", ct++, u, v); */
    /* showRepTree(t, NULL); */

    /* resetNode(&t[v]); */
    /* Lazy hash computation */
  //  t[v].Hf = 0; /* Use this value to mean non-computed Hash */
  //  t[v].Hb = 0; /* Use this value to mean non-computed Hash */
  }
}

void
selectExcluded(LCT t,
               int tp,
               int cp,
               LCT a,
               int ta,
               int* ou,
               int* ov
               )
{
  int idx;

  while(edgeQAux(a, ta, *ou, *ov)) {
    idx = rand()%(cp-1)+1; //arc4random_uniform(cp-1)+1;
    *ou = selectAux(t, tp, idx);
    *ov = successor(t, *ou);
  }
}

void
mark(LCT t,
     int u,
     int v
     )
{
  if(edgeQ(t, u, v)) { /* Edge is on the tree, so mark it */
    splay(t, v);
/* #warning TODO: Manage bitmasks here. */
  }
}

int
selectMarkedAux(LCT t,
                LCT m, /* Marking tree */
                int start,
                int end
                )
{
  int j; /* Simple counter */
  int ou; /* First vertex */
  int ov; /* Next vertex */
  int c;

  c = cycle(t, start, end);
  j = 0;
  do {
    j++;
    ou = selectAux(t, start, j);
    ov = successor(t, ou);
  } while(j < c && !edgeQ(m, ou, ov));

  return ou;
}

int
selectUnmarkedAux(LCT t,
                  LCT m, /* Marking tree */
                  int start,
                  int end
                  )
{
  int j; /* Simple counter */
  int ou; /* First vertex */
  int ov; /* Next vertex */
  int c;

  c = cycle(t, start, end);
  j = 0;
  do {
    j++;
    ou = selectAux(t, start, j);
    ov = successor(t, ou);
  } while (j < c && edgeQ(m, ou, ov));

  return ou;
}

int
singularUnmarkedAux(LCT t,
                    LCT m, /* Marking tree */
                    int start,
                    int end
                    )
{
  int j; /* Simple counter */
  int ou; /* First vertex */
  int ov; /* Next vertex */
  int c;
  int r;

/* #warning TODO: Implement a faster version O(log n) */

  c = cycle(t, start, end);
  j = 0;
  do {
    j++;
    ou = selectAux(t, start, j);
    ov = successor(t, ou);
  } while (j < c && edgeQ(m, ou, ov));

  r = ou;

  if (j < c) {
    do {
      j++;
      ou = selectAux(t, start, j);
      ov = successor(t, ou);
    } while (j < c && edgeQ(m, ou, ov));

    if (j < c)
      r = 0;
  }

  return r;
}

/*
int main() {
  // (1,2) (2,3) (1,4)
  LCT t = allocLCT(5);
  link(t,2,3);
  showLCT(t,"teste");
  showRepTree(t,"rep.dot"); //use with GraphViz online
  return 0;
}*/
