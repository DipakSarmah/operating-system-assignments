#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHM_SIZE 1024

int main(int argc,char * argv[])
{
    key_t key;
    int shmid;
    char* data;
    int mode;

    if(argc>2){
        fprintf(stderr, "usage: shmdemo [data_to_write]\n");
        exit(1);
    }
    /*make the key: */
    if((key=ftok("prog1.c",'R'))==-1)
    {
        perror("ftok");
        exit(1);
    }

    /* connect to (and possibly create) the segment: */
    if((shmid=shmget(key,SHM_SIZE,0644|IPC_CREAT))==-1){
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it*/
    if((data=(char *)shmat(shmid,(void *)0,0))==(char* )(-1)){
        perror("shmat");
        exit(1);
    }
    printf("you wrote: \"%s\"\n",(char *) data);
    if(shmdt(data)==-1){
        perror("shmdt");
        exit(1);
    }
    return 0;
}