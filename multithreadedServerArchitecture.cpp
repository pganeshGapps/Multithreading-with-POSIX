#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string>
using namespace std;

const unsigned int aSECOND=1000000;// 1000000 microseconds=1sec. ... using for POSIX unix : usleep(microseconds);
pthread_mutex_t mtx1;
int requestCtr; //No of requests come to servers
//class Server
class Server{
    private:
        //static int serverCtr;//No of server created so far
    public:
        Server(){requestCtr=0; /*serverCtr+=0;*/}
        void respondRequest(string msg,int count);
};
void Server::respondRequest(string msg,int count){
    cout<<endl<<"Server is Processing Request ..."<<endl;
    usleep(2*aSECOND);

    pthread_mutex_lock(&mtx1);
    ++requestCtr;
    pthread_mutex_unlock(&mtx1);
    while(count--){
        //pthread_mutex_lock(&mtx1);
        cout<<msg<<" from Request: "<<requestCtr<<endl;
        //pthread_mutex_unlock(&mtx1);
    }
    cout<<"Responded!! Number of requests serviced by all servers= "<<requestCtr<<endl;
}

//Class Client
class Client{
    private:
        //static int clientCtr;//No of clients created so far
        int sentReqCtr;
    public:
        Client(){sentReqCtr=0;}
        void sendRequest(string msg,int count,Server s);//count=NoOfRepetition
};
void Client::sendRequest(string msg,int count,Server s){
    ++sentReqCtr;
    cout<<"Sending Request Number: "<<sentReqCtr<<endl;
    s.respondRequest(msg,count);
}

class Request{
    public:
        string msg;
        int count;
        Server s;
        Request(){ msg=""; count=0;}
        Request(string str,int cnt,Server ser){msg=str; count=cnt; s=ser;}
};
void * runner(void * args);

//Main
int main(){
    cout<<"Welcome to Multithreaded Server Architecture"<<endl;

    Client c;
    Server s;
    string str;
    int count;
    short req=0;

    //for without multithreading Find //##111
    Request R;

    cout<<"How many requests do you want to make? :"; cin>>req;
    pthread_t tid[req];

    int i=0;
    while( i<req){
        cout<<"\t\tEnter Details"<<endl;
        cout<<"Message : ";  cin>>str;                                 //getline(cin,str); //msg to be sent
         count=2;//cout<<"Count : "; cin>>count;                       //how many times/copies

        //start new thread of execution
        R.msg=str;  R.count=count;  R.s=s;
        pthread_create(&tid[0],NULL,runner,(void *)&R);
        ++i;
    }

    void* result;
    i=0;
    while(i<req){
    pthread_join(tid[0],&result);
    ++i;
    }

    cout<<"Exiting main..."<<endl;
    return 0;
}
void* runner(void * args){
    Request r;
    r.msg=((Request*)args)->msg;
    r.count=((Request*)args)->count;
    r.s=((Request*)args)->s;
    r.s.respondRequest(r.msg,r.count);

    pthread_exit(NULL);
}

//##111
// while(1){
//     cout<<"\t\tEnter Details"<<endl;
//     cout<<"Message : "; cin.ignore(); getline(cin,str);
//     cout<<"Count : "; cin>>count;
//     c.sendRequest(str,count,s);
//     cout<<"Do you want to Continue?(0=NO or 1=YES) :"; cin>>contFlag;
//     if(contFlag==0){ break;}
// }
