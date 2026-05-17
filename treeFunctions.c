//CHELES Giorgio - 312CB
#include "treeFunctions.h"

char *getFileWord(FILE *src)
{
	long size = 10;
	long chpos = 0;

	int ch = fgetc(src);
	if (ch == EOF) {
		return NULL;
	}
	//we skip all space characters
	while (isspace(ch)) {
		ch = fgetc(src);
		if (ch == EOF) {
			return NULL;
		}
	}
	char *string = malloc(size * sizeof(char));
	if (!string) {
		return NULL;
	}
	//this is where the word starts
	while (!isspace(ch) && ch != EOF) {
		if (chpos >= size - 1) {
			size *= 2;
			char *temp = realloc(string, size);
			if (!temp) {
				free(string);
				return NULL;
			}
			string = temp;
		}
		string[chpos++] = ch;
		ch = fgetc(src);
	}
	//add null character at the end
	string[chpos] =  '\0';
    char *finalString = realloc(string, chpos + 1);
    if (!finalString)
    {
        //the string is good, just not resized properly
        return string;
    }
    return finalString;
}

int addFileToRefList(TRefList *dest, TFilePtr associatedCell)
{
    TRefList aux = (TRefList)malloc(sizeof(TRefCell));

    if (!aux) {
        perror("ADD_FILE_TO_REF_LIST: Failed to allocate memory for new entry!\n");
        return 0;
    }

    aux->ref = associatedCell;

    TRefList p, prevp = NULL;
    p = (*dest);
    // Find new cell's place in the list
    while(p != NULL && strcmp(aux->ref->ID, p->ref->ID) > 0) {
        prevp = p;
        p = p->next;
    }
    // Check if its already there
    if (p != NULL && strcmp(aux->ref->ID, p->ref->ID) == 0) {
        free(aux);
        return 0;
    }
    
    //If we found the correct place, we insert it there
    aux->next = p;
    if (prevp != NULL) {
        prevp->next = aux;
    } else {
        (*dest) = aux;
    }
    return 1;
}

/*
    Return Values:
     0 = keyword already present
     1 = keyword added
    -1 = error
*/
int addKeywordToTree(Tree dest, char *keyword, TFilePtr associatedFile)
{
    Tree currentNode = dest, copy;
    int kwLen = strlen(keyword), i, treeModified = 0;
    char childPresent;

    for (i = 0; i < kwLen; i++) {
        //check if character already in place
        childPresent = 0;
        copy = currentNode;
        for (currentNode = currentNode->child; currentNode != NULL; currentNode = currentNode->next) {
            if (keyword[i] == currentNode->c) {
                //found character, we can proceed
                break;
            }
            childPresent = 1;
        }
        if (currentNode) {
            continue;
        }
        
        //allocate memory for the new node
        Tree aux = (Tree)malloc(sizeof(Node));
        if (!aux)
        {
            perror("ADD_KEYWORD_TO_TREE: Failed to add keyword to tree!\n");
            return -1;
        }
        treeModified = 1;
        aux->c = keyword[i];
        aux->next = NULL;
        aux->child = NULL;
        aux->fileRefList = NULL;

        if (!childPresent) {
            copy->child = aux;
            currentNode = copy->child;
            continue;
        }
        Tree prevNode = NULL;
        for (currentNode = copy->child; currentNode != NULL && aux->c > currentNode->c; prevNode = currentNode, currentNode = currentNode->next);
        if (!prevNode){
            copy->child = aux;
        } else {
            prevNode->next = aux;
        }
        aux->next = currentNode;
        currentNode = aux;
        
    }
    if (!addFileToRefList(&currentNode->fileRefList, associatedFile)) {
        perror("ADD_KEYWORD_TO_TREE: Failed to add file reference to reference list!\n");
        return -1;
    }
    return treeModified;
}

void addFileToFileList(TFileList *dest, TFilePtr x)
{
    if (dest->head == NULL) {
        dest->head = x;
        dest->tail = x;
        return;
    }
    dest->tail->next = x;
    x->prev = dest->tail;
    dest->tail = x;
}

