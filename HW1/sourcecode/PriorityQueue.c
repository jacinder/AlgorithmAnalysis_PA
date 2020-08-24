#include "PriorityQueue.h"
#include <stdio.h>
#include <string.h>

void PrintMenu(void){
    printf("\n*********** MENU ***********\n");
    printf("I : Insert new element into queue.\n");
    printf("D : Delete element with largest key from queue.\n");
    printf("R : Retrieve element with largest key from queue.\n");
    printf("K : Increase key of element in queue.\n");
    printf("P : Print out all element in queue.\n");
    printf("Q : Quit.\n");
}
void HeapInit(Heap *ph){ ph->numOfElem = 0;}
int GetParentIDX(int idx){ return idx/2;}
int GetLChildIDX(int idx){ return idx*2;}
int GetRChildIDX(int idx){ return GetLChildIDX(idx)+1;}
//return child index whose priority is higher.
int GetHiPriChildIDX(Heap *ph, int idx){ 
    //return the index of child whose key is higher than the other one.
    if(GetLChildIDX(idx) > ph->numOfElem)
        return 0;
    else if(GetLChildIDX(idx) == ph->numOfElem)
        return GetLChildIDX(idx);
    else{
        if(ph->heapArr[GetLChildIDX(idx)].key > ph->heapArr[GetRChildIDX(idx)].key)
            return GetLChildIDX(idx);
        else
            return GetRChildIDX(idx);
    }
}
//insert a HeapElem to the heap.
void HeapInsert(Heap * ph, KEY k, NAME n){
    int idx = ph->numOfElem+1;
    Element newelem = {k,"\0"}; //to declare a new HeapElem
    strcpy(newelem.name, n);

    //to find its right place.
    while(idx != 1){

        //if the new HeapElem's priority is higher than of parent.
        if(k > (ph->heapArr[GetParentIDX(idx)].key)){

            ph->heapArr[idx] = ph->heapArr[GetParentIDX(idx)]; //parent -> child
            idx = GetParentIDX(idx);

        }
        else break;
    }
    //new elem will go to the place of parent
    ph->heapArr[idx] = newelem;
    ph->numOfElem += 1;
}

void HeapDelete(Heap * ph, Element* deleted){

    strcpy(deleted->name, (ph->heapArr[1]).name);
    deleted->key = (ph->heapArr[1].key);    
	Element lastElem = ph->heapArr[ph->numOfElem];

	int parentIdx = 1;
	int childIdx = 0;

	while(1){
        childIdx = GetHiPriChildIDX(ph, parentIdx);
		if(lastElem.key >= ph->heapArr[childIdx].key)
			break;
		ph->heapArr[parentIdx] = ph->heapArr[childIdx];
		parentIdx = childIdx;
	}
	ph->heapArr[parentIdx] = lastElem;
	ph->numOfElem -= 1;
}

void IncreaseKey(Heap* ph, int idx, KEY k){
    int index = idx;
    if (k < ph->heapArr[idx].key){
        printf("new key is smallerthan current key\n");
        return ;
    }
    ph->heapArr[idx].key = k;
    while(index>1 && (ph->heapArr[GetParentIDX(index)].key < ph->heapArr[index].key)){
        //exchange A[i] with A [parent(i)]
        char namebuffer[20]="\0";
        strcpy(namebuffer, ph->heapArr[index].name);

        ph->heapArr[index].key = ph->heapArr[GetParentIDX(index)].key;
        strcpy(ph->heapArr[index].name, ph->heapArr[GetParentIDX(index)].name);

        ph->heapArr[GetParentIDX(index)].key = k;
        strcpy(ph->heapArr[GetParentIDX(index)].name,namebuffer);

    }
}

void PrintHeap(Heap *ph){
    for(int i=1; i <= ph->numOfElem ; i++)
        printf("[%s, %d] ",ph->heapArr[i].name, ph->heapArr[i].key);
    printf("\n");
}

void clearBuffer(void){
    while (getchar() != '\n');
}