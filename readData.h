//
// Created by 张世超 on 2019-01-29.
//

#include "graph.h"
#include "BSTree.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef ASS2_READDATA_H
#define ASS2_READDATA_H

int GetCollection(char ***);

Graph GetGraph(char **, int);

Tree GetInvertedList(char **, int);

#endif //ASS2_READDATA_H