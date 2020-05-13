#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

#define TRUE 1
#define FALSE 0

#define LEN 30

typedef char NAME[20];
typedef int KEY;

typedef struct _Element{

    KEY key;
    NAME name;
    
} Element;

typedef struct _heap{

    int numOfElem; //Number of stored data
    Element heapArr[LEN];
    
} Heap;

void PrintMenu(void);
int GetParentIDX(int idx);
int GetLChildIDX(int idx);
int GetRChildIDX(int idx);
int GetHiPriChildIDX(Heap *ph, int idx);
void HeapInit(Heap* ph);
void HeapInsert(Heap * ph, KEY k, NAME n);
void HeapDelete(Heap* ph, Element* deleted);
void IncreaseKey(Heap* ph, int idx, KEY k);
void PrintHeap(Heap *ph);
void clearBuffer(void);


#endif