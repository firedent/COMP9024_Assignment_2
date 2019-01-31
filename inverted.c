//
// Created by 张世超 on 2019-01-29.
//
#include "readData.h"
#include <stdio.h>

int main(int argc, char **argv){
    char **List_of_Urls;
    int urlNum;
    urlNum = GetCollection(&List_of_Urls);
    for(int i = 0; i<urlNum; i++){
        printf("%s\n",List_of_Urls[i]);
    }
    return 1;
}
