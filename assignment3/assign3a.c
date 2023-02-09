/*
Write a C program to handle the reception of INTERRUPT FROM THE
KEYBOARD signal by executing a particular (user) function, which function is
responsible for creating a child process by using fork() system call. Also, the child
process will take an input N (N is the no. of terms to generate the Fibonacci series)
from the user and generate the Fibonacci series up to N terms. Then the child
process will send the Fibonacci series terms one by one to the parent process by
using pipe() system call and the parent process will read the Fibonacci series terms
one by one and will display it.
(N.B. – Before sending the Fibonacci terms one by one from the child
process, first you send the Process ID of the child process to the parent process and
read and display it in the parent process. Again after sending the terms one by one
from the child process to the parent process, from the child process you send the
Signal ID for which handler function has been generated and read and display it in
the parent process.)
Hints:-
• Learn about signal () system call, pipe () system call, fork () system call, write
() system call, read () system call.
• To keep synchronize the child process execution with parent process
execution and vice versa, you may need to use the sleep () system call.
• For the above mentioned system call, follow the man pages.
• Try to understand the following diagram (Diagram1).

*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigint_handler(int signum) 
{
	int s = 0;
    printf("\nInterrupted!\n");
    int pipefd[2];
    int N;
    pid_t pid;

    printf("\nEnter an integer N:\t");
    scanf("%d", &N);

    int arr[N];

    pipe(pipefd);

    switch (pid = fork()) {
    	case -1:	// something went wromg!
    		perror("fork");
            exit(1);

        case 0:		// inside child process
        	printf(" CHILD: This is the child process!\n");
        	// closing the read-descriptor
        	close(pipefd[0]);

        	arr[0] = 0;
        	arr[1] = 1;

        	for(int i = 2; i < N; i++) {
        		arr[i] = arr[i - 1] + arr[i - 2];
        	}

        	printf("Writing into the pipe\n");
        	for(int i = 0; i < N; i++) {
        		write(pipefd[1], &arr[i], sizeof(int));
        	}

        	// closing the write-descriptor
        	close(pipefd[1]);

        	exit(0);

        default:
        	wait(&s);
        	printf("PARENT: This is the parent process!\n");
        	// closing the write-descriptor
        	close(pipefd[1]); 
	        int fib;
	        printf("The Fibonacci series is: \n");
	        for (int i = 0; i < N; i++) { 
	        	// reading from the pipe and storing it in the variable 'fib' 
	            read(pipefd[0], &fib, sizeof(int)); 
	            printf("%d ", fib); 
	        }
	        printf("\n");
        	
        	close(pipefd[0]); 
    }

    exit(0);
}

int main() 
{ 
  
	signal(SIGINT, sigint_handler);
    while(1) {
    	printf("Inside Main\n");
    	sleep(1);
    }


    return 0; 
}
