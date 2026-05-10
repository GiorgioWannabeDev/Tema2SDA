#include "structs.h"
#include "treeFunctions.h"

// Helper to print the list of keywords
void printTKWList(TKWList list)
{
    printf("[");
    for (TKWList p = list; p != NULL; p = p->next) {
        printf("%s%s", p->keyword, p->next ? ", " : "");
    }
    printf("]");
}

// Print the main doubly linked list of files
void printFileList(TFileList *list)
{
    if (!list || !list->head) {
        printf("--- File List is Empty ---\n");
        return;
    }
    
    printf("--- File List ---\n");
    for (TFilePtr p = list->head; p != NULL; p = p->next) {
        printf("File ID: %-10s | Score: %-4d | Keywords: ", p->ID, p->score);
        printTKWList(p->keywords);
        printf("\n");
    }
    printf("-----------------\n");
}

// Recursive function to traverse and print the Trie
void printTreeRecursive(Tree node, char *buffer, int depth)
{
    if (node == NULL) {
        return;
    }

    // 1. Add current character to our word buffer
    buffer[depth] = node->c;

    // 2. If this node has a fileRefList, it marks the end of a valid word
    if (node->fileRefList != NULL) {
        buffer[depth + 1] = '\0'; // Null-terminate to safely print
        printf("Keyword: %-15s -> Found in: ", buffer);
        
        // Print all files associated with this keyword
        for (TRefList r = node->fileRefList; r != NULL; r = r->next) {
            // Assuming r->ref points to a TFilePtr
            printf("[%s] ", r->ref->ID); 
        }
        printf("\n");
    }

    // 3. Recurse down to children (next letter in the current word)
    printTreeRecursive(node->child, buffer, depth + 1);

    // 4. Recurse across to siblings (alternative letters at the same position)
    // Note: depth stays the same for siblings because they replace the current letter
    printTreeRecursive(node->next, buffer, depth);
}

// Wrapper function to initialize the buffer and start the recursion safely
void printTreeTemp(Tree root)
{
    if (!root) return;
    
    printf("\n--- Trie (Inverted Index) ---\n");
    // Allocate a buffer large enough for the longest expected word (e.g., 256 chars)
    char buffer[256]; 
    
    // Assuming 'root' is a dummy node, we start traversing from its first child
    printTreeRecursive(root->child, buffer, 0);
    printf("-----------------------------\n");
}

void printTreeStructure(Tree root, int level) {
    if (!root) return;
    
    for (int i = 0; i < level; i++)
        printf("  ");  /* indentare */ 
    
    printf("↳ %c\n", root->c);

    printTreeStructure(root->child, level + 1);  
    printTreeStructure(root->next, level);     
}

int main()
{
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

    commandLine(&myFileList, &myTreeRoot);

    //printFileList(&myFileList);
    //printTreeTemp(myTreeRoot);
    //printTreeStructure(myTreeRoot, 10);

    return 0;
}