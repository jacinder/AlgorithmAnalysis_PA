#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int max_benefit=0;

typedef struct _element{
    int benefit;
    int weight;
    float bound;
} Element;

typedef struct _tree{
    Element* treeArr;
    int* leafArr;
} Tree;

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
    float pivot = items[left].BenefitPerWeight; 
    int start = left+1;
    int end = right;

 
    while (start <= end){
        while (start <= right && pivot <= items[start].BenefitPerWeight){
            start++; 
        }
        while (end >= (left+1)  && pivot >= items[end].BenefitPerWeight){
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

int maxLeaf(Tree* tree, int N, int LEN){
    // tree->leafArr에서 가장 큰 값의 바운드를 가진 노드의 인덱스를 리턴하는 함수.
    float max = 0;
    int max_idx = 0;
    for(int i=0;i<LEN;i++){
        if(tree->leafArr[i] == 1){
            if(tree->treeArr[i].bound > max){
                max = tree->treeArr[i].bound;
                max_idx = i;
            }
        }
    }
    return max_idx;
}

float Getbound(Item* items, int benefit,int available, int N, int item_idx){
    // 이 상황에서 앞으로 나올 수 있는 최고의 경우
    float bound = benefit; //지금까지의 benefit
    // printf("\n지금까지 benefit %d\n",benefit);
    // printf("item %d번부터 더할거임\n",item_idx);
    for(int i=item_idx;i<N;i++){
        if(items[i].weight > available) {
            // ("item %d는 일부만 더함\n",i);
            bound += items[i].BenefitPerWeight * available;
            break;
        }
        else{
            // printf("item %d는 whole로 더함\n",i);
            bound += items[i].benefit;
            available -= items[i].weight;
            if(available == 0) break;
        }
    }
    return bound;
}

int MakeLeftNode(Tree* tree, Item* items, int idx, int item_idx, int N, int W){
    int parentIDX = idx/2;
    // printf("node %d : ",idx);
    
    int benefit = tree->treeArr[parentIDX].benefit + items[item_idx].benefit; //benefit
    // printf("benefit %d ",benefit);
    
    int weight = tree->treeArr[parentIDX].weight + items[item_idx].weight; //weight
    if(weight > W) return 0;
    // printf("weight %d ",weight);
    
    float bound = Getbound(items, benefit, W-weight, N, item_idx+1); //bound
    // printf("bound %f\n",bound);
    // printf("max_benefit %d\n",max_benefit);

    if(benefit > max_benefit) max_benefit = benefit; //update max_benefit
    tree->treeArr[idx].benefit = benefit;
    tree->treeArr[idx].weight = weight;
    tree->treeArr[idx].bound = bound;
    return 1;
}

int MakeRightNode(Tree* tree, Item* items, int idx, int item_idx, int N, int W){
    // printf("node %d : ",idx);
    int parentIDX = (idx-1)/2;
    int benefit = tree->treeArr[parentIDX].benefit;
    if(benefit > max_benefit) max_benefit = benefit;
    // printf("benefit %d ",benefit);

    int weight = tree->treeArr[parentIDX].weight;
    // printf("weight %d ",weight);

    float bound = Getbound(items, benefit, W-weight, N, item_idx+1);
    // printf("bound %f\n",bound);
    // printf("max_benefit %d\n",max_benefit);

    tree->treeArr[idx].benefit = benefit;
    tree->treeArr[idx].weight = weight;
    tree->treeArr[idx].bound = bound;
    return 1;
}

void BranchAndBound(Tree* tree, Item* items, int N, int W){
    for(int i=0;i<N;i++){
        printf("item %d : ",i);
        printf("benefit %d ",items[i].benefit);
        printf("weight %d ",items[i].weight);
        printf("BenefitPerWeight %f\n",items[i].BenefitPerWeight);
    }
    int FLAG = 0;
    
    clock_t start, curr;
    start = clock();
    //first node
    int available=W;
    float bound = Getbound(items, 0, W, N, 0); //bound of first node
    tree->treeArr[1].benefit = 0;
    tree->treeArr[1].weight = 0;
    tree->treeArr[1].bound = bound;
    
    printf("node 1 : benefit 0 weight 0 bound %f\n",bound);
    tree->leafArr[1]=1;
    
    for(int item=0;item<N;item++){
        int parent = maxLeaf(tree, N, pow(2,N));
        curr = clock();
        if(((double)(curr - start)/CLOCKS_PER_SEC) > 900 ){
            if(FLAG == 0){
                printf("over 15mins\n");
                FLAG = 1;
            }
        }
        if((tree->treeArr[parent].bound > max_benefit) && (tree->treeArr[parent].weight <= W)){
            int leftChild = parent*2;
            int rightChild = parent*2+1;

            // printf("parent : %d\n",parent);
            int left = MakeLeftNode(tree, items, leftChild, item, N, W); //include this item
            int right = MakeRightNode(tree, items, rightChild, item, N, W); //not include this item

            tree->leafArr[parent] = 0;
            if(left == 1) tree->leafArr[leftChild] = 1;
            if(right == 1) tree->leafArr[rightChild] = 1;
        }
    }
    curr = clock();
    printf("duration : %f\n",(float)(curr - start)/CLOCKS_PER_SEC);
    printf("best benefit : %d\n",max_benefit);
}

void Greedy(Item* items, int N, int W){
    // sorting items by benefit/weight
    QuickSort(items, 0, N-1);
    // for(int i=0;i<N;i++){
    //     printf("item %d : ",i);
    //     printf("benefit %d ",items[i].benefit);
    //     printf("weight %d ",items[i].weight);
    //     printf("BenefitPerWeight %f\n",items[i].BenefitPerWeight);
    // }

    int FLAG = 0;
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
            total_benefit += items[i].BenefitPerWeight * available;
            break;
        }
        else{
            total_benefit += items[i].benefit;
            available -= items[i].weight;
            if(available == 0) break;
        }
    }
    curr = clock();
    printf("duration : %f\n",(float)(curr - start)/CLOCKS_PER_SEC);
    printf("best benefit : %d\n", total_benefit);
}

