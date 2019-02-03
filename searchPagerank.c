//
// Created by 张世超 on 2019-01-29.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "readData.h"
#include "BStree_double.h"

#define MAXSTRING 1000

static char *PAGERANK_FILE_NAME = "pagerankList.txt";
static char *INVERTED_INDEX_FILE_NAME = "invertedIndex.txt";

typedef struct Range {
    int start, end;
} Range;

Range new_Range(int s, int e) {
    Range r;
    r.start = s;
    r.end = e;
    return r;
}

void swap(URL **a, URL **b) {
    URL *tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void order(URL **arr, int len) {
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
        double mid_time = arr[(range.start + range.end) / 2]->times;
        double mid_PR = arr[(range.start + range.end) / 2]->pagerank;
        int left = range.start, right = range.end;
        do {
            while (arr[left]->times > mid_time || (arr[left]->times == mid_time && arr[left]->pagerank > mid_PR)) {
                ++left;
            }
            while (arr[right]->times < mid_time || (arr[right]->times == mid_time && arr[right]->pagerank < mid_PR)) {
                --right;
            }
            if (left <= right) {
                swap(&arr[left], &arr[right]);
                left++;
                right--;
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

void visit(DTree t){
    printf("%s: %f\n",Dkey(t),Dvalue(t));
}

void addToHead(DTree t, URL **URL_p, DTree urlPageRankTree, DTree urlContainTermNumTree){
    if(*URL_p == NULL){
        *URL_p = (URL *)malloc(sizeof(URL));
        (*URL_p)->next = NULL;
        (*URL_p)->name = Dkey(t);
        (*URL_p)->pagerank = Dvalue(DTreeSearch(urlPageRankTree, (*URL_p)->name));
        (*URL_p)->times = Dvalue(DTreeSearch(urlContainTermNumTree, (*URL_p)->name));
    }else{
        URL *t_url = (URL *)malloc(sizeof(URL));
        t_url->next = *URL_p;
        t_url->name = Dkey(t);
        t_url->pagerank = Dvalue(DTreeSearch(urlPageRankTree, t_url->name));
        t_url->times = Dvalue(DTreeSearch(urlContainTermNumTree, t_url->name));
        *URL_p = t_url;
    }
}

bool existIn(DTree searchTermsTree, char *string){
    if(DTreeSearch(searchTermsTree,string) != NULL){
        return true;
    }
    return false;
}

DTree getUrlTreeWithPR() {
    FILE *f;
    char line[MAXSTRING];
    static char *PAGERANK_FILE_DELIM = ", ";
    char *token;

    //    add url name to BSTree with page rank.
    if ((f = fopen (PAGERANK_FILE_NAME, "r")) == NULL) {
        printf("Error!\n");
        return (0);
    }

    DTree urlTreeWithPageRank = DnewTree();
//    int lnCount = 0;

    while(fgets(line, MAXSTRING, f) != NULL)  {
//        printf( "> line %d] \n", ++lnCount);
        /* first token */
        token = strtok(line, PAGERANK_FILE_DELIM);

        int count = 0;
        /* iterate over the rest of the tokens */
        DData item;
        while( token != NULL ) {
            if(count == 0){
                item.key = token;
//                printf( "      token %d) >>>|%s|<<<\n", count, item.key );
            } else if (count == 2){
                item.value = strtod(token,NULL);
//                printf( "      token %d) >>>|%.7f|<<<\n", count, item.value );
                DTreeInsert(&urlTreeWithPageRank, item);
            }
            count += 1;
            token = strtok(NULL, PAGERANK_FILE_DELIM);
        }
    }
    fclose(f);
    return urlTreeWithPageRank;
}

DTree getUrlTreeWithMatchTime(DTree searchTerms, int searchTermsNum, int *urlMatchedTime) {
    FILE *f;
    char line[MAXSTRING];
    static char *PAGERANK_FILE_DELIM = ", ";
    char *token;
    //    count how much search term in each url
    if ((f = fopen (INVERTED_INDEX_FILE_NAME, "r")) == NULL) {
        printf("Error!\n");
        return (0);
    }
//    int lnCount = 0;
    int alreadyFound = 0;
    DTree urlTreeWithSearchTermNum = DnewTree();
    while(fgets(line, MAXSTRING, f) != NULL)  {
//        printf( "> line %d] \n", ++lnCount);

        /* first token */
        token = strtok(line, PAGERANK_FILE_DELIM);

//        if token does not exist in search term, next line
        if( !existIn(searchTerms,token)){
            continue;
        }

        alreadyFound += 1;
//        printf( "search term: >>>|%s|<<<\n", token);
        token = strtok(NULL, PAGERANK_FILE_DELIM);

        /* iterate over the rest of the tokens */
        while( token != NULL ) {
            char *strippedToken = stripBackslashN(token);
//            printf( "\tURL: >>>|%s|<<<\n", stripBackslashN(strippedToken));
            DTreeInsertKeyIncreaseValue(&urlTreeWithSearchTermNum, strippedToken, urlMatchedTime);
            token = strtok(NULL, PAGERANK_FILE_DELIM);
        }

        if(alreadyFound >= searchTermsNum)
            break;
    }
    fclose(f);
    return urlTreeWithSearchTermNum;
}

int main(int argc, char **argv){

//    get search terms and store into binary tree.
    DTree searchTerms = DnewTree();
    if (argc == 1){
        return 0;
    }
    for(int i = 0;i<argc-1;i++){
        DData it;
        it.key = argv[i+1];
        DTreeInsert(&searchTerms, it);
//        printf("searchTerms[%d]: %s\n",i,it.key);
    }


    DTree urlTreeWithPageRank = getUrlTreeWithPR();
    int urlNum = 0;
    DTree urlTreeWithSearchTermNum = getUrlTreeWithMatchTime(searchTerms, argc-1, &urlNum);
//    DPreOrder(urlTreeWithPageRank,&visit);
//    DPreOrder(urlTreeWithSearchTermNum, &visit);
//    printf("node number: %d\n", urlNum);

    URL *urlArray = NULL;
    URL *urlPointerArray[urlNum];
    convertTreeToLinkList(urlTreeWithSearchTermNum, &urlArray, urlTreeWithPageRank, urlTreeWithSearchTermNum, &addToHead);
    URL *u = urlArray;
    for(int i = 0; i<urlNum; i++){
        urlPointerArray[i] = u;
        u = u->next;
    }

//    for(int i = 0; i<urlNum; i++){
//        printf("name: %s PR: %f, time: %f\n",urlPointerArray[i]->name, urlPointerArray[i]->pagerank, urlPointerArray[i]->times);
//    }

    order(urlPointerArray, urlNum);
//    printf("---------\n");
    for(int i = 0; i<urlNum && i < 30; i++){
        printf("%s\n",urlPointerArray[i]->name);
//        printf("name: %s PR: %f, time: %f\n",urlPointerArray[i]->name, urlPointerArray[i]->pagerank, urlPointerArray[i]->times);
    }
    return 0;
}