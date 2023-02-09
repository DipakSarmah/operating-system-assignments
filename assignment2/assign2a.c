/*
Assignment 2(a):-
Information:
The goal of this assignment is to make you understand about basic inter-process
communication semantics using signal/interrupt handling. The method of using
through handling the signals i.e. a process executes a particular (user) function on
receipt of the signal.
Problem:
Write a C program using signal() system call to handle the reception of SIGINT
signal which allows to execute a designated (user) function. Here the function is
responsible for creating a child process by using fork() system call and then you
have to display the PROCESS ID and PARENT PROCESS ID from the parent
process as well as from the child process.
 You need to put explanatory comment in your program to demonstrate the
purpose and why you have used the system calls 
Hints:
* For generating, SIGINT (SIGINT is a keyboard interrupt signal) signal, you
have to press Ctrl+C. So, by default pressing Ctrl+C in a running program leads to
the termination of the running process. But, your program should provide a way to
handle the keyboard interrupt through signal() system call by executing user
defined function as mentioned above.
* To know more about, see signal(2) man page (command: “man 2 signal”) and refer
W.R. Steven Book,Vol-2
*/

#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int sig);

int main(){

signal(SIGINT,sigint_handler);
while(1){   // infinite loop to print the PID of the main process
     
	printf("Present in main function()...\n");
	sleep(2);
	
}

return 0;

}

// an interrupt handler is always of void return type with an integer as its argument   
void sigint_handler(int sig){ 
	printf("\nInside signal handler after interrupt \n\n");
	sleep(2);//sleep is a computer program which set the process to wait until the specified amount of time
	pid_t pid;
	int status;
	pid=fork();
	if(pid==-1){
		perror("error in fork call!\n");
		exit(1);
	}
	if(pid==0){
		printf("Inside child process...\n\n");
		printf("The child process id is: %d \nAnd parent process id is: %d\n",getpid(),getppid());
		exit(0);
	}
	else{
		//pid becomes child process id;
		wait(&status);
		printf("Inside main process..\n\n");
		printf("The child process id is: %d and main process id is: %d\n",pid,getpid());
		printf("The main process parent id is : %d\n",getppid());
		exit(0);
	}
	

}


    
 

