//
// Created by 张世超 on 2019-01-29.
//
#include "readData.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct Range {
    int start, end;
} Range;

Range new_Range(int s, int e) {
    Range r;
    r.start = s;
    r.end = e;
    return r;
}

void swap(VNode **a, VNode **b) {
    VNode *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

double *calaulatePageRank(Graph g, double d, double diffPR, int maxiterations) {
    int N = getVertexNum(g);
    double *oldPR = (double *) malloc(sizeof(double) * N);
    double *newPR = (double *) malloc(sizeof(double) * N);
    double *tmpPR = newPR;
    for (int i = 0; i < N; i++) {
        oldPR[i] = getVNode(g, i)->rank;
    }
    int iteration = 0;
    double diff = diffPR;

    while (iteration < maxiterations && diff >= diffPR) {
        iteration++;
        for (int i = 0; i < N; i++) {
            double sum = 0;
            ArcNode *reference = getVNode(g, i)->first;
            while (reference != NULL) {
                sum += oldPR[reference->adjvex] / getVNode(g, reference->adjvex)->outDegree;
                reference = reference->next;
            }
            newPR[i] = (1 - d) / N + d * sum;
        }

//        calculate diff
        double tmp_diff = 0;
        for (int i = 0; i < N; i++) {
            tmp_diff += fabs(newPR[i] - oldPR[i]);
        }
        diff = tmp_diff;
        newPR = oldPR;
        oldPR = tmpPR;
        tmpPR = newPR;
    }
    free(newPR);
    return oldPR;
}

void order(VNode **arr, int len) {
    if (len <= 0)
        return;
    Range r[len];
    int p = 0;
    r[p++] = new_Range(0, len - 1);
    while (p) {
        Range range = r[--p];
        if (range.start >= range.end) {
            continue;
        }
        double mid = arr[(range.start + range.end) / 2]->rank; // 選取中間點為基準點
        int left = range.start, right = range.end;
        do {
            while (arr[left]->rank < mid) {
                ++left;   // 檢測基準點左側是否符合要求
            }
            while (arr[right]->rank > mid) {
                --right; //檢測基準點右側是否符合要求
            }
            if (left <= right) {
                swap(&arr[left], &arr[right]);
                left++;
                right--;               // 移動指針以繼續
            }
        } while (left <= right);
        if (range.start < right) {
            r[p++] = new_Range(range.start, right);
        }
        if (range.end > left) {
            r[p++] = new_Range(left, range.end);
        }
    }
}


int main(int argc, char **argv) {
    if(argc!=4){
        printf("some problem about the number of argument\n");
        exit(1);
    }
    char **url_list;
    int url_num;
    char file_name[] = "pagerankList.txt";
    url_num = GetCollection(&url_list);
//    printf("collection: %d\n",url_num);

//    for(int i = 0;i<url_num;i++){
//        printf("\t%s\n",url_list[i]);
//    }

    Graph g = GetGraph(url_list, url_num);

    for (int i = 0; i < url_num; i++) {
        VNode *VNode = getVNode(g, i);
//        printf("\nNode %s:\n\tout: %d\n\tin: %d\n", VNode->url_name, VNode->outDegree, VNode->inDegree);
//        printf("\tIn:\n");
        ArcNode *an = VNode->first;
        while (an != NULL) {
//            printf("\t\t%s\n", getVNode(g, an->adjvex)->url_name);
            an = an->next;
        }
    }
    char *end;
    double *List_Urls_PageRanks = calaulatePageRank(g, strtod(argv[1], NULL), strtod(argv[2], NULL), (int)strtol(argv[3], NULL, 10));

    VNode *listPointerVNode[url_num];
    for (int i = 0; i < url_num; i++) {
        listPointerVNode[i] = getVNode(g, i);
        listPointerVNode[i]->rank = List_Urls_PageRanks[i];
    }

    order(listPointerVNode, url_num);
    FILE *f;
    if ((f = fopen(file_name, "w")) == NULL) {
        printf("Error!\n");
        exit(0);
    }
    for (int i = url_num - 1; i >= 0; i--) {
        VNode *n = listPointerVNode[i];
        fprintf(f,"%s, %d, %.7f\n", n->url_name, n->outDegree, n->rank);
//        printf("%s, %d, %.7f\n", n->url_name, n->outDegree, n->rank);
    }
    fclose(f);
    freeGraph(g);
    free(List_Urls_PageRanks);
    return 0;
}