TFilePtr createFile(char *id, int score, TKWList keywords)
{
    TFilePtr aux = (TFilePtr)malloc(sizeof(TFileCell));
    if (!aux) {
        perror("CREATE_FILE: Failed to create file object!\n");
        return NULL;
    }
    aux->ID = id;
    aux->keywords = keywords;
    aux->score = score;
    aux->next = NULL;
    aux->prev = NULL;
    return aux;
}

int addKeywordToTKList(TKWList *dest, char *keyword)
{
    for (TKWList p =  *dest; p != NULL; p = p->next) {
        if (strcmp(p->keyword, keyword) == 0) {
            return 0;
        }
    }
    TKWList aux = (TKWList)malloc(sizeof(TKWCell));
    if (!aux) {
        perror("ADD_KEYWORD_TO_TKLIST: Failed to create keyword object!\n");
        return 0;
    }
    aux->keyword = keyword;
    aux->next = (*dest);
    (*dest) = aux;
    return 1;
}

int checkFileExists(TFileList *fileList, char *fileName)
{
    TFilePtr p;
    for (p = fileList->head; p != NULL; p = p->next) {
        if (strcmp(p->ID, fileName) == 0) {
            return 1;
        }
    }
    return 0;
}

int addCommand(FILE *input, FILE *output, TFileList *fileList, Tree root)
{
    int c;
    TFilePtr newFile;
    char *ID, *keyword;
    int score, KWCount;
    TKWList keywords = NULL;

    ID = getFileWord(input);
    if (!ID) {
        return 0; //Empty input
    }
    
    if (checkFileExists(fileList, ID)) {
        fprintf(output ,"EXISTS\n");
        free(ID);
        //consume the rest of the line, no point in reading it
        while ((c = fgetc(input)) != '\n' && c != EOF);
        return 1;
    }
    fscanf(input, " %d %d", &score, &KWCount);
    for (int i = 0; i < KWCount; i++) {
        keyword = getFileWord(input);
        if (!keyword){
            perror("ADD_COMMAND: Failed to allocate keyword memory!\n");
            free(ID);
            emptyTKWList(&keywords);
            return 0;
        }
        if (!addKeywordToTKList(&keywords, keyword)) {
            free(keyword);
        }
    }
    newFile = createFile(ID, score, keywords);
    if (!newFile) {
        free(ID);
        emptyTKWList(&keywords);
        return 0;
    }
    addFileToFileList(fileList, newFile);
    for (TKWList i = keywords; i != NULL; i = i->next) {
        if (addKeywordToTree(root, i->keyword, newFile) == -1) {
            emptyTKWList(&newFile->keywords);
            free(newFile->ID);
            free(newFile);
            return 0;
        }
    }
    
    fprintf(output ,"OK\n");
    return 1;
}

void emptyTKWList(TKWList *list)
{
    TKWList aux;
    while ((*list)) {
        free((*list)->keyword);
        aux = (*list)->next;
        free((*list));
        (*list) = aux;
    }
}

void emptyFileList(TFileList *list)
{
    TFilePtr p = (*list).head, aux;
    while(p) {
        aux = p->next;
        free(p->ID);
        emptyTKWList(&p->keywords);
        if (p != (*list).tail) {
            free(p);
            p = aux;
        } else {
            free(p);
            p = NULL;
        }
    }
}

