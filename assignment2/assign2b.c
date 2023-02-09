// Assignment 2(b):-
// Write a C program which will take the Process ID and signal ID as input to
// demonstrate the use of kill( ) system call.
// /* You need to put explanatory comment in your program to demonstrate the
// purpose and why you have used the system calls */
// Hints:
// * For demonstrating so, you modify the assignment 2(a) to handle each and every
// signal (as possible as). Run command kill -l, to know about signal type and ID. Now
// run the modified assignment 2(a).
// * Again from another terminal, run the assignment 2(b) which will take the Process
// ID of the modified assignment 2(a) and any valid signal value as input.
// * Your signal handler function of the modified assignment 2(b) should be able to
// display the signal ID of the generated signal.
// * To know more about kill( ), see kill(2) man page and for signal type, value, action
// and comment, see signal(7) man page.

#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int sig);
void sighub_handler(int sig);
void sigquit_handler(int sig);

int main(){
	pid_t p=getpid();
	signal(SIGINT,sigint_handler);
        signal(SIGHUP,sighub_handler);
        signal(SIGQUIT,sigquit_handler);


while(1){

        printf("Present in main function()...process id: %d\n",p);
        sleep(2);
        
}
return 0;

}
void sighub_handler(int sig){
        printf("sighub is to kill a child and a parent process!!");
        exit(0);
}
void sigquit_handler(int sig){
        printf("quit signal is received!\n");
}

void sigint_handler(int sig){
        printf("Inside signal handler!!\n");
        sleep(2);
        pid_t pid;
        pid=fork();
        if(pid==-1){
                printf("error in fork call!\n");
                exit(1);
        }
        if(pid==0){
                printf("Inside child process...\n\n");
                printf("The child process id is: %d and parent process id is: %d\n",getpid(),getppid());
        }
        else{
                //pid becomes child process id;
                printf("Inside main process..\n\n");
                printf("The child process id is: %d and main process id is: %d\n",pid,getpid());
                printf("The main process parent id is : %d\n",getppid());
        }

}
