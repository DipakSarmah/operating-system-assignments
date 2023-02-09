/*
Problem Statement:
Write a C program which will be able to read a file using read()
system call. Now you provide a mechanism to set a lock by using fcntl()
system call and to release the lock by using the same fcntl() system call.
For setting up and releasing out a lock, you have to fill up a structure flock
(struct flock) with proper values.
Now you run the program from two different terminals simultaneously and
demonstrate that if one process has got the lock, then that process only can read
the file. But the other one is not permitted to read the file until the lock is
released by the first process. That means, only one process who acquires the
lock can read the file at any instant of time.
Related Information:
 Read read(), write() and fcntl()system call from W. R.
Steven's "Unix Network programming" book .
 Also open the man page for fcntl() to learn quickly about its usage
pattern.
*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>

using namespace std;

int main(int argc, char *argv[]){
			   /*l_type l_whence l_start l_len l_pid
			   l_type ---> F_RDLCK,F_WRLCK, F_UNLCK
			   l_whence ---> SEEK_SET, SEEK_CUR, SEEK_END
			   l_len ---> if 0 then lock to end-of-file
			   */
			  /*l_type, l_whence, l_start, l_len, l_pid */ 	
	struct flock fl = {
	F_WRLCK, 	//F_RDLCK,F_WRLCK, F_UNLCK (type of lock you want to set)
	SEEK_SET, 	//SEEK_SET(for beginning of file), SEEK_CUR (current file position), SEEK_END (eof) (determises where l_start start from)
	0, 		//offset from l_whence
	40, 		//length, 0=to eof
	0		//pid
	};//defined at fcntl.h header file
	int fd;
	char buff[50];
	
	fl.l_pid = getpid();
	
	if(argc > 1)
		fl.l_type = F_RDLCK;
		
	if ((fd = open("text.txt", O_RDWR)) == -1){
		perror("open");
		exit(1);
	}
	
	printf("Press Enter to lock: ");
	getchar();
	printf("locking...");
	if(fcntl(fd, F_SETLKW, &fl) == -1){
		perror("fcntl");
		exit(1);
	}
	
	printf("got locked\n");
	printf("press Enter to release lock: ");
	getchar();
	
	fl.l_type = F_UNLCK ; /* set to unlock same region */
	
	if(fcntl(fd,F_SETLK, &fl) == -1){//3 parameters 
		perror("fcntl");
		exit(1);
	}
	printf("unlocked.\n");
	read(fd,buff,30);
	printf("%s",buff);
	
	close(fd);
	return 0;
}