int delKeywordFromTree(Tree dest, char *keyword)
{
    //Node is null, return 0
    if (dest == NULL){
        return 0;
    }
    Tree node, prevNode = NULL, nextNode;
    //Search for the current character
    for (node = dest->child; node != NULL; prevNode = node, node = node->next) {
        if (node->c == keyword[0]) {
            break;
        }
    }
    //If character not found, return 0
    if (node == NULL) {
        return 0;
    }
    nextNode = node->next;
    //If this is the last character, we are going to need to start returning
    if (keyword[1] == '\0') {
        //If there are no descendants
        if (node->child == NULL) {
            //If this node has no siblings
            if (prevNode == NULL) {
                dest->child = nextNode;
            } else { // If it does have siblings
                prevNode->next = nextNode;
            }
            free(node);
            return 1; // Signal that we can start deleting
        } else {
            return 0; // Signal that the node is in use by other keyword, thus we cant delete
        }
    } else {
        //Recursively call the function for the next character
        int deleted = delKeywordFromTree(node, keyword + 1);
        /*If: we received the signal that we can continue deleting,
        the node has no descendants left and also no file references
        we continue deletion, otherwise we stop and start returning 0es*/
        if (deleted && node->child == NULL && node->fileRefList == NULL) {
            if (prevNode == NULL) {
                dest->child = nextNode;
            } else {
                prevNode->next = nextNode;
            }
            free(node);
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

/*
Return values:
0 = deleted reference, node still contains references
1 = deleted the last reference in the node
*/
int delRefFromTree(Tree dest, char *keyword, TFilePtr associatedFile)
{
    if (dest == NULL){
        return 0;
    }
    
    TRefList fileRef, prevFileRef = NULL;
    Tree p = dest;
    int len = strlen(keyword);
    for (int i = 0; i < len; i++) {
        Tree found = NULL;
        for (Tree curr = p->child; curr != NULL; curr = curr->next) {
            if (curr->c == keyword[i]) {
                found = curr;
                break;
            }
        }
        if (found == NULL) {
            return 0;
        }
        p = found;
        
    }
    for (fileRef = p->fileRefList; fileRef != NULL; prevFileRef = fileRef, fileRef = fileRef->next) {
        if (fileRef->ref == associatedFile) {
            break;
        }
    }

    if (fileRef == NULL) return 0;
    
    if (prevFileRef == NULL) {
        p->fileRefList = fileRef->next;
    } else {
        prevFileRef->next = fileRef->next;
    }
    
    free(fileRef);

    return (p->fileRefList == NULL) ? 1 : 0;
}

int delFileFromFileList(Tree root, TFileList *src, char *id)
{
    TFilePtr target;
    for (target = (*src).head; target != NULL; target = target->next) {
        if (strcmp(target->ID, id) == 0) {
            break;
        }
    }
    if (!target) {
        return 0;
    }

    if (target->prev != NULL) {
        target->prev->next = target->next;
    } else {
        (*src).head = target->next;
    }
    
    if (target->next != NULL) {
        target->next->prev = target->prev;
    } else {
        (*src).tail = target->prev;
    }
    
    //tree logic...
    TKWList keyword = target->keywords, aux;
    while (keyword != NULL) {
        if (delRefFromTree(root, keyword->keyword, target)) {
            delKeywordFromTree(root, keyword->keyword);
        }
        aux = keyword->next;
        free(keyword->keyword);
        free(keyword);
        keyword = aux;
    }
    
    free(target->ID);
    free(target);
    return 1;
}


int addKWToFile(Tree root, TFileList *dest, char *id, char *keyword)
{
    TFilePtr i = NULL;
    for (i = dest->head; i != NULL; i = i->next) {
        if (!strcmp(i->ID, id)) {
            break;
        }
    }
    if (i == NULL) {
        return 0;
    }
    if (addKeywordToTKList(&i->keywords, keyword)) {
        addKeywordToTree(root, keyword, i);
    }

    return 1;
}

int delKWFromFile(Tree root, TFileList *dest, char *id, char *keyword)
{
    TFilePtr i = NULL;
    for (i = dest->head; i != NULL; i = i->next) {
        if (!strcmp(i->ID, id)) {
            break;
        }
    }
    if (i == NULL) {
        return 0; //No such file
    }
    TKWList p, prevp = NULL;
    for (p = i->keywords; p != NULL; prevp = p, p = p->next) {
        if (!strcmp(p->keyword, keyword)) {
            break;
        }
    }
    if (p == NULL) {
        return 1; // it is a success nonetheless
    }
    if (delRefFromTree(root, keyword, i)) {
        delKeywordFromTree(root, keyword);
    }
    if (prevp == NULL) {
        i->keywords = p->next;
    } else {
        prevp->next = p->next;
    }
    free(p->keyword);
    free(p);
    if (i->keywords == NULL) { //file is EMPTY
        delFileFromFileList(root, dest, i->ID);
    }
    return 1;
}

void find(int *returnValue, TRefList *list, Tree root, char *keyword)
{
    *returnValue = 0;
    *list = NULL;
    Tree node = root;
    int len = strlen(keyword);
    for (int i = 0; i < len; i++) {
        for (node = node->child; node != NULL; node = node->next) {
            if (node->c == keyword[i]) {
                break;
            }
        }
        if (node == NULL) {
            return;
        }
    }
    (*returnValue) = getRefListCount(node->fileRefList);
    (*list) = node->fileRefList;
}

int getRefListCount(TRefList list)
{
    int counter = 0;
    TRefList p = list;
    while (p) {
        counter++;
        p = p->next;
    }
    return counter;
}

void findCommand(FILE *input, FILE *output, Tree root)
{
    char *keyword = getFileWord(input);
    if (!keyword){
        perror("FIND_COMMAND: Failed to get user input!\n");
        return;
    }
    
    int filesNmb;
    TRefList list;
    find(&filesNmb, &list, root, keyword);
    if (!filesNmb) {
        fprintf(output, "EMPTY\n");
        free(keyword);
        return;
    }
    fprintf(output, "%d", filesNmb);
    for (int i = 0; i < filesNmb; i++)
    {
        fprintf(output, " %s", list->ref->ID);
        list = list->next;
    }
    fprintf(output ,"\n");
    free(keyword);
}

void commandLine(FILE *input, FILE *output, TFileList *fileList, Tree *root)
{
    char *command, c;
    int n;
    fscanf(input, " %d", &n);
    for (int i = 0; i < n; i++) {
        command = getFileWord(input);
        if (command == NULL) {
            break;
        }
        
        if (!strcmp(command, "ADD")) {
            addCommand(input, output, fileList, *root);
        } else if (!strcmp(command, "DEL")) {
            delCommand(input, output, *root, fileList);
        } else if (!strcmp(command, "ADDKW")) {
            addkwCommand(input, output, *root, fileList);
        } else if (!strcmp(command, "DELKW")) {
            delkwCommand(input, output, *root, fileList);
        } else if (!strcmp(command, "FIND")) {
            findCommand(input, output, *root);
        } else if (!strcmp(command, "PRINT")) {
            printCommand(output, *root);
        } else if (!strcmp(command, "TOPK")) {
            topkCommand(input, output, *root);
        } else {
            fprintf(output, "Invalid command\n");
            while ((c = fgetc(input)) != '\n' && c != EOF);
        }
        free(command);
    }
}

int delCommand(FILE *input, FILE *output, Tree root, TFileList *src)
{
    char *id = getFileWord(input);
    if (!id) return 0;
    
    if (!delFileFromFileList(root, src, id)) {
        fprintf(output, "NOT FOUND\n");
        free(id);
        return 0;
    }
    fprintf(output, "OK\n");
    free(id);
    return 1;
}

int addkwCommand(FILE *input, FILE *output, Tree root, TFileList *dest)
{
    char *id = getFileWord(input);
    char *keyword = getFileWord(input);
    if (!keyword)
    {
        if (!id) {
            return 0;
        }
        free(id);
        return 0;
    }
    
    if(!addKWToFile(root, dest, id, keyword)) {
        free(id);
        free(keyword);
        fprintf(output, "NOT FOUND\n");
        return 0;
    }
    free(id);
    fprintf(output, "OK\n");
    return 1;
}

int delkwCommand(FILE *input, FILE *output, Tree root, TFileList *src)
{
    char *id = getFileWord(input);
    char *keyword = getFileWord(input);
    if (!keyword)
    {
        if (!id) {
            return 0;
        }
        free(id);
        return 0;
    }
    if (!delKWFromFile(root, src, id, keyword)) {
        fprintf(output, "NOT FOUND\n");
        free(id);
        free(keyword);
        return 0;
    }
    free(id);
    free(keyword);
    fprintf(output, "OK\n");
    return 1;
}

void printRefList(FILE *output, TRefList list)
{
    while(list) {
        fprintf(output, " %s", list->ref->ID);
        list = list->next;
    }
}

void printTree(FILE *output, Tree root, char *word, int wordOffset)
{
    if (!root) {
        return;
    }

    //process this node
    word[wordOffset] = root->c;
    if (root->fileRefList) {
        word[wordOffset + 1] = '\0';
        fprintf(output, "%s %d", word, getRefListCount(root->fileRefList));
        printRefList(output, root->fileRefList);
        fprintf(output, "\n");
    }

    for (Tree i = root->child; i != NULL; i = i->next) {
        printTree(output, i, word, wordOffset + 1);
    }
}

void printCommand(FILE *output, Tree root)
{
    int printed = 0;
    //allocate memory for a 101 word, the maximum length according to the task
    char *buffer = malloc(101 * sizeof(char));
    if (!buffer) {
        perror("PRINT_COMMAND: Failed to allocate memory\n");
        return;
    }
    for (Tree i = root->child; i != NULL; i = i->next) {
        printed = 1;
        printTree(output, i, buffer, 0);
    }
    if (!printed) {
        fprintf(output, "EMPTY\n");
    }
    free(buffer);
}

TRefCell *maxHeapify(TRefList list)
{
    int heapSize = getRefListCount(list);
    if (!heapSize) {
        return NULL;
    }
    TRefCell *heap = malloc(heapSize * sizeof(TRefCell));
    if (!heap) {
        perror("MAX_HEAPIFY: Failed to allocate memory\n");
        return NULL;
    }
    
    for (int i = 0; i < heapSize; i++)
    {
        insertHeap(heap, *list, i);
        list = list->next;
    }
    return heap;
}

void insertHeap(TRefCell *heap, TRefCell value, int pos)
{
    heap[pos] = value;
    while (pos > 0 &&
           (heap[pos].ref->score > heap[(pos - 1) / 2].ref->score ||
           (heap[pos].ref->score == heap[(pos - 1) / 2].ref->score &&
           strcmp(heap[pos].ref->ID, heap[(pos - 1) / 2].ref->ID) < 0))) {
        TRefCell aux = heap[pos];
        heap[pos] = heap[(pos - 1) / 2];
        heap[(pos - 1) / 2] = aux;
        pos = (pos - 1) / 2;
    }
}

void siftHeap(TRefCell *heap, int i, int heapSize)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = left + 1;

    if (left < heapSize && (heap[left].ref->score > heap[largest].ref->score ||
        (heap[left].ref->score == heap[largest].ref->score && strcmp(heap[left].ref->ID, heap[largest].ref->ID) < 0))) {
        largest = left;
    }

    if (right < heapSize && (heap[right].ref->score > heap[largest].ref->score ||
        (heap[right].ref->score == heap[largest].ref->score && strcmp(heap[right].ref->ID, heap[largest].ref->ID) < 0))) {
        largest = right;
    }
    
    if (largest != i) {
        TRefCell aux = heap[i];
        heap[i] = heap[largest];
        heap[largest] = aux;
        siftHeap(heap, largest, heapSize);
    }
}

void topkCommand(FILE *input, FILE *output, Tree root)
{
    TRefList list;
    int k, heapSize;
    char *keyword = getFileWord(input);
    if (!keyword) {
        return;
    }
    
    fscanf(input, " %d", &k);
    find(&heapSize, &list, root, keyword);
    if (heapSize == 0) {
        fprintf(output, "EMPTY\n");
        return;
    }
    TRefCell *heap = maxHeapify(list);
    if (k > heapSize) {
        k = heapSize;
    }
    fprintf(output, "%d", k);
    
    for (int i = 0; i < k; i++) {
        fprintf(output, " %s", heap[0].ref->ID);
        heap[0] = heap[--heapSize];
        siftHeap(heap, 0, heapSize);
    }
    fprintf(output, "\n");
    free(heap);
    free(keyword);
}

void emptyTree(Tree root)
{
    if (!root) {
        return;
    }
    Tree j;
    for (Tree i = root->child; i != NULL; i = j) {
        j = i->next;
        emptyTree(i);
    }
    TRefList ref = root->fileRefList;
    while (ref) {
        TRefList nextRef = ref->next;
        free(ref);
        ref = nextRef;
    }
    
    free(root);
}
