#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include<ctype.h>
//#define DEBUG

int getTotalLine(char *filename){
    FILE *fp;
    int line=0;
    char buffer[128];
    char c = 0;
    fp=fopen(filename,"r");
    while(!feof(fp)){
        fgets(buffer, sizeof(buffer)/sizeof(char), fp);
        line++;
    }
    fclose(fp);
    return line;
}

void fileToMatrix(char* filename, char** element, int** distance, int N){
    char buffer[256];
    int cursor = 0;
    int col = 0, row = 0;
    FILE * fp;
    fp = fopen(filename,"r");
    if(fp == NULL){
        printf("Failed to open file\n");
        exit(0);
    }

    fgets(buffer, sizeof(buffer)/sizeof(char), fp);

    col = 0;
    while(!feof(fp)){
        fgets(buffer, sizeof(buffer)/sizeof(char), fp);
        for(int i=0;buffer[i]!='\n';i++){
            if(buffer[i]=='I' && buffer[i+1]=='N' && buffer[i+2]=='F'){
                distance[row][col] = 99999;
                col++;
            }
            if(isdigit(buffer[i])){
                int j = 0;
                char digitbuffer[16];
                for(int i=0;i<16;i++) digitbuffer[i]='\0';
                while(isdigit(buffer[i])){
                    digitbuffer[j] = buffer[i];
                    i++;
                    j++;
                }
                distance[row][col] = atoi(digitbuffer);
                col++;
            }
        }
        row++;
        col = 0;
    }

    printf("Input : \n");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d\t",distance[i][j]);
        }
        printf("\n");
    }
    printf("========================================\n");

    fclose(fp);
}
int calDist(int** dist, int* list, int* path, int new, int N){
    //원래 리스트보다 새로 추가된 path를 고려했을 때 값이 더 작으면 업데이트
    //새로운 path란 path[new]를 말한다!
    int base = list[new];
    for(int i=0;i<N;i++){
        if(base + dist[new][i] < list[i]){
            list[i] = base + dist[new][i];
        }
    }

    int min = 99999;
    int min_idx = -1;
    int pathlen=0;
    int used = 0;
    #ifdef DEBUG
        printf("cal dist path\n");
        for(int i=0;i<N;i++){
            printf("%d ",path[i]);
        }
        printf("\n");
        #endif
    for(int i=0;i<N;i++){
        if(path[pathlen] == -1) break;
        pathlen++;
    }
    #ifdef DEBUG
    printf("pathlen is %d\n",pathlen);
    #endif
    //path에 없는 친구들 중에 list의 min값
    for(int i=0;i<N;i++){
        used = 0;
        if(list[i] < min){
            for(int j=0;j<=pathlen;j++){
                if(path[j]==i){
                    used = 1;
                }
            }
            if(used == 0){
                min = list[i];
                min_idx = i;
            }
        }
    }

    #ifdef DEBUG
    printf("path :\n");
    for(int i=0;i<N;i++){
        printf("%d ",path[i]);
    }
    printf("\nlist :\n");
    for(int i=0;i<N;i++){
        printf("%d ",list[i]);
    }
    printf("\n");
    #endif

    return min_idx;
}

void dijkstra(int start, int N, int** distance){
    //start -> shortest path를 찾는다.
    int* path;
    path = (int*)malloc(sizeof(int)*N);

    for(int i=0;i<N;i++) path[i] = -1; //initialize the path
    path[0] = start;

    int* list;
    list = (int*)malloc(sizeof(int)*N);
    for(int i=0;i<N;i++) list[i] = 99999;
    list[start]=0;
    
    //리스트 만들고 min값 찾고... 이거 N번 반복
    int next = start;
    for(int i=0;i<N;i++){
        path[i] = next;
        #ifdef DEBUG
        printf("\ndijstra path\n");
        for(int i=0;i<N;i++){
            printf("%d ",path[i]);
        }
        printf("\n");
        #endif
        int buf=calDist(distance, list, path, next, N);
        next = buf;
        #ifdef DEBUG
        printf("next : %d\n",next);
        #endif
    }

    for(int i=0;i<N;i++){ //update the distance matrix
        distance[start][i]=list[i];
    }
}

void floyd(int** distance, int N){
    for(int k=0;k<N;k++){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(distance[i][j] > distance[i][k] + distance[k][j]){
                    distance[i][j] = distance[i][k] + distance[k][j];
                }
            }
        }
    }
}

int main(){
    char filename[64];
    int N = 0;
    int** distance, **dijkstra_dist, **floyd_dist;
    char** element;
    clock_t start, end;

    printf("Enter the name of file : ");
    scanf("%s",filename);
    N = getTotalLine(filename)-1; //N : 노드 몇개인지!
    element = (char**)malloc(sizeof(char)*N);
    for(int i=0;i<N;i++){
        element[i]=(char*)malloc(sizeof(char)*32);
    }
    distance = (int**)malloc(sizeof(int*)*N);
    dijkstra_dist = (int**)malloc(sizeof(int*)*N);
    floyd_dist = (int**)malloc(sizeof(int*)*N);

    for(int i=0;i<N;i++){
        distance[i]=(int*)malloc(sizeof(int)*N);
        dijkstra_dist[i]=(int*)malloc(sizeof(int)*N);
        floyd_dist[i]=(int*)malloc(sizeof(int)*N);
    }

    fileToMatrix(filename, element, distance, N);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            floyd_dist[i][j]=dijkstra_dist[i][j] = distance[i][j];

    start = clock();
    for(int start = 0;start<N;start++){
        dijkstra(start, N, dijkstra_dist);
    }
    end = clock();
    printf("It took %f seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n",((float)(end-start))/CLOCKS_PER_SEC);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d\t",dijkstra_dist[i][j]);
        }
        printf("\n");
    }
    printf("========================================\n");
 

    //floyd
    start = clock();
    floyd(floyd_dist, N);
    end = clock();
    printf("It took %f seconds to compute shortest path between cities with Floyd algorithm as follows.\n",((float)(end-start))/CLOCKS_PER_SEC);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            printf("%d\t",floyd_dist[i][j]);
        }
        printf("\n");
    }
    printf("========================================\n");

    return 0;
}