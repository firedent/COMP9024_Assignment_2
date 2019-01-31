//
// Created by 张世超 on 2019-01-31.
//

#include "list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Node {
    List_Value_Type value;
    struct Node *next;
} Node;

typedef struct ListRep {
    int size;
    Node *head;
} ListRep;

bool isEqual(List_Value_Type *a, List_Value_Type *b);
void initNode(Node *, List_Value_Type *);

List newList(){
    ListRep *L = (ListRep *)malloc(sizeof(ListRep));
    L->size = 0;
    L->head = NULL;
    return L;
}

void insertToHead(List L,List_Value_Type v){
    if(exist(L, v)){
        return;
    }
    Node *newNode = (Node *)malloc(sizeof(Node));
    initNode(newNode, &v);
    newNode->next = L->head;
    L->head = newNode;
    L->size += 1;
}

bool exist(List L, List_Value_Type item){
    Node *willCheck = L->head;
    while(willCheck != NULL){
        if(isEqual(&willCheck->value,&item)){
            return true;
        }
        willCheck = willCheck->next;
    }
    return false;
}

void freeList(List L){
    if(L == NULL)
        return;
    Node *nextFree = L->head;
    Node *curFree = NULL;
    while (nextFree != NULL){
        curFree = nextFree;
        free(nextFree);
        nextFree = curFree->next;
    }
    free(L);
}
bool isEqual(List_Value_Type *a, List_Value_Type *b){
    if(strcmp(*a,*b) == 0){
        return true;
    }
    return false;
}

void initNode(Node *n, List_Value_Type *v){
    n->value = *v;
    n->next = NULL;
}

void printList(List L){
    Node *n = L->head;
    while(n!= NULL){
        printf("\t%s\n",n->value);
        n= n->next;
    }
}

int convertToArray(List L, List_Value_Type **arr){
    *arr = (List_Value_Type *)malloc(sizeof(List_Value_Type)*L->size);
    int i = 0;
    Node *n = L->head;
    while (n!=NULL){
        (*arr)[i] = n->value;
        n = n->next;
        i++;
    }
    return L->size;
}