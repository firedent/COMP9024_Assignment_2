//
// Created by 张世超 on 2019-01-31.
//

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#ifndef ASS2_LIST_H
#define ASS2_LIST_H

typedef struct ListRep *List;
typedef char *List_Value_Type;

bool exist(List ,List_Value_Type);
List newList();
void insertToHead(List ,List_Value_Type);
void freeList(List);
void printList(List);
int convertToArray(List, List_Value_Type **);

#endif //ASS2_LIST_H
