#include<iostream>
#include<pthread.h>
#include<time.h>
#include<cstdlib>
#include<string>
#include<thread>
#include<unistd.h>
#include<semaphore.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
struct Fibo
{
    int count=0;
    int number=0;
    long long int sum=0;
    int tid;
    pthread_mutex_t lock;
};
double pi(int n)
{
    srand(5);
    int count=0;
    double x,y;
    for(int i=0;i<n;++i)
    {
        x=(double)rand()/RAND_MAX;
        y=(double)rand()/RAND_MAX;
        if(x*x+y*y<=1)++count;
    }
    return (double)count/n*4;
}
void *Fibonacci_number(void *F)
{
    time_t start,end;
    start=time(NULL);
    pi(1e8);
    Fibo *f=(Fibo *)F;
    f->tid=gettid();
    long long int count1=0,count2=1;
    for(int i=1;i<f->count;i++)
    {
        f->sum=count1+count2;
        count1=count2;
        count2=f->sum;
    }
    long long int sum=f->sum;
    int digit=0;
    while(sum>0)
    {
        sum/=10;
        digit++;
    }
    end=time(NULL);
    double diff =double(end-start);
    cout<<"[Child "<<f->number<<" tid="<<f->tid<<"] F("<<f->count<<") "<<digit<<" digits "<<diff<<" ms\n";    return 0;
}

int main(void)
{
    time_t start,end;
    start=time(NULL);
    pi(1e8);
    pthread_t fibonacci;
    Fibo F;
    int n=0,n1=0;
    while(n>5||n<1)
        cin>>n;
    int count[n];

    while(n1!=n)
    {    
        while(count[n1]>50||count[n1]<10)
            cin>>count[n1];            
        n1++;    
    }
    for(int i=0;i<n;i++)
    {
        pthread_mutex_init(&(F.lock),NULL);
        F.count=count[i];
        F.number=i+1;
        pthread_create(&fibonacci,NULL,Fibonacci_number,&F);
        pthread_join(fibonacci,NULL);   
        cout<<"[Main thread] Thread ID of F("<<count[i]<<"): "<< F.tid << endl;   
        cout<<"[Main thread] F("<<count[i]<<"): "<< F.sum << endl;  
    }
    end=time(NULL);
    double diff =difftime(end,start);
    cout<<"[Main thread]"<< diff << "ms" <<endl;  

    return 0;
}
