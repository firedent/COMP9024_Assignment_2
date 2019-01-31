//
// Created by 张世超 on 2019-01-31.
//

// Binary Search Tree ADT implementation ... COMP9024 17s2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "BSTree.h"
#include "stack.h"

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

// free memory associated with Tree
void freeTree(Tree t) {
    if (t != NULL) {
        freeTree(left(t));
        freeTree(right(t));
        free(t);
    }
}
