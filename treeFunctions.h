#ifndef TREE_FUNCTIONS_H
#define TREE_FUNCTIONS_H

#include "structs.h"

int addFileToRefList(TRefList *dest, TFilePtr associatedCell);

int addKeywordToTree(Tree dest, char *keyword, TFilePtr associatedFile);

void addFileToFileList(TFileList *dest, TFilePtr x);

TFilePtr createFile(char *id, int score, TKWList keywords);

int addKeywordToTKList(TKWList *dest, char *keyword);

int checkFileExists(TFileList *fileList, char *fileName);

int addCommand(TFileList *fileList, Tree root);

void emptyTKWList(TKWList *list);

void emptyFileList(TFileList *list);

int delKeywordFromTree(Tree dest, char *keyword);

int delRefFromTree(Tree dest, char *keyword, TFilePtr associatedFile);

int delFileFromFileList(Tree root, TFileList *src, char *id);

int addKWToFile(Tree root, TFileList *dest, char *id, char *keyword);

int delKWFromFile(Tree root, TFileList *dest, char *id, char *keyword);

void find(int *returnValue, TRefList *list, Tree root, char *keyword);

int getRefListCount(TRefList list);

void findCommand(Tree root);

void commandLine(TFileList *fileList, Tree *root);

int delCommand(Tree root, TFileList *src);

int addkwCommand(Tree root, TFileList *dest);

int delkwCommand(Tree root, TFileList *src);

void printCommand(Tree root);

void printTree(Tree root, char *word, int wordOffset);

void printCommand(Tree root);

TRefCell *maxHeapify(TRefList list);

void insertHeap(TRefCell *heap, TRefCell value, int pos);

void topkCommand(Tree root);

#endif