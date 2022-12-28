#include<iostream>
#include<pthread.h>
#include<time.h>
#include<thread>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
using namespace std;    
time_t now=time(0); // wall-clock time
tm* ltm =localtime(&now);   
bool sit[3]={false,false,false};    //紀錄椅子是否有人坐
bool inject=false;  //判斷是否有人在施打
int order[3]={-1,-1,-1};    //紀錄來的順序
pthread_mutex_t lock;
struct student
{
    bool use=false; //未使用
    bool sitting=false; //判斷該學生是否坐下
    int sit_num;    //儲存該學生坐哪張椅子
    int count=0;    //施打疫苗次數
    int wait;   //第一次施打的等待時間
    int EX; //沒椅子坐難過地去排隊
    int que;    //施打完後重新排隊
    int number; //學生編號
};
bool SIT_empty()    //判斷椅子是否全是空的，是則回傳true，否則回傳false
{
    for(int i=0;i<3;i++)
    {
        if(sit[i]==true)
            return false;
    }
    return true;
}
void *injection(void *S)    //施打or等待的過程
{
    student* s=(student *)S;
    while(s->count!=3)
    {
        if(s->count==0&&!s->sitting)
            sleep(s->wait*5);    
        if(!inject && 
                ((SIT_empty())
                    ||((s->sitting)&&order[s->sit_num]==0)))
        {    
            pthread_mutex_lock(&lock);
            inject=true;
            if(s->sitting)
            {
                sit[s->sit_num]=false;
                s->sitting=false;
                for(int j=0;j<3;j++)
                    if(order[j]>0&&sit[j])
                        order[j]-=1;
            }
            now=time(0);
            ltm =localtime(&now);    
            cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<" Student"<<s->number<<": ";
            cout<<"Entering to get a shot"<<endl;    
            s->count+=1;
            sleep(3);        
            inject=false;
            now=time(0);
            ltm =localtime(&now);    
            cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<" Student"<<s->number<<": ";
            cout<<"Leaving to write YZU CS305 programs\n";
            pthread_mutex_unlock(&lock);
            sleep(s->que*10);
        }
        else
        {                
            for(int i=1;i<=3;i++)
            {
                if((!sit[i-1])&&(!s->sitting))
                {
                    s->sit_num=i-1;
                    s->sitting=true;
                    sit[i-1]=true;
                    now=time(0);
                    ltm =localtime(&now);          
                    cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<" Student"<<s->number<<": ";
                    cout<<"Sitting #"<< i<<endl;
                    int CT=-1;
                    for(int j=0;j<3;j++)
                        if(sit[j])
                            CT++;
                    order[s->sit_num]=CT;
                    for(int j=0;j<2;j++)
                        for(int k=j+1;k<3;k++)
                            if(order[j]==order[k]&&order[j]!=0)
                            {
                                order[j]-=1;
                                order[k]-=1;
                                break;
                            }
                    break;
                }
                if(i==3&&!s->sitting)
                {
                    now=time(0);
                    ltm =localtime(&now);  
                    cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<" Student"<<s->number<<": ";
                    cout<<"Leaving to write YZU CS305 programs\n";
                    sleep(s->EX*5);
                }
            }
        }
    }
    pthread_exit((NULL)); 
}
int main()
{
    int n=0,RAND=-1;
    while(n<10||n>20)
    {
        cout<<"input number of student:";
        cin>>n;
    }
    while(RAND<0||RAND>100)
    {
        cout<<"input rand number:";
        cin>>RAND;
    }
    srand(RAND);
    int Rand=rand();
    pthread_t Student[n];
    student S[n];
    pthread_mutex_init(&lock,NULL);
    now=time(0);
    ltm =localtime(&now); 
    cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec<<" Injector: Sleep"<<endl;
    for(int i=0;i<n;i++)
    {
        S[i].wait=Rand%2+1;
        S[i].EX=Rand%2+1;
        S[i].que=Rand%3+1;
        S[i].number=i+1;
        pthread_create(&(Student[i]),NULL,injection,&S[i]);
    }
    for(int i=0;i<n;i++)
        pthread_join(Student[i],NULL);
    now=time(0);
    ltm =localtime(&now); 
    cout<<ltm->tm_hour<<":"<<ltm->tm_min<<":"<<ltm->tm_sec
        <<" Injector: All "<<n<<" students receive vaccines."<<endl;
}