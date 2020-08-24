 /* This assignment was conducted on repl.it */
#include "PriorityQueue.h"
#include <stdio.h>

int main(){

    int FLAG = TRUE;
    Heap heap; //declare heap.
    HeapInit(&heap); //Initialize the heap.


    while(FLAG == TRUE){

        char menu=0;
        char buffername[20]="\0";
        int bufferkey=0;
        int bufferindex=0;
        Element deleted = {0,"\0"};

        PrintMenu(); //print menu
        scanf("%c",&menu);
        getchar();

        switch(menu){
            
            case 'I':
                if(heap.numOfElem < LEN){
                    printf("Enter name of element: ");
                    scanf("%s",buffername);
                    printf("Enter key value of element: ");
                    scanf(" %d",&bufferkey);
                
                    HeapInsert(&heap, bufferkey, buffername);
                    printf("New element [%s,%d] is inserted.\n",buffername,bufferkey);
                    clearBuffer();
                }
                else{
                    printf("Your PriorityQueue is already full.\n");
                }
                break;
            
            case 'D':
                if(heap.numOfElem != 0){
                    HeapDelete(&heap, &deleted);
                    printf("[%s, %d] is deleted.\n",deleted.name,deleted.key);
                }
                else{
                    printf("Your PriorityQueue is already empty\n");
                }
                break;
            
            case 'R':
                if(heap.numOfElem != 0)
                    printf("[%s, %d]\n",heap.heapArr[1].name,heap.heapArr[1].key);
                else
                    printf("Your PriorityQueue is empty\n");
                break;
            
            case 'K':
                if(heap.numOfElem != 0){
                    printf("Enter index of element: ");
                    scanf("%d",&bufferindex);
                    printf("Enter new key value: ");
                    scanf("%d",&bufferkey);

                    IncreaseKey(&heap, bufferindex, bufferkey);
                    clearBuffer();
                }
                else{
                    printf("Your PriorityQueue is empty\n");
                }
                break;
    
            case 'P':
                if(heap.numOfElem != 0)
                    PrintHeap(&heap);
                else
                    printf("Your PriorityQueue is empty\n");
                break;
            
            case 'Q':
                FLAG = FALSE;
                break;
            
            default: ; //do nothing
        }
    }
    printf("Thank you. Bye!\n");
    return 0;
}
