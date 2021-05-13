#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc(x) mymalloc(x, __FILE__, __LINE__)
#define free(x) myfree(x, __FILE__, __LINE__)

void *mymalloc(int numberBytes, const char *fptr, int line);
void myfree(void *target, const char *fptr, int line);

typedef struct LLNode{
    unsigned int size;
    unsigned int index; 
    char* address;
    struct LLNode* next;
} node;


//will call mymalloc function when malloc is called
//will also be used to print errors and where they are located

void findsize();
void addNode(node *add, int dataSize, int index);
#endif
