#ifndef STRUCTS_H
#define STRUCTS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct cell1{
    char *keyword;
    struct cell1 *next;
} TKWCell, *TKWList;

typedef struct fileCell{
    char *ID;
    int score;
    TKWList keywords;
    struct fileCell *next, *prev;
} TFileCell, *TFilePtr;

typedef struct fileList{
    TFilePtr head;
    TFilePtr tail;
} TFileList;

typedef struct cell2{
    TFilePtr ref;
    struct cell2 *next;
} TRefCell, *TRefList;


typedef struct node {
    char c;
    TRefList fileRefList;
    struct node* child;   
    struct node* next;  
} Node, *Tree;

#endif