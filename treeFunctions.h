//CHELES Giorgio - 312CB
#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H

#include "structs.h"

char *getFileWord(FILE *src);

int addFileToRefList(TRefList *dest, TFilePtr associatedCell);

int addKeywordToTree(Tree dest, char *keyword, TFilePtr associatedFile);

void addFileToFileList(TFileList *dest, TFilePtr x);

TFilePtr createFile(char *id, int score, TKWList keywords);

int addKeywordToTKList(TKWList *dest, char *keyword);

int checkFileExists(TFileList *fileList, char *fileName);

int addCommand(FILE *input, FILE *output, TFileList *fileList, Tree root);

void emptyTKWList(TKWList *list);

void emptyFileList(TFileList *list);

int delKeywordFromTree(Tree dest, char *keyword);

int delRefFromTree(Tree dest, char *keyword, TFilePtr associatedFile);

int delFileFromFileList(Tree root, TFileList *src, char *id);

int addKWToFile(Tree root, TFileList *dest, char *id, char *keyword);

int delKWFromFile(Tree root, TFileList *dest, char *id, char *keyword);

void find(int *returnValue, TRefList *list, Tree root, char *keyword);

int getRefListCount(TRefList list);

void findCommand(FILE *input, FILE *output, Tree root);

void commandLine(FILE *input, FILE *output, TFileList *fileList, Tree *root);

int delCommand(FILE *input, FILE *output, Tree root, TFileList *src);

int addkwCommand(FILE *input, FILE *output, Tree root, TFileList *dest);

int delkwCommand(FILE *input, FILE *output, Tree root, TFileList *src);

void printTree(FILE *input, Tree root, char *word, int wordOffset);

void printCommand(FILE *output, Tree root);

TRefCell *maxHeapify(TRefList list);

void insertHeap(TRefCell *heap, TRefCell value, int pos);

void siftHeap(TRefCell *heap, int i, int heapSize);

void topkCommand(FILE *input, FILE *output, Tree root);

void emptyTree(Tree root);

#endif