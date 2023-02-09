/*
Assignment 7
Problem (A):
Write two C program named program1.c and program2.c and both will be responsible
for reading the contents of a single file. Now you put the concept of semaphore to provide
synchronization between the process program1 and program2, so that program2 can read the
contents of the specified file only after reading the contents of the file by program1. Again if
the program1 runs for n times, then program2 can read the contents of the file at most n times
only, not more than that. Moreover you have to remove the semaphore that is created for your
above mentioned operations properly by using proper system call. Try to demonstrate properly.
*/

// Assignment 7 Program1
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<errno.h>
#include<sys/shm.h>
#include<string.h>
#include<unistd.h>

#define MAX_RETRIES 15

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};

int initsem(key_t key, int nsems) 
{
    union semun arg;
    struct semid_ds buf;
    struct sembuf sb;
    int semid;
    semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

    if (semid >= 0) {
        sb.sem_op = 1;
        sb.sem_flg = 0;
        arg.val = 1;
        printf("Program1: Press enter:\n");
        getchar();
        for (sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++) {
            if (semop(semid, &sb, 1) == -1) {
                int e = errno;
                semctl(semid, 0, IPC_RMID);
                errno = e;
                return -1; 
            }
        }
    } else if (errno == EEXIST) { 
        int ready = 0;
        semid = semget(key, nsems, 0); 
        
        if (semid < 0) {
            return semid; 
        }
        
        arg.buf = &buf;

        for (int i = 0; i < MAX_RETRIES && !ready; i++) {
            semctl(semid, nsems - 1, IPC_STAT, arg);

            if (arg.buf->sem_otime != 0) {
                ready = 1;
            } else {
                sleep(1);
            }
        }

        if (!ready) {
            errno = ETIME;
            return -1;
        }
    } else {
        return semid; 
    }

    return semid;
}

int main(void) {
    key_t key,message;
    int semid;
    struct sembuf sb;
    
    key = ftok("program1.c", 'D');
    semid = initsem(key, 2);
    message = ftok("program1.c", 'S');

    int smid = shmget(message,1024,0644|IPC_CREAT);

    sb.sem_flg = SEM_UNDO;

    printf("Program1: Trying to write... \n");
    sb.sem_num = 0;
    sb.sem_op = -1; 
    semop(semid, &sb, 1);

    printf("Program1: Inside critical section!\n");
    char *buf = shmat(smid,(void*)0,0);
    snprintf(buf,20,"%s","I am program1");
    shmdt(buf);

    printf("Program1: Writing completed!\n");
    printf("Program1: Press enter to exit critical section:\n");
    getchar();

    sb.sem_op = 1;
    semop(semid, &sb, 1);

    printf("Program1: Waiting for Program2...\n");
    sleep(1);
    sb.sem_num=1;
    sb.sem_op=-1;
    semop(semid, &sb, 1);
    sb.sem_op=1;
    semop(semid, &sb, 1);

    printf("Program1: Press enter to delete semaphore: ");
    getchar();
    semctl(semid, 0, IPC_RMID);

    return 0;
}