/*
Problem (A):
Write two C programs named program1.c and program2.c to demonstrate the concept of shared
memory where program1 (process1) will be responsible for writing it’s process id and program2 (
process2) will be responsible for reading the contents whatever program1(process1) writes. Moreover
try to remove the shared memory segment that is created for your above mentioned operations
properly by using proper system call. Try to demonstrate properly.
Hints:
 For creating a shared memory segment or accessing an existing shared memory segment you
need to use system call-shmget(key_t key, size_t size, int oflag) .
 For detaching the segment you need to use system call-shmdt(const void *shmaddr)
 To know how to implement variety of operations on a shared memory segment, you need to go
through the system call-shmctl(int shmid, int cmd, struct shmid_ds *buff)
 For better understanding about the system call API, go through the man pages and UNIX
NETWORK PROGRAMMING by W.RICHARD STEVENS.
 Check with command ipcs
*/

//writing process
#include<stdio.h>
#include<stdlib.h>
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
    if((data=shmat(shmid,(void *)0,0))==(char*)(-1)){
        perror("shmat");
        exit(1);
    }
    if(argc==2){
        printf("writing to segment: \"%s\"\n", argv[1]);
        strncpy(data,argv[1],SHM_SIZE);
    }
    if(shmdt(data)==-1){
        perror("shmdt");
        exit(1);
    }
    return 0;
}