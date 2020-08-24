//“my program works for greedy and DP solution only.”
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
	// return index of node whose bound is highest in the leafArr
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
    float bound = benefit;
    for(int i=item_idx;i<N;i++){
        if(items[i].weight > available) {
            bound += items[i].BenefitPerWeight * available;
            break;
        }
        else{
            bound += items[i].benefit;
            available -= items[i].weight;
            if(available == 0) break;
        }
    }
    return bound;
}

int MakeLeftNode(Tree* tree, Item* items, int idx, int item_idx, int N, int W){
    int parentIDX = idx/2;
    int benefit = tree->treeArr[parentIDX].benefit + items[item_idx].benefit; //benefit
    int weight = tree->treeArr[parentIDX].weight + items[item_idx].weight; //weight
    if(weight > W) return 0;
    float bound = Getbound(items, benefit, W-weight, N, item_idx+1); //bound

    if(benefit > max_benefit) max_benefit = benefit; //update max_benefit
    tree->treeArr[idx].benefit = benefit;
    tree->treeArr[idx].weight = weight;
    tree->treeArr[idx].bound = bound;
    return 1;
}

int MakeRightNode(Tree* tree, Item* items, int idx, int item_idx, int N, int W){

    int parentIDX = (idx-1)/2;
    int benefit = tree->treeArr[parentIDX].benefit;
    if(benefit > max_benefit) max_benefit = benefit;
    int weight = tree->treeArr[parentIDX].weight;
    float bound = Getbound(items, benefit, W-weight, N, item_idx+1);

    tree->treeArr[idx].benefit = benefit;
    tree->treeArr[idx].weight = weight;
    tree->treeArr[idx].bound = bound;
    return 1;
}

void BranchAndBound(Item* items, int N, int W, FILE* fp){
    printf("\n");
    int FLAG = 0;
	float LEN = 2;
	//declare and initialize tree
    Tree tree;
    tree.treeArr = (Element*)malloc(sizeof(Element)*2);
    tree.leafArr = (int*)malloc(sizeof(int)*2);
    
    clock_t start, curr;
    start = clock();
    //first node
    int available=W;
    float bound = Getbound(items, 0, W, N, 0); //bound of first node
    tree.treeArr[1].benefit = 0;
    tree.treeArr[1].weight = 0;
    tree.treeArr[1].bound = bound;
    tree.leafArr[1]=1;
    
    for(int item=0;item<N;item++){
        int parent = maxLeaf(&tree, N, pow(2,N));
        curr = clock();
        if(((double)(curr - start)/CLOCKS_PER_SEC*1000) > 0.9 ){
            if(FLAG == 0){
                printf("over 15mins\n");
                FLAG = 1;
            }
        }
        if((tree.treeArr[parent].bound > max_benefit) && (tree.treeArr[parent].weight <= W)){
            int LEN = pow(2,item+2);
			Element* treeArrReturn = realloc(tree.treeArr ,sizeof(Element) * LEN);
			if(treeArrReturn == NULL){
				printf("Error with reallocation of treeArr\n");
                return ;
            }
            tree.treeArr = treeArrReturn;
			
			int* leafArrReturn = realloc(tree.leafArr ,sizeof(int) * LEN);
			if(leafArrReturn == NULL){
				printf("Error with reallocation of leafArr\n");
                return ;
            }
            tree.leafArr = leafArrReturn;

            int leftChild = parent*2;
            int rightChild = parent*2+1;

            int left = MakeLeftNode(&tree, items, leftChild, item, N, W); //include this item
            int right = MakeRightNode(&tree, items, rightChild, item, N, W); //not include this item

            tree.leafArr[parent] = 0;
            if(left == 1) tree.leafArr[leftChild] = 1;
            if(right == 1) tree.leafArr[rightChild] = 1;
        }
    }
    free(tree.treeArr);
    free(tree.leafArr);
    curr = clock();
	float duration = (float)(curr - start)/CLOCKS_PER_SEC/1000;
    printf("duration : %f\n",duration);
    printf("best benefit : %d\n",max_benefit);
	fprintf(fp,"%f/%d\n",duration, max_benefit);
}

void Greedy(Item* items, int N, int W, FILE* fp){
    // sorting items by benefit/weight
    QuickSort(items, 0, N-1);

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
        if(((double)(curr - start)/CLOCKS_PER_SEC*1000) > 0.9 ){
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
	float duration = (float)(curr - start)/CLOCKS_PER_SEC/1000;
    printf("duration : %f\n",duration);
    printf("best benefit : %d\n", total_benefit);
	fprintf(fp,"%f/%d\t",duration,total_benefit);
}

void DynamicProgramming(Item* items,int N, int W,FILE * fp){
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
            if(((double)(curr - start)/CLOCKS_PER_SEC*1000) > 0.9 ){
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
    curr = clock();
	float duration = (float)(curr - start)/CLOCKS_PER_SEC/1000;
    printf("duration : %f\n",duration);
    printf("best benefit : %d\n", B[N][W]);
	fprintf(fp,"%f/%d\t",duration, B[N][W]);
	free(B);
}


int main(){
    int N;
	int num[9]={10,100,500,1000,3000,5000,7000,9000,10000};
	FILE * fp = fopen("output.txt","w");
    srand(time(NULL));
    // printf("Enter the number of items : ");
    // scanf("%d",&N);
	fprintf(fp,"item#\tGreedy\tDP\tB&B\n");
	for(int n=0;n<9;n++){
		N=num[n];
		fprintf(fp,"%d\t",N);
		int W = N * 40;
		Item item[N];
		Item sorted_item[N];
		for(int i=0;i<N;i++){
			sorted_item[i].benefit = item[i].benefit  = rand() % 300 + 1;
			sorted_item[i].weight =item[i].weight = rand() % 100 + 1;
			sorted_item[i].BenefitPerWeight = item[i].BenefitPerWeight = ((float)item[i].benefit / item[i].weight);
		}

		printf("1. Greedy\n");
		Greedy(&sorted_item[0],N,W,fp);

		printf("\n2. Dynamic Programming\n");
		DynamicProgramming(&item[0],N,W,fp);

		printf("\n3. Branch and bound");
		BranchAndBound(&sorted_item[0],N,W,fp);
        printf("\n");
	}
    fclose(fp);

    return 0;
}
