// Binary Search Tree ADT interface ... COMP9024 17s2

#include <stdbool.h>
#include "list.h"
#ifndef ASS2_BSTREE_H
#define ASS2_BSTREE_H

#define key(tree)  ((tree)->data.key)
#define urls(tree)  ((tree)->data.urlsList)
#define left(tree)  ((tree)->left)
#define right(tree) ((tree)->right)

typedef struct Node *Tree;

typedef struct Item{
    char *word;
    char *url;
} Item;

typedef struct Data {
    char *key;
    List urlsList;
} Data;

typedef struct Node {
    Data data;
    struct Node *left, *right;
} Node;

Tree newTree(); // create an empty Tree

void freeTree(Tree);   // free memory associated with Tree

void TreeInsert(Tree *, Item);    // insert a new item into a Tree

void InOrder(Tree , void (*)(Tree));

#endif //ASS2_BSTREE_H

