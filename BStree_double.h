//
// Created by 张世超 on 2019-02-02.
//

#ifndef ASS2_BSTREE_DOUBLE_H
#define ASS2_BSTREE_DOUBLE_H
#define Dkey(Dtree)  ((Dtree)->data.key)
#define Dvalue(Dtree)  ((Dtree)->data.value)
#define Dleft(Dtree)  ((Dtree)->left)
#define Dright(Dtree) ((Dtree)->right)

typedef struct DNode *DTree;

typedef struct DData {
    char *key;
    double value;
} DData;

typedef struct DNode {
    DData data;
    struct DNode *left, *right;
} DNode;

typedef struct URL {
    char *name;
    double times;
    double pagerank;
    struct URL *next;
} URL;

DTree DnewTree(); // create an empty Tree

void DfreeTree(DTree);   // free memory associated with Tree

void DTreeInsert(DTree *, DData);    // insert a new item into a Tree

void DTreeInsertKeyIncreaseValue(DTree *, char *, int *);

void DPreOrder(DTree, void (*)(DTree));

void DInOrder(DTree, void (*)(DTree));

void convertTreeToLinkList(DTree, URL **, DTree, DTree, void (*visit)(DTree , URL **, DTree, DTree));

int DTreeNumNodes(DTree t);

DNode *DTreeSearch(DTree, char *);

DTree DrotateRight(DTree);

DTree DrotateLeft(DTree);

DTree Dpartition(DTree, int);

DTree Drebalance(DTree);

#endif //ASS2_BSTREE_DOUBLE_H
