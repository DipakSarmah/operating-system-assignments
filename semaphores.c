#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int shared=1;
sem_t s;

void* function1()
{
    int x;
    sem_wait(&s);
    x=shared;
    printf("thread1 read the value as : %d\n",shared);
    x--;
    printf("local updatation by thread1 is %d\n",x);
    sleep(1);
    shared = x;
    printf("value of shared variable updated by thread 1 is : %d\n",shared);
    sem_post(&s);
}
void* function2()
{
    int y;
    sem_wait(&s);
    y=shared;
    printf("thread2 read the value as :  %d\n",y);
    y++;
    printf("local updation by thread 2 is : %d\n",y);
    sleep(1);
    shared=y;
    printf("value of shared variable updated by thread2 is :%d\n",shared);
    sem_post(&s);
}


int main(){
    sem_init(&s,0,1);
    pthread_t thread1,thread2;

    pthread_create(&thread1,NULL,function1,NULL);
    pthread_create(&thread2,NULL,function2,NULL);

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    printf("the final value of the shared variable is : %d",shared);
    
}

