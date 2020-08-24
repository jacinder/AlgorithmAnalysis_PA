#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
typedef struct _node{
    char            value;
    struct _node*   next; 
}node;

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

void fileToMatrix(char* filename, char* element, int** matrix){
    char buffer[128];
    int col = 0, row = 0;
    FILE * fp;
    fp = fopen(filename,"r");
    if(fp == NULL){
        printf("Failed to open file\n");
        exit(0);
    }

    fgets(buffer, sizeof(buffer)/sizeof(char), fp);
    for(int i=0;buffer[i]!='\0';i++){
        if(!isspace(buffer[i])){
            element[col]=buffer[i];
            col++;
        }
    }
    
    col = 0;
    while(!feof(fp)){
        fgets(buffer, sizeof(buffer)/sizeof(char), fp);
        for(int i=0;buffer[i]!='\n';i++){
            if(buffer[i] =='0' || buffer[i]=='1'){
                matrix[row][col] = buffer[i]-'0';
                col++;
            }
        }
        row++;
        col = 0;
    }
}

node* getLastNode(node* List, int i){
    node* curr = NULL;
    curr = &List[i];
    while(1){
        if(curr->next == NULL) break;
        curr = curr->next;
    }
    return curr;
}

void printList(node* List, int N){
    printf("\nadjacency list\n");
    node* curr = NULL;
    for(int i=0;i<N;i++){
        curr = &List[i];
        while(curr->next != NULL){
            printf("%c -> ",curr->value);
            curr = curr->next;
        }
        printf("%c\n",curr->value);
    }
}

void matrixToList(node* List, int** matrix,int N, char* element){
    node* lastNode = NULL;
    node* newNode = NULL;
    List = (node*)malloc(sizeof(node)*N);

    for(int i=0;i<N;i++){
        List[i].value = element[i];
        List[i].next = NULL;
    }

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            lastNode = getLastNode(List, i);
            if(matrix[i][j]==1){
                newNode = (node*)malloc(sizeof(node));
                newNode->value = element[j];
                newNode->next = NULL;
                lastNode->next = newNode;
            }
        }
    }
    printList(List, N);
}

int main(){
    char filename[64];
    int N = 0, temp = 0;
    int** matrix;
    char* element;
    node* List;

    //file to matrix
    printf("Enter the name of file : ");
    scanf("%s",filename);
    
    N = getTotalLine(filename) - 1;
    element = (char*)malloc(sizeof(char)*N);
    matrix = (int**)malloc(sizeof(int*)*N);
    for(int i=0;i<N;i++){
        matrix[i]=(int*)malloc(sizeof(int)*N);
    }
    fileToMatrix(filename, element, matrix);

    //matrix to list & print the list
    matrixToList(List, matrix, N, element);

    //convert graph transpose
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(j>i){ //only over diagonal
                temp = matrix[i][j];
                matrix[i][j] = matrix[j][i];
                matrix[j][i] = temp;
            }
        }
    }

    //matrix to list & print the list
    matrixToList(List, matrix, N, element);

    return 0;
}