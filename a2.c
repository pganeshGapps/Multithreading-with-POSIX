#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct timespec st,en;
double runtime;

// Input Matrix Variable (like their row and column count
int n,m,p;

// Input Matrix
int **inputMatrix1, **inputMatrix2;

// Resultant Matrix
int **resultMatrix;

int currRow = 0;
int currCol = 0;

// Number of worker thread
int threadCount;

// Thread pool variable
pthread_t * threads;

/* Mutex for the currentRow. */
pthread_mutex_t mutex_Row = PTHREAD_MUTEX_INITIALIZER;

void *matrixMul(int id){
    int i,j,myCol,myRow;
    while(1){
        pthread_mutex_lock(&mutex_Row);
        if(currRow>=n){
            pthread_mutex_unlock(&mutex_Row);
            pthread_exit(0);
            return;
        }
        myCol = currCol;
        myRow = currRow;
        currCol++;
        if(currCol==p){
            currCol = 0;
            currRow++;
        }
        pthread_mutex_unlock(&mutex_Row);
        for(i=0;i<m;i++){
            resultMatrix[myRow][myCol] += (inputMatrix1[myRow][i]*inputMatrix2[i][myCol]);
        }
    }
}

int main()
{
    int i,j;
    printf("Enter the value of n,m,p\t");
    scanf("%d %d %d", &n, &m, &p);

    /* Initilase Input Matrix 1 */
    inputMatrix1 = (int **) malloc(n * sizeof(int *));
    for(i=0;i<n;i++){
        inputMatrix1[i] = (int *)malloc(m * sizeof(int));
    }

    /* Initilase Input Matrix 2 */
    inputMatrix2 = (int **) malloc(m * sizeof(int *));
    for(i=0;i<m;i++){
        inputMatrix2[i] = (int *)malloc(p * sizeof(int));
    }

    /* Initilase Resultant Matrix*/
    resultMatrix = (int **) malloc(n * sizeof(int *));
    for(i=0;i<n;i++){
        resultMatrix[i] = (int *)malloc(p * sizeof(int));
    }

    /* Giving random input to input Matrix number between 1 to 100 */
    FILE *fptr;
    fptr=fopen("input1.txt","w");
    if(fptr==NULL){
        printf("Error!");
        exit(1);
    }
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            inputMatrix1[i][j] = rand()%100 + 1;
            fprintf(fptr,"%d ",inputMatrix1[i][j]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
    fptr=fopen("input2.txt","w");
    if(fptr==NULL){
        printf("Error!");
        exit(1);
    }
    for(i=0;i<m;i++){
        for(j=0;j<p;j++){
            inputMatrix2[i][j] = rand()%100 + 1;
            fprintf(fptr,"%d ",inputMatrix2[i][j]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);
    for(i=0;i<n;i++){
        for(j=0;j<p;j++){
            resultMatrix[i][j] = 0;
        }
    }

    // Taking input number of Worker threads
    printf("Enter the number of worker threads you want\t");
    scanf("%d", &threadCount);

    clock_gettime(CLOCK_MONOTONIC,&st);
    threads = (pthread_t *)malloc(sizeof(pthread_t) * threadCount);

    for(i=0;i<threadCount;i++){
        pthread_create(&threads[i], NULL, (void *(*) (void *)) matrixMul, (void *) (i + 1));
    }
    for (i = 0; i < threadCount; i++) {
      pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC,&en);
    runtime = en.tv_sec - st.tv_sec+(en.tv_nsec-st.tv_nsec)/(1e9);
    printf("\nRuntime with multithreading is %f\n", runtime);

    fptr=fopen("result.txt","w");
    if(fptr==NULL){
        printf("Error!");
        exit(1);
    }
    for(i=0;i<n;i++){
        for(j=0;j<p;j++){
            //inputMatrix2[i][j] = rand()%100 + 1;
            fprintf(fptr,"%d ",resultMatrix[i][j]);
        }
        fprintf(fptr,"\n");
    }
    fclose(fptr);


    int k;
    int sum = 0;
    clock_gettime(CLOCK_MONOTONIC,&st);

    for(i=0;i<n;i++)
    {
        for(j=0;j<p;j++){
            for(k=0;k<m;k++){
                sum = sum + inputMatrix1[i][k]*inputMatrix2[k][j];
            }
            //r[i][j] = sum;
            sum = 0;
        }
    }
    clock_gettime(CLOCK_MONOTONIC,&en);
    runtime = en.tv_sec - st.tv_sec+(en.tv_nsec-st.tv_nsec)/(1e9);
    printf("\nRuntime without multithreading is %f\n", runtime);

    /*
    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            printf("%d\t", inputMatrix1[i][j]);
        }
        printf("\n");
    }
    for(i=0;i<m;i++){
        for(j=0;j<p;j++){
            printf("%d\t", inputMatrix2[i][j]);
        }
        printf("\n");
    }
*/
/*
int cnt = 0;
for(i=0;i<n;i++){
    for(j=0;j<p;j++){
        //printf("%d %d\t",resultMatrix[i][j],r[i][j]);
        if(resultMatrix[i][j]!=r[i][j]){
            cnt++;
        }
    }
}

printf("\n%d",cnt);
*/
    return 0;
}
