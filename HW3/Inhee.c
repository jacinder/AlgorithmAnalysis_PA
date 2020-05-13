#include <stdio.h> 
#include <time.h> 

long long recur_subset(long long arr[], int k, int N ,long long start, long long TLen, long long T_F[], long long count);
long long dp_subset(long long arr[], int N, int k);

int main() {

   int N, k;
    long long recur_count = 0;
    long long dp_count = 0;
    
    printf("Enter the N, k: ");
    scanf("%d %d", &N, &k);
    long long arr[N];
    long long T_F[N]; 
    
    for(long long i = 0; i < N; i++){
        arr[i] = i;
    }

    //Recur ver.
    clock_t recur_time; 
    recur_time = clock();

    recur_count = recur_subset(arr, k, N, 0, 0, T_F, recur_count);
    printf("Recursive_count: %lld\n", recur_count);
    
    recur_time = clock() - recur_time; 
    double recur_time_taken = ((double)recur_time)/CLOCKS_PER_SEC;
    printf("Recursive took %f seconds to execute.\n\n", recur_time_taken);

    //DP ver.
    clock_t dp_time; 
    dp_time = clock();

    dp_count = dp_subset(arr, N, k);
    printf("Dynamic_Programming_count: %lld\n", dp_count);

    dp_time = clock() - recur_time; 
    double dp_time_taken = ((double)recur_time)/CLOCKS_PER_SEC;
    printf("DP took %f seconds to execute.\n\n", dp_time_taken);

    

  return 0;

}

//currLen: number of T
//start: index of arr
long long recur_subset(long long arr[], int k, int N ,long long start, long long TLen, long long T_F[], long long count) {

    if (TLen == k) {
      count++;
        return count;
    }

    if (start == N) {
        return count;
    }

    T_F[start] = 1;
    count = recur_subset(arr, k, N, start + 1, TLen + 1, T_F, count);
  
    T_F[start] = 0;
    count = recur_subset(arr, k, N, start + 1, TLen, T_F, count);

    return count;

}

long long dp_subset(long long arr[], int N, int k) { 
  long long dp[k][N]; 
  long long count = 0; 

  if (k == 0){ 
      count = 1; 

  }else if(k != 0){
    for(long long i = 0; i < k; i++){
        for(long long j = 0; j < N; j++){
            dp[i][j] = 0;
        }
    }

    for (long long i = 0; i < N; i++) {
        dp[0][i] = 1; 
    }
 
    for (long long l = 1; l < k; l++) { 
        for (long long i = l; i < N; i++) { 
            dp[l][i] = 0; 
            for (long long j = l - 1; j < i; j++) { 
                if (arr[j] < arr[i]) 
                    dp[l][i] += dp[l - 1][j]; 
            } 
        } 
    }
  
    for (long long i = k - 1; i < N; i++){ 
        count += dp[k - 1][i]; 
    }
  
  }
    
    return count; 
} 
