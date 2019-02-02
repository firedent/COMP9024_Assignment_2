//
// Created by 张世超 on 2019-02-02.
//

#include "BStree_double.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

// create a new empty Tree
DTree DnewTree() {
    return NULL;
}

// make a new node containing string
DTree DnewNode(DData item) {
    DTree t = (DTree) malloc(sizeof(DNode));
    Dkey(t) = strcpy((char *)malloc(sizeof(char)*(strlen(item.key)+1)), item.key);
    Dvalue(t) = item.value;
    Dleft(t) = Dright(t) = NULL;
    return t;
}

void DTreeInsert(DTree *t, DData item) {
    if (*t == NULL) {
        *t = DnewNode(item);
        return;
    }
    int cmp = strcmp(Dkey(*t), item.key);
    if (cmp == 0) {
        return;
    } else if (cmp > 0) {
        return DTreeInsert(&(Dleft(*t)), item);
    } else {
        return DTreeInsert(&(Dright(*t)), item);
    }
}

void DTreeInsertKeyIncreaseValue(DTree *t, char *key, int *treeNodeNum) {
    if (*t == NULL) {
        DData item = {key,1};
        *t = DnewNode(item);
        *treeNodeNum += 1;
        return;
    }
    int cmp = strcmp(Dkey(*t), key);
    if (cmp == 0) {
        Dvalue(*t) += 1;
        return;
    } else if (cmp > 0) {
        return DTreeInsertKeyIncreaseValue(&(Dleft(*t)), key, treeNodeNum);
    } else {
        return DTreeInsertKeyIncreaseValue(&(Dright(*t)), key, treeNodeNum);
    }
}

void DPreOrder(DTree t, void (*visit)(DTree)){
    if(t != NULL){
        (*visit)(t);
        DPreOrder(Dleft(t), visit);
        DPreOrder(Dright(t), visit);
    }
}

void DInOrder(DTree t, void (*visit)(DTree)){
    if(t != NULL){
        DInOrder(Dleft(t), visit);
        (*visit)(t);
        DInOrder(Dright(t), visit);
    }
}

void convertTreeToLinkList(DTree t, URL **urlArray, DTree urlPageRankTree, DTree urlContainTermNumTree, void (*visit)(DTree , URL **, DTree, DTree)){
    if(t != NULL){
        (*visit)(t, urlArray, urlPageRankTree, urlContainTermNumTree);
        convertTreeToLinkList(Dleft(t), urlArray, urlPageRankTree, urlContainTermNumTree, visit);
        convertTreeToLinkList(Dright(t), urlArray, urlPageRankTree, urlContainTermNumTree, visit);
    }
}

int DTreeNumNodes(DTree t) {
    if (t == NULL)
        return 0;
    else
        return 1 + DTreeNumNodes(Dleft(t)) + DTreeNumNodes(Dright(t));
}

DNode *DTreeSearch(DTree t, char *key){
    if (t == NULL)
        return NULL;
    int cmp = strcmp(Dkey(t), key);
    if (cmp == 0) {
        return t;
    } else if (cmp > 0) {
        return DTreeSearch(Dleft(t), key);
    } else {
        return DTreeSearch(Dright(t), key);
    }
}

// free memory associated with Tree
void DfreeTree(DTree t) {
    if (t != NULL) {
        DfreeTree(Dleft(t));
        DfreeTree(Dright(t));
        free(t->data.key);
        free(t);
    }
}

DTree DrotateRight(DTree n1) {
    if (n1 == NULL || Dleft(n1) == NULL)
        return n1;
    DTree n2 = Dleft(n1);
    Dleft(n1) = Dright(n2);
    Dright(n2) = n1;
    return n2;
}

DTree DrotateLeft(DTree n2) {
    if (n2 == NULL || Dright(n2) == NULL)
        return n2;
    DTree n1 = Dright(n2);
    Dright(n2) = Dleft(n1);
    Dleft(n1) = n2;
    return n1;
}

DTree Dpartition(DTree t, int i) {
    if (t != NULL) {
        assert(0 <= i && i < DTreeNumNodes(t));
        int m = DTreeNumNodes(Dleft(t));
        if (i < m) {
            Dleft(t) = Dpartition(Dleft(t), i);
            t = DrotateRight(t);
        } else if (i > m) {
            Dright(t) = Dpartition(Dright(t), i-m-1);
            t = DrotateLeft(t);
        }
    }
    return t;
}

DTree Drebalance(DTree t) {
    int n = DTreeNumNodes(t);
    if (n >= 3) {
        t = Dpartition(t, n/2);           // put node with median key at root
        Dleft(t) = Drebalance(Dleft(t));    // then rebalance each subtree
        Dright(t) = Drebalance(Dright(t));
    }
    return t;
}
