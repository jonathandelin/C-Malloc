#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "mymalloc.h"
#include <time.h>
#include <sys/time.h>

static char myblock[4096];

node *head = (void*)myblock; //first node 
void *freePtr = NULL;
int arrayMem = 4096;
//create linked list that stores pointers
//to address of each struct is in char array

void *mymalloc(int numberBytes, const char *fptr, int line){ //if 0 bytes requested
    //printf("allocating %d bytes\n", numberBytes);
    int index=0;
    int inUse = 0;
    int mem = 0;
    node *temp = head;
    findsize(); //remove amount of bytes being allocated from memory size
 
   if((numberBytes - 1) >= arrayMem){ //#bytes requested is greater than size of memory
        printf("ERROR: In file %s line %d, the number of bytes exceeds the limit!", fptr, line);
    } else if (numberBytes ==0){ //if 0 bytes are requested
        return NULL;
    } else if (numberBytes <0){ //if negative bytes are requested
        printf("ERROR: In file %s line %d, cannot allocate negative number of bytes!", fptr, line);
    } else {
        if(temp ==NULL){ //first malloc -> allocate head node
            //head = (node*)(sizeof(node));
            head->size = 4096-sizeof(node);
            //head->next=NULL;
//printf("does it reach here?\n"); 
            head->next = NULL; 
            temp = head; //set temp node = to head
            node *newNode = (void*)((void*)temp+numberBytes+sizeof(node));
            addNode(newNode, numberBytes, index);
            temp->next = newNode; //create link to next node;
            //printf("head is empty, inserted new node with size %d at location %p\n", numberBytes, newNode->address);
            return newNode->address;
        } else if (temp->next == NULL){ //head is already made but no second node
            node *newNode = (void*)((void*)temp+numberBytes+sizeof(node));
            addNode(newNode, numberBytes, index);
            temp->next = newNode;
            //printf("Head isn't empty but .next is so created node with size %d at location %p\n", numberBytes, newNode->address);
            return newNode->address;
        } else {
            //printf("list is not empty, allocating memory..\n");
            node *temp = head;
            while(temp->next!=NULL){ //if there is enough memory to allocate between this node and the next one:
                temp=temp->next;
                if(temp->next!=NULL && (temp->next->index - (temp->size + temp->index))>=numberBytes){
                    index = temp->size + temp->index;
                    node *newNode = (void*)((void*)temp+numberBytes+sizeof(node));
                    addNode(newNode, numberBytes, index);
                    newNode->next = temp->next->next;
                    temp->next = newNode;
                    inUse=1;
                    //printf("allocated block with size %d at address %p\n", newNode->size, newNode->address);
                    return newNode->address;
                } else { 
                // not enough space between the blocks to allocate the memory
                //therefore, iterate throughout array to either find block big enough for the
                //number of bytes requested or until end of memory is reached
                    mem += temp->size;
                    continue;

                }
            } //end of while loop
            //reached end of memory but did not allocate a node
            //if enough space insert node here
            if(!inUse && (4096-temp->index+temp->size)){
                index = temp->size + temp->index;
                node *newNode = (void*)((void*)temp+numberBytes+sizeof(node));
                addNode(newNode, numberBytes, index);
                temp->next = newNode;
                inUse = 1;
                //printf("allocated at END of block with size %d at address %p\n", newNode->size, newNode->address);
                return newNode->address;
            } else { //not enough memory to allocate data:
                printf("ERROR: In file %s line %d, cannot allocate memory size %d due to exceeding limit.\n", fptr, line, numberBytes);
            }
        }
    }
}

void myfree(void *target, const char *fptr, int line){
    int isFree = 0;
    node *temp = head; 

    //printf("Begin freeing pointer %p...\n", target);
	if(target == NULL){ //if trying to free null ptr
        printf("ERROR: In file %s line %d attempting to free NULL pointer\n", fptr, line);
		return;
	} else if(target == freePtr){ // if freeing ptr not malloced()
        printf("ERROR: In file %s line %d attempting to free pointer not allocated by malloc()\n", fptr, line);
        return;
    } else if(temp->next == NULL){ //if freeing addresses that are not pointers OR invalid pointer 
        printf("ERROR: In file %s line %d attempting to free invalid pointer or pointer not malloced().\n", fptr, line);
        return;
    } else {
        node *temp = head->next;
        node *prevNode = head;
        node *nextNode = temp->next;
        while(temp!=NULL){ //if this is target ptr to free:
            if(temp->address == target){
                prevNode->next = nextNode;
                isFree=1;
                target = NULL;
                //printf("Freed address %p\n", temp->address);
                break;
            } else {// did not find target ptr
                if(temp->next==NULL){
                    break;
                }
                temp=temp->next;
                prevNode = prevNode->next;
                nextNode=nextNode->next;
            }
        } //end loop

    }
    if(isFree == 0){
        printf("ERROR: In file %s line %d attempting to free pointer not malloced().\n", fptr, line);
        return;

    }
	freePtr = target;
    return;
}



    void findsize(){ 
    	node *temp = head;
        int allocated = 0;
        while(temp!=NULL){
            allocated += temp->size;
            temp = temp->next;
        }
        arrayMem = (4096-allocated);

    }

    void addNode(node *add, int dataSize, int index){
        //printf("In add node, size is %d\n", dataSize);
        add->address = &myblock[index];
        add->size = dataSize;
        add->index = index; 
        add->next=NULL;
    }
