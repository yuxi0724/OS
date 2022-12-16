#include<iostream>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include <cstring>
using namespace std;
int main(int argc, char**argv)
{
    pid_t PID=fork();
    if(PID>0)
    {
        int fd = shm_open("posixsm",O_CREAT|O_RDWR,0666);
        ftruncate(fd,0x400000);
        int *n=(int*)mmap(0,0x400000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        int ar[3];
        cin>>ar[0];
        *n=ar[0];
        n[1]=PID;
        cout<<"["<<getpid()<<" Parent]: Generate "<< PID << endl;
        sleep(6);
        cout<<"["<<getpid()<<" Parent]: "<<"  "<< n[0] <<"!="<<*n<< endl;
        munmap(n,0x400000);
        return 0;
    }
    else if(PID==0)
    {
        sleep(3);
        int fd = shm_open("posixsm",O_RDONLY, 0666);
        ftruncate(fd,0x400000);
        int *n=(int*)mmap(0,0x400000,PROT_READ,MAP_SHARED,fd,0);
        int sum=1;
        for(int i=1;i<=n[0];i++)
           sum*=i;
        int count=0;
        int sum2=sum;
        while(sum2>0)
        {
            count++;
            sum2/=10;
        }        
        cout<<"["<<n[1]<<" Child]: "<< count <<" digits\n";
        munmap(n,0x400000);
        n[2]=sum;
        return 0;
    }
    else
    {
        cout<<"fail";
        exit(-1);
    }
}