//
// Created by 张世超 on 2019-01-29.
//

#include "readData.h"

#define MAXSTRING 1000

//return the number of url
//urls_list_pointer is a pointer pointing to a array of  URL string
void readSection1(char *, Graph);

void readSection2(char *, Tree *);

char *strip(char *);

int GetCollection(char ***urls_list_pointer) {
    char **urls_list = NULL;
    char delim[2] = " ";
    char *token;
    char line[MAXSTRING];
    FILE *f;

    if ((f = fopen("collection.txt", "r")) == NULL) {
        printf("Error!\n");
        exit(0);
    }

    int url_num = 0;
//    int lnCount = 0;
    while (fgets(line, MAXSTRING, f) != NULL) {
        /* first token */
        token = strtok(line, delim);
        /* iterate over the rest of the tokens */
        while (token != NULL) {
            if (strcmp(token, "\n") != 0) {
                if (url_num == 0) {
                    urls_list = (char **) malloc(sizeof(char *));
                } else {
                    urls_list = (char **) realloc(urls_list, sizeof(char *) * (url_num + 1));
                }

                // if the last char is \n, delete
                size_t url_len = strlen(token);
                if (strcmp(&token[url_len - 1], "\n") == 0) {
                    int i = 0;
                    urls_list[url_num] = (char *) malloc(sizeof(char) * url_len);
                    while (i < url_len - 1) {
                        urls_list[url_num][i] = token[i];
                        i++;
                    }
                    urls_list[url_num][url_len - 1] = 0;
                } else {
                    urls_list[url_num] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
                    strcpy(urls_list[url_num], token);
                }
                url_num++;
            }
            token = strtok(NULL, delim);
        }

    }
    *urls_list_pointer = urls_list;
    return url_num;
}

Graph GetGraph(char **url_list, int url_list_len) {
    Graph g = initGraph(url_list, url_list_len);
//    for(int i = 0; i<url_list_len; i++){
//        VNode *v = getVNode(g, i);
//        printf("name: %s, rank: %f\n",v->url_name,v->rank);
//    }
    for (int i = 0; i < url_list_len; i++) {
        readSection1(url_list[i], g);
    }

    return g;
}

Tree GetInvertedList(char **url_list, int url_list_len){
    Tree t = newTree();
    for(int i = 0; i<url_list_len; i++){
//        printf("%s\n",url_list[i]);
        readSection2(url_list[i], &t);
    }
    return t;
}

void readSection1(char *url_name, Graph g) {
    char **url_list = NULL;
    char delim[2] = " ";
    char *token;
    char line[MAXSTRING];
    FILE *f;
    char file_name[strlen(url_name)+4+1];
    strcpy(file_name,url_name);
    strcat(file_name,".txt");
    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Error!\n");
        exit(0);
    }

    int url_num = 0;
    int lnCount = 0;
    int meetStart = 0;
    int meetEnd = 0;
    while (fgets(line, MAXSTRING, f) != NULL) {
        /* first token */
        token = strtok(line, delim);

//        printf("> line %d] \n", ++lnCount);

        if (meetStart == 0) {
            char *s_list[2];
            int i = 0;
            while (token != NULL && i < 2) {
                s_list[i] = strip(token);
//                printf("\ttoken: >|%s|<\n", s_list[i]);
                token = strtok(NULL, delim);
                i++;
            }
            if (i == 2 && strcmp(s_list[0], "#start") == 0 && strcmp(s_list[1], "Section-1") == 0) {
//                printf("遇到start\n");
                meetStart = 1;
            } else {
//                printf("没遇到，跳过\n");
            }
        } else {
            if(meetEnd == 1){
//                printf("发现end\n");
                break;
            }
            while (token != NULL) {
                if(strcmp(token, "#end") == 0 && strcmp(strip(strtok(NULL, delim)), "Section-1") == 0){
                    meetEnd = 1;
                    break;
                }
//                跳过空行
                if (strcmp(token, "\n") != 0) {
                    if (url_num == 0) {
                        url_list = (char **) malloc(sizeof(char *));
                    } else {
                        url_list = (char **) realloc(url_list, sizeof(char *) * (url_num + 1));
                    }

                    // if the last char is \n, delete \n
                    size_t url_len = strlen(token);
                    if (strcmp(&token[url_len - 1], "\n") == 0) {
//                       strip /n
                        int i = 0;
                        url_list[url_num] = (char *) malloc(sizeof(char) * url_len);
                        while (i < url_len - 1) {
                            url_list[url_num][i] = token[i];
                            i++;
                        }
                        url_list[url_num][url_len - 1] = 0;
                    } else {
                        url_list[url_num] = (char *) malloc(sizeof(char) * (strlen(token) + 1));
                        strcpy(url_list[url_num], token);
                    }
                    url_num++;
                }
                token = strtok(NULL, delim);
            }
        }
    }
    fclose(f);
