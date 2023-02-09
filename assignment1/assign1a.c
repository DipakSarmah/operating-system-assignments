/*
Assignment 1(a):-
Write a C program to create a child process using the system call fork( ). From the child process, display
the PID and PPID and then call again the fork( ) to create a grandchild and engage him to display your roll
no. From parent display the PID and PPID of all the processes and display the count of total no. of child
processes created also the number of grandchild processes created. Demonstrate the use of exit(0) and
exit(1).
Hints:
* There is a system call available in the LINUX to create a child process called fork( ). Again the fork( )
system call returns the PID of the child process to the parent process and returns the value 0 to the child
process. Moreover the return value type of the fork( ) system call is pid_t.
* For fork( ) system call , you need a header file
#include<sys/types.h>
* To get the PID and PPID , we have two more system call getpid( ) and getppid( ) and the return types of
both are int.


Note: Done in linux.
*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main()
{

	pid_t pid,gcpid,temp;
	int fd[2],countfd[2],cfd[2],stat,count;
	//a file descriptor FD is a unique identifier (handle) for a file or other input/output resource, such as a pipe //
	if(pipe(fd)==-1){
		printf("pipe error\n");
		exit(1);//Exit failure
	}
	if(pipe(countfd)==-1){
		printf("pipe error\n");
		exit(1);//Exit failure
	}
	if(pipe(cfd)==-1){
		printf("pipe error\n");
		exit(1);//Exit failure
	}
	pid=fork();//The fork system call creates a new process. fork() creates a new process(child process) by duplicating the calling process(parent process).//
	if(pid<0){
		printf("fork failed! \n");
		exit(1);
	}
	if(pid==0){
		gcpid=fork();
		if(gcpid<0){
			printf("fork failed! \n");
			exit(1);
		}
		if(gcpid==0){ //grand child process
			printf("Inside grandchild process\n\n");
			printf("Roll no is: CSB20021\n\n");
			//for counting the number of processes;
			count=1; 	//one grandchild process
			close(countfd[0]);//0 is for read and 1 is for write//
			write(countfd[1],&count,sizeof(int));
			close(countfd[1]);
			exit(0);
		}
		else{
			wait(&stat);
			printf("Inside child process!!\n\n"); //for convinence
			printf("child process id is: %d and parent process id is: %d\n",getpid(),getppid());
			
			//using pipe to store the grandchild pid and take it to parent process
			temp=gcpid;
			close(fd[0]);
			write(fd[1],&temp,sizeof(int));//writing grand child id//
			close(fd[1]);
			
			
			close(countfd[1]);
			read(countfd[0],&count,sizeof(int));
			close(countfd[0]);
			
			count++;//adding 1 for child
			
			//counting no of process
			close(cfd[0]);
			write(cfd[1],&count,sizeof(int));
			close(cfd[1]);
			//Successful execution of code
			exit(0);
		}
	}
	if(pid>0){//main process
		wait(&stat);
		pid_t gpid;

		close(fd[1]);
		read(fd[0],&gpid,sizeof(int));
		close(fd[0]);

		close(cfd[1]);
		read(cfd[0],&count,sizeof(int));
		close(cfd[0]);
		count++;
	
		printf("Inside main process\n\n");
		printf("the total number of process are: %d\n",count);
		printf("the main parent process id is: %d and main process id is: %d\n",getppid(),getpid());
		printf("the child process id is: %d and grandchild process id is: %d\n",pid,gpid);
		exit(0);
	}

	return 0;
}
