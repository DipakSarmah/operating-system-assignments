/*
	This program is to be executed from a new terminal.
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

// this function simply displays the entered Signal ID
void signal_handler(int sig) {
	printf("Signal ID %d of the signal has been called!\n", sig);
}

int main() {
	pid_t pid;
	int sid;

	printf("Enter the pid of the program : ");	// PID to whom a signal is to be sent
	scanf("%d", &pid);
	printf("\nEnter the id of the signal: ");	// see all signal IDs using kill -l command
	scanf("%d", &sid);

	signal_handler(sid);					// calling the handler function

	kill(pid, sid);

	return 0;
}