void DynamicProgramming(Item* items,int N, int W){
    int FLAG = 0;
    int** B;
    B=(int**)malloc(sizeof(int*)*(N+1));
    for(int i=0;i<N+1;i++){
        B[i]=(int*)malloc(sizeof(int)*(W+1));
    }
    clock_t start=0, curr=0;
    start = clock();

    for(int i=0;i<N+1;i++){
        B[i][0]=0;
    }
    for(int w=0;w<W+1;w++){
        B[0][w]=0;
    }
    for (int i = 1; i <= N; i++){
        for (int w = 1; w <= W; w++){
            curr = clock();
            if(((double)(curr - start)/CLOCKS_PER_SEC) > 900 ){
                if(FLAG == 0){
                    printf("over 15mins\n");
                    FLAG = 1;
                }
            }
            if (items[i-1].weight <= w){
                if(items[i-1].benefit + B[i-1][w-(items[i-1].weight)] > B[i-1][w]){
                    B[i][w] = items[i-1].benefit + B[i-1][w-(items[i-1].weight)];
                }
                else B[i][w] = B[i-1][w];
            }
            else
                B[i][w] = B[i-1][w];        
            }
    }
    // free(B)
    curr = clock();
    printf("duration : %f\n",(float)(curr - start)/CLOCKS_PER_SEC);
    printf("best benefit : %d\n", B[N][W]);
}


int main(){
    int N;
    srand(time(NULL));
    printf("Enter the number of items : ");
    scanf("%d",&N);

    int W = N * 40;
    //;
    int benefit[5]={286, 2, 89, 28, 101};
    int weight[5]={45,29,53,22,4};
    Item item[N];
    Item sorted_item[N];
    for(int i=0;i<N;i++){
        sorted_item[i].benefit = item[i].benefit  = rand() % 300 + 1;
        sorted_item[i].weight =item[i].weight = rand() % 100 + 1;
        sorted_item[i].BenefitPerWeight = item[i].BenefitPerWeight = ((float)item[i].benefit / item[i].weight);
    }

    //declare and initialize tree
    int LEN = pow(2,N);
    Tree tree;
    tree.treeArr = (Element*)malloc(sizeof(Element)*LEN);
    tree.leafArr = (int*)malloc(sizeof(int)*LEN);

    for(int i=0;i<LEN;i++){
        tree.treeArr[i].benefit = 0;
        tree.treeArr[i].weight = 0;
        tree.treeArr[i].bound = 0;
    }
    for(int i=0;i<LEN;i++) tree.leafArr[i] = 0;

    printf("1. Greedy\n");
    Greedy(&sorted_item[0],N,W);

    printf("\n2. Dynamic Programming\n");
    DynamicProgramming(&item[0],N,W);

    // printf("\n3. Branch and bound\n");
    // BranchAndBound(&tree,&sorted_item[0],N,W);

    return 0;
}