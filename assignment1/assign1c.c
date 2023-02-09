/*
Assignment 1(c):-
Write a C program like the assignment 1(b) and overlay a user designed program into the address space of
the child process using execv() system call. Again use wait() system call to synchronize the execution of
parent process in your program until child process finishes. Here use the macro WIFEXITED to capture
the returned status of the child in parent process. Also demonstrate the use of argument vector to print the
program name by the child process.
*/

#include <stdio.h>
#include <sys/types.h>
#include <math.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	pid_t q; 	//pid_t is a signed integer type which is capable of representing a process ID
	int s,status;
	q = fork();			//  system call to create a child process

	if (q < 0) {			// child creation unsuccessful
		printf("Error!");
		exit(1);// exit failure : abnormal termination of the program
	} 
	else if (q == 0)
	{	// child process
		 
		printf("Inside child process.\n");
		printf("Child process ID is %d\n", getpid());
		printf("The program name extracted using Argument Vector is: %s\n",argv[0]);
	

		char *args[] = {"/home/dipak/Documents/operating_system/assignment1/program", NULL};
		execv(args[0], args); 	//execv(const char* path,char *const args[]) it causes the current process to abandon the program that it is running and start running the program in file path.
		exit(0);	//exit success
	} 
	else {	// parent process

		wait(&status); //pid_t waitpid(pid_t pid, int *status, int options);
		printf("INSIDE PARENT PROCESS:\n\n");
		printf("exit status is: %d\n",WIFEXITED(status));
		printf("Parent has pID %d\n", getpid());
	}
	
	return 0;
}
