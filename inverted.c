//
// Created by 张世超 on 2019-01-29.
//
#include "readData.h"
#include <stdio.h>

typedef struct Range {
    int start, end;
} Range;

static FILE *f;

Range new_Range(int s, int e) {
    Range r;
    r.start = s;
    r.end = e;
    return r;
}

void swap(List_Value_Type *a, List_Value_Type *b) {
    List_Value_Type tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void order(List_Value_Type *arr, int len) {
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
        List_Value_Type mid = arr[(range.start + range.end) / 2]; // 選取中間點為基準點
        int left = range.start, right = range.end;
        do {
            while (strcmp(mid,arr[left]) > 0) {
                ++left;   // 檢測基準點左側是否符合要求
            }
            while (strcmp(mid,arr[right]) < 0) {
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

void visit(Tree tree){
    char **urlsArray;
    fprintf(f,"%s ",key(tree));
//    printf("%s ",key(tree));
    int num = convertToArray(urls(tree), &urlsArray);
    order(urlsArray, num);
    for(int i = 0; i<num;i++){
        fprintf(f," %s",urlsArray[i]);
//        printf(" %s",urlsArray[i]);
    }
    fprintf(f,"\n");
//    printf("\n");
    free(urlsArray);
}

int main(int argc, char **argv){
    char **List_of_Urls;
    int urlNum;
    char *file_name = "invertedindex.txt";
    urlNum = GetCollection(&List_of_Urls);
    Tree t = GetInvertedList(List_of_Urls, urlNum);
    if ((f = fopen(file_name, "w")) == NULL) {
        printf("Error!\n");
        exit(0);
    }
    InOrder(t, &visit);
    fclose(f);
    return 1;
}
