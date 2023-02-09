/*
Assignment 1(b):-
Write a C program like the assignment 1(a). But here use the system call wait() system to synchronize the
execution of parent program in your program until child process finishes. Now write a function to print to
find out who is logging in the local machine that is partially equivalent to the cmd "w" or "who" and engage
the grandchild to print the output into the stdout.
Hints:
*For wait( ) system call , you need a header file #include<sys/wait.h>
* Learn to see /var/run/utmp
* Source code for "struct utmp" is available in utmp.h. You need to print the member variable "ut_user" for
the logged in user. You can use command “man utmp” to see the manual.

*/


#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<utmp.h>

void user(){
	struct utmp* users=getutent();
	//The getutent() function reads in the next entry from a user-information file. If the file isn't already open, getutent() opens it. If the function reaches the end of the file, it fails.//
	int i;
	while(users!=NULL){
	//return value of getutent(): A pointer to a utmp structure for the next entry, or NULL if the file couldn't be read or reached the end of file.
	printf("the users are: ");
		for(i=0;(users->ut_user[i] != '\0');i++){
			printf("%c",users->ut_user[i]);
		}
	printf("\n");
	users=getutent();
	}

}

int main()
{

        pid_t pid,gcpid,temp;
        int fd[2],countfd[2],cfd[2],stat,count;
        if(pipe(fd)==-1){
                printf("pipe error\n");
                exit(1);//Exit failure
        }
        if(pipe(countfd)==-1){
                printf("pipe error\n");
                exit(1);//Exit failure//
        }
        if(pipe(cfd)==-1){
                printf("pipe error\n");
                exit(1);//Exit failure//
        }
        pid=fork();//The fork system call creates a new process. fork() creates a new process(child process) by duplicating the calling process(parent process).//
        if(pid<0){
                printf("fork failed! \n");
                exit(1);
        }
        if(pid==0){
                gcpid=fork();//for creating grand child process//
                if(gcpid<0){
                        printf("fork failed! \n");
                        exit(1);
                }
                if(gcpid==0){
                	//inside grand child process
                        printf("Inside grandchild process\n\n");
                        printf("Roll no is: CSB20021\n");
                        printf("list of logging in the local machine: \n");
			user();
			
			
			count=1; 	//one grandchild process
			close(countfd[0]);//0 is for read and 1 is for write//
			write(countfd[1],&count,sizeof(int));
			close(countfd[1]);
			exit(0);
                 
                }
                else{
                        wait(&stat);
                        temp=gcpid;
                        printf("Inside child process!!\n\n"); //for convinence
                        printf("child process id is: %d and parent process id is: %d\n",getpid(),getppid());
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
        if(pid>0){
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
