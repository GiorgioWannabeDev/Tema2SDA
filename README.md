### CHELES Giorgio - 312CB

# Structures

**TKWCell**

    This is a simply linked list cell that contains a string (*keyword) and a
    nter to the next cell (*next).

**TKWList**

    This is a pointer to a TKWCell.

**TFileCell**

    This is a doubly linked list cell that contains a string that acts as an
    indentifier (*ID), an integer value that doubles as a score (*score) and two
    pointers that point to the next cell and the previous cell respectively
    (*next/*previous).

**TFilePtr**

    This is a pointer to a TFileCell.

**TFileList**

    This a structure that contains two pointers(TFilePtr) to the head and the tail
    of the doubly linked list of files.

**TRefCell**

    This is simply linked list cell that contains a pointer(TFilePtr) to a file
    (ref) and a pointer to the next cell(*next).

**TRefList**

    This is a pointer to a TRefCell.

**Node**

    This is a cell that contains a character(c), a pointer(TRefList) to a simply
    linked list of references(fileRefList) and two pointers:

    *child - points to the direct descendant;
    *next - points to the next sibling of the current node;

**Tree**

    This is a pointer to Node.

# Functions

**getFileWord(FILE src)**

    Function reads and returns a pointer to a memory block readily allocated
    containing word from src by iterating characters in the source file until
    in encounters a space/newline/EOF or otherwise.

**addFileToRefList(dest, associatedCell)**

    Function adds a new TRefCell entry in the dest list. The entries are inserted
    in such a way that they remain lexicographically ordered. If such an entry
    already exists, we abort. Function returns 1 if we have successfully added a
    new cell, 0 if such a cell already exists or function failed to allocate
    memory for a new entry.

**addKeywordToTree(dest, keyword, associatedFile)**

    Function adds a keyword in the dest Tree by iterating through the characters
    in keyword, searching them at the current level in the tree and follows the
    path if the character is found or starts creating a new branch node by node
    if no such character exists. Function returns 1 if a new branch has been added,
    0 if the keyword was already present, or -1 if it has encountered any errors
    mid-run. In the case a keyword is present or has just been added, we call
    addFileToRefList to insert the associatedFile in the corresponding terminal node's
    fileRefList.

**addFileToFileList(dest, x)**

    Function adds TFilePtr x to the doubly linked list contained within dest.

**createFile(id, score, keywords)**

    Function takes a char pointer to an ID, an integer score TKWList of keywords
    and creates and returns a new fileCell structure containing all of them.
    Note: pointers are not created within the functions and neither are they copied.
    Once the function returns, the newly created structure will own these memory blocks.

**addKeywordToTKList(dest, keyword)**

    Function takes a keyword and attempts to add it to TKWLIst dest. If such a keyword
    already exists, it returns 0. Otherwise, it creates a new structure and it is
    inserted at the start of the list.
    Note: keyword pointer is not created within the functions and neither is it
    copied. Once the function returns, the newly created structure will own this memory
    block.

**checkFileExists(fileList, fileName)**
    
    Function iterates through the doubly linked list of files and if a file with
    ID == fileNames is found, it returns 1, otherwise 0.

**emptyTKWList(list)**

    Function empties keyword list by interating through the list and freeing all
    memory.

**emptyFileList(list)**

    Function empties file list by iterating through the list and freeing all memory.
    In order to free keywords list, emptyTKWList is called within this function.

**delKeywordFromTree(dest, keyword)**

    Function recursively deletes a branch that contains a certain keyword. Once it
    reaches the end of the keyword, it checks if that node has children, if it does,
    we return 0 to signal the previous recursive call to not delete anything. If it
    has no children, we start moving up the recursive chain until we reach a node
    that has a non-void fileRefList or it still has children even after our deletion.
    In that case we continue returning 0es to signal the rest of the calls to not
    delete any more nodes.

**delRefFromTree(dest, keyword, associatedFile)**

    Function iterates through keyword's associated branch until it reaches the last
    character node. Once its there, it deletes the associatedFile reference from the
    fileRefList and returns 1 if after this operation, said list remains empty.
    Otherwise, any other outcome that removes a reference or not is considered successful
    and returns 0.

**delFileFromFileList(root, src, id)**

    Function deletes file entry with id from src list by freeing memory, calling
    deleteRefFromTree and then delKeywordFromTree if the former returned 1. The file
    is found by iterating through the source.
    Function returns 1 if successful or 0 if such an id doesnt exist.

**addKWToFile(root, dest, id, keyword)**

    Function iterates through dest doubly linked list of files and if id is found,
    it attempts to add the keyword to the list of keywords and the tree.
    If there is no file with the provided it, function returns 0, else returns 1.

**delKWFromFile(root, dest, id, keyword)**

    Function deletes keyword from keywords list contained by file cell with id.
    If id is not found, we return 0. If keyword is not found among the found cell
    keywords, we return 1, we consider this a success. Function calls delRefFromTree
    and delKeywordFromTree to remove the keyword from the tree and, if the keywords
    list remains empty after the previous operations, we also call delFileFromFilelist
    in order to oust that file. We return 1 at the very end.

**find(returnValue, list, root, keyword)**

    Function returns the fileRefList pointer and the number of references it contains
    obtained from keyword's last character node. If no such word exists in the tree,
    we return 0 and NULL.

**getRefListCount(list)**

    Function terates, counts and returns the number of entries inside list.

**printTree(input, root, word, wordOffset)**

    Function searches the tree using a DFS algorithm. It contructs a word at every
    node based on the level and once it reaches a non void fileRefList, it prints
    the word, the reference count and calls printRefList to print said references.

**maxHeapify(list)**

    Function allocates memory for a vector that contains all references from list
    and inserts them into the heap one by one using the insertHeap function.
    Function returns the resulting heap.

**insertHeap(heap, value, pos)**

    Function inserts value at heap[pos] and its promoted until it reaches position 0
    or its ascendant's score is bigger. If the score is the same, it is promoted only
    if our value's id is lexicographically smaller than the its ascendant.


**siftHeap(heap, i, heapSize)**

    Function demotes("sifts") the cell placed at pos i while it's score is bigger
    than its descendants' or the cell's id is lexicographically smaller if a tiebreaker
    is required.

**emptyTree(root)**

    Function uses DFS algorithm to empty the tree and deallocate memory.


**commandLine(input, output, fileList, root)**

    This function is the core of our program. It reads from input a number of
    commands and enters a loop that reads a command at every iteration, calling
    the appropriate function that handles the parameters and the logic function calls.

    The command keywords our commandLine expects are as follows:
    ADD --> calls addCommand()
    DEL --> calls delCommand()
    ADDKW --> calls addkwCommand()
    DELKW --> calls delkwCommand()
    FIND --> calls findCommand()
    PRINT --> calls printCommand()
    TOPK --> calls topkCommand()
    everything else --> file cursor is moved to the next line

**addCommand(input, output, fileList, Tree root)**

    Function reads parameters. It check if the file ID is already in the system
    using checkFileExists() function, if it does, we allocate memory and add
    a new cell to the file system and tree and print 'OK', otherwise we print
    'EXISTS'.

**delCommand(input, output, root, src)**

    Function reads parameters. If the delFileFromFilelist() fails, we print
    'NOT FOUND', else 'OK'.

**addkwCommand(input, output, root, dest)**

    Function reads parameters. If addKWToFile() fails we print 'NOT FOUND' else
    'OK'.

**delkwCommand(input, output, root, src)**

    Function reads parameters. If delKWFromFile() fails we print 'NOT FOUND' else
    'OK'

**findCommand(input, output, root)**

    Function reads parameters. If find() returns 0 number of files found, we print
    'EMPTY', else we print the number of file references and the ids of said references.

**printCommand(output, root)**

    Function reads parameters. If root has no children then we have no keywords, so
    function outputs 'EMPTY'. Else, function iterates through root's children and
    calls printTree() on them.

**topkCommand(input, output, root)**

    Function reads parameters. find() is called and if the returned heapSize is 0
    then we output 'EMPTY'. Else, we create the heap using heapify(). If the provided
    k is way bigger than our heapSize then we equate k to heapSize. Function starts
    a for loop that iterates k times and each time it prints the top of the heap's ref
    ID, sets the top to the last element of our heap, removing the top element, and sifts
    it.
