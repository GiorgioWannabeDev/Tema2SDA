#include "structs.h"
#include "treeFunctions.h"


int main()
{
    FILE *input = fopen("indexare.in", "rt");
    if (!input) {
        perror("Failed to open file!\n");
        return -1;
    }
    FILE *output = fopen("indexare.out", "wt");
    if (!output) {
        perror("Failed to open file!\n");
        fclose(input);
        return -1;
    }
    TFileList myFileList;
    myFileList.head = NULL;
    myFileList.tail = NULL;

    Tree myTreeRoot = (Tree)malloc(sizeof(Node));
    if (!myTreeRoot) {
        perror("Failed to allocate root node!\n");
        return 1;
    }
    myTreeRoot->c = '\0'; 
    myTreeRoot->child = NULL;
    myTreeRoot->next = NULL;
    myTreeRoot->fileRefList = NULL;

    commandLine(input, output, &myFileList, &myTreeRoot);

    emptyFileList(&myFileList);
    emptyTree(myTreeRoot);
    fclose(input);
    fclose(output);
    return 0;
}