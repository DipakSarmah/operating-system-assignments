/*
Assignment 2(c):-
Write a C program to create a user level thread using system call
pthread_create( ) and assign the thread to display the “HELLO WORLD” . Use
pthread_exit() in your program (if possible) for terminating the thread.
You need to put explanatory comment in your program to demonstrate the
purpose and why you have used the system calls 
Hints:
* To know more about pthread_create( ), see pthread_create (3) man page and to
know more about pthread_exit( ), see pthread_exit (3).
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// starting routine
void* display() {
	printf("HELLO WORLD\n");
}

int main() {
	pthread_t t1;

	// pthread_create() is used to create User-Level Threads
	//pthread_create(&id,attributes, starting routine,arguments);
	// 1st parameter: The reference to the thread that we are trying to create
	// 2nd parameter:  attributes. If this parameter is NULL, then the default attributes are used.
	// 3rd parameter: The reference to the function(starting routine) that we are trying to execute through the thread.
	// 4th parameter: The arguments to be passed into the routine function. In case of no parameters, we just pass NULL.
	pthread_create(&t1, NULL, &display, NULL);
	pthread_exit(NULL);		// terminates the calling thread and makes the value_ptr available to any successful join with the terminating thread.

	return 0;
}
