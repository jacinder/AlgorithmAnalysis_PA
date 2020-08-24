#include<stdio.h>
#include<stdlib.h>
#include<time.h>
float recursive_solution(int n,int k);
float DP_solution(int n,int k);

int main(){
    int n=0,k=0;
    float result=0.F;
    clock_t start=0,end=0;
    float duration1=0.F,duration2=0.F;
	//User enter the numbers for n,k
	printf("Enter a number for n : ");
	scanf("%d",&n);
	printf("Enter a number for k : ");
	scanf("%d",&k);
	

	//recursive solution
	start = clock();
	result = recursive_solution(n,k);
	end = clock();
	duration1 = (float)(end-start)/CLOCKS_PER_SEC;
	printf("\nrecursive C(%d,%d)\t: %.0f\n",n,k,result);
	printf("duration time\t\t: %.2f sec\n",duration1);

	//DP solution
	start = clock();
	result=DP_solution(n,k);
	end = clock();
	duration2 = (float)(end-start)/CLOCKS_PER_SEC;
	printf("\nDP C(%d,%d)\t\t\t: %.0f\n",n,k,result);
	printf("duration time\t\t: %.2f sec\n",duration2);
	
	

    return 0;
}
float recursive_solution(int n,int k){
    if(n==k)
        return 1;
    else if(n>=0 && k==0)
        return 1;
    else if(n>0 && k>0)
        return recursive_solution(n-1,k-1)+recursive_solution(n-1,k);
    else {
        printf("Incorrect input value\n");
        exit(1);
    };
}
float DP_solution(int n,int k){

	int i=0,j=0;
    float table[n+1][k+1];
    //Initialize the table
    for(int i=0;i<n+1;i++)
        for(int j=0;j<k+1;j++)
            table[i][j]= -1;

	//for i <- 1 to n+1 do C(0,i)<-0
    for(i=0;i<=k;i++)
        table[0][i] = 0;

    //for i <- 1 to k+1 do C(i,i)<-1
    for(i=0;i<=k;i++)
        table[i][i] = 1;
    //for i <- 1 to n+1 do C(i,0)<-1
    for(i=0;i<=n;i++)
        table[i][0] = 1;

    //for i <- 2 to n+1
    //  for j<- 2 to k+1
    //      C(i,j)<-C(i-1,j-1)+C(i-1,j)
    for(i=1;i<=n;i++){
        for(j=1;j<=k;j++){
            //if table is empty
            if(table[i][j]==-1){
                table[i][j] = table[i-1][j-1] + table[i-1][j];
            }
        }
    }
    return table[n][k];
}