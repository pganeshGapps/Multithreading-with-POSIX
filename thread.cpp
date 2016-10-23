///Another similar example

#include <iostream>
#include <pthread.h>
using namespace std;

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;//PTHREAD_MUTEX_INITIALIZER
static int ctr=0;

void *print_message(void*)
{
    pthread_mutex_lock(&mtx);
    ++ctr;
    cout << "Threading: "<<ctr<<endl;
    pthread_mutex_unlock(&mtx);
}
int main()
{
    // pthread_t t1;
    // pthread_create(&t1, NULL, &print_message, NULL);
    // cout << "Hello";

    //Pool of Threads
    pthread_t t1[5];
    for(int i=0;i<5;++i){
    pthread_create(&t1[i], NULL, &print_message, NULL);
    cout << "Hello";
    }

    // void* result;
    // pthread_join(t1,&result);

    void* result;
    for(int k=0;k<5;++k){
    pthread_join(t1[k],&result);
    }

    cout<<"flag"<<endl;
    return 0;
}


/**
#include<iostream>
#include<cstdlib>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
using namespace std;

void* start_routine(void* args);

int main(){
    cout<<"Main starts"<<endl;
    pthread_t tid[5];

    //pthread_attr_t attr;
    // Initialize and set thread joinable
    //pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    int tNo=1;
    while(tNo<6){
    pthread_create(&tid[tNo-1],NULL,&start_routine,NULL);//(void*)tNo); // default attr is joinable
    ++tNo;
    }
    void* res;
    for(int k=0;k<5;++k){
        pthread_join(tid[k],&res);
    }

    cout<<"exiting main"<<endl;
    return 0;
}

void* start_routine(void* args){
    static int i=0;
    ++i;
    if(i==5){ cout<<" Thread: "<<i<<endl; pthread_exit(0); }
    short ctr=i,iter=1;
    do{
    cout<<" Thread: "<<i<<endl;
    ++iter;
    }while(ctr==i && iter!=5 );
    pthread_exit(0);
}

**/