//    printf("\n%s total: %d\n", url_name, url_num);
    int VNode_ID = searchVNode(g, url_name);

    if(url_num != 0){
        for(int i = 0;i<url_num;i++){
            addEdge(g, url_list[i],VNode_ID);
        }
    }
}

void readSection2(char *url_name, Tree *t){
    char delim[2] = " ";
    char *token;
    char buffer[MAXSTRING];
    char line[MAXSTRING];
    FILE *f;
    char file_name[strlen(url_name)+4+1];
    strcpy(file_name,url_name);
    strcat(file_name,".txt");
    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Error!\n");
        exit(0);
    }

    int meetStart = 0;
    int meetEnd = 0;
    while (fgets(line, MAXSTRING, f) != NULL) {
        /* first token */
        token = strtok(line, delim);

        if (meetStart == 0) {
            char *s_list[2];
            int i = 0;
            while (token != NULL && i < 2) {
                s_list[i] = strip(token);
//                printf("\ttoken: >|%s|<\n", s_list[i]);
                token = strtok(NULL, delim);
                i++;
            }
            if (i == 2 && strcmp(s_list[0], "#start") == 0 && strcmp(s_list[1], "Section-2") == 0) {
//                printf("遇到start\n");
                meetStart = 1;
            } else {
//                printf("没遇到，跳过\n");
            }
        } else {
            if(meetEnd == 1){
//                printf("发现end\n");
                break;
            }
            while (token != NULL) {
                if(strcmp(token, "#end") == 0 && strcmp(strip(strtok(NULL, delim)), "Section-2") == 0){
                    meetEnd = 1;
                    break;
                }
//                跳过空行
                if (strcmp(token, "\n") != 0) {
//                  ignore ',' '.' ';' '?' '\n'
                    int t_i = 0;
                    int b_i = 0;
                    Item it;
                    while(true){
                        if (token[t_i] == '\0'){
                            buffer[b_i] = '\0';
                            break;
                        }
                        if (token[t_i] == '\n'){
                            t_i += 1;
                            continue;
                        }
//                        only the last char of string is ',', ';', ';' or '?'
                        if ((token[t_i+1] == '\n'|| token[t_i+1] == '\0')&&(token[t_i] == ','||token[t_i] == '.'||token[t_i] == ';'||token[t_i] == '?')){
                            buffer[b_i] = '\0';
                            break;
                        }
                        buffer[b_i] = (char)tolower(token[t_i]);
                        t_i += 1;
                        b_i += 1;
                    }

                    it.url = url_name;
                    it.word = (char *)malloc(sizeof(char)*(b_i+1));
                    strncpy(it.word,buffer,b_i+1);
//                    printf(">|%s|<\n",it.word);
                    TreeInsert(t,it);

                }
                token = strtok(NULL, delim);
            }
        }
    }
    fclose(f);
}

char *strip(char *string) {
    if(string == NULL){
        return NULL;
    }
    size_t url_len = strlen(string);
    if (strcmp(&string[url_len - 1], "\n") == 0) {
        int i = 0;
        char tmp[url_len];
        while (i < url_len - 1) {
            tmp[i] = string[i];
            i++;
        }
        tmp[url_len - 1] = 0;
        strcpy(string, tmp);
        return string;
    } else {
        return string;
    }
}