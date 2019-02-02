//
// Created by 张世超 on 2019-01-31.
//

// Binary Search Tree ADT implementation ... COMP9024 17s2

#include "BSTree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

// create a new empty Tree
Tree newTree() {
    return NULL;
}

// make a new node containing string
Tree newNode(Item it) {
    Tree t = (Tree) malloc(sizeof(Node));
    key(t) = it.word;
    urls(t) = newList();
    insertToHead(urls(t), it.url);
    left(t) = right(t) = NULL;
    return t;
}

void TreeInsert(Tree *t, Item it) {
    if (*t == NULL) {
        *t = newNode(it);
        return;
    }
    int cmp = strcmp(key(*t), it.word);
    if (cmp == 0) {
        free(it.word);
        insertToHead(urls(*t), it.url);
        return;
    } else if (cmp > 0) {
        return TreeInsert(&(left(*t)), it);
    } else {
        return TreeInsert(&(right(*t)), it);
    }
}

//In-order traversal
void InOrder(Tree t, void (*visit)(Tree)){
    stack s = newStack();
    Tree p = t;
    while(p != NULL || !StackIsEmpty(s)){
        if(p!=NULL){
            StackPush(s, p);
            p = left(p);
        } else{
            p = StackPop(s);
            (*visit)(p);
            p = right(p);
        }
    }
}

int TreeNumNodes(Tree t) {
    if (t == NULL)
        return 0;
    else
        return 1 + TreeNumNodes(left(t)) + TreeNumNodes(right(t));
}

Node *TreeSearch(Tree t, char *key){
    if (t == NULL)
        return NULL;
    int cmp = strcmp(key(t), key);
    if (cmp == 0) {
        return t;
    } else if (cmp > 0) {
        return TreeSearch(left(t), key);
    } else {
        return TreeSearch(right(t), key);
    }
}

// free memory associated with Tree
void freeTree(Tree t) {
    if (t != NULL) {
        freeTree(left(t));
        freeTree(right(t));
        free(t->data.key);
        free(t);
    }
}

Tree rotateRight(Tree n1) {
    if (n1 == NULL || left(n1) == NULL)
        return n1;
    Tree n2 = left(n1);
    left(n1) = right(n2);
    right(n2) = n1;
    return n2;
}

Tree rotateLeft(Tree n2) {
    if (n2 == NULL || right(n2) == NULL)
        return n2;
    Tree n1 = right(n2);
    right(n2) = left(n1);
    left(n1) = n2;
    return n1;
}

Tree partition(Tree t, int i) {
    if (t != NULL) {
        assert(0 <= i && i < TreeNumNodes(t));
        int m = TreeNumNodes(left(t));
        if (i < m) {
            left(t) = partition(left(t), i);
            t = rotateRight(t);
        } else if (i > m) {
            right(t) = partition(right(t), i-m-1);
            t = rotateLeft(t);
        }
    }
    return t;
}

Tree rebalance(Tree t) {
    int n = TreeNumNodes(t);
    if (n >= 3) {
        t = partition(t, n/2);           // put node with median key at root
        left(t) = rebalance(left(t));    // then rebalance each subtree
        right(t) = rebalance(right(t));
    }
    return t;
}
