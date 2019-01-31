//
// Created by 张世超 on 2019-01-29.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef ASS2_GRAPH_H
#define ASS2_GRAPH_H

typedef struct GraphRep *Graph;

//edge
typedef struct ArcNode{
//    position of edge's source in vertexes(array)
    int adjvex;
    struct ArcNode *next;
} ArcNode;

//Vertex
typedef struct VNode{
    int outDegree;
    int inDegree;
    char *url_name;
    double rank;
    ArcNode *first;
}VNode;

//pass the number of Vertex
Graph initGraph(char **, int);

VNode* getVNode(Graph g, int index);

int searchVNode(Graph g, char *name);

void addEdge(Graph g, char *sourceName, int destinationId);

int getVertexNum(Graph g);

void freeGraph(Graph g);

#endif //ASS2_GRAPH_H
