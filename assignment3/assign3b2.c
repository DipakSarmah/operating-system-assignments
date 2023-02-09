/*
Write a C program called Program1 which is responsible for creating a 
named PIPE or FIFO with necessary permission for all users by using the 
mknod( ) system call. Now, you keep the facility to read the information 
from the FIFO if any other process write some information into the 
FIFO and as well as keep the facility to write some information into 
the FIFO, so that Program2 can read it.   

Again write another C program called Program2 which is responsible for taking the 
user input and write it back to the FIFO so that Program1 can read it
. Moreover, you have to provide the facility to read the FIFO in this 
program. 
*/
// Program 2
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main() {
	int mkret;
    int fifofd;
    char str[20];
    int choice = 0;
    char buffer[20];

    mkret = mknod("myfifo", S_IFIFO|0666, 0);

	if (mkret == -1 && errno != EEXIST) {
	    perror("mknod() failed!\n");
	    exit(0);
	}

	printf("FIFO has been created successfully!\n");

	while (1) {
		printf("Enter a string(max 20 characters):\n");
        fgets(str, 20, stdin);
		fifofd = open("myfifo", O_WRONLY);

        if (fifofd == -1) {
            perror("File opening failed!\n");
            exit(0);
        }

        // Code below will be executed once the FIFO is read from another program, because by default it is in BLOCKING Mode
        printf("File opened successfully!\n");

        write(fifofd, str, strlen(str) + 1);
        printf("PROG2: Successfully written to FIFO!\n");

        close(fifofd);

		printf("MENU:\n");
		printf("WRITE\t[1]\n");
		printf("READ\t[2]\n");
		printf("EXIT\t[3]\n");
		scanf("%d", &choice);

		if (choice == 2) {
			fifofd = open("myfifo", O_RDONLY);

            if (fifofd == -1) {
                perror("File opening failed!\n");
                exit(0);
            }

            printf("File opened successfully!\n");

            read(fifofd, buffer, 20);
            printf("PROG2: Successfully received data: %s\n", buffer);

            close(fifofd);
            exit(0);
		}

		if (choice == 3) {
			break;
		}
	}
}
