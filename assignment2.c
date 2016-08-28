#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


struct timespec st,en;//startTime and EndTime
double runtime;

// Input Matrix Variable (like their row and column count
int n,m,p;

// Input Matrix and Resultant Matrix
int **inputMatrix1, **inputMatrix2;
int **resultMatrix, **r; // r is for verification

int currRow = 0;
int currCol = 0;

// Number of worker thread
int threadCount,prevThreadCount=0;

// Thread pool variable (array)
pthread_t * threads;

/* Mutex for the currentRow. */
pthread_mutex_t mutex_Row = PTHREAD_MUTEX_INITIALIZER;

void *matrixMul(int id){
    int i,j,myCol;
   while(1){
        pthread_mutex_lock(&mutex_Row);
        if(currCol>=p){
            pthread_mutex_unlock(&mutex_Row);
            pthread_exit(0);
            return;
        }
        myCol = currCol;
        ++currCol;
        pthread_mutex_unlock(&mutex_Row);
        for(i=0;i<m;i++){
            resultMatrix[currRow][myCol] += (inputMatrix1[currRow][i]*inputMatrix2[i][myCol]);
        }
    }
}


int main()
{
    int i,j; // loop iterators

        currCol=currRow=0;
        printf("Enter the value of n,m,p\t");
        scanf("%d %d %d", &n, &m, &p);

        //Initialising 2D array ---- dynamic memory allocation

        /* Input Matrix 1 (n x m) */
        inputMatrix1 = (int **) malloc(n * sizeof(int *));
        for(i=0;i<n;i++){
            inputMatrix1[i] = (int *)malloc(m * sizeof(int));
        }

        /* Input Matrix 2  (m x p) */
        inputMatrix2 = (int **) malloc(m * sizeof(int *));
        for(i=0;i<m;i++){
            inputMatrix2[i] = (int *)malloc(p * sizeof(int));
        }

        /* Resultant Matrix (n x p)*/
        resultMatrix = (int **) malloc(n * sizeof(int *));
        for(i=0;i<n;i++){
            resultMatrix[i] = (int *)malloc(p * sizeof(int));
        }

        /* r Matrix (n x p)*/
        r = (int **) malloc(n * sizeof(int *));
        for(i=0;i<n;i++){
            r[i] = (int *)malloc(p * sizeof(int));
        }

        /* Giving random input to input Matrix number between 1 to 100 */
        FILE *fptr;

        //for input file 1 and matrix 1
        fptr=fopen("input1.txt","w");
        if(fptr==NULL){
            printf("Error in Opening File!");
            exit(1);
        }

        for(i=0;i<n;i++){
            for(j=0;j<m;j++){
    	    //store value to matrix element
                inputMatrix1[i][j] = rand()%4 + 1;
    	    //store value to inputMatrix1 file
                fprintf(fptr,"%d ",inputMatrix1[i][j]);
            }
            fprintf(fptr,"\n");
        }
        fclose(fptr);

        //for input file 1 and matrix 1
        fptr=fopen("input2.txt","w");
        if(fptr==NULL){
            printf("Error!");
            exit(1);
        }
        for(i=0;i<m;i++){
            for(j=0;j<p;j++){
                inputMatrix2[i][j] = rand()%4 + 1;
                fprintf(fptr,"%d ",inputMatrix2[i][j]);
            }
            fprintf(fptr,"\n");
        }
        fclose(fptr);

        //for result matrix
        for(i=0;i<n;i++){
            for(j=0;j<p;j++){
                resultMatrix[i][j] = 0;
            }
        }

        // Taking input number of Worker threads
        printf("Enter the number of worker threads you want\t");
        scanf("%d", &threadCount);
	
	printf("\n----Multithreaded multiplication starts now----\n");
        clock_gettime(CLOCK_MONOTONIC,&st); // set  startTime
    // Allocating required space for thread pool
     threads = (pthread_t *) malloc(sizeof(pthread_t) * threadCount);         

       while(currRow<n){
            for(i=0;i<threadCount;i++){
                pthread_create(&threads[i], NULL, (void *(*) (void *)) matrixMul, (void *) (i + 1));
    	    //arguments are :- pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg //
            }

            for (i = 0; i < threadCount; i++) {
    			pthread_join(threads[i], NULL);
            }
            currRow++;
            currCol = 0;
        }
     
    
        clock_gettime(CLOCK_MONOTONIC,&en); //set endTime
        runtime = en.tv_sec - st.tv_sec+(en.tv_nsec-st.tv_nsec)/(1e9);
        printf("\nRuntime:- %f\n", runtime);

        fptr=fopen("result.txt","w");
        if(fptr==NULL){
            printf("Error in opening Result File!");
            exit(1);
        }
        for(i=0;i<n;i++){
            for(j=0;j<p;j++){
                fprintf(fptr,"%d ",resultMatrix[i][j]);
            }
            fprintf(fptr,"\n");
        }
        fclose(fptr);
        printf("\nSize of thread pool : %d\n",threadCount);

        // EXTRA PART :-
        /** System Processing (simple Multiplication without using Multiithreading) *
        int k;
        int sum = 0;
        clock_gettime(CLOCK_MONOTONIC,&st);
        for(i=0;i<n;i++)
        {
            for(j=0;j<p;j++){
                for(k=0;k<m;k++){
                    sum = sum + inputMatrix1[i][k]*inputMatrix2[k][j];
                }
                r[i][j] = sum;
                sum = 0;
            }
        }
        clock_gettime(CLOCK_MONOTONIC,&en);
        runtime = en.tv_sec - st.tv_sec+(en.tv_nsec-st.tv_nsec)/(1e9);
        printf("\nRuntime2(single) :-  %f\n", runtime);
       *****/

	/*
        //INPUT MATRIX 1
        printf("\nINPUT 1 :- \n");
        for(i=0;i<n;i++){
            for(j=0;j<m;j++){
                printf("%d\t", inputMatrix1[i][j]);
            }
            printf("\n");
        }

        //INPUT MATRIX 2
        printf("\nINPUT 2 :- \n");
        for(i=0;i<m;i++){
            for(j=0;j<p;j++){
                printf("%d\t", inputMatrix2[i][j]);
            }
            printf("\n");
          }*/

      /*  printf("\nRESULT :- \n");
        int cnt = 0;//mismatch count
        for(i=0;i<n;i++){
        for(j=0;j<p;j++){
            printf("%d \t",resultMatrix[i][j]);
            /*if(resultMatrix[i][j]!=r[i][j]){
                cnt++;
            }*/
       /* }
       printf("\n");
    } */

        //printf("\nMismatches= %d\t\t percentage= %f \n",cnt,cnt*100/(m*p) );

       /** Deallocation **/
        int **ptr;

    	 ptr=inputMatrix1;
    	 for(int i = 0; i < n; i++)
           		free(ptr[i]);
     	 free(ptr);

       ptr=inputMatrix2;
    	 for(int i = 0; i < m; i++)
           		free(ptr[i]);
     	 free(ptr);

    	 ptr=resultMatrix;
    	 for(int i = 0; i < n; i++)
           		free(ptr[i]);
     	 free(ptr);

    	 ptr=r;
    	 for(int i = 0; i < n; i++)
           		free(ptr[i]);
     	 free(ptr);

       /******************/
    return 0;
}
