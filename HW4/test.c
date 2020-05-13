#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct _item{
    int benefit;
    int weight;
    float BenefitPerWeight;
} Item;

void Swap(Item* items, int a, int b){
    Item temp;
    temp.benefit = items[a].benefit;
    temp.weight = items[a].weight;
    temp.BenefitPerWeight = items[a].BenefitPerWeight;

    items[a].benefit = items[b].benefit;
    items[a].weight = items[b].weight;
    items[a].BenefitPerWeight = items[b].BenefitPerWeight;

    items[b].benefit = temp.benefit;
    items[b].weight = temp.weight;
    items[b].BenefitPerWeight = temp.BenefitPerWeight;
}

int Partition(Item* items, int left, int right){
    int pivot = items[left].BenefitPerWeight; 
    int start = left + 1;
    int end = right;
 
    while (start <= end){
        while (start <= right && pivot >= items[start].BenefitPerWeight){
            start++;
        }
        while (end >= (left+1)  && pivot <= items[end].BenefitPerWeight){
            end--; 
        }
        if (start <= end){
            Swap(items, start, end); 
        }
    }
    Swap(items, left, end);
    return end; 
 
}
 
void QuickSort(Item* items, int left, int right){
    if (left <= right){
        int pivot = Partition(items, left, right);
        QuickSort(items, left, pivot - 1);
        QuickSort(items, pivot + 1, right);
    }
}


void Greedy(Item* items, int N, int W){
    // sorting items by benefit/weight
    int FLAG = 0;
    QuickSort(items, 0, N);
    clock_t start, curr;
    start = clock();

    // initialize total_benefit and available weight
    int total_benefit = 0;
    int available = W;

    // if weight of item is over than available, split it
    // else, take it as whole
    for(int i=0 ; i<N ; i++){
        curr = clock();
        if(((double)(curr - start)/CLOCKS_PER_SEC) > 900 ){
            if(FLAG == 0){
                printf("over 15mins\n");
                FLAG = 1;
            }
        }
        if(items[i].weight > available) {
            total_benefit += items[i].benefit * available;
            break;
        }
        else{
            total_benefit += items[i].benefit;
            available -= items[i].weight;
            if(available == 0) break;
        }
    }
    printf("best benefit : %d\n", total_benefit);
}
void DynamicProgramming(Item* items,int N, int W){
    int ** B;
    int FLAG = 0;
    B = (int**) malloc ((W+1) * sizeof(int*));
    clock_t start, curr;
    start = clock();

    for(int i=0 ; i<W+1 ; i++) 
        B[i] = (int*) malloc ((N+1) * sizeof(int));  

    for(int w=0 ; w<W+1 ; w++)
        B[w][0] = 0;
    
    for(int i=0 ; i<N+1 ; i++)
        B[0][i] = 0;
    
    for(int w=1 ; w<W+1 ; w++){
        for(int i=1 ; i<N+1 ; i++){
            curr = clock();
            if(((double)(curr - start)/CLOCKS_PER_SEC) > 900 ){
                if(FLAG == 0){
                    printf("over 15mins\n");
                    FLAG = 1;
                }
            }
            //item i can be part of the solution
            if(items[i].weight <= w){
                if((items[i].benefit + B[w-items[i].weight][i-1])>B[w][i-1]){
                    B[w][i] = items[i].benefit + B[w-items[i].weight][i-1];
                }
                else B[w][i] = B[w][i-1];
            }
            else{
                B[w][i] = B[w][i-1];
            }
        }
    }
    curr = clock();
    printf("duration : %f\n",(float)(curr - start)/CLOCKS_PER_SEC);
    printf("best benefit : %d\n", B[W][N]);
}


int main(){
    int N;
    srand(time(NULL));
    printf("Enter the number of items : ");
    scanf("%d",&N);

    int W = N * 40;
    Item item[N];
    for(int i=0;i<N;i++){
        item[i].benefit = rand() % 300 + 1;
        item[i].weight = rand() % 100 + 1;
        item[i].BenefitPerWeight = item[i].benefit / item[i].weight;
    }

    printf("Choose option to solve knapsack problem\n");
    printf("1. Greedy\n");
    Greedy(&item[0],N,W);
    printf("\n\n2. Dynamic Programming\n");
    DynamicProgramming(&item[0],N,W);
    printf("3. Branch and bound\n");

    return 0;
}
